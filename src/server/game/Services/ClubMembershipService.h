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

// ClubMembershipService - Battle.net club membership service (Draconic layout: Services/ClubMembershipService).

#ifndef HAVEN_CLUB_MEMBERSHIP_SERVICE_H
#define HAVEN_CLUB_MEMBERSHIP_SERVICE_H

#include "WorldserverService.h"

namespace Battlenet
{
    // Club membership bridge. Subscribe / GetState expose the character's WoW
    // guild as a Battle.net guild club so Club Finder can resolve its posting.
    class ClubMembershipService : public WorldserverService<club::v1::membership::ClubMembershipService>
    {
        typedef WorldserverService<club::v1::membership::ClubMembershipService> BaseService;

    public:
        ClubMembershipService(WorldSession* session);

        uint32 HandleSubscribe(::bgs::protocol::club::v1::membership::SubscribeRequest const* request, ::bgs::protocol::club::v1::membership::SubscribeResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleUnsubscribe(::bgs::protocol::club::v1::membership::UnsubscribeRequest const* request, ::bgs::protocol::NoData* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
        uint32 HandleGetState(::bgs::protocol::club::v1::membership::GetStateRequest const* request, ::bgs::protocol::club::v1::membership::GetStateResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& continuation) override;
    };
}

#endif // HAVEN_CLUB_MEMBERSHIP_SERVICE_H
