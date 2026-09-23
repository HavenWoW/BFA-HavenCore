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
// Community name queries for guild clubs (CMSG_QUERY_PLAYER_NAME_BY_COMMUNITY_ID,
// CMSG_QUERY_PLAYER_NAMES_FOR_COMMUNITY). Moved unchanged from ClubFinderHandler.
// -----------------------------------------------------------------------------

#include "WorldSession.h"
#include "CharacterCache.h"
#include "DatabaseEnv.h"
#include "DB2Stores.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "World.h"
#include <algorithm>
#include <array>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "QueryPackets.h"

namespace
{
    bool ResolveGuildClubMember(WorldSession* session, uint64 communityDbId, ObjectGuid& characterGuid, Player*& onlinePlayer)
    {
        Player* requester = session ? session->GetPlayer() : nullptr;
        if (!requester || !requester->GetGuildId())
            return false;

        Guild* guild = sGuildMgr->GetGuildById(requester->GetGuildId());
        if (!guild)
            return false;

        uint64 guidLow = communityDbId & 0x0000FFFFFFFFFFFFULL;
        if (!guidLow)
            return false;

        characterGuid = ObjectGuid::Create<HighGuid::Player>(ObjectGuid::LowType(guidLow));
        if (!guild->IsMember(characterGuid))
            return false;

        onlinePlayer = ObjectAccessor::FindConnectedPlayer(characterGuid);
        return true;
    }
}

void WorldSession::HandleQueryPlayerNameByCommunityId(WorldPacket& recvData)
{
    // BFA 8.3.x request layout: packed BNet account GUID + uint64 Club member id.
    ObjectGuid bnetAccountGuid;
    uint64 communityDbId = 0;
    recvData >> bnetAccountGuid;
    recvData >> communityDbId;

    WorldPacket response(SMSG_QUERY_COMMUNITY_NAME_RESPONSE, 96);

    ObjectGuid characterGuid;
    Player* onlinePlayer = nullptr;
    WorldPackets::Query::PlayerGuidLookupData lookupData;
    bool resolved = ResolveGuildClubMember(this, communityDbId, characterGuid, onlinePlayer) &&
        lookupData.Initialize(characterGuid, onlinePlayer);

    // Retail echoes the lookup key before the PlayerGuidLookupData payload.
    response << uint8(resolved ? 0 : 1);
    response << bnetAccountGuid;
    response << uint64(communityDbId);
    if (resolved)
        response << lookupData;

    SendPacket(&response);
}

void WorldSession::HandleQueryPlayerNamesForCommunity(WorldPacket& recvData)
{
    // BFA bulk request: uint64 clubId, uint32 count, then count entries of
    // packed BNet account GUID + uint64 Club member id. Retail answers with
    // SMSG_PREPOPULATE_NAME_CACHE: clubId, resolved count, lookup records.
    uint64 clubId = 0;
    uint32 count = 0;
    recvData >> clubId;
    recvData >> count;

    if (count > 1024)
    {
        recvData.rfinish();
        return;
    }

    std::vector<WorldPackets::Query::PlayerGuidLookupData> resolvedMembers;
    resolvedMembers.reserve(count);

    for (uint32 i = 0; i < count; ++i)
    {
        ObjectGuid bnetAccountGuid;
        uint64 communityDbId = 0;
        recvData >> bnetAccountGuid;
        recvData >> communityDbId;

        ObjectGuid characterGuid;
        Player* onlinePlayer = nullptr;
        WorldPackets::Query::PlayerGuidLookupData lookupData;
        if (ResolveGuildClubMember(this, communityDbId, characterGuid, onlinePlayer) &&
            lookupData.Initialize(characterGuid, onlinePlayer))
            resolvedMembers.push_back(std::move(lookupData));
    }

    WorldPacket response(SMSG_PREPOPULATE_NAME_CACHE, 12 + resolvedMembers.size() * 64);
    response << uint64(clubId);
    response << uint32(resolvedMembers.size());
    for (WorldPackets::Query::PlayerGuidLookupData const& lookupData : resolvedMembers)
        response << lookupData;

    SendPacket(&response);
}
