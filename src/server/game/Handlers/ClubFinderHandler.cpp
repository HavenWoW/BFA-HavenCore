/*
 * 2026 BFA-HavenCore
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */


// -----------------------------------------------------------------------------
// Club Finder handlers (GUILD side only).
//
// Bridges the 8.3 client's CLUB_FINDER_* opcodes to the existing ClubFinderMgr,
// so a guild that is "listed" in the guild finder shows up in the Guild &
// Communities browse panel. Non-guild communities are not implemented.
//
// ClubID, PostingID and ClubFinderGUID are distinct protocol concepts. PostingID
// is persisted independently by ClubFinderMgr; ClubID remains the owning guild id.
// -----------------------------------------------------------------------------
//
// File split mirrors Draconic's Club Finder layout:
//   Guilds/ClubFinderMgr.*        persistent state, search and expiry
//   Handlers/ClubFinderHandler.* session/opcode policy and state -> packet mapping
//   Server/Packets/ClubFinderPackets.* wire parsing/serialization only
//
// Keep 8.3.7 packet decisions here rather than in the data manager. Recipe and
// community-query opcodes live in GuildRecipeHandler.cpp / CommunityQueryHandler.cpp.
// -----------------------------------------------------------------------------

#include "WorldSession.h"
#include "CharacterCache.h"
#include "DatabaseEnv.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "World.h"
#include <algorithm>
#include <string>
#include <unordered_map>
#include "ClubFinderPackets.h"
#include "ClubFinderMgr.h"

namespace
{
    // The LOOKUP reply envelope is a request/callback discriminator, not a
    // description of the returned posting. Echo it unchanged. Search requests
    // arrive as 0x20 and own/subscribed posting lookups as 0x60 on 8.3.7.
    constexpr uint8 DefaultLookupEnvelope = 0x60;

    uint32 GetGuildPostingID(Guild const* guild)
    {
        return guild ? sClubFinderMgr->GetPostingId(guild->GetGUID()) : 0;
    }

    ObjectGuid CreateGuildClubFinderGuid(Guild const* guild)
    {
        return sClubFinderMgr->GetClubFinderGuid(guild);
    }

    Guild* ResolveGuildFromClubFinderGuid(ObjectGuid const& clubFinderGuid)
    {
        if (clubFinderGuid.IsEmpty() || clubFinderGuid.GetHigh() != HighGuid::ClubFinder)
            return nullptr;

        if (Guild* guild = sGuildMgr->GetGuildById(ObjectGuid::LowType(clubFinderGuid.GetRawValue(0))))
            return guild;

        // Fallback through the independent posting id encoded in the high qword.
        uint32 postingId = uint32(clubFinderGuid.GetRawValue(1) & 0xFFFFFFFFu);
        ObjectGuid guildGuid = sClubFinderMgr->GetGuildGuidForPostingId(postingId);
        return guildGuid.IsEmpty() ? nullptr : sGuildMgr->GetGuildByGuid(guildGuid);
    }

    uint32 GetClosedToken(MembershipRequest const& request)
    {
        // Retail uses zero for Pending/Approved and a non-zero opaque token for
        // terminal states. Preserve the proven semantic contract only.
        return request.IsClosed() ? 1u : 0u;
    }

    char const* GetApplicationStatusName(uint8 status)
    {
        switch (status)
        {
            case WorldPackets::ClubFinder::RequestStatusNone:          return "None";
            case WorldPackets::ClubFinder::RequestStatusPending:       return "Pending";
            case WorldPackets::ClubFinder::RequestStatusAutoApproved:  return "AutoApproved";
            case WorldPackets::ClubFinder::RequestStatusDeclined:      return "Declined";
            case WorldPackets::ClubFinder::RequestStatusApproved:      return "Approved";
            case WorldPackets::ClubFinder::RequestStatusJoined:        return "Joined";
            case WorldPackets::ClubFinder::RequestStatusJoinedAnother: return "JoinedAnother";
            case WorldPackets::ClubFinder::RequestStatusCanceled:      return "Canceled";
            default:                                                    return "Unknown";
        }
    }

    WorldPackets::ClubFinder::ClubFinderApplicationUpdate BuildApplicationUpdate(Guild const* guild, MembershipRequest const& request)
    {
        WorldPackets::ClubFinder::ClubFinderApplicationUpdate application;
        application.ClubFinderGUID = CreateGuildClubFinderGuid(guild);
        application.PlayerGUID = request.GetPlayerGUID();
        application.Closed = GetClosedToken(request);
        application.LastUpdatedTime = request.GetUpdateTime() ? request.GetUpdateTime() : uint32(request.GetSubmitTime());
        application.RequestStatus = request.GetStatus();
        return application;
    }

    // Club Finder posting TabardInfo. Capture labels for style/color are swapped
    // relative to Haven's EmblemInfo semantics; keep Haven's original semantic
    // fields and the proven +1 sentinel encoding. Layout: style 8 bits, then
    // emblem color, border style, border color and background color at 6 bits each.
    uint32 PackClubFinderTabardInfo(EmblemInfo const& emblem)
    {
        auto field = [](uint32 value, uint32 mask) { return (value + 1) & mask; };
        return (field(emblem.GetStyle(), 0xFF) << 24)
             | (field(emblem.GetColor(), 0x3F) << 18)
             | (field(emblem.GetBorderStyle(), 0x3F) << 12)
             | (field(emblem.GetBorderColor(), 0x3F) << 6)
             |  field(emblem.GetBackgroundColor(), 0x3F);
    }

    bool BuildPostingFromGuild(LFGuildSettings const& guildSettings, WorldPackets::ClubFinder::ClubFinderPosting& out)
    {
        Guild* guild = sGuildMgr->GetGuildByGuid(guildSettings.GetGUID());
        if (!guild || guild->GetMembersCount() == 0)
            return false;

        out.ClubFinderGUID = CreateGuildClubFinderGuid(guild);
        out.NumActiveMembers = guild->GetMembersCount();
        out.SpecMask = guildSettings.GetSpecMask();

        constexpr uint32 FactionHordeFlag = 1u << 15;
        constexpr uint32 FactionAllianceFlag = 1u << 16;
        constexpr uint32 FactionNeutralFlag = 1u << 17;
        out.RecruitmentFlags = guildSettings.GetRecruitmentFlags();
        out.RecruitmentFlags &= ~(FactionHordeFlag | FactionAllianceFlag | FactionNeutralFlag);
        out.RecruitmentFlags |= guildSettings.GetTeam() == TEAM_HORDE ? FactionHordeFlag : FactionAllianceFlag;

        out.MinItemLevel = guildSettings.GetMinItemLevel();
        out.TabardInfo = PackClubFinderTabardInfo(guild->GetEmblemInfo());
        out.LastPosterGUID = guild->GetLeaderGUID();
        out.ClubID = guild->GetId();
        out.LastUpdatedTime = guildSettings.GetLastUpdatedTime();
        out.ClubName = guild->GetName();
        out.Comment = guildSettings.GetComment();
        sCharacterCache->GetCharacterNameByGuid(guild->GetLeaderGUID(), out.LeaderName);
        return true;
    }

    WorldPackets::ClubFinder::ClubFinderApplicantInfo BuildApplicantInfo(Guild const* guild, MembershipRequest const& request)
    {
        WorldPackets::ClubFinder::ClubFinderApplicantInfo applicant;
        applicant.ClubFinderGUID = CreateGuildClubFinderGuid(guild);
        applicant.PlayerGUID = request.GetPlayerGUID();
        applicant.Closed = GetClosedToken(request);
        applicant.LastUpdatedTime = request.GetUpdateTime() ? request.GetUpdateTime() : uint32(request.GetSubmitTime());
        applicant.RequestStatus = request.GetStatus();
        applicant.ItemLevel = request.GetItemLevel();
        applicant.SpecMask = request.GetSpecMask();
        applicant.Message = request.GetComment();

        if (CharacterCacheEntry const* info = sCharacterCache->GetCharacterCacheByGuid(request.GetPlayerGUID()))
        {
            applicant.Race = info->Race;
            applicant.Class = info->Class;
            applicant.Level = info->Level;
        }

        return applicant;
    }


    void SendApplicationUpdate(Player* receiver, Guild const* guild, MembershipRequest const& request, uint8 context)
    {
        if (!receiver || !guild)
            return;

        WorldPackets::ClubFinder::ClubFinderUpdateApplications update;
        update.Context = context;
        update.Applications.push_back(BuildApplicationUpdate(guild, request));
        WorldPacket const* packet = update.Write();
        TC_LOG_INFO("guild", "[CF-APP] push UPDATE_APPLICATIONS receiver=%s guild=%u player=%s status=%u(%s) closed=%u context=0x%02X size=%u",
            receiver->GetName().c_str(), guild->GetId(), request.GetPlayerGUID().ToString().c_str(),
            uint32(request.GetStatus()), GetApplicationStatusName(request.GetStatus()), GetClosedToken(request),
            uint32(context), uint32(packet->size()));
        receiver->SendDirectMessage(packet);
    }

    void SendApplicationUpdateToRecruiters(Guild* guild, MembershipRequest const& request)
    {
        if (!guild)
            return;

        auto notify = [guild, &request](Player* member)
        {
            if (guild->HasRecruitRight(member))
                SendApplicationUpdate(member, guild, request, 0x20);
        };
        guild->BroadcastWorker(notify);
    }

}

namespace
{
    // Applies the BFA Club Finder search filters to one listed guild posting.
    // Posting recruitmentFlags: focus bits 1-5 (0x3E), size bits 6-8 (0x1C0),
    // max-level-only 0x2000, locale+1 in bits 21-25. A filter with value 0 (or
    // a posting that recruits 'all', e.g. spec mask 0) never excludes.
    bool PostingMatchesSearch(WorldPackets::ClubFinder::ClubFinderRequestClubsList const& request,
        LFGuildSettings const& posting, Guild const* guild)
    {
        using Request = WorldPackets::ClubFinder::ClubFinderRequestClubsList;

        // Haven only has guild postings.
        if (request.RequestType != 1)
            return false;

        uint32 const flags = posting.GetRecruitmentFlags();

        if (Request::Filter const* focus = request.FindFilter(Request::FilterFocus))
            if ((focus->Value & 0x3E) && !(flags & focus->Value & 0x3E))
                return false;

        if (Request::Filter const* size = request.FindFilter(Request::FilterSize))
            if ((size->Value & 0x1C0) && !(flags & size->Value & 0x1C0))
                return false;

        if (Request::Filter const* itemLevel = request.FindFilter(Request::FilterItemLevel))
            if (posting.GetMinItemLevel() > itemLevel->Value)
                return false;

        if (Request::Filter const* level = request.FindFilter(Request::FilterLevel))
            if ((flags & 0x2000) && level->Value < uint64(sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)))
                return false;

        if (Request::Filter const* specs = request.FindFilter(Request::FilterSpecs))
            if (posting.GetSpecMask() && specs->Value && !(posting.GetSpecMask() & specs->Value))
                return false;

        if (Request::Filter const* locale = request.FindFilter(Request::FilterLocale))
        {
            uint32 const postingLocalePlusOne = (flags >> 21) & 0x1F;
            if (locale->Value && postingLocalePlusOne && !(locale->Value & (uint64(1) << (postingLocalePlusOne - 1))))
                return false;
        }

        if (!request.SearchTerms.empty())
        {
            std::string name = guild->GetName();
            std::string term = request.SearchTerms;
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            std::transform(term.begin(), term.end(), term.begin(), ::tolower);
            if (name.find(term) == std::string::npos)
                return false;
        }

        return true;
    }
}

void WorldSession::HandleClubFinderRequestClubsList(WorldPackets::ClubFinder::ClubFinderRequestClubsList& request)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    std::string filterText;
    for (auto const& filter : request.Filters)
        filterText += Trinity::StringFormat(" %u:%llu", uint32(filter.Type), (unsigned long long)filter.Value);
    TC_LOG_INFO("guild", "[CF-SEARCH] C->S REQUEST_CLUBS_LIST [%s] type=%u settings=0x%X search='%s' filters=[%s ]%s",
        GetPlayerInfo().c_str(), uint32(request.RequestType), request.Settings, request.SearchTerms.c_str(), filterText.c_str(),
        request.Malformed ? " MALFORMED" : "");

    LFGuildPlayer settings(player->GetGUID(), GUILDFINDER_ALL_ROLES, AVAILABILITY_ALWAYS, ALL_INTERESTS, ALL_GUILDFINDER_LEVELS);
    std::vector<LFGuildSettings const*> guildList = sClubFinderMgr->GetGuildsMatchingSetting(settings, player->GetTeamId());

    WorldPackets::ClubFinder::ReturnRecruitingClubs response;
    response.PostingIDs.reserve(guildList.size());
    for (LFGuildSettings const* guildSettings : guildList)
    {
        if (!guildSettings || !guildSettings->IsListed())
            continue;

        if (Guild* guild = sGuildMgr->GetGuildByGuid(guildSettings->GetGUID()))
            if (guild->GetMembersCount() > 0 && PostingMatchesSearch(request, *guildSettings, guild))
                response.PostingIDs.push_back(GetGuildPostingID(guild));
    }

    TC_LOG_DEBUG("guild", "SMSG_RETURN_RECRUITING_CLUBS [%s] postings=%u context=0x%02X",
        GetPlayerInfo().c_str(), uint32(response.PostingIDs.size()), uint32(response.Context));
    WorldPacket const* recruitingData = response.Write();
    SendPacket(recruitingData);
}

void WorldSession::HandleClubFinderRequestClubsData(WorldPackets::ClubFinder::ClubFinderRequestClubsData& request)
{
    WorldPackets::ClubFinder::LookupClubPostingsList response;
    response.Context = request.Context ? request.Context : DefaultLookupEnvelope;
    response.Postings.reserve(request.ClubFinderPostingIDs.size());

    for (uint32 postingId : request.ClubFinderPostingIDs)
    {
        ObjectGuid guildGuid = sClubFinderMgr->GetGuildGuidForPostingId(postingId);
        if (guildGuid.IsEmpty())
            continue;

        Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid);
        LFGuildSettings const* guildSettings = sClubFinderMgr->GetGuildSettingsByPostingId(postingId);
        if (!guild || guild->GetMembersCount() == 0 || !guildSettings || !guildSettings->IsListed())
            continue;

        WorldPackets::ClubFinder::ClubFinderPosting posting;
        if (BuildPostingFromGuild(*guildSettings, posting))
        {
            TC_LOG_INFO("guild", "[CF-CACHE] [%s] postingId=%u finderGuid=%s clubId=" UI64FMTD " updated=" UI64FMTD " requestType=%u linked=%u context=0x%02X",
                GetPlayerInfo().c_str(), postingId, posting.ClubFinderGUID.ToHexString().c_str(), posting.ClubID,
                posting.LastUpdatedTime, uint32(request.RequestType), request.LinkedLookup ? 1u : 0u, uint32(request.Context));
            response.Postings.push_back(std::move(posting));
        }
    }

    // Echo request.Context exactly. Rewriting 0x60 (own/subscribed lookup) to
    // 0x20 (search lookup) breaks the client's request completion path.

    TC_LOG_DEBUG("guild", "SMSG_CLUB_FINDER_LOOKUP_CLUB_POSTINGS_LIST [%s] requested=%u returned=%u filters=%u requestType=%u linked=%u context=0x%02X",
        GetPlayerInfo().c_str(), uint32(request.ClubFinderPostingIDs.size()), uint32(response.Postings.size()), request.FilterCount,
        uint32(request.RequestType), request.LinkedLookup ? 1u : 0u, uint32(response.Context));
    WorldPacket const* lookupData = response.Write();
    SendPacket(lookupData);
}

void WorldSession::HandleClubFinderRequestSubscribedClubPostingIDs(WorldPackets::ClubFinder::ClubFinderRequestSubscribedClubPostingIDs& request)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    WorldPackets::ClubFinder::ClubFinderGetClubPostingIDsResponse response;
    response.Entries.reserve(request.ClubIDs.size());

    for (uint64 clubId : request.ClubIDs)
    {
        Guild* guild = sGuildMgr->GetGuildById(ObjectGuid::LowType(clubId));
        if (!guild || guild->GetMembersCount() == 0)
        {
            continue;
        }

        // The client can retain an old Club ID after /gquit. Never map a former
        // guild back to an own/subscribed posting for a non-member.
        if (player->GetGuildId() != guild->GetId())
        {
            continue;
        }

        LFGuildSettings const& guildSettings = sClubFinderMgr->GetGuildSettings(guild->GetGUID());
        if (!guildSettings.IsListed())
        {
            continue;
        }

        uint32 postingId = GetGuildPostingID(guild);
        if (!postingId)
        {
            continue;
        }

        WorldPackets::ClubFinder::ClubFinderClubPostingID entry;
        entry.ClubID = clubId;
        entry.PostingID = postingId;
        response.Entries.push_back(entry);

    }

    WorldPacket const* mappingData = response.Write();
    SendPacket(mappingData);
}

void WorldSession::HandleClubFinderRequestMembershipToClub(WorldPackets::ClubFinder::ClubFinderRequestMembershipToClub& request)
{
    Player* player = GetPlayer();
    if (!player || player->GetGuildId())
        return;

    TC_LOG_INFO("guild", "[CF-APP] C->S REQUEST_MEMBERSHIP player=%s finderGuid=%s specMask=%llu commentLen=%u",
        GetPlayerInfo().c_str(), request.ClubFinderGUID.ToString().c_str(), uint64(request.SpecMask), uint32(request.Comment.size()));

    Guild* guild = ResolveGuildFromClubFinderGuid(request.ClubFinderGUID);
    if (!guild)
    {
        TC_LOG_INFO("guild", "[CF-APP] REQUEST_MEMBERSHIP rejected: finder GUID did not resolve");
        return;
    }

    LFGuildSettings const& guildSettings = sClubFinderMgr->GetGuildSettings(guild->GetGUID());
    if (!guildSettings.IsActiveListing())
        return;

    // BFA keeps applications for seven days. A guild decline also blocks a
    // re-application for seven days from the decline/update timestamp.
    if (sClubFinderMgr->HasRequest(player->GetGUID(), guild->GetGUID()))
        return;

    uint32 now = uint32(time(nullptr));
    uint32 itemLevel = uint32(std::max<float>(0.0f, player->GetAverageItemLevelEquipped()));
    MembershipRequest membershipRequest(player->GetGUID(), guild->GetGUID(), uint32(AVAILABILITY_ALWAYS), uint32(GUILDFINDER_ALL_ROLES),
        uint32(ALL_INTERESTS), request.Comment, time_t(now), request.SpecMask, itemLevel,
        uint8(WorldPackets::ClubFinder::RequestStatusPending), now);
    sClubFinderMgr->AddMembershipRequest(guild->GetGUID(), membershipRequest);
    TC_LOG_INFO("guild", "[CF-APP] state ADD guild=%u player=%s status=%u(%s) update=%u",
        guild->GetId(), player->GetGUID().ToString().c_str(), uint32(membershipRequest.GetStatus()),
        GetApplicationStatusName(membershipRequest.GetStatus()), membershipRequest.GetUpdateTime());

    SendApplicationUpdate(player, guild, membershipRequest, 0x20);

    // Retail pushes the same application-state update to the recruiting guild so
    // its applicant view can update without a relog/manual refresh.
    SendApplicationUpdateToRecruiters(guild, membershipRequest);
}

void WorldSession::HandleClubFinderGetApplicantsList(WorldPackets::ClubFinder::ClubFinderGetApplicantsList& request)
{
    // Expiry is enforced lazily from these polls (throttled to once per hour).
    sClubFinderMgr->CleanupExpiredApplications();

    Player* player = GetPlayer();
    if (!player)
        return;

    TC_LOG_INFO("guild", "[CF-APP] C->S GET_APPLICANTS player=%s context=0x%02X rawSize=%u",
        GetPlayerInfo().c_str(), uint32(request.Context), uint32(request.GetRawPacket()->size()));

    Guild* guild = player->GetGuild();
    WorldPackets::ClubFinder::ReturnApplicantList response;
    response.Context = request.Context ? request.Context : uint8(0x20);

    if (!guild || (response.Context & 0x20) == 0 || !guild->HasRecruitRight(player))
    {
        SendPacket(response.Write());
        return;
    }

    response.ClubFinderGUID = CreateGuildClubFinderGuid(guild);
    if (std::unordered_map<ObjectGuid, MembershipRequest> const* requests = sClubFinderMgr->GetAllMembershipRequestsForGuild(guild->GetGUID()))
    {
        response.Applicants.reserve(requests->size());
        for (auto const& requestPair : *requests)
        {
            // A pending application past the 7-day window is shown as expired by
            // the client; do not list it as a live applicant. Decided ones stay
            // (Closed != 0) and populate Applicant History.
            MembershipRequest const& membershipRequest = requestPair.second;
            if (membershipRequest.GetStatus() == WorldPackets::ClubFinder::RequestStatusPending && membershipRequest.IsExpiredPlayerApplication())
                continue;

            response.Applicants.push_back(BuildApplicantInfo(guild, membershipRequest));
        }
    }

    TC_LOG_DEBUG("guild", "SMSG_RETURN_APPLICANT_LIST [%s] applicants=%u context=0x%02X",
        GetPlayerInfo().c_str(), uint32(response.Applicants.size()), uint32(response.Context));
    WorldPacket const* applicantPacket = response.Write();
    TC_LOG_INFO("guild", "[CF-APP] S->C RETURN_APPLICANT_LIST player=%s applicants=%u context=0x%02X size=%u",
        GetPlayerInfo().c_str(), uint32(response.Applicants.size()), uint32(response.Context),
        uint32(applicantPacket->size()));
    SendPacket(applicantPacket);

    // The client independently requests the selected posting through
    // CMSG_CLUB_FINDER_REQUEST_CLUBS_DATA. Do not send an unsolicited
    // LOOKUP_CLUB_POSTINGS_LIST here: that duplicates the recruitment-post
    // return event for one logical refresh and can feed the 8.3.7 UI loop.
}

void WorldSession::HandleClubFinderRequestPendingClubsList(WorldPackets::ClubFinder::ClubFinderRequestPendingClubsList& request)
{
    // Expiry is enforced lazily from these polls (throttled to once per hour).
    sClubFinderMgr->CleanupExpiredApplications();

    Player* player = GetPlayer();
    if (!player)
        return;

    TC_LOG_INFO("guild", "[CF-APP] C->S REQUEST_PENDING player=%s context=0x%02X rawSize=%u",
        GetPlayerInfo().c_str(), uint32(request.Context), uint32(request.GetRawPacket()->size()));

    WorldPackets::ClubFinder::ResponseCharacterApplicationList response;
    response.Context = request.Context ? request.Context : uint8(0x60);

    std::vector<MembershipRequest const*> requests = sClubFinderMgr->GetAllMembershipRequestsForPlayer(player->GetGUID());
    response.Applications.reserve(requests.size());
    for (MembershipRequest const* membershipRequest : requests)
    {
        if (!membershipRequest || membershipRequest->IsExpiredPlayerApplication())
            continue;

        Guild* guild = sGuildMgr->GetGuildByGuid(membershipRequest->GetGuildGuid());
        if (!guild)
            continue;

        // Retain Joined in the guild's applicant history, but it must not make
        // a former member look as if they still belong to that guild.
        if (membershipRequest->GetStatus() == WorldPackets::ClubFinder::RequestStatusJoined &&
            player->GetGuildId() != guild->GetId())
            continue;

        response.Applications.push_back(BuildApplicationUpdate(guild, *membershipRequest));
    }

    WorldPacket const* pendingPacket = response.Write();
    TC_LOG_INFO("guild", "[CF-APP] S->C RESPONSE_CHARACTER_APPLICATION_LIST player=%s applications=%u context=0x%02X size=%u",
        GetPlayerInfo().c_str(), uint32(response.Applications.size()), uint32(response.Context),
        uint32(pendingPacket->size()));
    SendPacket(pendingPacket);
}

void WorldSession::HandleClubFinderRespondToApplicant(WorldPackets::ClubFinder::ClubFinderRespondToApplicant& request)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    TC_LOG_INFO("guild", "[CF-APP] C->S RESPOND_TO_APPLICANT recruiter=%s finderGuid=%s playerGuid=%s flags=0x%02X accept=%u decline=%u",
        GetPlayerInfo().c_str(), request.ClubFinderGUID.ToString().c_str(), request.PlayerGUID.ToString().c_str(),
        uint32(request.Flags), request.IsAccept() ? 1u : 0u, request.IsDecline() ? 1u : 0u);

    if (!request.IsAccept() && !request.IsDecline())
    {
        TC_LOG_INFO("guild", "[CF-APP] RESPOND_TO_APPLICANT rejected: unknown flags=0x%02X", uint32(request.Flags));
        return;
    }

    Guild* guild = ResolveGuildFromClubFinderGuid(request.ClubFinderGUID);
    if (!guild || player->GetGuildId() != guild->GetId())
        return;

    if (!guild->HasRecruitRight(player))
        return;

    MembershipRequest const* existing = sClubFinderMgr->GetMembershipRequest(request.PlayerGUID, guild->GetGUID());
    if (!existing || existing->GetStatus() != WorldPackets::ClubFinder::RequestStatusPending)
        return;

    uint8 const newStatus = request.IsAccept()
        ? uint8(WorldPackets::ClubFinder::RequestStatusApproved)
        : uint8(WorldPackets::ClubFinder::RequestStatusDeclined);

    uint32 now = uint32(time(nullptr));
    if (!sClubFinderMgr->UpdateMembershipRequestStatus(request.PlayerGUID, guild->GetGUID(), newStatus, now))
        return;

    MembershipRequest const* updated = sClubFinderMgr->GetMembershipRequest(request.PlayerGUID, guild->GetGUID());
    if (!updated)
        return;

    TC_LOG_INFO("guild", "[CF-APP] state RECRUITER_RESPONSE guild=%u player=%s status=%u(%s) update=%u",
        guild->GetId(), request.PlayerGUID.ToString().c_str(), uint32(updated->GetStatus()),
        GetApplicationStatusName(updated->GetStatus()), updated->GetUpdateTime());

    // Retail uses context 0x20 for the recruiting guild and 0x60 for the
    // applicant-side pending/invitation list. Decline is status 3.
    SendApplicationUpdateToRecruiters(guild, *updated);
    if (Player* applicant = ObjectAccessor::FindConnectedPlayer(request.PlayerGUID))
        SendApplicationUpdate(applicant, guild, *updated, 0x60);

    // MITIGATION (step 50): the 8.3.7 recruiter UI only moves the answered
    // applicant from Applicants to History after the window is reopened
    // (GET_APPLICANTS_LIST). Until the native merge of the 0x20 update is
    // traced, give the responding recruiter the refreshed list right away;
    // it only raises CLUB_FINDER_RECRUITS_UPDATED (BuildList, no request).
    WorldPackets::ClubFinder::ClubFinderGetApplicantsList refreshRequest(WorldPacket(CMSG_CLUB_FINDER_GET_APPLICANTS_LIST, 1));
    refreshRequest.Context = 0x20;
    HandleClubFinderGetApplicantsList(refreshRequest);
}

void WorldSession::HandleClubFinderApplicationResponse(WorldPackets::ClubFinder::ClubFinderApplicationResponse& request)
{
    Player* player = GetPlayer();
    if (!player)
        return;

    TC_LOG_INFO("guild", "[CF-APP] C->S APPLICATION_RESPONSE player=%s finderGuid=%s flags=0x%02X accept=%u guildId=%u",
        GetPlayerInfo().c_str(), request.ClubFinderGUID.ToString().c_str(), uint32(request.Flags),
        request.IsAccept() ? 1u : 0u, player->GetGuildId());

    if (request.IsDecline())
    {
        // The applicant withdraws a pending application or declines the guild's
        // invitation. Retail keeps the record as history with status Canceled (7),
        // closed, and the applicant's pending list shows it that way.
        Guild* guild = ResolveGuildFromClubFinderGuid(request.ClubFinderGUID);
        if (!guild)
            return;

        MembershipRequest const* existing = sClubFinderMgr->GetMembershipRequest(player->GetGUID(), guild->GetGUID());
        if (!existing || (existing->GetStatus() != WorldPackets::ClubFinder::RequestStatusPending
            && existing->GetStatus() != WorldPackets::ClubFinder::RequestStatusApproved))
        {
            TC_LOG_INFO("guild", "[CF-APP] APPLICATION_RESPONSE decline ignored: no pending/approved application (status=%u)",
                existing ? uint32(existing->GetStatus()) : 0u);
            return;
        }

        uint32 now = uint32(time(nullptr));
        if (!sClubFinderMgr->UpdateMembershipRequestStatus(player->GetGUID(), guild->GetGUID(),
            uint8(WorldPackets::ClubFinder::RequestStatusCanceled), now))
            return;

        MembershipRequest const* updated = sClubFinderMgr->GetMembershipRequest(player->GetGUID(), guild->GetGUID());
        if (!updated)
            return;

        TC_LOG_INFO("guild", "[CF-APP] state APPLICANT_DECLINE guild=%u player=%s status=%u(%s) update=%u",
            guild->GetId(), player->GetGUID().ToString().c_str(), uint32(updated->GetStatus()),
            GetApplicationStatusName(updated->GetStatus()), updated->GetUpdateTime());

        SendApplicationUpdate(player, guild, *updated, 0x60);
        SendApplicationUpdateToRecruiters(guild, *updated);
        return;
    }

    if (!request.IsAccept() || player->GetGuildId())
    {
        TC_LOG_INFO("guild", "[CF-APP] APPLICATION_RESPONSE ignored: flags=0x%02X currentGuild=%u",
            uint32(request.Flags), player->GetGuildId());
        return;
    }

    Guild* guild = ResolveGuildFromClubFinderGuid(request.ClubFinderGUID);
    if (!guild)
        return;

    MembershipRequest const* existing = sClubFinderMgr->GetMembershipRequest(player->GetGUID(), guild->GetGUID());
    if (!existing || existing->GetStatus() != WorldPackets::ClubFinder::RequestStatusApproved)
        return;

    uint32 now = uint32(time(nullptr));
    if (!sClubFinderMgr->UpdateMembershipRequestStatus(player->GetGUID(), guild->GetGUID(),
        uint8(WorldPackets::ClubFinder::RequestStatusJoined), now))
        return;

    MembershipRequest const* joinedRequest = sClubFinderMgr->GetMembershipRequest(player->GetGUID(), guild->GetGUID());
    if (joinedRequest)
    {
        TC_LOG_INFO("guild", "[CF-APP] state APPLICANT_ACCEPT guild=%u player=%s status=%u(%s) update=%u",
            guild->GetId(), player->GetGUID().ToString().c_str(), uint32(joinedRequest->GetStatus()),
            GetApplicationStatusName(joinedRequest->GetStatus()), joinedRequest->GetUpdateTime());
        SendApplicationUpdate(player, guild, *joinedRequest, 0x60);
        SendApplicationUpdateToRecruiters(guild, *joinedRequest);
    }

    // Retail only creates actual guild membership after the applicant accepts the
    // Club Finder invitation (Approved -> Joined), not when the leader approves it.
    CharacterDatabaseTransaction trans(nullptr);
    if (!guild->AddMember(trans, player->GetGUID()))
    {
        // Keep the Finder state truthful if guild membership creation fails after
        // the applicant accepted the invitation. The client has already seen the
        // transient Joined update, so immediately restore Approved on both views.
        uint32 rollbackTime = uint32(time(nullptr));
        if (sClubFinderMgr->UpdateMembershipRequestStatus(player->GetGUID(), guild->GetGUID(),
            uint8(WorldPackets::ClubFinder::RequestStatusApproved), rollbackTime))
        {
            if (MembershipRequest const* restored = sClubFinderMgr->GetMembershipRequest(player->GetGUID(), guild->GetGUID()))
            {
                SendApplicationUpdate(player, guild, *restored, 0x60);
                SendApplicationUpdateToRecruiters(guild, *restored);
            }
        }
    }
}
