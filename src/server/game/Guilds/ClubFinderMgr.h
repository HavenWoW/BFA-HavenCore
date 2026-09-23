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
// ClubFinderMgr - data layer of the Club Finder (guild side).
//
// Owns recruitment postings (club_finder_posting) and applications
// (club_finder_application): loading, persistence, lookups and expiry.
// Session/opcode translation lives in Handlers/ClubFinderHandler.cpp; raw
// packet layout lives in Server/Packets/ClubFinderPackets.*. The legacy
// LF_GUILD compatibility path remains in GuildFinderHandler.cpp.
//
// Formerly GuildFinderMgr on the guild_finder_guild_settings /
// guild_finder_applicant tables; those were renamed (data kept) by the
// characters update that introduced this file. The file itself sits beside
// Guild.* like Draconic's ClubFinderMgr because guild-side Club Finder state
// is lifecycle data, not packet/wire code.
//
// Application lifetime:
//   - live (Pending/Approved/...) for ApplicationExpirySeconds after the
//     submission or last update, then expired;
//   - deleted ApplicationHistorySeconds after it stopped being live (decision
//     time for decided applications, expiry time for expired pending ones).
// -----------------------------------------------------------------------------

#ifndef HAVEN_CLUB_FINDER_MGR_H
#define HAVEN_CLUB_FINDER_MGR_H

#include "Common.h"
#include "ObjectGuid.h"
#include "SharedDefines.h"
#include <unordered_map>

class Guild;
class Player;

constexpr uint32 ApplicationExpirySeconds  = 7 * DAY;   // live window of an application
constexpr uint32 ApplicationHistorySeconds = 30 * DAY;  // kept as history after it ends
constexpr uint32 ApplicationCleanupIntervalSeconds = HOUR;

enum GuildFinderOptionsInterest
{
    INTEREST_QUESTING        = 0x01,
    INTEREST_DUNGEONS        = 0x02,
    INTEREST_RAIDS           = 0x04,
    INTEREST_PVP             = 0x08,
    INTEREST_ROLE_PLAYING    = 0x10,
    ALL_INTERESTS            = INTEREST_QUESTING | INTEREST_DUNGEONS | INTEREST_RAIDS | INTEREST_PVP | INTEREST_ROLE_PLAYING
};

enum GuildFinderOptionsAvailability
{
    AVAILABILITY_WEEKDAYS     = 0x1,
    AVAILABILITY_WEEKENDS     = 0x2,
    AVAILABILITY_ALWAYS       = AVAILABILITY_WEEKDAYS | AVAILABILITY_WEEKENDS
};

enum GuildFinderOptionsRoles
{
    GUILDFINDER_ROLE_TANK        = 0x1,
    GUILDFINDER_ROLE_HEALER      = 0x2,
    GUILDFINDER_ROLE_DPS         = 0x4,
    GUILDFINDER_ALL_ROLES        = GUILDFINDER_ROLE_TANK | GUILDFINDER_ROLE_HEALER | GUILDFINDER_ROLE_DPS
};

enum GuildFinderOptionsLevel
{
    ANY_FINDER_LEVEL       = 0x1,
    MAX_FINDER_LEVEL       = 0x2,
    ALL_GUILDFINDER_LEVELS = ANY_FINDER_LEVEL | MAX_FINDER_LEVEL
};

/// Holds all required informations about a membership request
struct MembershipRequest
{
    public:
        MembershipRequest();

        MembershipRequest(ObjectGuid const& playerGUID, ObjectGuid const& guildId, uint32 availability, uint32 classRoles, uint32 interests,
            std::string comment, time_t submitTime, uint64 specMask = 0, uint32 itemLevel = 0, uint8 status = 1, uint32 updateTime = 0);

        ObjectGuid const& GetGuildGuid() const   { return _guildId; }
        ObjectGuid const& GetPlayerGUID() const  { return _playerGUID; }
        uint8 GetAvailability() const             { return _availability; }
        uint8 GetClassRoles() const               { return _classRoles; }
        uint8 GetInterests() const                { return _interests; }
        time_t GetSubmitTime() const              { return _time; }
        time_t GetLastChangeTime() const          { return time_t(GetUpdateTime() ? GetUpdateTime() : uint32(_time)); }
        time_t GetExpiryTime() const              { return GetLastChangeTime() + ApplicationExpirySeconds; }
        /// Time at which the application stops being kept as history and is deleted.
        time_t GetPurgeTime() const               { return (IsActive() ? GetExpiryTime() : GetLastChangeTime()) + ApplicationHistorySeconds; }
        std::string const& GetComment() const     { return _comment; }
        uint64 GetSpecMask() const                { return _specMask; }
        uint32 GetItemLevel() const               { return _itemLevel; }
        uint8 GetStatus() const                   { return _status; }
        uint32 GetUpdateTime() const              { return _updateTime; }

        bool IsActive() const { return _status == 1 || _status == 2 || _status == 4; }
        bool IsClosed() const { return _status == 3 || _status == 5 || _status == 6 || _status == 7; }
        bool IsWithinApplicationWindow(time_t now = time(nullptr)) const { return now < GetExpiryTime(); }
        bool IsExpiredPlayerApplication(time_t now = time(nullptr)) const { return (_status == 1 || _status == 2 || _status == 3 || _status == 4) && !IsWithinApplicationWindow(now); }
        bool BlocksReapply(time_t now = time(nullptr)) const { return (IsActive() || _status == 3) && IsWithinApplicationWindow(now); }
        void SetStatus(uint8 status, uint32 updateTime) { _status = status; _updateTime = updateTime; }

    private:
        std::string _comment;

        ObjectGuid _guildId;
        ObjectGuid _playerGUID;

        uint8 _availability;
        uint8 _classRoles;
        uint8 _interests;
        uint64 _specMask = 0;
        uint32 _itemLevel = 0;
        uint8 _status = 1;
        uint32 _updateTime = 0;

        time_t _time;
};

/// Holds all informations about a player's finder settings. _NOT_ stored in database.
struct LFGuildPlayer
{
    public:
        LFGuildPlayer()
        {
            _roles = 0;
            _availability = 0;
            _interests = 0;
            _level = 0;
        }

        LFGuildPlayer(ObjectGuid const& guid, uint8 role, uint8 availability, uint8 interests, uint8 level)
        {
            _guid = guid;
            _roles = role;
            _availability = availability;
            _interests = interests;
            _level = level;
        }

        LFGuildPlayer(ObjectGuid const& guid, uint8 role, uint8 availability, uint8 interests, uint8 level, std::string& comment) : _comment(comment)
        {
            _guid = guid;
            _roles = role;
            _availability = availability;
            _interests = interests;
            _level = level;
        }

        ObjectGuid const& GetGUID() const         { return _guid; }
        uint8 GetClassRoles() const    { return _roles; }
        uint8 GetAvailability() const  { return _availability; }
        uint8 GetInterests() const     { return _interests; }
        uint8 GetLevel() const         { return _level; }
        std::string const& GetComment() const { return _comment; }

    private:
        std::string _comment;
        ObjectGuid _guid;
        uint8 _roles;
        uint8 _availability;
        uint8 _interests;
        uint8 _level;
};

/// Holds settings for a guild in the finder system. Saved to database.
struct LFGuildSettings : public LFGuildPlayer
{
    public:
        LFGuildSettings() : LFGuildPlayer(), _listed(false), _team(TEAM_ALLIANCE) {}

        LFGuildSettings(bool listed, TeamId team) : LFGuildPlayer(), _listed(listed), _team(team) {}

        LFGuildSettings(bool listed, TeamId team, ObjectGuid const& guid, uint8 role, uint8 availability, uint8 interests, uint8 level) :
            LFGuildPlayer(guid, role, availability, interests, level), _listed(listed), _team(team) {}

        LFGuildSettings(bool listed, TeamId team, ObjectGuid const& guid, uint8 role, uint8 availability, uint8 interests, uint8 level, std::string& comment) :
            LFGuildPlayer(guid, role, availability, interests, level, comment), _listed(listed), _team(team) {}

        LFGuildSettings(bool listed, TeamId team, ObjectGuid const& guid, uint8 role, uint8 availability, uint8 interests, uint8 level,
            std::string& comment, uint64 specMask, uint32 recruitmentFlags, uint32 minItemLevel, uint32 lastUpdatedTime) :
            LFGuildPlayer(guid, role, availability, interests, level, comment), _listed(listed), _team(team), _specMask(specMask),
            _recruitmentFlags(recruitmentFlags), _minItemLevel(minItemLevel), _lastUpdatedTime(lastUpdatedTime) {}

        bool IsListed() const      { return _listed; }
        bool IsActiveListing(time_t now = time(nullptr)) const
        {
            return _listed && _lastUpdatedTime && now < time_t(_lastUpdatedTime + 30 * 24 * 3600);
        }
        void SetListed(bool state) { _listed = state; }

        TeamId GetTeam() const     { return _team; }
        uint64 GetSpecMask() const { return _specMask; }
        uint32 GetRecruitmentFlags() const { return _recruitmentFlags; }
        uint32 GetMinItemLevel() const { return _minItemLevel; }
        uint32 GetLastUpdatedTime() const { return _lastUpdatedTime; }
    private:
        bool _listed;
        TeamId _team;
        uint64 _specMask = 0;
        uint32 _recruitmentFlags = 0;
        uint32 _minItemLevel = 0;
        uint32 _lastUpdatedTime = 0;
};

typedef std::unordered_map<ObjectGuid /* guildGuid */, LFGuildSettings> LFGuildStore;

class ClubFinderMgr
{
    private:
        ClubFinderMgr();
        ~ClubFinderMgr();

        LFGuildStore _guildSettings;
        std::unordered_map<ObjectGuid /* guildGUID */, uint32 /* postingId */> _postingIdByGuild;
        std::unordered_map<uint32 /* postingId */, ObjectGuid /* guildGUID */> _guildByPostingId;
        uint32 _maxPostingId = 0;

        std::unordered_map<ObjectGuid /*guildGUID*/, std::unordered_map<ObjectGuid /*playerGUID*/, MembershipRequest>> _membershipRequestsByGuild;
        std::unordered_map<ObjectGuid /*playerGUID*/, std::unordered_map<ObjectGuid /*guildGUID*/, MembershipRequest>> _membershipRequestsByPlayer;

        void LoadGuildSettings();
        void LoadMembershipRequests();
        uint32 EnsurePostingId(ObjectGuid const& guildGuid);

        time_t _lastApplicationCleanup = 0;

    public:
        void LoadFromDB();

        /**
         * @brief Deletes applications past their history window (see GetPurgeTime).
         * Runs at most once per ApplicationCleanupIntervalSeconds unless forced;
         * triggered by startup and by the applicant / pending-list requests.
         * Silent: sends no list updates (the requests that trigger it answer
         * with fresh lists anyway).
         */
        void CleanupExpiredApplications(bool force = false);

        /**
         * @brief Stores guild settings and begins an asynchronous database insert
         * @param guildGuid The guild's database guid.
         * @param LFGuildSettings The guild's settings storage.
         */
        void SetGuildSettings(ObjectGuid const& guildGuid, LFGuildSettings const& settings);

        /**
         * @brief Returns settings for a guild.
         * @param guildGuid The guild's database guid.
         */
        LFGuildSettings const& GetGuildSettings(ObjectGuid const& guildGuid) { return _guildSettings[guildGuid]; }

        /// Returns the stable Club Finder posting id for a guild, or 0 when none exists.
        uint32 GetPostingId(ObjectGuid const& guildGuid) const;

        /// Builds the single canonical BFA ClubFinder GUID used by every finder packet.
        /// Keep GUID layout decisions here so posting/application responses cannot diverge.
        ObjectGuid GetClubFinderGuid(Guild const* guild) const;

        /// Resolves a Club Finder posting id back to its owning guild GUID.
        ObjectGuid GetGuildGuidForPostingId(uint32 postingId) const;

        /// Returns the posting settings addressed by posting id, or nullptr.
        LFGuildSettings const* GetGuildSettingsByPostingId(uint32 postingId) const;

        /**
         * @brief Files a membership request to a guild
         * @param guildGuid The guild's database GUID.
         * @param MembershipRequest An object storing all data related to the request.
         */
        void AddMembershipRequest(ObjectGuid const& guildGuid, MembershipRequest const& request);

        /// Updates the persistent Club Finder application state (Pending/Approved/Joined/...).
        bool UpdateMembershipRequestStatus(ObjectGuid const& playerId, ObjectGuid const& guildId, uint8 status, uint32 updateTime);

        /// Called by Guild::AddMember. Finder applications are retained as history:
        /// the joined guild becomes Joined and other active applications become JoinedAnother.
        void OnPlayerJoinedGuild(ObjectGuid const& playerId, ObjectGuid const& guildId);

        MembershipRequest const* GetMembershipRequest(ObjectGuid const& playerId, ObjectGuid const& guildId) const;

        /**
         * @brief Removes all membership request from a player.
         * @param playerId The player's database guid whose application shall be deleted.
         */
        void RemoveAllMembershipRequestsFromPlayer(ObjectGuid const& playerId);

        /**
         * @brief Removes a membership request to a guild.
         * @param playerId The player's database guid whose application shall be deleted.
         * @param guildId  The guild's database guid
         */
        void RemoveMembershipRequest(ObjectGuid const& playerId, ObjectGuid const& guildId);

        /// Wipes everything related to a guild. Used when that guild is disbanded
        void DeleteGuild(ObjectGuid const& guildId);

        /**
         * @brief Returns a set of membership requests for a guild
         * @param guildGuid The guild's database guid.
         */
        std::unordered_map<ObjectGuid, MembershipRequest> const* GetAllMembershipRequestsForGuild(ObjectGuid const& guildGuid)
        {
            auto itr = _membershipRequestsByGuild.find(guildGuid);
            return itr != _membershipRequestsByGuild.end() ? &itr->second : nullptr;
        }

        /**
         * @brief Returns a list of membership requests for a player.
         * @param playerGuid The player's database guid.
         */
        std::vector<MembershipRequest const*> GetAllMembershipRequestsForPlayer(ObjectGuid const& playerGuid);

        /**
         * @brief Returns a store of guilds matching the settings provided, using bitmask operators.
         * @param settings The player's finder settings
         * @param teamId   The player's faction (TEAM_ALLIANCE or TEAM_HORDE)
         */
        std::vector<LFGuildSettings const*> GetGuildsMatchingSetting(LFGuildPlayer& settings, TeamId faction);

        /// Provided a player guid and a guild guid, determines if a pending request is filed with these keys.
        bool HasRequest(ObjectGuid const& playerId, ObjectGuid const& guildId);

        /// Counts the amount of pending membership requests, given the player's db guid.
        uint8 CountRequestsFromPlayer(ObjectGuid const& playerId);

        static void SendApplicantListUpdate(Guild* guild);
        static void SendMembershipRequestListUpdate(Player* player);

        static ClubFinderMgr* instance();
};

#define sClubFinderMgr ClubFinderMgr::instance()

#endif // HAVEN_CLUB_FINDER_MGR_H
