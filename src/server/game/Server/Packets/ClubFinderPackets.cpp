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

#include "ClubFinderPackets.h"
#include <algorithm>

// Wire (8.3.7 client writer 0x65A420):
//   Bits<9> searchLength, Bits<3> requestType, flush
//   uint32 filterCount, uint32 settings, searchString
//   filterCount x { Bits<3> type, flush, Bits<3> kind, flush, value }
//   value: kind 1 = uint32, kind 3 = uint64
void WorldPackets::ClubFinder::ClubFinderRequestClubsList::Read()
{
    uint32 searchLength = _worldPacket.ReadBits(9);
    RequestType = uint8(_worldPacket.ReadBits(3));
    _worldPacket.ResetBitPos();

    uint32 filterCount = 0;
    _worldPacket >> filterCount;
    _worldPacket >> Settings;
    SearchTerms = _worldPacket.ReadString(searchLength);

    filterCount = std::min<uint32>(filterCount, 16);
    Filters.reserve(filterCount);
    for (uint32 i = 0; i < filterCount; ++i)
    {
        Filter filter;
        filter.Type = uint8(_worldPacket.ReadBits(3));
        _worldPacket.ResetBitPos();
        filter.Kind = uint8(_worldPacket.ReadBits(3));
        _worldPacket.ResetBitPos();

        if (filter.Kind == 1)
            filter.Value = _worldPacket.read<uint32>();
        else if (filter.Kind == 3)
            filter.Value = _worldPacket.read<uint64>();
        else
        {
            // Unknown value kind: the rest cannot be framed, stop safely.
            Malformed = true;
            break;
        }

        Filters.push_back(filter);
    }

    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket.rfinish();
}

// [8.3.7 LAYOUT] VERIFIED in-game: count, uint32 posting IDs, envelope LAST.
// The 8.3.7 client requests exactly the returned posting IDs afterwards, so this
// order decodes correctly. The envelope position is per packet in 8.3.7.
WorldPacket const* WorldPackets::ClubFinder::ReturnRecruitingClubs::Write()
{
    _worldPacket << uint32(PostingIDs.size());
    for (uint32 id : PostingIDs)
        _worldPacket << id;

    _worldPacket << Context;
    return &_worldPacket;
}

void WorldPackets::ClubFinder::ClubFinderRequestClubsData::Read()
{
    // Observed 8.3.7 own-posting request:
    //   uint32 postingCount, uint32 filterCount, postingCount * uint32 postingId,
    //   optional filter payload, trailing one-byte envelope.
    // The envelope stores request type in bits 7..5 and linked-lookup in bit 4.
    uint32 count = 0;
    if (_worldPacket.rpos() + sizeof(uint32) <= _worldPacket.size())
        _worldPacket >> count;
    if (_worldPacket.rpos() + sizeof(uint32) <= _worldPacket.size())
        _worldPacket >> FilterCount;

    count = std::min<uint32>(count, 1000);
    ClubFinderPostingIDs.reserve(count);
    for (uint32 i = 0; i < count && _worldPacket.rpos() + sizeof(uint32) <= _worldPacket.size(); ++i)
    {
        uint32 postingId = 0;
        _worldPacket >> postingId;
        ClubFinderPostingIDs.push_back(postingId);
    }

    // BFA filter records are not yet decoded here. The envelope is known to be
    // the final byte, so read it from the tail instead of treating the first
    // byte after the posting IDs as the envelope when FilterCount is non-zero.
    if (_worldPacket.rpos() < _worldPacket.size())
    {
        Context = _worldPacket[_worldPacket.size() - 1];
        RequestType = Context >> 5;
        LinkedLookup = (Context & 0x10) != 0;
        _worldPacket.rfinish();
    }
}

WorldPacket const* WorldPackets::ClubFinder::LookupClubPostingsList::Write()
{
    // 8.3.7 wire: uint32 count, envelope byte (request type << 5), then records.
    // The 12.1 client moved the envelope to the END of this packet; this writer
    // was originally built from a 12.1 capture and used that order, which the
    // 8.3.7 client silently dropped (no CLUB_FINDER_CLUB_LIST_RETURNED /
    // CLUB_FINDER_RECRUITMENT_POST_RETURNED event, empty client posting store).
    // Verified in-game: with the envelope after the count the guild appears in
    // the finder search and the recruitment settings dialog opens.
    // NOTE: the position is per packet in 8.3.7 - RETURN_RECRUITING_CLUBS decodes
    // correctly with its envelope last - so never move others without evidence.
    // [8.3.7 LAYOUT] VERIFIED in-game (search list populates, own posting decodes).
    _worldPacket << uint32(Postings.size());
    _worldPacket << Context;
    for (ClubFinderPosting const& posting : Postings)
        _worldPacket << posting;

    return &_worldPacket;
}

void WorldPackets::ClubFinder::ClubFinderRequestMembershipToClub::Read()
{
    _worldPacket >> ClubFinderGUID;

    // Observed 8.3.7 application request shape: uint64 recruiting-spec mask,
    // then a 10-bit comment byte length and comment bytes. The previous parser
    // skipped one byte before SpecMask; live submissions proved that this pulled
    // the comment-length byte into the high end of the stored mask.
    _worldPacket >> SpecMask;

    uint32 commentLength = _worldPacket.ReadBits(10);
    _worldPacket.ResetBitPos();

    // ReadString is intentionally strict: a declared length larger than the
    // remaining packet throws ByteBufferPositionException instead of silently
    // accepting a truncated application.
    Comment = _worldPacket.ReadString(commentLength);

    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket.rfinish();
}

void WorldPackets::ClubFinder::ClubFinderRequestSubscribedClubPostingIDs::Read()
{
    uint32 count = 0;
    if (_worldPacket.rpos() + sizeof(uint32) <= _worldPacket.size())
        _worldPacket >> count;

    count = std::min<uint32>(count, 1000);
    ClubIDs.reserve(count);
    for (uint32 i = 0; i < count && _worldPacket.rpos() + sizeof(uint64) <= _worldPacket.size(); ++i)
    {
        uint64 clubId = 0;
        _worldPacket >> clubId;
        ClubIDs.push_back(clubId);
    }

    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket.rfinish();
}

// [8.3.7] One 16-byte mapping record per subscribed club. Keep the second
// value semantically as PostingID: the Club Finder data layer now owns an
// independent posting sequence rather than aliasing it to the guild id.
WorldPacket const* WorldPackets::ClubFinder::ClubFinderGetClubPostingIDsResponse::Write()
{
    _worldPacket << uint32(Entries.size());
    for (ClubFinderClubPostingID const& entry : Entries)
    {
        _worldPacket << entry.ClubID;
        _worldPacket << entry.PostingID;
    }

    return &_worldPacket;
}

void WorldPackets::ClubFinder::ClubFinderGetApplicantsList::Read()
{
    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket >> Context;
    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket.rfinish();
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::ClubFinder::ClubFinderApplicationUpdate const& application)
{
    data << application.ClubFinderGUID;
    data << application.PlayerGUID;
    data << application.Closed;
    data << application.LastUpdatedTime;
    data << uint8((application.RequestStatus & 0x0F) << 4);
    return data;
}

// [8.3.7 LAYOUT PROBE] Non-empty application updates are ignored when the
// 12.1 framing is used (count, records, context). 8.3.7 already requires the
// request envelope immediately after the count for LOOKUP_CLUB_POSTINGS_LIST.
// Test the same BFA framing here: count, context, records.
WorldPacket const* WorldPackets::ClubFinder::ClubFinderUpdateApplications::Write()
{
    _worldPacket << uint32(Applications.size());
    _worldPacket << Context;
    for (ClubFinderApplicationUpdate const& application : Applications)
        _worldPacket << application;

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::ClubFinder::ClubFinderApplicantInfo const& applicant)
{
    // [8.3.7 LAYOUT] VERIFIED natively (Wow.exe 8.3.7: per-record reader 0x839330,
    // conversion to ClubFinderApplicantInfo in CGClubFinder 0x13786F3).
    // name/level/classID are NOT taken from this record: the client fills them
    // from its own character lookup (lookupSuccess). uint32 #2, #3, #5, the name
    // and the trailing bit are read but not used by the applicant list; sent as 0/empty.
    data << applicant.ClubFinderGUID;               // type must be ClubFinder (0x31)
    data << applicant.PlayerGUID;                   // type must be Player
    data << uint32(applicant.Closed);               // -> closed
    data << uint32(0);                              // not consumed by 8.3.7 applicant UI
    data << uint32(0);                              // not consumed by 8.3.7 applicant UI
    data << uint32(applicant.ItemLevel);            // -> ilvl
    data << uint32(0);                              // not consumed by 8.3.7 applicant UI
    data << uint64(applicant.SpecMask);             // -> specIds (mask converted client-side)
    data << uint64(applicant.LastUpdatedTime);      // -> lastUpdatedTime

    uint32 messageLength = uint32(std::min<std::size_t>(applicant.Message.size(), 0x3FF));
    data.WriteBits(0, 6);                           // name length (name comes from lookup)
    data.WriteBits(messageLength, 10);              // message length
    data.WriteBits(applicant.RequestStatus, 4);     // -> requestStatus (PlayerClubRequestStatus)
    data.WriteBit(false);                           // unknown flag, not consumed
    data.FlushBits();

    if (messageLength)
        data.WriteString(applicant.Message.substr(0, messageLength));

    return data;
}

// [8.3.7 LAYOUT PROBE] A non-empty recruiter list reaches the client but does
// not populate ReturnClubApplicantList(). Move the request envelope in front of
// the records so BFA sees: clubFinderGUID, count, context, records.
WorldPacket const* WorldPackets::ClubFinder::ReturnApplicantList::Write()
{
    _worldPacket << ClubFinderGUID;
    _worldPacket << uint32(Applicants.size());
    _worldPacket << Context;
    for (ClubFinderApplicantInfo const& applicant : Applicants)
        _worldPacket << applicant;

    return &_worldPacket;
}

void WorldPackets::ClubFinder::ClubFinderRequestPendingClubsList::Read()
{
    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket >> Context;
    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket.rfinish();
}

// [8.3.7 LAYOUT PROBE] Runtime proves Haven returns one stored application while
// PlayerReturnPendingGuildsList() remains empty with 12.1 framing. Put the BFA
// request envelope immediately after the count: count, context, records.
WorldPacket const* WorldPackets::ClubFinder::ResponseCharacterApplicationList::Write()
{
    _worldPacket << uint32(Applications.size());
    _worldPacket << Context;
    for (ClubFinderApplicationUpdate const& application : Applications)
        _worldPacket << application;

    return &_worldPacket;
}

void WorldPackets::ClubFinder::ClubFinderRespondToApplicant::Read()
{
    _worldPacket >> ClubFinderGUID;
    _worldPacket >> PlayerGUID;
    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket >> Flags;
    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket.rfinish();
}

void WorldPackets::ClubFinder::ClubFinderApplicationResponse::Read()
{
    _worldPacket >> ClubFinderGUID;
    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket >> Flags;
    if (_worldPacket.rpos() < _worldPacket.size())
        _worldPacket.rfinish();
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::ClubFinder::ClubFinderPosting const& posting)
{
    data.WriteBits(posting.ClubName.length(), 7);
    data.WriteBits(posting.Comment.length(), 12);
    data.WriteBits(posting.LeaderName.length(), 6);
    data.FlushBits();

    data << posting.ClubFinderGUID;
    data << posting.NumActiveMembers;
    data << posting.SpecMask;
    data << posting.RecruitmentFlags;
    data << posting.MinItemLevel;
    data << posting.TabardInfo;
    data << posting.LastPosterGUID;
    data << posting.ClubID;
    data << posting.LastUpdatedTime;

    data.WriteString(posting.ClubName);
    data.WriteString(posting.Comment);
    data.WriteString(posting.LeaderName);
    return data;
}
