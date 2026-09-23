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

#include "ClubStreamHistoryMgr.h"
#include "CharacterCache.h"
#include "ClubUtils.h"
#include "DatabaseEnv.h"
#include "Guild.h"
#include "Log.h"
#include "Timer.h"
#include <algorithm>
#include <initializer_list>
#include <iterator>

using namespace Battlenet::ClubUtils;

ClubStreamHistoryMgr* ClubStreamHistoryMgr::instance()
{
    static ClubStreamHistoryMgr instance;
    return &instance;
}

void ClubStreamHistoryMgr::LoadFromDB()
{
    uint32 oldMSTime = getMSTime();
    _streams.clear();
    _viewMarkers.clear();

    // Apply retention synchronously before loading stream history.
    uint64 const cutoff = uint64(std::max<time_t>(time(nullptr) - time_t(MessageRetentionSeconds), 0));
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_EXPIRED_CLUB_MESSAGES);
    stmt->setUInt64(0, cutoff);
    CharacterDatabase.DirectExecute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_EXPIRED_CLUB_MEMBER_MENTIONS);
    stmt->setUInt64(0, cutoff);
    CharacterDatabase.DirectExecute(stmt);

    uint32 messageCount = 0;
    if (QueryResult result = CharacterDatabase.Query("SELECT clubId, streamId, epoch, position, authorAccountId, authorGuid, content, createdTime, destroyerGuid, destroyTime "
        "FROM club_message ORDER BY clubId, streamId, epoch, position"))
    {
        do
        {
            Field* fields = result->Fetch();
            Stream& stream = _streams[StreamKey(fields[0].GetUInt64(), fields[1].GetUInt64())];

            ClubStreamMessage message;
            message.Epoch           = fields[2].GetUInt64();
            message.Position        = fields[3].GetUInt64();
            message.AuthorAccountId = fields[4].GetUInt32();
            message.AuthorGuid      = fields[5].GetUInt64();
            message.Content         = fields[6].GetString();
            message.CreatedTime     = fields[7].GetUInt64();
            message.DestroyerGuid   = fields[8].GetUInt64();
            message.DestroyTime     = fields[9].GetUInt64();

            stream.NextPosition = std::max(stream.NextPosition, message.Position + 1);
            stream.Messages.push_back(std::move(message));
            ++messageCount;
        }
        while (result->NextRow());
    }

    uint32 markerCount = 0;
    if (QueryResult result = CharacterDatabase.Query("SELECT clubId, streamId, memberGuid, lastViewTime FROM club_stream_view_marker"))
    {
        do
        {
            Field* fields = result->Fetch();
            _viewMarkers[MarkerKey(fields[0].GetUInt64(), fields[1].GetUInt64(), fields[2].GetUInt64())] = fields[3].GetUInt64();
            ++markerCount;
        }
        while (result->NextRow());
    }

    TC_LOG_INFO("server.loading", ">> Loaded %u club stream messages in %u streams and %u view markers in %u ms.",
        messageCount, uint32(_streams.size()), markerCount, GetMSTimeDiffToNow(oldMSTime));
}

ClubStreamHistoryMgr::Stream* ClubStreamHistoryMgr::FindStream(uint64 clubId, uint64 streamId)
{
    auto itr = _streams.find(StreamKey(clubId, streamId));
    return itr != _streams.end() ? &itr->second : nullptr;
}

ClubStreamHistoryMgr::Stream const* ClubStreamHistoryMgr::FindStream(uint64 clubId, uint64 streamId) const
{
    auto itr = _streams.find(StreamKey(clubId, streamId));
    return itr != _streams.end() ? &itr->second : nullptr;
}

uint64 ClubStreamHistoryMgr::GetLastMessageTime(Guild const* guild, uint64 streamId) const
{
    Stream const* stream = FindStream(GetGuildClubId(guild), streamId);
    return stream && !stream->Messages.empty() ? stream->Messages.back().Epoch : 0;
}

uint64 ClubStreamHistoryMgr::GetLastReadTime(Guild const* guild, ObjectGuid memberGuid, uint64 streamId) const
{
    auto itr = _viewMarkers.find(MarkerKey(GetGuildClubId(guild), streamId, memberGuid.GetCounter()));
    return itr != _viewMarkers.end() ? itr->second : 0;
}

void ClubStreamHistoryMgr::StoreViewTime(Guild const* guild, ObjectGuid memberGuid, uint64 streamId, uint64 viewTime)
{
    if (!guild)
        return;

    uint64 const clubId = GetGuildClubId(guild);
    _viewMarkers[MarkerKey(clubId, streamId, memberGuid.GetCounter())] = viewTime;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CLUB_STREAM_VIEW_MARKER);
    stmt->setUInt64(0, clubId);
    stmt->setUInt64(1, streamId);
    stmt->setUInt64(2, memberGuid.GetCounter());
    stmt->setUInt64(3, viewTime);
    CharacterDatabase.Execute(stmt);
}

uint64 ClubStreamHistoryMgr::StoreChatMessage(Guild const* guild, ObjectGuid authorGuid, bool officerStream, std::string const& content)
{
    if (!guild || content.empty())
        return 0;

    uint64 const clubId = GetGuildClubId(guild);
    uint64 const streamId = officerStream ? GUILD_OFFICER_STREAM_ID : GUILD_GENERAL_STREAM_ID;
    Stream& stream = _streams[StreamKey(clubId, streamId)];

    ClubStreamMessage message;
    message.Epoch = GetClubTimeNow();
    message.Position = stream.NextPosition++;
    message.AuthorAccountId = sCharacterCache->GetCharacterAccountIdByGuid(authorGuid);
    message.AuthorGuid = authorGuid.GetCounter();
    message.Content = content;
    message.CreatedTime = message.Epoch / 1000000;

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CLUB_MESSAGE);
    stmt->setUInt64(0, clubId);
    stmt->setUInt64(1, streamId);
    stmt->setUInt64(2, message.Epoch);
    stmt->setUInt64(3, message.Position);
    stmt->setUInt32(4, message.AuthorAccountId);
    stmt->setUInt64(5, message.AuthorGuid);
    stmt->setString(6, message.Content);
    stmt->setUInt64(7, message.CreatedTime);
    CharacterDatabase.Execute(stmt);

    uint64 const epoch = message.Epoch;
    stream.Messages.push_back(std::move(message));
    TrimExpired(clubId, streamId, stream);
    return epoch;
}

void ClubStreamHistoryMgr::TrimExpired(uint64 clubId, uint64 streamId, Stream& stream)
{
    // Keep a bounded time horizon so a stream cannot grow forever.
    uint64 const cutoff = uint64(std::max<time_t>(time(nullptr) - time_t(MessageRetentionSeconds), 0));
    auto firstKept = std::find_if(stream.Messages.begin(), stream.Messages.end(),
        [cutoff](ClubStreamMessage const& message) { return message.CreatedTime >= cutoff; });
    if (firstKept == stream.Messages.begin())
        return;

    stream.Messages.erase(stream.Messages.begin(), firstKept);

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_OLD_CLUB_MESSAGES);
    stmt->setUInt64(0, clubId);
    stmt->setUInt64(1, streamId);
    stmt->setUInt64(2, cutoff);
    CharacterDatabase.Execute(stmt);
}

std::vector<ClubStreamMessage const*> ClubStreamHistoryMgr::GetHistory(Guild const* guild, uint64 streamId, uint64 fetchUntil, uint32 maxEvents) const
{
    std::vector<ClubStreamMessage const*> history;
    Stream const* stream = FindStream(GetGuildClubId(guild), streamId);
    if (!stream)
        return history;

    for (auto itr = stream->Messages.rbegin(); itr != stream->Messages.rend() && history.size() < maxEvents; ++itr)
        if (itr->Epoch < fetchUntil)
            history.push_back(&*itr);

    return history;
}

ClubStreamMessage const* ClubStreamHistoryMgr::FindMessage(Guild const* guild, uint64 streamId, uint64 epoch) const
{
    Stream const* stream = FindStream(GetGuildClubId(guild), streamId);
    if (!stream)
        return nullptr;

    // Newest match first (the old SQL used ORDER BY id DESC LIMIT 1).
    for (auto itr = stream->Messages.rbegin(); itr != stream->Messages.rend(); ++itr)
        if (itr->Epoch == epoch)
            return &*itr;

    return nullptr;
}

bool ClubStreamHistoryMgr::DestroyMessage(Guild const* guild, uint64 streamId, uint64 epoch, ObjectGuid destroyerGuid, uint64 destroyTime)
{
    uint64 const clubId = GetGuildClubId(guild);
    Stream* stream = FindStream(clubId, streamId);
    if (!stream)
        return false;

    for (auto itr = stream->Messages.rbegin(); itr != stream->Messages.rend(); ++itr)
    {
        if (itr->Epoch != epoch)
            continue;

        if (itr->IsDestroyed())
            return false;

        // Soft delete: keep a tombstone but wipe the text for real.
        itr->Content.clear();
        itr->DestroyerGuid = destroyerGuid.GetCounter();
        itr->DestroyTime = destroyTime;

        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CLUB_MESSAGE_DESTROY);
        stmt->setUInt64(0, itr->DestroyerGuid);
        stmt->setUInt64(1, destroyTime);
        stmt->setUInt64(2, clubId);
        stmt->setUInt64(3, streamId);
        stmt->setUInt64(4, itr->Epoch);
        stmt->setUInt64(5, itr->Position);
        CharacterDatabase.Execute(stmt);
        return true;
    }

    return false;
}

void ClubStreamHistoryMgr::DeleteClub(uint64 clubId, CharacterDatabaseTransaction& trans)
{
    for (auto itr = _streams.begin(); itr != _streams.end();)
        itr = itr->first.first == clubId ? _streams.erase(itr) : std::next(itr);

    for (auto itr = _viewMarkers.begin(); itr != _viewMarkers.end();)
        itr = std::get<0>(itr->first) == clubId ? _viewMarkers.erase(itr) : std::next(itr);

    for (CharacterDatabaseStatements statementId : { CHAR_DEL_CLUB_MESSAGES, CHAR_DEL_CLUB_STREAM_VIEW_MARKERS, CHAR_DEL_CLUB_MEMBER_MENTIONS })
    {
        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(statementId);
        stmt->setUInt64(0, clubId);
        CharacterDatabase.ExecuteOrAppend(trans, stmt);
    }
}

void ClubStreamHistoryMgr::DeleteMember(uint64 clubId, ObjectGuid::LowType memberGuid, CharacterDatabaseTransaction& trans)
{
    for (auto itr = _viewMarkers.begin(); itr != _viewMarkers.end();)
        itr = (std::get<0>(itr->first) == clubId && std::get<2>(itr->first) == memberGuid) ? _viewMarkers.erase(itr) : std::next(itr);

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CLUB_STREAM_VIEW_MARKER_MEMBER);
    stmt->setUInt64(0, clubId);
    stmt->setUInt64(1, memberGuid);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

// Declared in WorldserverService.h; kept for existing callers (Guild.cpp, ClubService).
uint64 Battlenet::StoreGuildClubChatMessage(Guild const* guild, ObjectGuid authorGuid, bool officerStream, std::string const& content)
{
    return sClubStreamHistoryMgr->StoreChatMessage(guild, authorGuid, officerStream, content);
}
