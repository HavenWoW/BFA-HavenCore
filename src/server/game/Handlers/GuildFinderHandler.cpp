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

#include "CharacterCache.h"
#include "WorldSession.h"
#include "Guild.h"
#include "ClubFinderMgr.h"
#include "GuildFinderPackets.h"
#include "GuildMgr.h"
#include "Log.h"
#include "Object.h"
#include "Player.h"
#include "SharedDefines.h"
#include "World.h"
#include "WorldPacket.h"

void WorldSession::HandleGuildFinderAddRecruit(WorldPackets::GuildFinder::LFGuildAddRecruit& lfGuildAddRecruit)
{
    if (sClubFinderMgr->CountRequestsFromPlayer(GetPlayer()->GetGUID()) >= 10)
        return;

    if (!lfGuildAddRecruit.GuildGUID.IsGuild())
        return;
    if (!(lfGuildAddRecruit.ClassRoles & GUILDFINDER_ALL_ROLES) || lfGuildAddRecruit.ClassRoles > GUILDFINDER_ALL_ROLES)
        return;
    if (!(lfGuildAddRecruit.Availability & AVAILABILITY_ALWAYS) || lfGuildAddRecruit.Availability > AVAILABILITY_ALWAYS)
        return;
    if (!(lfGuildAddRecruit.PlayStyle & ALL_INTERESTS) || lfGuildAddRecruit.PlayStyle > ALL_INTERESTS)
        return;

    MembershipRequest request = MembershipRequest(GetPlayer()->GetGUID(), lfGuildAddRecruit.GuildGUID, lfGuildAddRecruit.Availability,
        lfGuildAddRecruit.ClassRoles, lfGuildAddRecruit.PlayStyle, lfGuildAddRecruit.Comment, time(nullptr));
    sClubFinderMgr->AddMembershipRequest(lfGuildAddRecruit.GuildGUID, request);
}

void WorldSession::HandleGuildFinderBrowse(WorldPackets::GuildFinder::LFGuildBrowse& lfGuildBrowse)
{
    if (!(lfGuildBrowse.ClassRoles & GUILDFINDER_ALL_ROLES) || lfGuildBrowse.ClassRoles > GUILDFINDER_ALL_ROLES)
        return;
    if (!(lfGuildBrowse.Availability & AVAILABILITY_ALWAYS) || lfGuildBrowse.Availability > AVAILABILITY_ALWAYS)
        return;
    if (!(lfGuildBrowse.PlayStyle & ALL_INTERESTS) || lfGuildBrowse.PlayStyle > ALL_INTERESTS)
        return;
    if (lfGuildBrowse.CharacterLevel > int32(sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)) || lfGuildBrowse.CharacterLevel < 1)
        return;

    Player* player = GetPlayer();

    LFGuildPlayer settings(player->GetGUID(), lfGuildBrowse.ClassRoles, lfGuildBrowse.Availability, lfGuildBrowse.PlayStyle, ANY_FINDER_LEVEL);
    std::vector<LFGuildSettings const*> guildList = sClubFinderMgr->GetGuildsMatchingSetting(settings, player->GetTeamId());

    WorldPackets::GuildFinder::LFGuildBrowseResult lfGuildBrowseResult;
    lfGuildBrowseResult.Post.resize(guildList.size());

    for (std::size_t i = 0; i < guildList.size(); ++i)
    {
        LFGuildSettings const* guildSettings = guildList[i];
        WorldPackets::GuildFinder::LFGuildBrowseData& guildData = lfGuildBrowseResult.Post[i];
        Guild* guild = ASSERT_NOTNULL(sGuildMgr->GetGuildByGuid(guildSettings->GetGUID()));

        guildData.GuildName = guild->GetName();
        guildData.GuildGUID = guild->GetGUID();
        guildData.GuildVirtualRealm = GetVirtualRealmAddress();
        guildData.GuildMembers = guild->GetMembersCount();
        guildData.GuildAchievementPoints = guild->GetAchievementMgr().GetAchievementPoints();
        guildData.PlayStyle = guildSettings->GetInterests();
        guildData.Availability = guildSettings->GetAvailability();
        guildData.ClassRoles = guildSettings->GetClassRoles();
        guildData.LevelRange = guildSettings->GetLevel();
        guildData.EmblemStyle = guild->GetEmblemInfo().GetStyle();
        guildData.EmblemColor = guild->GetEmblemInfo().GetColor();
        guildData.BorderStyle = guild->GetEmblemInfo().GetBorderStyle();
        guildData.BorderColor = guild->GetEmblemInfo().GetBorderColor();
        guildData.Background = guild->GetEmblemInfo().GetBackgroundColor();
        guildData.Comment = guildSettings->GetComment();
        guildData.Cached = 0;
        guildData.MembershipRequested = sClubFinderMgr->HasRequest(player->GetGUID(), guild->GetGUID());
    }

    player->SendDirectMessage(lfGuildBrowseResult.Write());
}

void WorldSession::HandleGuildFinderDeclineRecruit(WorldPackets::GuildFinder::LFGuildDeclineRecruit& lfGuildDeclineRecruit)
{
    if (!GetPlayer()->GetGuild())
        return;

    if (!lfGuildDeclineRecruit.RecruitGUID.IsPlayer())
        return;

    sClubFinderMgr->RemoveMembershipRequest(lfGuildDeclineRecruit.RecruitGUID, GetPlayer()->GetGuild()->GetGUID());
}

void WorldSession::HandleGuildFinderGetApplications(WorldPackets::GuildFinder::LFGuildGetApplications& /*lfGuildGetApplications*/)
{
    std::vector<MembershipRequest const*> applicatedGuilds = sClubFinderMgr->GetAllMembershipRequestsForPlayer(GetPlayer()->GetGUID());
    WorldPackets::GuildFinder::LFGuildApplications lfGuildApplications;
    lfGuildApplications.Application.resize(applicatedGuilds.size());
    lfGuildApplications.NumRemaining = 10 - sClubFinderMgr->CountRequestsFromPlayer(GetPlayer()->GetGUID());

    for (std::size_t i = 0; i < applicatedGuilds.size(); ++i)
    {
        MembershipRequest const* application = applicatedGuilds[i];
        WorldPackets::GuildFinder::LFGuildApplicationData& applicationData = lfGuildApplications.Application[i];

        Guild* guild = ASSERT_NOTNULL(sGuildMgr->GetGuildByGuid(application->GetGuildGuid()));
        LFGuildSettings const& guildSettings = sClubFinderMgr->GetGuildSettings(application->GetGuildGuid());

        applicationData.GuildGUID = application->GetGuildGuid();
        applicationData.GuildVirtualRealm = GetVirtualRealmAddress();
        applicationData.GuildName = guild->GetName();
        applicationData.ClassRoles = guildSettings.GetClassRoles();
        applicationData.PlayStyle = guildSettings.GetInterests();
        applicationData.Availability = guildSettings.GetAvailability();
        applicationData.SecondsSinceCreated = time(nullptr) - application->GetSubmitTime();
        applicationData.SecondsUntilExpiration = application->GetExpiryTime() - time(nullptr);
        applicationData.Comment = application->GetComment();
    }

    GetPlayer()->SendDirectMessage(lfGuildApplications.Write());
}

void WorldSession::HandleGuildFinderGetGuildPost(WorldPackets::GuildFinder::LFGuildGetGuildPost& /*lfGuildGetGuildPost*/)
{
    Player* player = GetPlayer();

    Guild* guild = player->GetGuild();
    if (!guild) // Player must be in guild
        return;

    WorldPackets::GuildFinder::LFGuildPost lfGuildPost;
    if (guild->GetLeaderGUID() == player->GetGUID())
    {
        LFGuildSettings const& settings = sClubFinderMgr->GetGuildSettings(guild->GetGUID());
        lfGuildPost.Post.emplace();
        lfGuildPost.Post->Active = settings.IsListed();
        lfGuildPost.Post->PlayStyle = settings.GetInterests();
        lfGuildPost.Post->Availability = settings.GetAvailability();
        lfGuildPost.Post->ClassRoles = settings.GetClassRoles();
        lfGuildPost.Post->LevelRange = settings.GetLevel();
        lfGuildPost.Post->Comment = settings.GetComment();
    }

    player->SendDirectMessage(lfGuildPost.Write());
}

// Lists all recruits for a guild - Misses times
void WorldSession::HandleGuildFinderGetRecruits(WorldPackets::GuildFinder::LFGuildGetRecruits& /*lfGuildGetRecruits*/)
{
    Player* player = GetPlayer();
    Guild* guild = player->GetGuild();
    if (!guild)
        return;

    time_t now = time(nullptr);
    WorldPackets::GuildFinder::LFGuildRecruits lfGuildRecruits;
    lfGuildRecruits.UpdateTime = now;
    if (std::unordered_map<ObjectGuid, MembershipRequest> const* recruitsList = sClubFinderMgr->GetAllMembershipRequestsForGuild(guild->GetGUID()))
    {
        lfGuildRecruits.Recruits.resize(recruitsList->size());
        std::size_t i = 0;
        for (auto const& recruitRequestPair : *recruitsList)
        {
            WorldPackets::GuildFinder::LFGuildRecruitData& recruitData = lfGuildRecruits.Recruits[i++];
            recruitData.RecruitGUID = recruitRequestPair.first;
            recruitData.RecruitVirtualRealm = GetVirtualRealmAddress();
            recruitData.Comment = recruitRequestPair.second.GetComment();
            recruitData.ClassRoles = recruitRequestPair.second.GetClassRoles();
            recruitData.PlayStyle = recruitRequestPair.second.GetInterests();
            recruitData.Availability = recruitRequestPair.second.GetAvailability();
            recruitData.SecondsSinceCreated = now - recruitRequestPair.second.GetSubmitTime();
            recruitData.SecondsUntilExpiration = recruitRequestPair.second.GetExpiryTime() - now;
            if (CharacterCacheEntry const* charInfo = sCharacterCache->GetCharacterCacheByGuid(recruitRequestPair.first))
            {
                recruitData.Name = charInfo->Name;
                recruitData.CharacterClass = charInfo->Class;
                recruitData.CharacterGender = charInfo->Sex;
                recruitData.CharacterLevel = charInfo->Level;
            }
        }
    }

    player->SendDirectMessage(lfGuildRecruits.Write());
}

void WorldSession::HandleGuildFinderRemoveRecruit(WorldPackets::GuildFinder::LFGuildRemoveRecruit& lfGuildRemoveRecruit)
{
    if (!lfGuildRemoveRecruit.GuildGUID.IsGuild())
        return;

    sClubFinderMgr->RemoveMembershipRequest(GetPlayer()->GetGUID(), lfGuildRemoveRecruit.GuildGUID);
}

// Sent any time a guild master sets an option in the interface and when listing / unlisting his guild
void WorldSession::HandleGuildFinderSetGuildPost(WorldPackets::GuildFinder::LFGuildSetGuildPost& lfGuildSetGuildPost)
{
    // Level sent is zero if untouched, force to any (from interface). Idk why
    if (!lfGuildSetGuildPost.LevelRange)
        lfGuildSetGuildPost.LevelRange = ANY_FINDER_LEVEL;

    if (!(lfGuildSetGuildPost.ClassRoles & GUILDFINDER_ALL_ROLES) || lfGuildSetGuildPost.ClassRoles > GUILDFINDER_ALL_ROLES)
        return;
    if (!(lfGuildSetGuildPost.Availability & AVAILABILITY_ALWAYS) || lfGuildSetGuildPost.Availability > AVAILABILITY_ALWAYS)
        return;
    if (!(lfGuildSetGuildPost.PlayStyle & ALL_INTERESTS) || lfGuildSetGuildPost.PlayStyle > ALL_INTERESTS)
        return;
    if (!(lfGuildSetGuildPost.LevelRange & ALL_GUILDFINDER_LEVELS) || lfGuildSetGuildPost.LevelRange > ALL_GUILDFINDER_LEVELS)
        return;

    Player* player = GetPlayer();

    if (!player->GetGuildId()) // Player must be in guild
        return;

    Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId());
    if (!guild)
        return;

    // Player must be guild master
    if (guild->GetLeaderGUID() != player->GetGUID())
        return;

    // BFA still has the legacy LF_GUILD post opcode alongside the 8.3
    // CLUB_FINDER API. Keep both paths writing the same persistent state.
    // The old seven-field constructor left recruitmentFlags/lastUpdatedTime at
    // zero, which made IsActiveListing() reject an otherwise listed guild and
    // left the Communities recruitment dialog with no posting age to display.
    constexpr uint32 DungeonsFlag      = 1u << 1;
    constexpr uint32 RaidsFlag         = 1u << 2;
    constexpr uint32 PvpFlag           = 1u << 3;
    constexpr uint32 RolePlayingFlag   = 1u << 4;
    constexpr uint32 SocialFlag        = 1u << 5;
    constexpr uint32 EnableListingFlag = 1u << 12;
    constexpr uint32 MaxLevelOnlyFlag  = 1u << 13;
    constexpr uint32 LegacyFinderFlags = DungeonsFlag | RaidsFlag | PvpFlag | RolePlayingFlag | SocialFlag |
        EnableListingFlag | MaxLevelOnlyFlag;

    LFGuildSettings const& previousSettings = sClubFinderMgr->GetGuildSettings(guild->GetGUID());
    uint32 recruitmentFlags = previousSettings.GetRecruitmentFlags() & ~LegacyFinderFlags;
    if (lfGuildSetGuildPost.PlayStyle & INTEREST_DUNGEONS)     recruitmentFlags |= DungeonsFlag;
    if (lfGuildSetGuildPost.PlayStyle & INTEREST_RAIDS)        recruitmentFlags |= RaidsFlag;
    if (lfGuildSetGuildPost.PlayStyle & INTEREST_PVP)          recruitmentFlags |= PvpFlag;
    if (lfGuildSetGuildPost.PlayStyle & INTEREST_ROLE_PLAYING) recruitmentFlags |= RolePlayingFlag;
    if (lfGuildSetGuildPost.PlayStyle & INTEREST_QUESTING)     recruitmentFlags |= SocialFlag;
    if (lfGuildSetGuildPost.Active)                             recruitmentFlags |= EnableListingFlag;
    if (lfGuildSetGuildPost.LevelRange == MAX_FINDER_LEVEL)    recruitmentFlags |= MaxLevelOnlyFlag;

    uint64 specMask = previousSettings.GetSpecMask();
    uint32 minItemLevel = previousSettings.GetMinItemLevel();
    uint32 now = uint32(time(nullptr));

    LFGuildSettings settings(lfGuildSetGuildPost.Active, player->GetTeamId(), guild->GetGUID(), lfGuildSetGuildPost.ClassRoles,
        lfGuildSetGuildPost.Availability, lfGuildSetGuildPost.PlayStyle, lfGuildSetGuildPost.LevelRange, lfGuildSetGuildPost.Comment,
        specMask, recruitmentFlags, minItemLevel, now);
    sClubFinderMgr->SetGuildSettings(guild->GetGUID(), settings);
}

void WorldSession::HandleClubFinderPost(WorldPackets::GuildFinder::ClubFinderPost& packet)
{
    WorldPackets::GuildFinder::ClubFinderResponsePostRecruitmentMessage response;

    Player* player = GetPlayer();
    Guild* guild = player ? player->GetGuild() : nullptr;
    if (!player || !guild || packet.ClubID != guild->GetId() || guild->GetLeaderGUID() != player->GetGUID())
    {
        // Failure response layout has not been sniffed. An empty ClubFinderGUID
        // with zero flags is safer than the old invented int32 result payload.
        response.ClubFinderGUID = ObjectGuid::Empty;
        response.Flags = 0;
        SendPacket(response.Write());
        return;
    }

    constexpr uint32 DungeonsFlag      = 1u << 1;
    constexpr uint32 RaidsFlag         = 1u << 2;
    constexpr uint32 PvpFlag           = 1u << 3;
    constexpr uint32 RolePlayingFlag   = 1u << 4;
    constexpr uint32 SocialFlag        = 1u << 5;
    constexpr uint32 EnableListingFlag = 1u << 12;
    constexpr uint32 MaxLevelOnlyFlag  = 1u << 13;

    constexpr uint64 TankSpecMask   = 0x0000000880002910ULL;
    constexpr uint64 HealerSpecMask = 0x0000000208181008ULL;
    constexpr uint64 DamageSpecMask = 0x0000000577E7C6E7ULL;

    uint8 interests = 0;
    if (packet.RecruitmentFlags & DungeonsFlag)    interests |= INTEREST_DUNGEONS;
    if (packet.RecruitmentFlags & RaidsFlag)       interests |= INTEREST_RAIDS;
    if (packet.RecruitmentFlags & PvpFlag)         interests |= INTEREST_PVP;
    if (packet.RecruitmentFlags & RolePlayingFlag) interests |= INTEREST_ROLE_PLAYING;
    if (packet.RecruitmentFlags & SocialFlag)      interests |= INTEREST_QUESTING;
    if (!interests)
        interests = ALL_INTERESTS;

    uint8 roles = 0;
    if (!packet.SpecMask)
        roles = GUILDFINDER_ALL_ROLES;
    else
    {
        if (packet.SpecMask & TankSpecMask)   roles |= GUILDFINDER_ROLE_TANK;
        if (packet.SpecMask & HealerSpecMask) roles |= GUILDFINDER_ROLE_HEALER;
        if (packet.SpecMask & DamageSpecMask) roles |= GUILDFINDER_ROLE_DPS;

        // Never persist an active posting with no legacy role bits. Finder's
        // matching code treats ClassRoles=0 as matching nobody. This also makes
        // the bridge tolerant of specialization bits unknown to this BFA table.
        if (!roles)
            roles = GUILDFINDER_ALL_ROLES;
    }

    bool listed = (packet.RecruitmentFlags & EnableListingFlag) != 0;
    uint8 level = (packet.RecruitmentFlags & MaxLevelOnlyFlag) ? MAX_FINDER_LEVEL : ANY_FINDER_LEVEL;
    uint32 now = uint32(time(nullptr));
    LFGuildSettings const& previousSettings = sClubFinderMgr->GetGuildSettings(guild->GetGUID());
    bool const updatingExistingPosting = previousSettings.GetLastUpdatedTime() != 0;

    LFGuildSettings settings(listed, player->GetTeamId(), guild->GetGUID(), roles, AVAILABILITY_ALWAYS,
        interests, level, packet.Description, packet.SpecMask, packet.RecruitmentFlags, packet.MinItemLevel, now);
    sClubFinderMgr->SetGuildSettings(guild->GetGUID(), settings);

    // Use the canonical ClubFinder GUID builder shared by all posting/application paths.
    response.ClubFinderGUID = sClubFinderMgr->GetClubFinderGuid(guild);
    // Captures show 0x04 for the first post and 0x24 for an update/repost.
    response.Flags = updatingExistingPosting ? 0x24 : 0x04;

    TC_LOG_DEBUG("guild", "Stored BFA Club Finder post for guild %u (flags=0x%08X, minIlvl=%u, unknown32=%u, header5=%u, finder=%s)",
        guild->GetId(), packet.RecruitmentFlags, packet.MinItemLevel, packet.Unknown32, uint32(packet.Unknown5), response.ClubFinderGUID.ToString().c_str());

    SendPacket(response.Write());
}
