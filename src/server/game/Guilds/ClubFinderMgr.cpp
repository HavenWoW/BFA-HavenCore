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

#include "ClubFinderMgr.h"
#include "DatabaseEnv.h"
#include "DB2Stores.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "GuildFinderPackets.h"
#include "ClubFinderPackets.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "World.h"
#include <algorithm>
#include <vector>

namespace
{
    constexpr uint32 ClubFinderEnableListingFlag = 1u << 12;

    void BindPostingStatement(CharacterDatabasePreparedStatement* stmt, uint32 postingId, LFGuildSettings const& settings)
    {
        Guild* guild = sGuildMgr->GetGuildByGuid(settings.GetGUID());

        stmt->setUInt32(0, postingId);
        stmt->setUInt64(1, settings.GetGUID().GetCounter());
        stmt->setString(2, guild ? guild->GetName() : std::string());
        stmt->setString(3, settings.GetComment());
        stmt->setUInt64(4, settings.GetSpecMask());
        stmt->setUInt32(5, settings.GetRecruitmentFlags());
        stmt->setUInt32(6, settings.GetMinItemLevel());
        stmt->setUInt32(7, 0); // avatarId - guild postings use the guild tabard instead
        stmt->setUInt32(8, 0); // displayFlags - moderation state is not implemented yet
        stmt->setUInt8(9, 1);  // ClubFinderRequestType::Guild
        stmt->setBool(10, false);
        stmt->setUInt64(11, guild ? guild->GetLeaderGUID().GetCounter() : 0);
        stmt->setUInt64(12, settings.GetLastUpdatedTime());
        stmt->setUInt8(13, settings.GetAvailability());
        stmt->setUInt8(14, settings.GetClassRoles());
        stmt->setUInt8(15, settings.GetInterests());
        stmt->setUInt8(16, settings.GetLevel());
    }

    void BindApplicationStatement(CharacterDatabasePreparedStatement* stmt, uint32 postingId, MembershipRequest const& request)
    {
        stmt->setUInt32(0, postingId);
        stmt->setUInt64(1, request.GetPlayerGUID().GetCounter());
        stmt->setString(2, request.GetComment());
        stmt->setUInt64(3, request.GetSpecMask());
        stmt->setUInt8(4, request.GetStatus());
        stmt->setUInt64(5, request.GetUpdateTime() ? request.GetUpdateTime() : uint32(request.GetSubmitTime()));
        stmt->setUInt8(6, request.GetAvailability());
        stmt->setUInt8(7, request.GetClassRoles());
        stmt->setUInt8(8, request.GetInterests());
        stmt->setUInt64(9, uint64(request.GetSubmitTime()));
        stmt->setUInt32(10, request.GetItemLevel());
    }

    std::string HexDumpApplicationPacket(ByteBuffer const& buffer)
    {
        static char constexpr Hex[] = "0123456789ABCDEF";
        std::string out;
        out.reserve(buffer.size() * 3);
        for (size_t i = 0; i < buffer.size(); ++i)
        {
            uint8 byte = buffer.read<uint8>(i);
            if (i)
                out += ' ';
            out += Hex[(byte >> 4) & 0x0F];
            out += Hex[byte & 0x0F];
        }
        return out;
    }
}

MembershipRequest::MembershipRequest() : _availability(0), _classRoles(0), _interests(0), _time(time(nullptr))
{
    _updateTime = uint32(_time);
}

MembershipRequest::MembershipRequest(ObjectGuid const& playerGUID, ObjectGuid const& guildId, uint32 availability, uint32 classRoles, uint32 interests,
    std::string comment, time_t submitTime, uint64 specMask, uint32 itemLevel, uint8 status, uint32 updateTime) :
    _comment(std::move(comment)), _guildId(guildId), _playerGUID(playerGUID), _availability(availability), _classRoles(classRoles), _interests(interests),
    _specMask(specMask), _itemLevel(itemLevel), _status(status), _updateTime(updateTime ? updateTime : uint32(submitTime)), _time(submitTime)
{
}

ClubFinderMgr::ClubFinderMgr()
{
}

ClubFinderMgr::~ClubFinderMgr()
{
}

void ClubFinderMgr::LoadFromDB()
{
    _guildSettings.clear();
    _postingIdByGuild.clear();
    _guildByPostingId.clear();
    _membershipRequestsByGuild.clear();
    _membershipRequestsByPlayer.clear();
    _maxPostingId = 0;

    LoadGuildSettings();
    LoadMembershipRequests();
    CleanupExpiredApplications(true);
}

uint32 ClubFinderMgr::GetPostingId(ObjectGuid const& guildGuid) const
{
    auto itr = _postingIdByGuild.find(guildGuid);
    return itr != _postingIdByGuild.end() ? itr->second : 0;
}

ObjectGuid ClubFinderMgr::GetClubFinderGuid(Guild const* guild) const
{
    if (!guild)
        return ObjectGuid::Empty;

    uint32 postingId = GetPostingId(guild->GetGUID());
    if (!postingId)
        return ObjectGuid::Empty;

    // Use Haven's expansion-specific ClubFinder ObjectGuid format. The extra
    // bit-32 marker tried in step 38 came only from 12.1 captures and did not
    // resolve the 8.3.7 cache loop, so do not synthesize it here.
    return ObjectGuid::Create<HighGuid::ClubFinder>(1, postingId, guild->GetGUID().GetCounter());
}

ObjectGuid ClubFinderMgr::GetGuildGuidForPostingId(uint32 postingId) const
{
    auto itr = _guildByPostingId.find(postingId);
    return itr != _guildByPostingId.end() ? itr->second : ObjectGuid::Empty;
}

LFGuildSettings const* ClubFinderMgr::GetGuildSettingsByPostingId(uint32 postingId) const
{
    ObjectGuid guildGuid = GetGuildGuidForPostingId(postingId);
    if (guildGuid.IsEmpty())
        return nullptr;

    auto itr = _guildSettings.find(guildGuid);
    return itr != _guildSettings.end() ? &itr->second : nullptr;
}

uint32 ClubFinderMgr::EnsurePostingId(ObjectGuid const& guildGuid)
{
    if (uint32 postingId = GetPostingId(guildGuid))
        return postingId;

    uint32 postingId = ++_maxPostingId;
    _postingIdByGuild[guildGuid] = postingId;
    _guildByPostingId[postingId] = guildGuid;
    return postingId;
}

void ClubFinderMgr::CleanupExpiredApplications(bool force /*= false*/)
{
    time_t const now = time(nullptr);
    if (!force && now - _lastApplicationCleanup < time_t(ApplicationCleanupIntervalSeconds))
        return;

    _lastApplicationCleanup = now;

    std::vector<std::pair<ObjectGuid /*player*/, ObjectGuid /*guild*/>> purge;
    for (auto const& guildPair : _membershipRequestsByGuild)
        for (auto const& requestPair : guildPair.second)
            if (now >= requestPair.second.GetPurgeTime())
                purge.emplace_back(requestPair.first, guildPair.first);

    if (purge.empty())
        return;

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    for (auto const& entry : purge)
    {
        ObjectGuid const& playerId = entry.first;
        ObjectGuid const& guildId = entry.second;

        auto guildItr = _membershipRequestsByGuild.find(guildId);
        if (guildItr != _membershipRequestsByGuild.end())
        {
            guildItr->second.erase(playerId);
            if (guildItr->second.empty())
                _membershipRequestsByGuild.erase(guildItr);
        }

        auto playerItr = _membershipRequestsByPlayer.find(playerId);
        if (playerItr != _membershipRequestsByPlayer.end())
        {
            playerItr->second.erase(guildId);
            if (playerItr->second.empty())
                _membershipRequestsByPlayer.erase(playerItr);
        }

        if (uint32 postingId = GetPostingId(guildId))
        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CLUB_FINDER_APPLICATION);
            stmt->setUInt32(0, postingId);
            stmt->setUInt64(1, playerId.GetCounter());
            trans->Append(stmt);
        }
    }
    CharacterDatabase.CommitTransaction(trans);

    TC_LOG_INFO("guild", "ClubFinderMgr: purged %u application(s) past the %u-day history window.",
        uint32(purge.size()), ApplicationHistorySeconds / DAY);
}

void ClubFinderMgr::LoadGuildSettings()
{
    TC_LOG_INFO("server.loading", "Loading club finder postings...");
    //                                      0          1       2            3                 4                 5                     6                7                   8                 9                10           11
    QueryResult result = CharacterDatabase.Query("SELECT cfp.postingId, cfp.clubId, cfp.description, cfp.recruitingSpecs, cfp.recruitmentFlags, cfp.itemLevelRequirement, cfp.lastUpdatedTime, "
                                                 "cfp.legacyAvailability, cfp.legacyClassRoles, cfp.legacyInterests, cfp.legacyLevel, c.race "
                                                 "FROM club_finder_posting cfp "
                                                 "LEFT JOIN guild g ON g.guildid = cfp.clubId "
                                                 "LEFT JOIN characters c ON c.guid = g.leaderguid");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 club finder postings. Table `club_finder_posting` is empty.");
        return;
    }

    constexpr uint32 DungeonsFlag      = 1u << 1;
    constexpr uint32 RaidsFlag         = 1u << 2;
    constexpr uint32 PvpFlag           = 1u << 3;
    constexpr uint32 RolePlayingFlag   = 1u << 4;
    constexpr uint32 SocialFlag        = 1u << 5;
    constexpr uint32 MaxLevelOnlyFlag  = 1u << 13;
    constexpr uint32 FocusFlags = DungeonsFlag | RaidsFlag | PvpFlag | RolePlayingFlag | SocialFlag;

    uint32 count = 0;
    uint32 oldMSTime = getMSTime();
    do
    {
        Field* fields = result->Fetch();
        uint32 postingId = fields[0].GetUInt32();
        ObjectGuid guildId = ObjectGuid::Create<HighGuid::Guild>(fields[1].GetUInt64());
        std::string comment = fields[2].GetString();
        uint64 specMask = fields[3].GetUInt64();
        uint32 recruitmentFlags = fields[4].GetUInt32();
        uint32 minItemLevel = fields[5].GetUInt32();
        uint32 lastUpdatedTime = fields[6].GetUInt32();
        uint8 availability = fields[7].GetUInt8();
        uint8 classRoles = fields[8].GetUInt8();
        uint8 interests = fields[9].GetUInt8();
        uint8 level = fields[10].GetUInt8();

        bool listed = (recruitmentFlags & ClubFinderEnableListingFlag) != 0;
        bool rewriteRow = false;

        if (!availability)
        {
            availability = AVAILABILITY_ALWAYS;
            rewriteRow = true;
        }

        if (!classRoles)
        {
            classRoles = GUILDFINDER_ALL_ROLES;
            rewriteRow = true;
        }

        if (!interests)
        {
            if (recruitmentFlags & DungeonsFlag)    interests |= INTEREST_DUNGEONS;
            if (recruitmentFlags & RaidsFlag)       interests |= INTEREST_RAIDS;
            if (recruitmentFlags & PvpFlag)         interests |= INTEREST_PVP;
            if (recruitmentFlags & RolePlayingFlag) interests |= INTEREST_ROLE_PLAYING;
            if (recruitmentFlags & SocialFlag)      interests |= INTEREST_QUESTING;
            if (!interests)
                interests = ALL_INTERESTS;
            rewriteRow = true;
        }

        if (!level)
        {
            level = (recruitmentFlags & MaxLevelOnlyFlag) ? MAX_FINDER_LEVEL : ANY_FINDER_LEVEL;
            rewriteRow = true;
        }

        if (listed && !(recruitmentFlags & FocusFlags))
        {
            if (interests & INTEREST_DUNGEONS)     recruitmentFlags |= DungeonsFlag;
            if (interests & INTEREST_RAIDS)        recruitmentFlags |= RaidsFlag;
            if (interests & INTEREST_PVP)          recruitmentFlags |= PvpFlag;
            if (interests & INTEREST_ROLE_PLAYING) recruitmentFlags |= RolePlayingFlag;
            if (interests & INTEREST_QUESTING)     recruitmentFlags |= SocialFlag;
            rewriteRow = true;
        }

        if (listed && !lastUpdatedTime)
        {
            lastUpdatedTime = uint32(time(nullptr));
            rewriteRow = true;
        }

        TeamId guildTeam = TEAM_NEUTRAL;
        if (ChrRacesEntry const* raceEntry = sChrRacesStore.LookupEntry(fields[11].GetUInt8()))
            guildTeam = TeamId(raceEntry->Alliance);

        LFGuildSettings settings(listed, guildTeam, guildId, classRoles, availability, interests, level, comment,
            specMask, recruitmentFlags, minItemLevel, lastUpdatedTime);

        _guildSettings[guildId] = settings;
        _postingIdByGuild[guildId] = postingId;
        _guildByPostingId[postingId] = guildId;
        _maxPostingId = std::max(_maxPostingId, postingId);

        if (rewriteRow)
        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CLUB_FINDER_POSTING);
            BindPostingStatement(stmt, postingId, settings);
            CharacterDatabase.Execute(stmt);
        }

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u club finder postings in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ClubFinderMgr::LoadMembershipRequests()
{
    TC_LOG_INFO("server.loading", "Loading club finder applications...");
    //                                      0           1             2               3            4             5          6             7        8          9       10
    QueryResult result = CharacterDatabase.Query("SELECT cfp.clubId, cfa.playerGuid, cfa.availability, cfa.classRole, cfa.interests, cfa.comment, cfa.submitTime, cfa.specs, cfa.itemLevel, cfa.status, cfa.lastUpdatedTime "
                                                 "FROM club_finder_application cfa "
                                                 "INNER JOIN club_finder_posting cfp ON cfp.postingId = cfa.postingId");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 club finder applications. Table `club_finder_application` is empty.");
        return;
    }

    uint32 count = 0;
    uint32 oldMSTime = getMSTime();
    do
    {
        Field* fields = result->Fetch();
        ObjectGuid guildId = ObjectGuid::Create<HighGuid::Guild>(fields[0].GetUInt64());
        ObjectGuid playerId = ObjectGuid::Create<HighGuid::Player>(fields[1].GetUInt64());
        uint8 availability = fields[2].GetUInt8();
        uint8 classRoles = fields[3].GetUInt8();
        uint8 interests = fields[4].GetUInt8();
        std::string comment = fields[5].GetString();
        uint32 submitTime = fields[6].GetUInt32();
        uint64 specMask = fields[7].GetUInt64();
        uint32 itemLevel = fields[8].GetUInt32();
        uint8 status = fields[9].GetUInt8();
        uint32 updateTime = fields[10].GetUInt32();

        MembershipRequest request(playerId, guildId, availability, classRoles, interests, std::move(comment), time_t(submitTime),
            specMask, itemLevel, status ? status : uint8(1), updateTime);

        _membershipRequestsByGuild[guildId][playerId] = request;
        _membershipRequestsByPlayer[playerId][guildId] = request;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u club finder applications in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ClubFinderMgr::AddMembershipRequest(ObjectGuid const& guildGuid, MembershipRequest const& request)
{
    uint32 postingId = GetPostingId(guildGuid);
    if (!postingId)
    {
        TC_LOG_ERROR("guild", "ClubFinderMgr: cannot persist application for guild %llu without a posting id.",
            uint64(guildGuid.GetCounter()));
        return;
    }

    _membershipRequestsByGuild[guildGuid][request.GetPlayerGUID()] = request;
    _membershipRequestsByPlayer[request.GetPlayerGUID()][guildGuid] = request;

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CLUB_FINDER_APPLICATION);
    BindApplicationStatement(stmt, postingId, request);
    trans->Append(stmt);
    CharacterDatabase.CommitTransaction(trans);

    // Notify the applicant his submittion has been added
    if (Player* player = ObjectAccessor::FindPlayer(request.GetPlayerGUID()))
        SendMembershipRequestListUpdate(player);

    // Notify the guild master and officers the list changed
    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
        SendApplicantListUpdate(guild);
}

bool ClubFinderMgr::UpdateMembershipRequestStatus(ObjectGuid const& playerId, ObjectGuid const& guildId, uint8 status, uint32 updateTime)
{
    auto guildItr = _membershipRequestsByGuild.find(guildId);
    auto playerItr = _membershipRequestsByPlayer.find(playerId);
    if (guildItr == _membershipRequestsByGuild.end() || playerItr == _membershipRequestsByPlayer.end())
        return false;

    auto guildRequestItr = guildItr->second.find(playerId);
    auto playerRequestItr = playerItr->second.find(guildId);
    if (guildRequestItr == guildItr->second.end() || playerRequestItr == playerItr->second.end())
        return false;

    guildRequestItr->second.SetStatus(status, updateTime);
    playerRequestItr->second.SetStatus(status, updateTime);

    MembershipRequest const& request = guildRequestItr->second;
    uint32 postingId = GetPostingId(guildId);
    if (!postingId)
        return false;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CLUB_FINDER_APPLICATION);
    BindApplicationStatement(stmt, postingId, request);
    CharacterDatabase.Execute(stmt);

    if (Player* player = ObjectAccessor::FindPlayer(playerId))
        SendMembershipRequestListUpdate(player);
    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildId))
        SendApplicantListUpdate(guild);

    return true;
}

MembershipRequest const* ClubFinderMgr::GetMembershipRequest(ObjectGuid const& playerId, ObjectGuid const& guildId) const
{
    auto playerItr = _membershipRequestsByPlayer.find(playerId);
    if (playerItr == _membershipRequestsByPlayer.end())
        return nullptr;

    auto requestItr = playerItr->second.find(guildId);
    return requestItr != playerItr->second.end() ? &requestItr->second : nullptr;
}

void ClubFinderMgr::OnPlayerJoinedGuild(ObjectGuid const& playerId, ObjectGuid const& guildId)
{
    auto playerItr = _membershipRequestsByPlayer.find(playerId);
    if (playerItr == _membershipRequestsByPlayer.end())
        return;

    uint32 now = uint32(time(nullptr));
    std::vector<ObjectGuid> guildsToNotify;
    guildsToNotify.reserve(playerItr->second.size());

    for (auto& guildRequestPair : playerItr->second)
    {
        MembershipRequest& playerRequest = guildRequestPair.second;
        if (!playerRequest.IsActive())
            continue;

        uint8 newStatus = guildRequestPair.first == guildId
            ? uint8(WorldPackets::ClubFinder::RequestStatusJoined)
            : uint8(WorldPackets::ClubFinder::RequestStatusJoinedAnother);
        playerRequest.SetStatus(newStatus, now);

        auto guildItr = _membershipRequestsByGuild.find(guildRequestPair.first);
        if (guildItr != _membershipRequestsByGuild.end())
        {
            auto reqItr = guildItr->second.find(playerId);
            if (reqItr != guildItr->second.end())
                reqItr->second.SetStatus(newStatus, now);
        }

        if (uint32 postingId = GetPostingId(playerRequest.GetGuildGuid()))
        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CLUB_FINDER_APPLICATION);
            BindApplicationStatement(stmt, postingId, playerRequest);
            CharacterDatabase.Execute(stmt);
        }
        guildsToNotify.push_back(guildRequestPair.first);
    }

    for (ObjectGuid const& notifyGuild : guildsToNotify)
        if (Guild* guild = sGuildMgr->GetGuildByGuid(notifyGuild))
            SendApplicantListUpdate(guild);

    if (Player* player = ObjectAccessor::FindPlayer(playerId))
        SendMembershipRequestListUpdate(player);
}

void ClubFinderMgr::RemoveAllMembershipRequestsFromPlayer(ObjectGuid const& playerId)
{
    auto playerItr = _membershipRequestsByPlayer.find(playerId);
    if (playerItr == _membershipRequestsByPlayer.end())
        return;

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    for (auto& guildRequestPair : playerItr->second)
    {
        if (uint32 postingId = GetPostingId(guildRequestPair.first))
        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CLUB_FINDER_APPLICATION);
            stmt->setUInt32(0, postingId);
            stmt->setUInt64(1, playerId.GetCounter());
            trans->Append(stmt);
        }

        // Notify the guild master and officers the list changed
        if (Guild* guild = sGuildMgr->GetGuildByGuid(guildRequestPair.first))
            SendApplicantListUpdate(guild);

        auto guildItr = _membershipRequestsByGuild.find(guildRequestPair.first);
        if (guildItr == _membershipRequestsByGuild.end())
            continue;

        guildItr->second.erase(playerId);
        if (guildItr->second.empty())
            _membershipRequestsByGuild.erase(guildItr);
    }

    CharacterDatabase.CommitTransaction(trans);

    _membershipRequestsByPlayer.erase(playerItr);
}

void ClubFinderMgr::RemoveMembershipRequest(ObjectGuid const& playerId, ObjectGuid const& guildId)
{
    auto guildItr = _membershipRequestsByGuild.find(guildId);
    if (guildItr != _membershipRequestsByGuild.end())
    {
        guildItr->second.erase(playerId);
        if (guildItr->second.empty())
            _membershipRequestsByGuild.erase(guildItr);
    }

    auto playerItr = _membershipRequestsByPlayer.find(playerId);
    if (playerItr != _membershipRequestsByPlayer.end())
    {
        playerItr->second.erase(guildId);
        if (playerItr->second.empty())
            _membershipRequestsByPlayer.erase(playerItr);
    }

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    if (uint32 postingId = GetPostingId(guildId))
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CLUB_FINDER_APPLICATION);
        stmt->setUInt32(0, postingId);
        stmt->setUInt64(1, playerId.GetCounter());
        trans->Append(stmt);
    }

    CharacterDatabase.CommitTransaction(trans);

    // Notify the applicant his submittion has been removed
    if (Player* player = ObjectAccessor::FindPlayer(playerId))
        SendMembershipRequestListUpdate(player);

    // Notify the guild master and officers the list changed
    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildId))
        SendApplicantListUpdate(guild);
}

std::vector<MembershipRequest const*> ClubFinderMgr::GetAllMembershipRequestsForPlayer(ObjectGuid const& playerGuid)
{
    std::vector<MembershipRequest const*> resultSet;
    auto playerItr = _membershipRequestsByPlayer.find(playerGuid);
    if (playerItr == _membershipRequestsByPlayer.end())
        return resultSet;

    resultSet.reserve(playerItr->second.size());
    for (auto const& guildRequestPair : playerItr->second)
        resultSet.push_back(&guildRequestPair.second);

    return resultSet;
}

uint8 ClubFinderMgr::CountRequestsFromPlayer(ObjectGuid const& playerId)
{
    auto playerItr = _membershipRequestsByPlayer.find(playerId);
    if (playerItr == _membershipRequestsByPlayer.end())
        return 0;

    uint8 count = 0;
    for (auto const& request : playerItr->second)
        if (request.second.IsActive() && request.second.IsWithinApplicationWindow())
            ++count;
    return count;
}

std::vector<LFGuildSettings const*> ClubFinderMgr::GetGuildsMatchingSetting(LFGuildPlayer& settings, TeamId faction)
{
    std::vector<LFGuildSettings const*> resultSet;
    for (LFGuildStore::const_iterator itr = _guildSettings.begin(); itr != _guildSettings.end(); ++itr)
    {
        LFGuildSettings const& guildSettings = itr->second;

        if (!guildSettings.IsActiveListing())
            continue;

        if (guildSettings.GetTeam() != faction)
            continue;

        if (!(guildSettings.GetAvailability() & settings.GetAvailability()))
            continue;

        if (!(guildSettings.GetClassRoles() & settings.GetClassRoles()))
            continue;

        if (!(guildSettings.GetInterests() & settings.GetInterests()))
            continue;

        if (!(guildSettings.GetLevel() & settings.GetLevel()))
            continue;

        resultSet.push_back(&itr->second);
    }

    return resultSet;
}

bool ClubFinderMgr::HasRequest(ObjectGuid const& playerId, ObjectGuid const& guildId)
{
    auto guildItr = _membershipRequestsByGuild.find(guildId);
    if (guildItr == _membershipRequestsByGuild.end())
        return false;

    auto requestItr = guildItr->second.find(playerId);
    return requestItr != guildItr->second.end() && requestItr->second.BlocksReapply();
}

void ClubFinderMgr::SetGuildSettings(ObjectGuid const& guildGuid, LFGuildSettings const& settings)
{
    uint32 postingId = EnsurePostingId(guildGuid);
    _guildSettings[guildGuid] = settings;

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CLUB_FINDER_POSTING);
    BindPostingStatement(stmt, postingId, settings);
    trans->Append(stmt);
    CharacterDatabase.CommitTransaction(trans);
}

void ClubFinderMgr::DeleteGuild(ObjectGuid const& guildId)
{
    uint32 postingId = GetPostingId(guildId);
    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    auto guildItr = _membershipRequestsByGuild.find(guildId);
    if (guildItr != _membershipRequestsByGuild.end())
    {
        for (auto playerRequestPair : guildItr->second)
        {
            if (postingId)
            {
                CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CLUB_FINDER_APPLICATION);
                stmt->setUInt32(0, postingId);
                stmt->setUInt64(1, playerRequestPair.first.GetCounter());
                trans->Append(stmt);
            }

            auto playerItr = _membershipRequestsByPlayer.find(playerRequestPair.first);
            if (playerItr != _membershipRequestsByPlayer.end())
            {
                playerItr->second.erase(guildId);
                if (playerItr->second.empty())
                    _membershipRequestsByPlayer.erase(playerItr);
            }

            if (Player* player = ObjectAccessor::FindPlayer(playerRequestPair.first))
                SendMembershipRequestListUpdate(player);
        }
    }

    if (postingId)
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CLUB_FINDER_POSTING);
        stmt->setUInt32(0, postingId);
        trans->Append(stmt);
    }

    CharacterDatabase.CommitTransaction(trans);

    _membershipRequestsByGuild.erase(guildId);
    _guildSettings.erase(guildId);
    if (postingId)
    {
        _postingIdByGuild.erase(guildId);
        _guildByPostingId.erase(postingId);
    }

    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildId))
        SendApplicantListUpdate(guild);
}

void ClubFinderMgr::SendApplicantListUpdate(Guild* guild)
{
    if (!guild)
        return;

    // Keep the legacy LF_GUILD notification for old UI paths.
    WorldPackets::GuildFinder::LFGuildApplicantListChanged applicantListChanged;
    guild->BroadcastPacketToRank(applicantListChanged.Write(), GR_OFFICER);
    if (Player* player = ObjectAccessor::FindPlayer(guild->GetLeaderGUID()))
        player->SendDirectMessage(applicantListChanged.GetRawPacket());

    // The BFA Communities applicant view is driven by CLUB_FINDER updates.
    // Push the complete application-state snapshot so officers see joins/leaves
    // immediately rather than only after reopening the panel or relogging.
    WorldPackets::ClubFinder::ClubFinderUpdateApplications clubUpdate;
    clubUpdate.Context = 0x20;

    auto mgr = ClubFinderMgr::instance();
    auto requests = mgr->GetAllMembershipRequestsForGuild(guild->GetGUID());
    if (requests)
    {
        clubUpdate.Applications.reserve(requests->size());
        ObjectGuid finderGuid = mgr->GetClubFinderGuid(guild);
        for (auto const& requestPair : *requests)
        {
            MembershipRequest const& request = requestPair.second;
            WorldPackets::ClubFinder::ClubFinderApplicationUpdate application;
            application.ClubFinderGUID = finderGuid;
            application.PlayerGUID = request.GetPlayerGUID();
            application.Closed = request.IsClosed() ? 1u : 0u;
            application.LastUpdatedTime = request.GetUpdateTime() ? request.GetUpdateTime() : uint32(request.GetSubmitTime());
            application.RequestStatus = request.GetStatus();
            clubUpdate.Applications.push_back(std::move(application));
        }
    }

    WorldPacket const* clubPacket = clubUpdate.Write();
    TC_LOG_INFO("guild", "[CF-APP] manager UPDATE_APPLICATIONS guild=%u applications=%u context=0x%02X size=%u bytes=[%s]",
        guild->GetId(), uint32(clubUpdate.Applications.size()), uint32(clubUpdate.Context),
        uint32(clubPacket->size()), HexDumpApplicationPacket(*clubPacket).c_str());
    guild->BroadcastPacketToRank(clubPacket, GR_OFFICER);
    if (Player* leader = ObjectAccessor::FindConnectedPlayer(guild->GetLeaderGUID()))
        leader->SendDirectMessage(clubPacket);
}

void ClubFinderMgr::SendMembershipRequestListUpdate(Player* player)
{
    if (!player)
        return;

    player->SendDirectMessage(WorldPackets::GuildFinder::LFGuildApplicationsListChanged().Write());

    WorldPackets::ClubFinder::ResponseCharacterApplicationList response;
    response.Context = 0x60;
    for (MembershipRequest const* request : ClubFinderMgr::instance()->GetAllMembershipRequestsForPlayer(player->GetGUID()))
    {
        if (!request || request->IsExpiredPlayerApplication())
            continue;

        Guild* guild = sGuildMgr->GetGuildByGuid(request->GetGuildGuid());
        if (!guild)
            continue;

        // Keep historical Joined entries for recruiters, not as active
        // membership in a former member's own Finder application list.
        if (request->GetStatus() == WorldPackets::ClubFinder::RequestStatusJoined &&
            player->GetGuildId() != guild->GetId())
            continue;

        WorldPackets::ClubFinder::ClubFinderApplicationUpdate application;
        application.ClubFinderGUID = ClubFinderMgr::instance()->GetClubFinderGuid(guild);
        application.PlayerGUID = request->GetPlayerGUID();
        application.Closed = request->IsClosed() ? 1u : 0u;
        application.LastUpdatedTime = request->GetUpdateTime() ? request->GetUpdateTime() : uint32(request->GetSubmitTime());
        application.RequestStatus = request->GetStatus();
        response.Applications.push_back(std::move(application));
    }

    WorldPacket const* applicationPacket = response.Write();
    TC_LOG_INFO("guild", "[CF-APP] manager RESPONSE_CHARACTER_APPLICATION_LIST player=%s applications=%u context=0x%02X size=%u bytes=[%s]",
        player->GetName().c_str(), uint32(response.Applications.size()), uint32(response.Context),
        uint32(applicationPacket->size()), HexDumpApplicationPacket(*applicationPacket).c_str());
    player->SendDirectMessage(applicationPacket);
}

ClubFinderMgr* ClubFinderMgr::instance()
{
    static ClubFinderMgr instance;
    return &instance;
}
