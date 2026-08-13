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

#ifndef SC_ACMGR_H
#define SC_ACMGR_H

#include "Common.h"
#include "SharedDefines.h"
#include "ScriptMgr.h"
#include "AnticheatData.h"
#include "Chat.h"
#include "Player.h"

class Player;

enum ReportTypes
{
    SPEED_HACK_REPORT = 0,
    FLY_HACK_REPORT,
    WALK_WATER_HACK_REPORT,
    JUMP_HACK_REPORT,
    TELEPORT_PLANE_HACK_REPORT,
    CLIMB_HACK_REPORT,
};

enum DetectionTypes
{
    SPEED_HACK_DETECTION            = 1,
    FLY_HACK_DETECTION              = 2,
    WALK_WATER_HACK_DETECTION       = 4,
    JUMP_HACK_DETECTION             = 8,
    TELEPORT_PLANE_HACK_DETECTION   = 16,
    CLIMB_HACK_DETECTION            = 32
};

class TC_GAME_API AnticheatMgr
{
    AnticheatMgr();
    ~AnticheatMgr();

    public:
    static AnticheatMgr* instance()
        {
           static AnticheatMgr* instance = new AnticheatMgr();
           return instance;
        }

        void StartHackDetection(Player* player, MovementInfo const& movementInfo, uint32 opcode);
        void SavePlayerData(Player* player);

        void StartScripts();

        void HandlePlayerLogin(Player* player);
        void HandlePlayerLogout(Player* player);

        uint32 GetTotalReports(Player* player);
        float GetAverage(Player* player);
        uint32 GetTypeReports(Player* player, uint8 type);

        void AnticheatGlobalCommand(ChatHandler* handler);
        void AnticheatDeleteCommand(Player* player);
        void AnticheatDeleteAllCommand();

        void ResetDailyReportStates();
    private:
        void SpeedHackDetection(Player* player, AnticheatData& data, MovementInfo const& movementInfo);
        void FlyHackDetection(Player* player, AnticheatData& data);
        void WalkOnWaterHackDetection(Player* player, AnticheatData& data);
        void JumpHackDetection(Player* player, AnticheatData& data, uint32 opcode);
        void TeleportPlaneHackDetection(Player* player, AnticheatData& data, MovementInfo const& movementInfo);
        void ClimbHackDetection(Player* player, AnticheatData& data, MovementInfo const& movementInfo, uint32 opcode);

        void BuildReport(Player* player, AnticheatData& data, uint8 reportType);

        bool MustCheckTempReports(uint8 type);
};

#define sAnticheatMgr AnticheatMgr::instance()

#endif
