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

#ifndef ClubFinderPackets_h__
#define ClubFinderPackets_h__

#include "Packet.h"
#include "ObjectGuid.h"
#include <string>
#include <vector>

namespace WorldPackets
{
    namespace ClubFinder
    {
        // PlayerClubRequestStatus from the 8.3 Blizzard ClubFinder API.
        enum PlayerClubRequestStatus : uint8
        {
            RequestStatusNone          = 0,
            RequestStatusPending       = 1,
            RequestStatusAutoApproved  = 2,
            RequestStatusDeclined      = 3,
            RequestStatusApproved      = 4,
            RequestStatusJoined        = 5,
            RequestStatusJoinedAnother = 6,
            RequestStatusCanceled      = 7
        };

        // RecruitingClubInfo wire payload used by
        // SMSG_CLUB_FINDER_LOOKUP_CLUB_POSTINGS_LIST.
        //
        // The fixed field order is byte-for-byte accounted for by the supplied
        // retail capture. It also matches the 8.3 C_ClubFinder RecruitingClubInfo
        // API surface. Retail wire carries the recruiting specializations as
        // the same uint64 SpecMask used by CMSG_CLUB_FINDER_POST. Guild tabard
        // data is resolved separately through guild-info responses.
        struct ClubFinderPosting
        {
            ObjectGuid ClubFinderGUID;
            uint32 NumActiveMembers = 0;
            uint64 SpecMask = 0;
            uint32 RecruitmentFlags = 0;
            uint32 MinItemLevel = 0;
            // Retail carries one additional uint32 before LastPosterGUID. Its
            // semantics are not proven by the supplied capture. Zero is a
            // captured valid value, so do not invent a meaning for it here.
            uint32 TabardInfo = 0;   // packed guild emblem, see PackClubFinderTabardInfo
            ObjectGuid LastPosterGUID;
            uint64 ClubID = 0;
            uint64 LastUpdatedTime = 0;
            std::string ClubName;
            std::string Comment;
            std::string LeaderName;
        };

        class ClubFinderRequestClubsList final : public ClientPacket
        {
        public:
            ClubFinderRequestClubsList(WorldPacket&& packet) : ClientPacket(CMSG_CLUB_FINDER_REQUEST_CLUBS_LIST, std::move(packet)) { }

            void Read() override;

            // [8.3.7 LAYOUT] VERIFIED natively (client writer 0x65A420, filter
            // builder 0x137E7E0) and identical in 32 retail 12.1 requests.
            enum FilterType : uint8
            {
                FilterFocus     = 1,   // uint32, settings & 0x3E
                FilterSize      = 2,   // uint32, settings & 0x1C0
                FilterItemLevel = 3,   // uint32, the player's item level
                FilterLevel     = 4,   // uint32, the player's level
                FilterSpecs     = 5,   // uint64 spec mask
                FilterLocale    = 6    // uint32 language mask (bit = locale)
            };

            struct Filter
            {
                uint8 Type = 0;
                uint8 Kind = 0;        // 1 = uint32, 3 = uint64
                uint64 Value = 0;
            };

            uint8 RequestType = 0;     // 1 = guilds, 2 = communities
            uint32 Settings = 0;
            std::string SearchTerms;
            std::vector<Filter> Filters;
            bool Malformed = false;

            Filter const* FindFilter(uint8 type) const
            {
                for (Filter const& filter : Filters)
                    if (filter.Type == type)
                        return &filter;
                return nullptr;
            }
        };

        class ReturnRecruitingClubs final : public ServerPacket
        {
        public:
            ReturnRecruitingClubs() : ServerPacket(SMSG_RETURN_RECRUITING_CLUBS, 5) { }

            WorldPacket const* Write() override;

            std::vector<uint32> PostingIDs;
            uint8 Context = 0x20; // guild
        };

        class ClubFinderRequestClubsData final : public ClientPacket
        {
        public:
            ClubFinderRequestClubsData(WorldPacket&& packet) : ClientPacket(CMSG_CLUB_FINDER_REQUEST_CLUBS_DATA, std::move(packet)) { }

            void Read() override;

            uint32 FilterCount = 0;
            std::vector<uint32> ClubFinderPostingIDs;
            uint8 RequestType = 0;
            bool LinkedLookup = false;
            uint8 Context = 0;
        };

        class LookupClubPostingsList final : public ServerPacket
        {
        public:
            LookupClubPostingsList() : ServerPacket(SMSG_CLUB_FINDER_LOOKUP_CLUB_POSTINGS_LIST, 5) { }

            WorldPacket const* Write() override;

            std::vector<ClubFinderPosting> Postings;
            uint8 Context = 0x60;
        };

        // Observed 8.3.7 application request: finder GUID, uint64 recruiting-spec
        // mask, then a 10-bit comment byte length and UTF-8 data.
        class ClubFinderRequestMembershipToClub final : public ClientPacket
        {
        public:
            ClubFinderRequestMembershipToClub(WorldPacket&& packet) : ClientPacket(CMSG_CLUB_FINDER_REQUEST_MEMBERSHIP_TO_CLUB, std::move(packet)) { }

            void Read() override;

            ObjectGuid ClubFinderGUID;
            uint64 SpecMask = 0;
            std::string Comment;
        };

        class ClubFinderRequestSubscribedClubPostingIDs final : public ClientPacket
        {
        public:
            ClubFinderRequestSubscribedClubPostingIDs(WorldPacket&& packet) : ClientPacket(CMSG_CLUB_FINDER_REQUEST_SUBSCRIBED_CLUB_POSTING_IDS, std::move(packet)) { }

            void Read() override;

            std::vector<uint64> ClubIDs;
        };

        struct ClubFinderClubPostingID
        {
            uint64 ClubID = 0;
            uint64 PostingID = 0;
        };

        class ClubFinderGetClubPostingIDsResponse final : public ServerPacket
        {
        public:
            ClubFinderGetClubPostingIDsResponse() : ServerPacket(SMSG_CLUB_FINDER_GET_CLUB_POSTING_IDS_RESPONSE, 4) { }

            WorldPacket const* Write() override;

            std::vector<ClubFinderClubPostingID> Entries;
        };

        class ClubFinderGetApplicantsList final : public ClientPacket
        {
        public:
            ClubFinderGetApplicantsList(WorldPacket&& packet) : ClientPacket(CMSG_CLUB_FINDER_GET_APPLICANTS_LIST, std::move(packet)) { }

            void Read() override;

            uint8 Context = 0x20;
        };

        struct ClubFinderApplicationUpdate
        {
            ObjectGuid ClubFinderGUID;
            ObjectGuid PlayerGUID;
            uint32 Closed = 0;
            uint64 LastUpdatedTime = 0;
            uint8 RequestStatus = RequestStatusPending;
        };

        class ClubFinderUpdateApplications final : public ServerPacket
        {
        public:
            ClubFinderUpdateApplications() : ServerPacket(SMSG_CLUB_FINDER_UPDATE_APPLICATIONS, 5) { }

            WorldPacket const* Write() override;

            std::vector<ClubFinderApplicationUpdate> Applications;
            uint8 Context = 0x20;
        };

        // Full applicant entry for the guild officer/leader applicant screen.
        // The public API fields are represented directly. Several fixed scalar
        // values in the captured record still have unknown semantics; the writer
        // intentionally preserves those proven retail values instead of assigning
        // invented meanings to them.
        struct ClubFinderApplicantInfo
        {
            ObjectGuid ClubFinderGUID;
            ObjectGuid PlayerGUID;
            uint32 Closed = 0;
            uint64 LastUpdatedTime = 0;
            uint8 RequestStatus = RequestStatusPending;
            uint8 Race = 0;
            uint8 Class = 0;
            uint8 Level = 0;
            uint32 ItemLevel = 0;
            uint64 SpecMask = 0;
            std::string Message;
        };

        class ReturnApplicantList final : public ServerPacket
        {
        public:
            ReturnApplicantList() : ServerPacket(SMSG_RETURN_APPLICANT_LIST, 18) { }

            WorldPacket const* Write() override;

            ObjectGuid ClubFinderGUID;
            std::vector<ClubFinderApplicantInfo> Applicants;
            uint8 Context = 0x20;
        };

        class ClubFinderRequestPendingClubsList final : public ClientPacket
        {
        public:
            ClubFinderRequestPendingClubsList(WorldPacket&& packet) : ClientPacket(CMSG_CLUB_FINDER_REQUEST_PENDING_CLUBS_LIST, std::move(packet)) { }

            void Read() override;

            uint8 Context = 0x20;
        };

        class ResponseCharacterApplicationList final : public ServerPacket
        {
        public:
            ResponseCharacterApplicationList() : ServerPacket(SMSG_CLUB_FINDER_RESPONSE_CHARACTER_APPLICATION_LIST, 5) { }

            WorldPacket const* Write() override;

            std::vector<ClubFinderApplicationUpdate> Applications;
            uint8 Context = 0x60;
        };

        class ClubFinderRespondToApplicant final : public ClientPacket
        {
        public:
            ClubFinderRespondToApplicant(WorldPacket&& packet) : ClientPacket(CMSG_CLUB_FINDER_RESPOND_TO_APPLICANT, std::move(packet)) { }

            void Read() override;

            ObjectGuid ClubFinderGUID;
            ObjectGuid PlayerGUID;
            uint8 Flags = 0;

            // Supplied retail captures prove 0x90 for guild-leader accept and
            // 0x10 for guild-leader decline.
            bool IsAccept() const { return Flags == 0x90; }
            bool IsDecline() const { return Flags == 0x10; }
        };

        class ClubFinderApplicationResponse final : public ClientPacket
        {
        public:
            ClubFinderApplicationResponse(WorldPacket&& packet) : ClientPacket(CMSG_CLUB_FINDER_APPLICATION_RESPONSE, std::move(packet)) { }

            void Read() override;

            ObjectGuid ClubFinderGUID;
            uint8 Flags = 0;

            // The supplied retail capture proves 0x2C for applicant acceptance.
            // Top 3 bits = response (retail 12.1 0x2C accept / 0x64 cancel; 8.3.7
            // client 0x2C accept / 0x6C decline-invitation). Low bits are context.
            bool IsAccept() const { return (Flags >> 5) == 1; }
            bool IsDecline() const { return (Flags >> 5) == 3; }
        };
    }
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::ClubFinder::ClubFinderPosting const& posting);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::ClubFinder::ClubFinderApplicationUpdate const& application);
ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::ClubFinder::ClubFinderApplicantInfo const& applicant);

#endif // ClubFinderPackets_h__
