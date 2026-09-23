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
// ClubStreamHistoryMgr - guild club stream history (Draconic layout:
// Clubs/ClubStreamHistoryMgr).
//
// Write-through cache: every message and view marker is loaded into memory at
// startup (LoadFromDB) and every change is written to the database as it
// happens, so a crash loses at most what was still queued for the database.
// History and unread checks are answered from memory.
//
// Tables (characters DB, see 2026_09_21_04_characters_club_stream_history.sql):
//   club_message             Draconic layout + Haven tombstone columns
//   club_stream_view_marker  per member read marker (microseconds)
//   club_member_mention / club_mention_view_marker  created for parity; mentions
//                            are not implemented yet (only cleaned up).
//
// [8.3.7] Deleted messages stay as tombstones (destroyer, destroyTime, no
// content): retail returns them in GetStreamHistory. Draconic hard-deletes.
// The stored position is Draconic's per-stream counter; the wire still sends
// position 0 (unchanged behaviour, see FillStreamMessage).
// -----------------------------------------------------------------------------

#ifndef HAVEN_CLUB_STREAM_HISTORY_MGR_H
#define HAVEN_CLUB_STREAM_HISTORY_MGR_H

#include "Common.h"
#include "Define.h"
#include "DatabaseEnvFwd.h"
#include "ObjectGuid.h"
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class Guild;

struct ClubStreamMessage
{
    uint64 Epoch = 0;              // microseconds since unix epoch
    uint64 Position = 0;           // monotonic per (club, stream)
    uint32 AuthorAccountId = 0;
    ObjectGuid::LowType AuthorGuid = 0;
    std::string Content;
    uint64 CreatedTime = 0;        // unix seconds
    ObjectGuid::LowType DestroyerGuid = 0;
    uint64 DestroyTime = 0;        // microseconds, 0 = not deleted

    bool IsDestroyed() const { return DestroyTime != 0; }
};

class TC_GAME_API ClubStreamHistoryMgr
{
public:
    static constexpr uint32 MessageRetentionSeconds = 30 * DAY;

    static ClubStreamHistoryMgr* instance();

    void LoadFromDB();

    // --- read state ---------------------------------------------------------
    uint64 GetLastMessageTime(Guild const* guild, uint64 streamId) const;
    uint64 GetLastReadTime(Guild const* guild, ObjectGuid memberGuid, uint64 streamId) const;
    void StoreViewTime(Guild const* guild, ObjectGuid memberGuid, uint64 streamId, uint64 viewTime);

    // --- messages -----------------------------------------------------------
    // Persists one guild/officer chat line (verbatim, so |H links keep their
    // tooltips). Returns the message epoch (microseconds).
    uint64 StoreChatMessage(Guild const* guild, ObjectGuid authorGuid, bool officerStream, std::string const& content);

    // Newest first, epoch < fetchUntil, at most maxEvents (tombstones included).
    std::vector<ClubStreamMessage const*> GetHistory(Guild const* guild, uint64 streamId, uint64 fetchUntil, uint32 maxEvents) const;
    ClubStreamMessage const* FindMessage(Guild const* guild, uint64 streamId, uint64 epoch) const;
    // Soft delete (tombstone). False if the message is unknown or already deleted.
    bool DestroyMessage(Guild const* guild, uint64 streamId, uint64 epoch, ObjectGuid destroyerGuid, uint64 destroyTime);

    // --- lifetime -----------------------------------------------------------
    void DeleteClub(uint64 clubId, CharacterDatabaseTransaction& trans);
    void DeleteMember(uint64 clubId, ObjectGuid::LowType memberGuid, CharacterDatabaseTransaction& trans);

private:
    ClubStreamHistoryMgr() = default;

    using StreamKey = std::pair<uint64 /*clubId*/, uint64 /*streamId*/>;
    using MarkerKey = std::tuple<uint64 /*clubId*/, uint64 /*streamId*/, ObjectGuid::LowType /*memberGuid*/>;

    struct Stream
    {
        std::vector<ClubStreamMessage> Messages; // ascending (epoch, position)
        uint64 NextPosition = 1;
    };

    Stream* FindStream(uint64 clubId, uint64 streamId);
    Stream const* FindStream(uint64 clubId, uint64 streamId) const;
    void TrimExpired(uint64 clubId, uint64 streamId, Stream& stream);

    std::map<StreamKey, Stream> _streams;
    std::map<MarkerKey, uint64> _viewMarkers;
};

#define sClubStreamHistoryMgr ClubStreamHistoryMgr::instance()

namespace Battlenet
{
namespace ClubUtils
{
    // Call-site compatibility for the club service code.
    inline uint64 GetGuildStreamLastMessageTime(Guild const* guild, uint64 streamId) { return sClubStreamHistoryMgr->GetLastMessageTime(guild, streamId); }
    inline uint64 GetGuildStreamLastReadTime(Guild const* guild, ObjectGuid memberGuid, uint64 streamId) { return sClubStreamHistoryMgr->GetLastReadTime(guild, memberGuid, streamId); }
    inline void StoreGuildStreamViewTime(Guild const* guild, ObjectGuid memberGuid, uint64 streamId, uint64 viewTime) { sClubStreamHistoryMgr->StoreViewTime(guild, memberGuid, streamId, viewTime); }
}
}

#endif // HAVEN_CLUB_STREAM_HISTORY_MGR_H
