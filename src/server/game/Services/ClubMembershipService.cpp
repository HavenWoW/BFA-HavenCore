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

#include "ClubMembershipService.h"
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

Battlenet::ClubMembershipService::ClubMembershipService(WorldSession* session) : BaseService(session)
{
}

uint32 Battlenet::ClubMembershipService::HandleSubscribe(::bgs::protocol::club::v1::membership::SubscribeRequest const* request, ::bgs::protocol::club::v1::membership::SubscribeResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    auto* state = response->mutable_state();
    FillGuildMembershipState(_session, request->has_options() ? &request->options() : nullptr, state);
    return ERROR_OK;
}

uint32 Battlenet::ClubMembershipService::HandleUnsubscribe(::bgs::protocol::club::v1::membership::UnsubscribeRequest const* /*request*/, ::bgs::protocol::NoData* /*response*/, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    return ERROR_OK;
}

uint32 Battlenet::ClubMembershipService::HandleGetState(::bgs::protocol::club::v1::membership::GetStateRequest const* request, ::bgs::protocol::club::v1::membership::GetStateResponse* response, std::function<void(ServiceBase*, uint32, ::google::protobuf::Message const*)>& /*continuation*/)
{
    auto* state = response->mutable_state();
    FillGuildMembershipState(_session, request->has_options() ? &request->options() : nullptr, state);
    return ERROR_OK;
}
