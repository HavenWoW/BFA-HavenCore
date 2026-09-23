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

// ClubService - Battle.net club service for WoW guild clubs (Draconic layout: Services/ClubService).

#ifndef HAVEN_CLUB_SERVICE_H
#define HAVEN_CLUB_SERVICE_H

#include "WorldserverService.h"

namespace Battlenet
{
    // Live bridge hooks called by Guild.cpp when persistent guild membership changes.
    // These keep the Battle.net Club cache in sync without requiring a relog.
    void NotifyGuildClubMemberAdded(Guild const* guild, ObjectGuid memberGuid, uint8 guildRank);
    void NotifyGuildClubMemberRemoved(Guild const* guild, ObjectGuid memberGuid, bool isKicked, bool isDisbanding);
    void NotifyGuildClubMemberRoleChanged(Guild const* guild, ObjectGuid memberGuid, uint8 guildRank);
    void NotifyGuildClubNameChanged(Guild const* guild);
    void NotifyGuildClubDescriptionChanged(Guild const* guild);
    void NotifyGuildClubBroadcastChanged(Guild const* guild, WorldSession* sourceSession);

    // Club service bridge for WoW guild clubs. GetClubType exposes the retail
    // guild role model and Subscribe sends the club/member state notification the
    // client uses to distinguish guild master, officer and regular member.
    class ClubService : public WorldserverService<club::v1::ClubService>
    {
        typedef WorldserverService<club::v1::ClubService> BaseService;

    public:
        ClubService(WorldSession* session);

        uint32 HandleSubscribe(::bgs::protocol::club::v1::SubscribeRequest const* request, ::bgs::protocol::NoData* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleUnsubscribe(::bgs::protocol::club::v1::UnsubscribeRequest const* request, ::bgs::protocol::NoData* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleGetClubType(::bgs::protocol::club::v1::GetClubTypeRequest const* request, ::bgs::protocol::club::v1::GetClubTypeResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleUpdateClubState(::bgs::protocol::club::v1::UpdateClubStateRequest const* request, ::bgs::protocol::NoData* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleUpdateClubSettings(::bgs::protocol::club::v1::UpdateClubSettingsRequest const* request, ::bgs::protocol::NoData* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleGetMembers(::bgs::protocol::club::v1::GetMembersRequest const* request, ::bgs::protocol::club::v1::GetMembersResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleGetInvitations(::bgs::protocol::club::v1::GetInvitationsRequest const* request, ::bgs::protocol::club::v1::GetInvitationsResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleGetStreams(::bgs::protocol::club::v1::GetStreamsRequest const* request, ::bgs::protocol::club::v1::GetStreamsResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleSubscribeStream(::bgs::protocol::club::v1::SubscribeStreamRequest const* request, ::bgs::protocol::NoData* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleUnsubscribeStream(::bgs::protocol::club::v1::UnsubscribeStreamRequest const* request, ::bgs::protocol::NoData* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleSetStreamFocus(::bgs::protocol::club::v1::SetStreamFocusRequest const* request, ::bgs::protocol::NoData* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleAdvanceStreamViewTime(::bgs::protocol::club::v1::AdvanceStreamViewTimeRequest const* request, ::bgs::protocol::NoData* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleGetStreamHistory(::bgs::protocol::club::v1::GetStreamHistoryRequest const* request, ::bgs::protocol::club::v1::GetStreamHistoryResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleCreateMessage(::bgs::protocol::club::v1::CreateMessageRequest const* request, ::bgs::protocol::club::v1::CreateMessageResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleDestroyMessage(::bgs::protocol::club::v1::DestroyMessageRequest const* request, ::bgs::protocol::club::v1::DestroyMessageResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
    };
}

#endif // HAVEN_CLUB_SERVICE_H
