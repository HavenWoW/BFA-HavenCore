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

#include "ClubService.h"
#include "ClubStreamHistoryMgr.h"
#include "ClubUtils.h"
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

using namespace Battlenet::ClubUtils;

namespace
{
void SendGuildClubSubscriptionSnapshot(WorldSession* session, Guild const* guild)
{
    if (!session || !guild)
        return;

    Player* player = session->GetPlayer();
    if (!player || player->GetGuildId() != guild->GetId())
        return;

    ::bgs::protocol::club::v1::SubscribeNotification notification;
    FillClubMemberId(session, player, notification.mutable_agent_id());
    notification.set_club_id(GetGuildClubId(guild));
    FillGuildClub(guild, notification.mutable_club());
    notification.mutable_settings();

    auto* member = notification.mutable_member();
    member->mutable_id()->CopyFrom(notification.agent_id());
    member->add_role(GetGuildClubRole(player, guild));
    member->set_presence_level(::bgs::protocol::club::v1::PRESENCE_LEVEL_RICH);
    member->set_moderator_mute(false);
    member->set_whisper_level(::bgs::protocol::club::v1::WHISPER_LEVEL_OPEN);
    member->set_note("");
    member->set_active(true);

    Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(session);
    listener.OnSubscribe(&notification, true, true);

    ::bgs::protocol::club::v1::SubscriberStateChangedNotification stateChanged;
    stateChanged.set_club_id(GetGuildClubId(guild));
    auto* assignment = stateChanged.add_assignment();
    assignment->mutable_member_id()->CopyFrom(notification.agent_id());
    assignment->set_active(true);
    listener.OnSubscriberStateChanged(&stateChanged, true, true);
}
}

void Battlenet::NotifyGuildClubMemberAdded(Guild const* guild, ObjectGuid memberGuid, uint8 guildRank)
{
    if (!guild)
        return;

    WorldSession* memberSession = FindOnlineGuildMemberSession(guild->GetId(), uint64(memberGuid.GetCounter()));

    // A newly joined member must not inherit unread state from messages sent
    // before they joined the guild. Establish the two fixed-stream baselines now.
    uint64 const joinViewTime = GetClubTimeNow();
    StoreGuildStreamViewTime(guild, memberGuid, GUILD_GENERAL_STREAM_ID, joinViewTime);
    StoreGuildStreamViewTime(guild, memberGuid, GUILD_OFFICER_STREAM_ID, joinViewTime);

    ::bgs::protocol::club::v1::MemberId memberId;
    FillClubMemberIdForGuildMember(guild, memberGuid, &memberId);

    // Retail sends ClubAdded to the character that just gained membership.
    // The client then subscribes to the new club and materializes its streams/roster.
    if (memberSession)
    {
        uint32 const guildId = guild->GetId();

        // BFA processes the WoW guild state and Battle.net Club membership through
        // separate client-side systems. Give the GuildGUID/rank/roster bootstrap
        // time to settle before announcing the new Club membership.
        //
        // This mirrors the delay required by the guild-leave path.
        if (Player* joinedPlayer = memberSession->GetPlayer())
        {
            joinedPlayer->m_Events.AddEventAtOffset([guildId, memberGuid, memberId]()
            {
                WorldSession* delayedSession = FindOnlinePlayerSession(memberGuid);
                if (!delayedSession)
                    return;

                Player* player = delayedSession->GetPlayer();
                if (!player || player->GetGuildId() != guildId)
                    return;

                Guild* delayedGuild = sGuildMgr->GetGuildById(guildId);
                if (!delayedGuild)
                    return;

                ::bgs::protocol::club::v1::membership::ClubAddedNotification clubAdded;
                clubAdded.mutable_agent_id()->CopyFrom(memberId);

                auto* membership = clubAdded.mutable_membership();
                membership->mutable_member_id()->CopyFrom(memberId);
                FillGuildDescription(delayedGuild, membership->mutable_club());

                Battlenet::WorldserverService<
                    ::bgs::protocol::club::v1::membership::ClubMembershipListener>
                    membershipListener(delayedSession);

                membershipListener.OnClubAdded(&clubAdded, true, true);
            }, Milliseconds(250));
        }
        // Do NOT push a subscription snapshot here. Retail sends ClubAdded only;
        // the client then runs Subscribe -> (one) OnSubscribe -> GetMembers ->
        // GetStreams -> SubscribeStream for the guild/officer streams (retail
        // sniff 2026-09-20 07:51:08). An unsolicited OnSubscribe before the
        // client's own Subscribe made the 8.3.7 client stop after GetMembers, so
        // it never subscribed to the chat streams and refused /g
        // (ERR_GUILD_PERMISSIONS) until the guild window was opened.
    }


    // Existing online guild members maintain a live member cache. Update that
    // cache immediately rather than waiting for the next GetMembers/relog.
    ::bgs::protocol::club::v1::MemberAddedNotification memberAdded;
    memberAdded.set_club_id(GetGuildClubId(guild));
    auto* member = memberAdded.add_member();
    member->mutable_id()->CopyFrom(memberId);
    member->add_role(GetGuildClubRole(guild, guildRank));
    member->set_presence_level(::bgs::protocol::club::v1::PRESENCE_LEVEL_RICH);
    member->set_moderator_mute(false);
    member->set_whisper_level(::bgs::protocol::club::v1::WHISPER_LEVEL_OPEN);
    member->set_note("");
    member->set_active(memberSession != nullptr);

    for (auto const& sessionPair : sWorld->GetAllSessions())
    {
        WorldSession* receiverSession = sessionPair.second;
        Player* receiver = receiverSession ? receiverSession->GetPlayer() : nullptr;
        if (!receiver || receiver->GetGuildId() != guild->GetId() || receiver->GetGUID() == memberGuid)
            continue;

        Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(receiverSession);
        listener.OnMemberAdded(&memberAdded, true, true);
    }
}

void Battlenet::NotifyGuildClubMemberRoleChanged(Guild const* guild, ObjectGuid memberGuid, uint8 guildRank)
{
    if (!guild)
        return;

    ::bgs::protocol::club::v1::MemberId memberId;
    FillClubMemberIdForGuildMember(guild, memberGuid, &memberId);

    ::bgs::protocol::club::v1::MemberRoleChangedNotification roleChanged;
    roleChanged.set_club_id(GetGuildClubId(guild));

    auto* assignment = roleChanged.add_assignment();
    assignment->mutable_member_id()->CopyFrom(memberId);
    assignment->add_role(GetGuildClubRole(guild, guildRank));

    // Send to every online guild member, including the changed member. The
    // changed member's own Communities UI derives Recruitment/Settings/Invite
    // availability from this role assignment and otherwise stays stale.
    for (auto const& sessionPair : sWorld->GetAllSessions())
    {
        WorldSession* receiverSession = sessionPair.second;
        Player* receiver = receiverSession ? receiverSession->GetPlayer() : nullptr;
        if (!receiver || receiver->GetGuildId() != guild->GetId())
            continue;

        Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(receiverSession);
        listener.OnMemberRoleChanged(&roleChanged, true, true);
    }
}

void Battlenet::NotifyGuildClubNameChanged(Guild const* guild)
{
    if (!guild)
        return;

    ::bgs::protocol::club::v1::StateChangedNotification stateChanged;
    stateChanged.set_club_id(GetGuildClubId(guild));
    auto* assignment = stateChanged.mutable_assignment();
    assignment->set_club_id(GetGuildClubId(guild));
    assignment->set_name(guild->GetName());

    for (auto const& sessionPair : sWorld->GetAllSessions())
    {
        WorldSession* receiverSession = sessionPair.second;
        Player* receiver = receiverSession ? receiverSession->GetPlayer() : nullptr;
        if (!receiver || receiver->GetGuildId() != guild->GetId())
            continue;

        Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(receiverSession);
        listener.OnStateChanged(&stateChanged, true, true);
    }
}

void Battlenet::NotifyGuildClubDescriptionChanged(Guild const* guild)
{
    if (!guild)
        return;

    ::bgs::protocol::club::v1::StateChangedNotification stateChanged;
    stateChanged.set_club_id(GetGuildClubId(guild));
    auto* assignment = stateChanged.mutable_assignment();
    assignment->set_club_id(GetGuildClubId(guild));
    assignment->set_description(guild->GetInfo());

    for (auto const& sessionPair : sWorld->GetAllSessions())
    {
        WorldSession* receiverSession = sessionPair.second;
        Player* receiver = receiverSession ? receiverSession->GetPlayer() : nullptr;
        if (!receiver || receiver->GetGuildId() != guild->GetId())
            continue;

        Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(receiverSession);
        listener.OnStateChanged(&stateChanged, true, true);
    }
}

void Battlenet::NotifyGuildClubBroadcastChanged(Guild const* guild, WorldSession* sourceSession)
{
    if (!guild)
        return;

    ::bgs::protocol::club::v1::StateChangedNotification stateChanged;
    stateChanged.set_club_id(GetGuildClubId(guild));
    auto* assignment = stateChanged.mutable_assignment();
    assignment->set_club_id(GetGuildClubId(guild));
    FillGuildBroadcast(guild, assignment->mutable_broadcast(), sourceSession, GetClubTimeNow());

    for (auto const& sessionPair : sWorld->GetAllSessions())
    {
        WorldSession* receiverSession = sessionPair.second;
        Player* receiver = receiverSession ? receiverSession->GetPlayer() : nullptr;
        if (!receiver || receiver->GetGuildId() != guild->GetId())
            continue;

        Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(receiverSession);
        listener.OnStateChanged(&stateChanged, true, true);
    }
}

void Battlenet::NotifyGuildClubMemberRemoved(Guild const* guild, ObjectGuid memberGuid, bool isKicked, bool isDisbanding)
{
    if (!guild)
        return;

    // This function is deliberately called after Guild::DeleteMember has
    // cleared Player::GetGuildId(). Find the online character by GUID instead
    // of requiring it to still report the old guild id.
    WorldSession* memberSession = FindOnlinePlayerSession(memberGuid);

    ::bgs::protocol::club::v1::MemberId memberId;
    FillClubMemberIdForGuildMember(guild, memberGuid, &memberId);
    auto reason = isDisbanding
        ? ::bgs::protocol::club::v1::CLUB_REMOVED_REASON_DESTROYED_BY_MEMBER
        : (isKicked
            ? ::bgs::protocol::club::v1::CLUB_REMOVED_REASON_MEMBER_KICKED
            : ::bgs::protocol::club::v1::CLUB_REMOVED_REASON_MEMBER_LEFT);

    if (memberSession)
    {
        // Retail first delivers the player's GuildGUID/GuildRank/GuildLevel
        // update and only then tears down the Club membership. The observed
        // /gquit capture has roughly 241 ms between SMSG_UPDATE_OBJECT and
        // OnUnsubscribe/OnClubRemoved. Sending the RPCs back-to-back with the
        // values update is too early for BFA's Communities UI: IsInGuild() is
        // still true while CLUB_REMOVED is handled, leaving Chat/Roster/etc.
        // selected until the window is reopened.
        //
        // Keep the delay on the removed Player rather than holding a Guild*;
        // during disband the Guild object can be destroyed before this runs.
        if (Player* removedPlayer = memberSession->GetPlayer())
        {
            uint64 clubId = GetGuildClubId(guild);
            removedPlayer->m_Events.AddEventAtOffset([memberGuid, clubId, memberId, reason]()
            {
                WorldSession* delayedSession = FindOnlinePlayerSession(memberGuid);
                if (!delayedSession)
                    return;

                ::bgs::protocol::club::v1::UnsubscribeNotification unsubscribe;
                unsubscribe.mutable_agent_id()->CopyFrom(memberId);
                unsubscribe.set_club_id(clubId);

                Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(delayedSession);
                listener.OnUnsubscribe(&unsubscribe, true, true);

                ::bgs::protocol::club::v1::membership::ClubRemovedNotification clubRemoved;
                clubRemoved.mutable_agent_id()->CopyFrom(memberId);
                clubRemoved.mutable_member_id()->CopyFrom(memberId);
                clubRemoved.set_club_id(clubId);
                clubRemoved.set_reason(reason);

                Battlenet::WorldserverService<::bgs::protocol::club::v1::membership::ClubMembershipListener> membershipListener(delayedSession);
                membershipListener.OnClubRemoved(&clubRemoved, true, true);
            }, Milliseconds(250));
        }
    }

    // A destroyed guild is disappearing for every member, so there is no
    // surviving club roster that needs individual MemberRemoved updates.
    if (isDisbanding)
        return;

    ::bgs::protocol::club::v1::MemberRemovedNotification memberRemoved;
    memberRemoved.set_club_id(GetGuildClubId(guild));
    auto* assignment = memberRemoved.add_member();
    assignment->mutable_id()->CopyFrom(memberId);
    assignment->set_reason(reason);

    for (auto const& sessionPair : sWorld->GetAllSessions())
    {
        WorldSession* receiverSession = sessionPair.second;
        Player* receiver = receiverSession ? receiverSession->GetPlayer() : nullptr;
        if (!receiver || receiver->GetGuildId() != guild->GetId() || receiver->GetGUID() == memberGuid)
            continue;

        Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(receiverSession);
        listener.OnMemberRemoved(&memberRemoved, true, true);
    }
}

// ---------------------------------------------------------------------------
// Club services for WoW guilds. BFA's Guild & Communities panel gets its
// roster and chat streams through these BNet RPCs, not only legacy guild
// world packets.
// ---------------------------------------------------------------------------

Battlenet::ClubService::ClubService(WorldSession* session) : BaseService(session)
{
}

uint32 Battlenet::ClubService::HandleGetClubType(::bgs::protocol::club::v1::GetClubTypeRequest const* request, ::bgs::protocol::club::v1::GetClubTypeResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    if (request->has_type())
    {
        response->mutable_type()->CopyFrom(request->type());
        if (request->type().program() == WOW_CLUB_PROGRAM && request->type().name() == "guild")
            FillGuildRoleSet(response->mutable_role_set());
    }

    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleUpdateClubState(::bgs::protocol::club::v1::UpdateClubStateRequest const* request, ::bgs::protocol::NoData* /*response*/, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    Player* player = _session->GetPlayer();
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);
    if (!player || !guild || !request->has_options())
        return ERROR_OK;

    ::bgs::protocol::club::v1::ClubStateOptions const& options = request->options();

    // Guild names are legacy guild state as well as Club state. Only the guild
    // master may rename the guild through this bridge; Guild::SetName performs
    // the normal validation/database update and now emits the live Club update.
    if (options.has_name() && player->GetGUID() == guild->GetLeaderGUID())
        guild->SetName(options.name());

    // Reuse the legacy permission check for guild info/description. The
    // Guild.cpp hook broadcasts the corresponding Club state assignment.
    if (options.has_description())
        guild->HandleSetInfo(_session, options.description());

    if (options.has_broadcast() && options.broadcast().has_content())
        guild->HandleSetMOTD(_session, options.broadcast().content());

    // Guild privacy/avatar/stream ordering are not represented by Haven's
    // legacy guild model. Accept unsupported optional fields rather than
    // returning RPC_NOT_IMPLEMENTED and forcing the BFA client to tear down
    // its Club session.
    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleUpdateClubSettings(::bgs::protocol::club::v1::UpdateClubSettingsRequest const* request, ::bgs::protocol::NoData* /*response*/, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    Player* player = _session->GetPlayer();
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);
    if (!player || !guild || !request->has_options())
        return ERROR_OK;

    ::bgs::protocol::club::v1::SettingsChangedNotification settingsChanged;
    FillClubMemberId(_session, player, settingsChanged.mutable_agent_id());
    settingsChanged.set_club_id(GetGuildClubId(guild));

    auto* assignment = settingsChanged.mutable_assignment();
    ::bgs::protocol::club::v1::ClubSettingsOptions const& options = request->options();
    for (int i = 0; i < options.stream_size(); ++i)
        assignment->add_stream()->CopyFrom(options.stream(i));
    if (options.has_settings())
        assignment->mutable_settings()->CopyFrom(options.settings());

    // The BFA client retries UpdateClubSettings and can tear down the Club RPC
    // session when the service answers 3015. Acknowledge valid guild settings
    // and fan the assignment out live so all online members share one state.
    for (auto const& sessionPair : sWorld->GetAllSessions())
    {
        WorldSession* receiverSession = sessionPair.second;
        Player* receiver = receiverSession ? receiverSession->GetPlayer() : nullptr;
        if (!receiver || receiver->GetGuildId() != guild->GetId())
            continue;

        Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(receiverSession);
        listener.OnSettingsChanged(&settingsChanged, true, true);
    }

    return ERROR_OK;
}


uint32 Battlenet::ClubService::HandleSubscribe(
    ::bgs::protocol::club::v1::SubscribeRequest const* request,
    ::bgs::protocol::NoData* response,
    std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation)
{
    Player* player = _session->GetPlayer();
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);

    if (!player || !guild || !request->has_club_id())
        return ERROR_NOT_EXISTS;

    // Complete the Subscribe RPC before sending the subscription snapshot.
    // club_service.pb.cc invokes the continuation again only if it is still set,
    // so clear it after sending the response here to avoid a duplicate response.
    if (continuation)
    {
        continuation(this, ERROR_OK, response);
        continuation = {};
    }

    SendGuildClubSubscriptionSnapshot(_session, guild);
    return ERROR_OK;
}


uint32 Battlenet::ClubService::HandleUnsubscribe(::bgs::protocol::club::v1::UnsubscribeRequest const* /*request*/, ::bgs::protocol::NoData* /*response*/, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    // Logout/character switching tears the Club subscription down after the
    // Player object may already be detached. Treat unsubscribe as idempotent.
    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleGetMembers(::bgs::protocol::club::v1::GetMembersRequest const* request, ::bgs::protocol::club::v1::GetMembersResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);
    if (!guild || !request->has_club_id())
        return ERROR_NOT_EXISTS;

    // Guild::Member is private in this Haven branch, so read the same persistent
    // rows GuildMgr loads. This also covers offline members, which is required for
    // a real roster rather than only showing currently connected characters.
    QueryResult result = CharacterDatabase.PQuery(
        "SELECT gm.guid, gm.`rank`, gm.pnote, c.account "
        "FROM guild_member gm INNER JOIN characters c ON c.guid = gm.guid "
        "WHERE gm.guildid = %u ORDER BY gm.`rank`, gm.guid", guild->GetId());

    if (!result)
        return ERROR_OK;

    do
    {
        Field* fields = result->Fetch();
        uint64 guidLow = fields[0].GetUInt64();
        uint8 rank = fields[1].GetUInt8();
        std::string publicNote = fields[2].GetString();
        uint32 gameAccountId = fields[3].GetUInt32();

        WorldSession* onlineSession = FindOnlineGuildMemberSession(guild->GetId(), guidLow);

        auto* member = response->add_member();
        auto* memberId = member->mutable_id();
        memberId->set_unique_id(CreateGuildClubMemberUniqueId(guidLow));

        uint32 battlenetAccountId = onlineSession ? onlineSession->GetBattlenetAccountId() : ResolveBattlenetAccountId(gameAccountId);
        if (battlenetAccountId)
            memberId->mutable_account_id()->set_id(battlenetAccountId);

        member->add_role(GetGuildClubRole(guild, rank));
        member->set_presence_level(::bgs::protocol::club::v1::PRESENCE_LEVEL_RICH);
        member->set_moderator_mute(false);
        member->set_whisper_level(::bgs::protocol::club::v1::WHISPER_LEVEL_OPEN);
        member->set_note(publicNote);
        member->set_active(onlineSession != nullptr);
    }
    while (result->NextRow());

    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleGetInvitations(::bgs::protocol::club::v1::GetInvitationsRequest const* request, ::bgs::protocol::club::v1::GetInvitationsResponse* /*response*/, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    // Guild invitations in Haven are still handled by the legacy guild system.
    // Retail requests this RPC when opening the panel; an empty successful list
    // is preferable to ERROR_RPC_NOT_IMPLEMENTED until BNet invitation state is
    // persisted separately.
    if (request->has_club_id() && !GetSessionGuild(_session, request->club_id()))
        return ERROR_NOT_EXISTS;

    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleGetStreams(::bgs::protocol::club::v1::GetStreamsRequest const* request, ::bgs::protocol::club::v1::GetStreamsResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);
    if (!guild || !request->has_club_id())
        return ERROR_NOT_EXISTS;

    FillGuildStream(response->add_stream(), guild, GUILD_GENERAL_STREAM_ID, "Guild", "COMMUNITIES_GUILD_GENERAL_CHANNEL_NAME", { 1u, 2u, 3u, 4u });
    FillGuildStream(response->add_stream(), guild, GUILD_OFFICER_STREAM_ID, "Officer", "COMMUNITIES_GUILD_OFFICER_CHANNEL_NAME", { 1u, 2u, 3u });

    Player* player = _session->GetPlayer();
    if (!player)
        return ERROR_NOT_EXISTS;

    FillGuildStreamView(response->add_view(), guild, player->GetGUID(), GUILD_GENERAL_STREAM_ID);
    FillGuildStreamView(response->add_view(), guild, player->GetGUID(), GUILD_OFFICER_STREAM_ID);
    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleSubscribeStream(::bgs::protocol::club::v1::SubscribeStreamRequest const* request, ::bgs::protocol::NoData* /*response*/, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);
    if (!guild || !request->has_club_id())
        return ERROR_NOT_EXISTS;

    if (!request->stream_id_size())
        return ERROR_INVALID_ARGS;

    for (int32 i = 0; i < request->stream_id_size(); ++i)
        if (!IsGuildStreamId(request->stream_id(i)))
            return ERROR_INVALID_ARGS;

    Player* player = _session->GetPlayer();
    if (!player)
        return ERROR_NOT_EXISTS;

    // Guild ranks are user-defined. Do not infer stream access from a numeric
    // rank index: use the rights attached to the current rank.
    for (int32 i = 0; i < request->stream_id_size(); ++i)
    {
        bool const officerStream = request->stream_id(i) == GUILD_OFFICER_STREAM_ID;
        if (!guild->HasChatRight(player, officerStream, false))
            return ERROR_DENIED;
    }

    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleUnsubscribeStream(::bgs::protocol::club::v1::UnsubscribeStreamRequest const* request, ::bgs::protocol::NoData* /*response*/, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    // During logout the Player object can already be detached while the BNet
    // client is still unwinding stream subscriptions. That is not an error.
    if (!_session->GetPlayer())
        return ERROR_OK;

    if (request->has_club_id() && !GetSessionGuild(_session, request->club_id()))
        return ERROR_NOT_EXISTS;

    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleSetStreamFocus(::bgs::protocol::club::v1::SetStreamFocusRequest const* request, ::bgs::protocol::NoData* /*response*/, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);
    if (!guild || !request->has_club_id())
        return ERROR_NOT_EXISTS;

    if (request->has_stream_id() && !IsGuildStreamId(request->stream_id()))
        return ERROR_INVALID_ARGS;

    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleAdvanceStreamViewTime(::bgs::protocol::club::v1::AdvanceStreamViewTimeRequest const* request, ::bgs::protocol::NoData* /*response*/, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    Player* player = _session->GetPlayer();
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);
    if (!player || !guild || !request->has_club_id())
        return ERROR_NOT_EXISTS;

    if (!request->stream_id_size())
        return ERROR_INVALID_ARGS;

    uint64 const viewTime = GetClubTimeNow();
    ::bgs::protocol::club::v1::StreamAdvanceViewTimeNotification notification;
    FillClubMemberId(_session, player, notification.mutable_agent_id());
    notification.set_club_id(GetGuildClubId(guild));

    for (int32 i = 0; i < request->stream_id_size(); ++i)
    {
        uint64 streamId = request->stream_id(i);
        if (!IsGuildStreamId(streamId))
            return ERROR_INVALID_ARGS;

        bool const officerStream = streamId == GUILD_OFFICER_STREAM_ID;
        if (!guild->HasChatRight(player, officerStream, false))
            return ERROR_DENIED;

        StoreGuildStreamViewTime(guild, player->GetGUID(), streamId, viewTime);
        auto* view = notification.add_view();
        view->set_stream_id(streamId);
        view->set_view_time(viewTime);
    }

    Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(_session);
    listener.OnStreamAdvanceViewTime(&notification, true, true);
    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleGetStreamHistory(::bgs::protocol::club::v1::GetStreamHistoryRequest const* request, ::bgs::protocol::club::v1::GetStreamHistoryResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);
    if (!guild || !request->has_club_id())
        return ERROR_NOT_EXISTS;

    if (!request->has_stream_id() || !IsGuildStreamId(request->stream_id()))
        return ERROR_INVALID_ARGS;

    Player* player = _session->GetPlayer();
    if (!player)
        return ERROR_NOT_EXISTS;

    bool const officerStream = request->stream_id() == GUILD_OFFICER_STREAM_ID;
    if (!guild->HasChatRight(player, officerStream, false))
        return ERROR_DENIED;

    uint64 fetchUntil = std::numeric_limits<uint64>::max();
    uint32 maxEvents = 100;

    if (request->has_options())
    {
        if (request->options().has_fetch_until() && request->options().fetch_until())
            fetchUntil = request->options().fetch_until();

        if (request->options().has_max_events() && request->options().max_events())
            maxEvents = std::min<uint32>(request->options().max_events(), 100);
    }

    uint32 returned = 0;
    uint64 oldestEpoch = 0;

    // Served from ClubStreamHistoryMgr's cache (newest first, tombstones included).
    {
        for (ClubStreamMessage const* stored : sClubStreamHistoryMgr->GetHistory(guild, request->stream_id(), fetchUntil, maxEvents))
        {
            ObjectGuid authorGuid = ObjectGuid::Create<HighGuid::Player>(stored->AuthorGuid);
            uint64 epoch = stored->Epoch;
            std::string const& content = stored->Content;
            uint64 destroyerLow = stored->DestroyerGuid;
            uint64 destroyTime = stored->DestroyTime;

            ::bgs::protocol::club::v1::MemberId authorId;
            FillClubMemberIdForGuildMember(guild, authorGuid, &authorId);
            if (destroyTime)
            {
                // Retail keeps destroyed messages in history as tombstones: id,
                // author, destroyer, destroyed, destroyTime and no content chain.
                ::bgs::protocol::club::v1::StreamMessage* message = response->add_message();
                message->mutable_id()->set_epoch(epoch);
                message->mutable_id()->set_position(0);
                message->mutable_author()->mutable_id()->CopyFrom(authorId);
                FillClubMemberIdForGuildMember(guild, ObjectGuid::Create<HighGuid::Player>(destroyerLow), message->mutable_destroyer()->mutable_id());
                message->set_destroyed(true);
                message->set_destroy_time(destroyTime);
            }
            else
                FillStreamMessage(response->add_message(), content, epoch, authorId);

            oldestEpoch = epoch;
            ++returned;
        }
    }

    // Retail pages backwards in time. Captures use the oldest returned epoch
    // minus one millisecond as the next fetch_until cursor.
    if (returned == maxEvents && oldestEpoch > 1000)
        response->set_continuation(oldestEpoch - 1000);
    else
        response->set_continuation(0);

    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleCreateMessage(::bgs::protocol::club::v1::CreateMessageRequest const* request, ::bgs::protocol::club::v1::CreateMessageResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    Player* player = _session->GetPlayer();
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);
    if (!player || !guild || !request->has_club_id())
        return ERROR_NOT_EXISTS;

    if (!request->has_stream_id() || !IsGuildStreamId(request->stream_id()) || !request->has_options() || !request->options().has_content())
        return ERROR_INVALID_ARGS;

    bool const officerStream = request->stream_id() == GUILD_OFFICER_STREAM_ID;
    if (!guild->HasChatRight(player, officerStream, true))
        return ERROR_DENIED;

    std::string const& content = request->options().content();
    if (content.empty())
        return ERROR_INVALID_ARGS;

    // BFA renders fixed guild Club streams in the normal chat frame itself.
    // Retail answers CreateMessage through the Club service and does not send a
    // second legacy SMSG_CHAT for the same text. Sending both makes one logical
    // guild message appear twice in the chat frame.

    uint64 messageTime = Battlenet::StoreGuildClubChatMessage(guild, player->GetGUID(), officerStream, content);

    ::bgs::protocol::club::v1::MemberId authorId;
    FillClubMemberId(_session, player, &authorId);
    FillStreamMessage(response->mutable_message(), content, messageTime, authorId);

    // Retail advances the sender's view immediately after CreateMessage. Persist
    // it so the sender's own post cannot come back as unread after relog.
    uint64 const senderViewTime = GetClubTimeNow();
    StoreGuildStreamViewTime(guild, player->GetGUID(), request->stream_id(), senderViewTime);

    ::bgs::protocol::club::v1::StreamAdvanceViewTimeNotification viewNotification;
    viewNotification.mutable_agent_id()->CopyFrom(authorId);
    viewNotification.set_club_id(GetGuildClubId(guild));
    auto* senderView = viewNotification.add_view();
    senderView->set_stream_id(request->stream_id());
    senderView->set_view_time(senderViewTime);

    Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> senderListener(_session);
    senderListener.OnStreamAdvanceViewTime(&viewNotification, true, true);

    ::bgs::protocol::club::v1::StreamMessageAddedNotification notification;
    notification.mutable_agent_id()->CopyFrom(authorId);
    notification.set_club_id(GetGuildClubId(guild));
    notification.set_stream_id(request->stream_id());
    FillStreamMessage(notification.mutable_message(), content, messageTime, authorId);

    // Club listeners are the canonical delivery path for messages created in
    // the Communities UI. The sender receives the CreateMessage response.
    for (auto const& sessionPair : sWorld->GetAllSessions())
    {
        WorldSession* receiverSession = sessionPair.second;
        Player* receiver = receiverSession ? receiverSession->GetPlayer() : nullptr;
        if (!receiver || receiver == player || receiver->GetGuildId() != guild->GetId())
            continue;

        if (!guild->HasChatRight(receiver, officerStream, false))
            continue;

        Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(receiverSession);
        listener.OnStreamMessageAdded(&notification, true, true);
    }

    return ERROR_OK;
}

uint32 Battlenet::ClubService::HandleDestroyMessage(::bgs::protocol::club::v1::DestroyMessageRequest const* request, ::bgs::protocol::club::v1::DestroyMessageResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    Player* player = _session->GetPlayer();
    Guild* guild = GetSessionGuild(_session, request->has_club_id() ? request->club_id() : 0);
    if (!player || !guild || !request->has_club_id())
        return ERROR_NOT_EXISTS;

    if (!request->has_stream_id() || !IsGuildStreamId(request->stream_id()) || !request->has_message_id() || !request->message_id().has_epoch())
        return ERROR_INVALID_ARGS;

    bool const officerStream = request->stream_id() == GUILD_OFFICER_STREAM_ID;
    if (!guild->HasChatRight(player, officerStream, false))
        return ERROR_DENIED;

    uint64 const messageEpoch = request->message_id().epoch();
    ClubStreamMessage const* stored = sClubStreamHistoryMgr->FindMessage(guild, request->stream_id(), messageEpoch);
    if (!stored)
        return ERROR_NOT_EXISTS;

    ObjectGuid authorGuid = ObjectGuid::Create<HighGuid::Player>(stored->AuthorGuid);
    if (stored->IsDestroyed())
        return ERROR_NOT_EXISTS; // already destroyed

    // Club moderation is a role capability, not a fixed guild-rank index.
    // Custom ranks with officer privileges map to the moderator Club role.
    if (authorGuid != player->GetGUID() && GetGuildClubRole(player, guild) > 3)
        return ERROR_DENIED;

    uint64 destroyTime = uint64(std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());

    // Soft delete: keep a tombstone (retail history still returns destroyed
    // messages) but wipe the text so the deleted content is really gone.
    if (!sClubStreamHistoryMgr->DestroyMessage(guild, request->stream_id(), messageEpoch, player->GetGUID(), destroyTime))
        return ERROR_NOT_EXISTS;

    ::bgs::protocol::club::v1::MemberId authorId;
    FillClubMemberIdForGuildMember(guild, authorGuid, &authorId);
    ::bgs::protocol::club::v1::MemberId destroyerId;
    FillClubMemberId(_session, player, &destroyerId);

    auto fillDestroyedMessage = [&](::bgs::protocol::club::v1::StreamMessage* message)
    {
        message->mutable_id()->CopyFrom(request->message_id());
        message->mutable_author()->mutable_id()->CopyFrom(authorId);
        message->mutable_destroyer()->mutable_id()->CopyFrom(destroyerId);
        message->set_destroyed(true);
        message->set_destroy_time(destroyTime);
    };

    fillDestroyedMessage(response->mutable_message());

    ::bgs::protocol::club::v1::StreamMessageUpdatedNotification notification;
    notification.mutable_agent_id()->CopyFrom(destroyerId);
    notification.set_club_id(GetGuildClubId(guild));
    notification.set_stream_id(request->stream_id());
    fillDestroyedMessage(notification.mutable_message());

    for (auto const& sessionPair : sWorld->GetAllSessions())
    {
        WorldSession* receiverSession = sessionPair.second;
        Player* receiver = receiverSession ? receiverSession->GetPlayer() : nullptr;
        if (!receiver || receiver == player || receiver->GetGuildId() != guild->GetId())
            continue;

        if (!guild->HasChatRight(receiver, officerStream, false))
            continue;

        Battlenet::WorldserverService<::bgs::protocol::club::v1::ClubListener> listener(receiverSession);
        listener.OnStreamMessageUpdated(&notification, true, true);
    }

    return ERROR_OK;
}
