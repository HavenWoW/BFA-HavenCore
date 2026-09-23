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
// ClubUtils - shared helpers of the Battle.net Club services for WoW guild
// clubs (Draconic layout: Services/ClubUtils). Club / member ids, the guild
// role model and privileges, club description / broadcast / leader, membership
// state and session lookups. Stream history state lives in
// Clubs/ClubStreamHistoryMgr. Moved unchanged from WorldserverService.cpp.
// -----------------------------------------------------------------------------

#ifndef HAVEN_CLUB_UTILS_H
#define HAVEN_CLUB_UTILS_H

#include "WorldserverService.h"
#include "Guild.h"
#include "Player.h"
#include "WorldSession.h"
#include <initializer_list>
#include <string>

namespace Battlenet
{
namespace ClubUtils
{
    constexpr uint32 WOW_CLUB_PROGRAM = 5730135;
    constexpr uint64 GUILD_GENERAL_STREAM_ID = 1;
    constexpr uint64 GUILD_OFFICER_STREAM_ID = 2;

    uint64 GetGuildClubId(Guild const* guild);
    uint64 CreateGuildClubMemberUniqueId(uint64 guidLow);
    uint64 CreateGuildClubMemberUniqueId(ObjectGuid guid);
    void FillGuildClubType(::bgs::protocol::club::v1::UniqueClubType* type);
    void FillClubMemberId(WorldSession const* session, Player const* player, ::bgs::protocol::club::v1::MemberId* id);
    WorldSession* FindOnlineGuildMemberSession(uint32 guildId, uint64 guidLow);
    WorldSession* FindOnlinePlayerSession(ObjectGuid guid);
    uint32 ResolveBattlenetAccountId(uint32 gameAccountId);
    void FillClubMemberIdForGuildMember(Guild const* guild, ObjectGuid memberGuid, ::bgs::protocol::club::v1::MemberId* id);
    void FillBaseGuildPrivileges(::bgs::protocol::club::v1::ClubPrivilegeSet* privilege);
    void FillModeratorGuildPrivileges(::bgs::protocol::club::v1::ClubPrivilegeSet* privilege);
    void FillLeaderGuildPrivileges(::bgs::protocol::club::v1::ClubPrivilegeSet* privilege);
    void FillGuildRoleSet(::bgs::protocol::club::v1::ClubRoleSet* roleSet);
    uint64 GetClubTimeNow();
    void FillGuildBroadcast(Guild const* guild, ::bgs::protocol::club::v1::Broadcast* broadcast, WorldSession* creatorSession = nullptr, uint64 creationTime = 0);
    void FillGuildDescription(Guild const* guild, ::bgs::protocol::club::v1::ClubDescription* club);
    void FillGuildClub(Guild const* guild, ::bgs::protocol::club::v1::Club* club);
    uint32 GetGuildClubRole(Guild const* guild, uint8 guildRank);
    uint32 GetGuildClubRole(Player const* player, Guild const* guild);
    Guild* GetSessionGuild(WorldSession* session, uint64 requestedClubId = 0);
    bool IsGuildStreamId(uint64 streamId);
    void FillGuildStream(::bgs::protocol::club::v1::Stream* stream, Guild const* guild, uint64 streamId, char const* name, char const* globalStringTag, std::initializer_list<uint32> accessRoles);
    void FillGuildStreamView(::bgs::protocol::club::v1::StreamView* view, Guild const* guild, ObjectGuid memberGuid, uint64 streamId);
    void FillStreamMessage(::bgs::protocol::club::v1::StreamMessage* message, std::string const& content, uint64 messageTime, ::bgs::protocol::club::v1::MemberId const& authorId);

    // --- templates (must stay in the header) ---


    template <class ClubType>
    void FillGuildLeader(Guild const* guild, ClubType* club)
    {
        if (!guild || !club)
            return;

        auto* leader = club->add_leader();
        leader->mutable_id()->set_unique_id(CreateGuildClubMemberUniqueId(guild->GetLeaderGUID()));

        if (WorldSession* leaderSession = FindOnlineGuildMemberSession(guild->GetId(), uint64(guild->GetLeaderGUID().GetCounter())))
            if (leaderSession->GetBattlenetAccountId())
                leader->mutable_id()->mutable_account_id()->set_id(leaderSession->GetBattlenetAccountId());
    }

    template <class Options>
    void FillGuildMembershipState(WorldSession* session, Options const* /*options*/, ::bgs::protocol::club::v1::ClubMembershipState* state)
    {
        state->mutable_mention_view()->set_last_read_time(0);
        state->mutable_mention_view()->set_last_message_time(0);

        Player* player = session ? session->GetPlayer() : nullptr;
        Guild* guild = GetSessionGuild(session);
        if (!player || !guild)
            return;

        auto* membership = state->add_description();

        // The BFA client sends a guild filter MemberId with unique_id present but
        // set to 0. That value is only a subscription-filter placeholder; it is
        // not the character's Club member id. Always return the authoritative
        // realm-qualified character id, matching retail/Trinity behavior.
        FillClubMemberId(session, player, membership->mutable_member_id());

        FillGuildDescription(guild, membership->mutable_club());
    }
}
}

#endif // HAVEN_CLUB_UTILS_H
