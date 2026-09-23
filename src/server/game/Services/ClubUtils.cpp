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

#include "ClubUtils.h"
#include "ClubStreamHistoryMgr.h"
#include "WorldserverService.h"
#include "BattlenetRpcErrorCodes.h"
#include "CharacterCache.h"
#include "DatabaseEnv.h"
#include "IpAddress.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Language.h"
#include "Player.h"
#include "Log.h"
#include "ProtobufJSON.h"
#include "Realm.h"
#include "RealmList.h"
#include "RealmList.pb.h"
#include "World.h"
#include <algorithm>
#include <chrono>
#include <initializer_list>
#include <limits>
#include <zlib.h>

namespace Battlenet
{
namespace ClubUtils
{

    uint64 GetGuildClubId(Guild const* guild)
    {
        // Club ids are opaque to the client. Haven has no persisted BNet club id
        // for guilds, so use the persistent guild id everywhere in this bridge.
        return guild ? uint64(guild->GetId()) : 0;
    }


    uint64 CreateGuildClubMemberUniqueId(uint64 guidLow)
    {
        // Retail Club member ids are not plain character counters. They carry the
        // realm id in bits 48..59 and the character DB guid/counter in the low 48
        // bits. This matches both the supplied retail sniff and Trinity master.
        return (guidLow & 0x0000FFFFFFFFFFFFULL) | (uint64(realm.Id.Realm & 0x0FFF) << 48);
    }


    uint64 CreateGuildClubMemberUniqueId(ObjectGuid guid)
    {
        return CreateGuildClubMemberUniqueId(uint64(guid.GetCounter()));
    }


    void FillGuildClubType(::bgs::protocol::club::v1::UniqueClubType* type)
    {
        type->set_program(WOW_CLUB_PROGRAM);
        type->set_name("guild");
    }


    void FillClubMemberId(WorldSession const* session, Player const* player, ::bgs::protocol::club::v1::MemberId* id)
    {
        if (!player || !id)
            return;

        id->set_unique_id(CreateGuildClubMemberUniqueId(player->GetGUID()));
        if (session && session->GetBattlenetAccountId())
            id->mutable_account_id()->set_id(session->GetBattlenetAccountId());
    }


    WorldSession* FindOnlineGuildMemberSession(uint32 guildId, uint64 guidLow)
    {
        for (auto const& sessionPair : sWorld->GetAllSessions())
        {
            WorldSession* session = sessionPair.second;
            Player* player = session ? session->GetPlayer() : nullptr;
            if (!player || player->GetGuildId() != guildId)
                continue;

            if (uint64(player->GetGUID().GetCounter()) == guidLow)
                return session;
        }

        return nullptr;
    }


    WorldSession* FindOnlinePlayerSession(ObjectGuid guid)
    {
        for (auto const& sessionPair : sWorld->GetAllSessions())
        {
            WorldSession* session = sessionPair.second;
            Player* player = session ? session->GetPlayer() : nullptr;
            if (player && player->GetGUID() == guid)
                return session;
        }

        return nullptr;
    }


    uint32 ResolveBattlenetAccountId(uint32 gameAccountId)
    {
        if (!gameAccountId)
            return 0;

        LoginDatabasePreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_BNET_ACCOUNT_ID_BY_GAME_ACCOUNT);
        stmt->setUInt32(0, gameAccountId);
        if (PreparedQueryResult result = LoginDatabase.Query(stmt))
            return (*result)[0].GetUInt32();

        return 0;
    }


    void FillClubMemberIdForGuildMember(Guild const* guild, ObjectGuid memberGuid, ::bgs::protocol::club::v1::MemberId* id)
    {
        if (!guild || !id)
            return;

        id->set_unique_id(CreateGuildClubMemberUniqueId(memberGuid));

        // MemberIds must be stable for online and offline members. GetMembers
        // already includes the Battle.net account id for offline members; live
        // removal/role notifications must use the same composite id or the
        // client cannot match the cached roster row.
        uint32 battlenetAccountId = 0;
        if (WorldSession* session = FindOnlineGuildMemberSession(guild->GetId(), uint64(memberGuid.GetCounter())))
            battlenetAccountId = session->GetBattlenetAccountId();

        if (!battlenetAccountId)
            battlenetAccountId = ResolveBattlenetAccountId(sCharacterCache->GetCharacterAccountIdByGuid(memberGuid));

        if (battlenetAccountId)
            id->mutable_account_id()->set_id(battlenetAccountId);
    }


    void FillBaseGuildPrivileges(::bgs::protocol::club::v1::ClubPrivilegeSet* privilege)
    {
        privilege->set_can_set_own_member_attribute(true);
        privilege->set_can_set_own_voice_state(true);
        privilege->set_can_set_own_presence_level(true);
        privilege->set_can_set_own_whisper_level(true);
        privilege->set_can_set_own_member_note(true);
        privilege->set_can_use_voice(true);
        privilege->set_can_suggest_member(true);
        privilege->set_can_create_message(true);
        privilege->set_can_destroy_own_message(true);
        privilege->set_can_edit_own_message(true);
        privilege->set_can_mention_member(true);
        privilege->set_can_mention_role(true);
    }


    void FillModeratorGuildPrivileges(::bgs::protocol::club::v1::ClubPrivilegeSet* privilege)
    {
        FillBaseGuildPrivileges(privilege);
        privilege->set_can_set_attribute(true);
        privilege->set_can_set_broadcast(true);
        privilege->set_can_kick_member(true);
        privilege->set_can_set_other_member_attribute(true);
        privilege->set_can_set_other_member_note(true);
        privilege->set_can_voice_mute_member_for_all(true);
        privilege->set_can_get_invitation(true);
        privilege->set_can_send_invitation(true);
        privilege->set_can_revoke_own_invitation(true);
        privilege->set_can_revoke_other_invitation(true);
        privilege->set_can_get_suggestion(true);
        privilege->set_can_approve_member(true);
        privilege->set_can_get_ticket(true);
        privilege->set_can_create_ticket(true);
        privilege->set_can_destroy_ticket(true);
        privilege->set_can_set_stream_voice_level(true);
        privilege->set_can_destroy_other_message(true);
        privilege->set_can_mention_all(true);
        privilege->set_can_mention_here(true);
    }


    void FillLeaderGuildPrivileges(::bgs::protocol::club::v1::ClubPrivilegeSet* privilege)
    {
        FillModeratorGuildPrivileges(privilege);
        privilege->set_can_set_name(true);
        privilege->set_can_set_description(true);
        privilege->set_can_set_avatar(true);
        privilege->set_can_set_privacy_level(true);
        privilege->set_can_get_ban(true);
        privilege->set_can_add_ban(true);
        privilege->set_can_remove_ban(true);
        privilege->set_can_create_stream(true);
        privilege->set_can_destroy_stream(true);
        privilege->set_can_set_stream_position(true);
        privilege->set_can_set_stream_attribute(true);
        privilege->set_can_set_stream_name(true);
        privilege->set_can_set_stream_subject(true);
        privilege->set_can_set_stream_access(true);
    }


    void FillGuildRoleSet(::bgs::protocol::club::v1::ClubRoleSet* roleSet)
    {
        auto* owner = roleSet->add_role();
        owner->set_id(1);
        auto* ownerState = owner->mutable_state();
        ownerState->set_name("owner");
        for (uint32 role : { 1u, 2u, 3u, 4u })
            ownerState->add_assignable_role(role);
        ownerState->set_required(true);
        ownerState->set_unique(true);
        ownerState->set_relegation_role(2);
        for (uint32 role : { 2u, 3u, 4u })
            ownerState->add_kickable_role(role);
        for (uint32 role : { 2u, 3u })
            ownerState->add_removable_role(role);
        for (uint32 role : { 1u, 2u, 3u })
            ownerState->add_mentionable_role(role);
        FillLeaderGuildPrivileges(owner->mutable_privilege());
        owner->mutable_privilege()->set_can_destroy(true);
        owner->set_always_grant_stream_access(true);

        auto* leader = roleSet->add_role();
        leader->set_id(2);
        auto* leaderState = leader->mutable_state();
        leaderState->set_name("leader");
        leaderState->add_assignable_role(3);
        leaderState->add_assignable_role(4);
        leaderState->set_relegation_role(3);
        leaderState->add_kickable_role(3);
        leaderState->add_kickable_role(4);
        leaderState->add_removable_role(3);
        for (uint32 role : { 1u, 2u, 3u })
            leaderState->add_mentionable_role(role);
        FillLeaderGuildPrivileges(leader->mutable_privilege());
        leader->set_always_grant_stream_access(true);

        auto* moderator = roleSet->add_role();
        moderator->set_id(3);
        auto* moderatorState = moderator->mutable_state();
        moderatorState->set_name("moderator");
        moderatorState->set_relegation_role(4);
        moderatorState->add_kickable_role(4);
        for (uint32 role : { 1u, 2u, 3u })
            moderatorState->add_mentionable_role(role);
        FillModeratorGuildPrivileges(moderator->mutable_privilege());

        auto* member = roleSet->add_role();
        member->set_id(4);
        member->mutable_state()->set_name("member");
        for (uint32 role : { 1u, 2u, 3u })
            member->mutable_state()->add_mentionable_role(role);
        FillBaseGuildPrivileges(member->mutable_privilege());
        member->set_allow_in_club_slot(true);

        roleSet->add_default_role(4);
        roleSet->set_assignment_respects_relegation_chain(true);
        roleSet->set_subtype("group");
    }


    uint64 GetClubTimeNow()
    {
        return uint64(std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
    }


    void FillGuildBroadcast(Guild const* guild, ::bgs::protocol::club::v1::Broadcast* broadcast, WorldSession* creatorSession /*= nullptr*/, uint64 creationTime /*= 0*/)
    {
        if (!guild || !broadcast)
            return;

        broadcast->set_content(guild->GetMOTD());

        if (creatorSession && creatorSession->GetPlayer() && creatorSession->GetPlayer()->GetGuildId() == guild->GetId())
            FillClubMemberId(creatorSession, creatorSession->GetPlayer(), broadcast->mutable_creator()->mutable_id());
        else
            FillClubMemberIdForGuildMember(guild, guild->GetLeaderGUID(), broadcast->mutable_creator()->mutable_id());

        broadcast->set_creation_time(creationTime ? creationTime : uint64(guild->GetCreatedDate()) * 1000000ULL);
    }


    void FillGuildDescription(Guild const* guild, ::bgs::protocol::club::v1::ClubDescription* club)
    {
        club->set_id(GetGuildClubId(guild));
        FillGuildClubType(club->mutable_type());
        club->set_name(guild->GetName());
        club->set_description(guild->GetInfo());
        club->set_privacy_level(::bgs::protocol::club::v1::PRIVACY_LEVEL_OPEN);
        club->set_visibility_level(::bgs::protocol::club::v1::VISIBILITY_LEVEL_PRIVATE);
        club->set_member_count(guild->GetMembersCount());
        club->set_creation_time(uint64(guild->GetCreatedDate()) * 1000000ULL);
        FillGuildLeader(guild, club);
    }


    void FillGuildClub(Guild const* guild, ::bgs::protocol::club::v1::Club* club)
    {
        club->set_id(GetGuildClubId(guild));
        FillGuildClubType(club->mutable_type());
        club->set_name(guild->GetName());
        club->set_description(guild->GetInfo());
        FillGuildBroadcast(guild, club->mutable_broadcast());
        club->set_privacy_level(::bgs::protocol::club::v1::PRIVACY_LEVEL_OPEN);
        club->set_visibility_level(::bgs::protocol::club::v1::VISIBILITY_LEVEL_PRIVATE);
        club->set_member_count(guild->GetMembersCount());
        club->set_creation_time(uint64(guild->GetCreatedDate()) * 1000000ULL);
        FillGuildRoleSet(club->mutable_role_set());
        FillGuildLeader(guild, club);
    }


    uint32 GetGuildClubRole(Guild const* guild, uint8 guildRank)
    {
        if (guildRank == GR_GUILDMASTER)
            return 1;

        // Match Trinity's guild-to-Club role bridge: ranks with officer-chat
        // access are represented as Club moderators; all other ranks are
        // ordinary members. This respects custom rank ordering/rights.
        if (guild && guild->HasAnyRankRight(guildRank, GR_RIGHT_OFFCHATLISTEN | GR_RIGHT_OFFCHATSPEAK))
            return 3;

        return 4;
    }


    uint32 GetGuildClubRole(Player const* player, Guild const* guild)
    {
        return GetGuildClubRole(guild, player->GetGuildRank());
    }


    Guild* GetSessionGuild(WorldSession* session, uint64 requestedClubId /*= 0*/)
    {
        Player* player = session ? session->GetPlayer() : nullptr;
        if (!player || !player->GetGuildId())
            return nullptr;

        Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId());
        if (!guild || guild->GetMembersCount() == 0)
            return nullptr;

        if (requestedClubId && requestedClubId != GetGuildClubId(guild))
            return nullptr;

        return guild;
    }


    bool IsGuildStreamId(uint64 streamId)
    {
        return streamId == GUILD_GENERAL_STREAM_ID || streamId == GUILD_OFFICER_STREAM_ID;
    }


    void FillGuildStream(::bgs::protocol::club::v1::Stream* stream, Guild const* guild, uint64 streamId, char const* name, char const* globalStringTag, std::initializer_list<uint32> accessRoles)
    {
        stream->set_club_id(GetGuildClubId(guild));
        stream->set_id(streamId);

        auto* attribute = stream->add_attribute();
        attribute->set_name("global_strings_tag");
        attribute->mutable_value()->set_string_value(globalStringTag);

        stream->set_name(name);
        stream->set_subject("");
        for (uint32 role : accessRoles)
            stream->mutable_access()->add_role(role);

        // Supplied retail guild capture uses OPEN_MIC for both fixed guild streams.
        stream->set_voice_level(::bgs::protocol::club::v1::VOICE_LEVEL_OPEN_MIC);
        stream->set_creation_time(uint64(guild->GetCreatedDate()) * 1000000ULL);
    }


    void FillGuildStreamView(::bgs::protocol::club::v1::StreamView* view, Guild const* guild, ObjectGuid memberGuid, uint64 streamId)
    {
        view->set_club_id(GetGuildClubId(guild));
        view->set_stream_id(streamId);
        view->mutable_marker()->set_last_read_time(GetGuildStreamLastReadTime(guild, memberGuid, streamId));
        view->mutable_marker()->set_last_message_time(GetGuildStreamLastMessageTime(guild, streamId));
    }


    void FillStreamMessage(::bgs::protocol::club::v1::StreamMessage* message, std::string const& content, uint64 messageTime, ::bgs::protocol::club::v1::MemberId const& authorId)
    {
        message->mutable_id()->set_epoch(messageTime);
        message->mutable_id()->set_position(0);
        message->mutable_author()->mutable_id()->CopyFrom(authorId);

        auto* chain = message->add_content_chain();
        chain->set_content(content);
        chain->set_edit_time(messageTime);
    }
}
}
