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

#ifndef GuildFinderPackets_h__
#define GuildFinderPackets_h__

#include "Packet.h"
#include "ObjectGuid.h"
#include "Optional.h"

namespace WorldPackets
{
    namespace GuildFinder
    {
        class LFGuildAddRecruit final : public ClientPacket
        {
        public:
            LFGuildAddRecruit(WorldPacket&& packet) : ClientPacket(CMSG_LF_GUILD_ADD_RECRUIT, std::move(packet)) { }

            void Read() override;

            ObjectGuid GuildGUID;
            int32 Availability = 0;
            int32 ClassRoles = 0;
            int32 PlayStyle = 0;
            std::string Comment;
        };

        class LFGuildApplicationsListChanged final : public ServerPacket
        {
        public:
            LFGuildApplicationsListChanged() : ServerPacket(SMSG_LF_GUILD_APPLICATIONS_LIST_CHANGED, 0) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        class LFGuildApplicantListChanged final : public ServerPacket
        {
        public:
            LFGuildApplicantListChanged() : ServerPacket(SMSG_LF_GUILD_APPLICANT_LIST_CHANGED, 0) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };

        class LFGuildBrowse final : public ClientPacket
        {
        public:
            LFGuildBrowse(WorldPacket&& packet) : ClientPacket(CMSG_LF_GUILD_BROWSE, std::move(packet)) { }

            void Read() override;

            int32 CharacterLevel = 0;
            int32 Availability = 0;
            int32 ClassRoles = 0;
            int32 PlayStyle = 0;
        };

        struct LFGuildBrowseData
        {
            std::string GuildName;
            ObjectGuid GuildGUID;
            uint32 GuildVirtualRealm = 0;
            int32 GuildMembers = 0;
            int32 GuildAchievementPoints = 0;
            int32 PlayStyle = 0;
            int32 Availability = 0;
            int32 ClassRoles = 0;
            int32 LevelRange = 0;
            int32 EmblemStyle = 0;
            int32 EmblemColor = 0;
            int32 BorderStyle = 0;
            int32 BorderColor = 0;
            int32 Background = 0;
            std::string Comment;
            int8 Cached = 0;
            int8 MembershipRequested = 0;
        };

        class LFGuildBrowseResult final : public ServerPacket
        {
        public:
            LFGuildBrowseResult() : ServerPacket(SMSG_LF_GUILD_BROWSE) { }

            WorldPacket const* Write() override;

            std::vector<LFGuildBrowseData> Post;
        };

        class LFGuildDeclineRecruit final : public ClientPacket
        {
        public:
            LFGuildDeclineRecruit(WorldPacket&& packet) : ClientPacket(CMSG_LF_GUILD_DECLINE_RECRUIT, std::move(packet)) { }

            void Read() override;

            ObjectGuid RecruitGUID;
        };

        class LFGuildGetApplications final : public ClientPacket
        {
        public:
            LFGuildGetApplications(WorldPacket&& packet) : ClientPacket(CMSG_LF_GUILD_GET_APPLICATIONS, std::move(packet)) { }

            void Read() override { }
        };

        struct LFGuildApplicationData
        {
            ObjectGuid GuildGUID;
            uint32 GuildVirtualRealm = 0;
            std::string GuildName;
            int32 ClassRoles = 0;
            int32 PlayStyle = 0;
            int32 Availability = 0;
            uint32 SecondsSinceCreated = 0;
            uint32 SecondsUntilExpiration = 0;
            std::string Comment;
        };

        class LFGuildApplications final : public ServerPacket
        {
        public:
            LFGuildApplications() : ServerPacket(SMSG_LF_GUILD_APPLICATIONS) { }

            WorldPacket const* Write() override;

            std::vector<LFGuildApplicationData> Application;
            int32 NumRemaining = 0;
        };

        class LFGuildGetGuildPost final : public ClientPacket
        {
        public:
            LFGuildGetGuildPost(WorldPacket&& packet) : ClientPacket(CMSG_LF_GUILD_GET_GUILD_POST, std::move(packet)) { }

            void Read() override { }
        };

        struct GuildPostData
        {
            bool Active = false;
            int32 PlayStyle = 0;
            int32 Availability = 0;
            int32 ClassRoles = 0;
            int32 LevelRange = 0;
            int32 SecondsRemaining = 0;
            std::string Comment;
        };

        class LFGuildPost final : public ServerPacket
        {
        public:
            LFGuildPost() : ServerPacket(SMSG_LF_GUILD_POST, 1 + 1 + 4 + 4 + 4 + 4 + 4 + 255) { }

            WorldPacket const* Write() override;

            Optional<GuildPostData> Post;
        };

        class LFGuildGetRecruits final : public ClientPacket
        {
        public:
            LFGuildGetRecruits(WorldPacket&& packet) : ClientPacket(CMSG_LF_GUILD_GET_RECRUITS, std::move(packet)) { }

            void Read() override;

            uint32 LastUpdate = 0;
        };

        struct LFGuildRecruitData
        {
            ObjectGuid RecruitGUID;
            std::string Name;
            uint32 RecruitVirtualRealm = 0;
            std::string Comment;
            int32 CharacterClass = 0;
            int32 CharacterGender = 0;
            int32 CharacterLevel = 0;
            int32 ClassRoles = 0;
            int32 PlayStyle = 0;
            int32 Availability = 0;
            uint32 SecondsSinceCreated = 0;
            uint32 SecondsUntilExpiration = 0;
        };

        class LFGuildRecruits final : public ServerPacket
        {
        public:
            LFGuildRecruits() : ServerPacket(SMSG_LF_GUILD_RECRUITS) { }

            WorldPacket const* Write() override;

            std::vector<LFGuildRecruitData> Recruits;
            time_t UpdateTime = time_t(0);
        };

        class LFGuildRemoveRecruit final : public ClientPacket
        {
        public:
            LFGuildRemoveRecruit(WorldPacket&& packet) : ClientPacket(CMSG_LF_GUILD_REMOVE_RECRUIT, std::move(packet)) { }

            void Read() override;

            ObjectGuid GuildGUID;
        };

        class LFGuildSetGuildPost final : public ClientPacket
        {
        public:
            LFGuildSetGuildPost(WorldPacket&& packet) : ClientPacket(CMSG_LF_GUILD_SET_GUILD_POST, std::move(packet)) { }

            void Read() override;

            int32 Availability = 0;
            int32 PlayStyle = 0;
            int32 ClassRoles = 0;
            int32 LevelRange = 0;
            bool Active = false;
            std::string Comment;
        };

        // BFA 8.3.7 Club Finder wire protocol (CMSG 0x3717).
        class ClubFinderPost final : public ClientPacket
        {
        public:
            ClubFinderPost(WorldPacket&& packet) : ClientPacket(CMSG_CLUB_FINDER_POST, std::move(packet)) { }

            void Read() override;

            uint64 ClubID = 0;
            uint64 SpecMask = 0;
            uint32 RecruitmentFlags = 0;
            uint32 MinItemLevel = 0;
            // Some retail builds carry one additional uint32 between
            // MinItemLevel and the variable strings. Keep it opaque and only
            // consume it when packet size proves that word is present.
            uint32 Unknown32 = 0;
            uint8 Unknown5 = 0;
            std::string Name;
            std::string Description;
        };

        class ClubFinderResponsePostRecruitmentMessage final : public ServerPacket
        {
        public:
            ClubFinderResponsePostRecruitmentMessage() : ServerPacket(SMSG_CLUB_FINDER_RESPONSE_POST_RECRUITMENT_MESSAGE, 14) { }

            WorldPacket const* Write() override;

            ObjectGuid ClubFinderGUID;
            uint8 Flags = 0;
        };
    }
}

#endif // GuildFinderPackets_h__
