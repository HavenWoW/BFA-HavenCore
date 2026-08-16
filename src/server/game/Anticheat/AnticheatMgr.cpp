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

#include "AnticheatMgr.h"
#include "AnticheatScripts.h"
#include "ChatPackets.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "Packet.h"
#include "Optional.h"
#include "PacketUtilities.h"
#include "Player.h"
#include "World.h"
#include "WorldSession.h"
#include <cmath>

namespace
{
    constexpr uint32 DEEPRUN_TRAM_MAP_ID = 369;
    constexpr float CLIMB_MAX_SLOPE_DEGREES = 85.0f;
    constexpr float CLIMB_MAX_SLOPE_RADIANS = CLIMB_MAX_SLOPE_DEGREES * float(M_PI) / 180.0f;
    constexpr float CLIMB_MIN_DISTANCE_2D = 0.1f;
    constexpr uint32 TEMP_REPORT_WINDOW_MS = 3000;
    constexpr uint32 TEMP_REPORTS_BEFORE_REPORT = 3;
}

AnticheatMgr::AnticheatMgr()
{
}

AnticheatMgr::~AnticheatMgr()
{
}

void AnticheatMgr::JumpHackDetection(Player* player, AnticheatData& data, uint32 opcode)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & JUMP_HACK_DETECTION) == 0)
        return;

    if (data.GetLastOpcode() == CMSG_MOVE_JUMP && opcode == CMSG_MOVE_JUMP)
    {
        BuildReport(player, data, JUMP_HACK_REPORT);
        TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Jump-Hack detected player GUID (low) " UI64FMTD, player->GetGUID().GetCounter());
    }
}

void AnticheatMgr::WalkOnWaterHackDetection(Player* player, AnticheatData& data)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & WALK_WATER_HACK_DETECTION) == 0)
        return;

    if (!data.GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
        return;

    // if we are a ghost we can walk on water
    if (!player->IsAlive())
        return;

    if (player->HasAuraType(SPELL_AURA_FEATHER_FALL) ||
        player->HasAuraType(SPELL_AURA_SAFE_FALL) ||
        player->HasAuraType(SPELL_AURA_WATER_WALK))
        return;

    TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Walk on Water - Hack detected player GUID (low) " UI64FMTD, player->GetGUID().GetCounter());
    BuildReport(player, data, WALK_WATER_HACK_REPORT);
}

void AnticheatMgr::FlyHackDetection(Player* player, AnticheatData& data)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & FLY_HACK_DETECTION) == 0)
        return;

    if (!data.GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_FLYING))
        return;

    if (player->HasAuraType(SPELL_AURA_FLY) ||
        player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) ||
        player->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED))
        return;

    TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Fly-Hack detected player GUID (low) " UI64FMTD, player->GetGUID().GetCounter());
    BuildReport(player, data, FLY_HACK_REPORT);
}

void AnticheatMgr::TeleportPlaneHackDetection(Player* player, AnticheatData& data, MovementInfo const& movementInfo)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & TELEPORT_PLANE_HACK_DETECTION) == 0)
        return;

    if (data.GetLastMovementInfo().pos.GetPositionZ() != 0 ||
        movementInfo.pos.GetPositionZ() != 0)
        return;

    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING))
        return;

    float x, y, z;
    player->GetPosition(x, y, z);
    float ground_Z = player->GetMap()->GetHeight(player->GetPhaseShift(), x, y, z);
    float z_diff = fabs(ground_Z - z);

    // we are not really walking there
    if (z_diff > 1.0f)
    {
        TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Teleport To Plane - Hack detected player GUID (low) " UI64FMTD, player->GetGUID().GetCounter());
        BuildReport(player, data, TELEPORT_PLANE_HACK_REPORT);
    }
}

void AnticheatMgr::StartHackDetection(Player* player, MovementInfo const& movementInfo, uint32 opcode)
{
    if (!sWorld->getBoolConfig(CONFIG_ANTICHEAT_ENABLE))
        return;

    if (player->IsGameMaster() || player->GetSession()->GetSecurity() > SEC_PLAYER)
        return;

    AnticheatData& data = player->GetSession()->GetAnticheatData();

    if (player->IsInFlight() || player->GetTransport() || player->GetVehicle())
    {
        data.SetLastMovementInfo(movementInfo);
        data.SetLastOpcode(opcode);
        return;
    }

    SpeedHackDetection(player, data, movementInfo);
    FlyHackDetection(player, data);
    WalkOnWaterHackDetection(player, data);
    JumpHackDetection(player, data, opcode);
    TeleportPlaneHackDetection(player, data, movementInfo);
    ClimbHackDetection(player, data, movementInfo, opcode);

    data.SetLastMovementInfo(movementInfo);
    data.SetLastOpcode(opcode);
}

// basic detection
void AnticheatMgr::ClimbHackDetection(Player* player, AnticheatData& data, MovementInfo const& movementInfo, uint32 opcode)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & CLIMB_HACK_DETECTION) == 0)
        return;

    if (opcode != CMSG_MOVE_HEARTBEAT ||
        data.GetLastOpcode() != CMSG_MOVE_HEARTBEAT)
        return;

    // in this case we don't care if they are "legal" flags, they are handled in another parts of the Anticheat Manager.
    if (player->IsInWater() ||
        player->IsFlying() ||
        player->IsFalling())
        return;

    Position const& lastPos = data.GetLastMovementInfo().pos;

    float deltaZ = std::fabs(lastPos.GetPositionZ() - movementInfo.pos.GetPositionZ());
    float deltaXY = movementInfo.pos.GetExactDist2d(lastPos);

    if (deltaXY < CLIMB_MIN_DISTANCE_2D)
        return;

    float angle = std::atan2(deltaZ, deltaXY);

    if (angle > CLIMB_MAX_SLOPE_RADIANS)
    {
        TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Climb-Hack detected player GUID (low) " UI64FMTD, player->GetGUID().GetCounter());
        BuildReport(player, data, CLIMB_HACK_REPORT);
    }
}

void AnticheatMgr::SpeedHackDetection(Player* player, AnticheatData& data, MovementInfo const& movementInfo)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & SPEED_HACK_DETECTION) == 0)
        return;

    // We also must check the map because the movementFlag can be modified by the client.
    // If we just check the flag, they could always add that flag and always skip the speed hacking detection.
    if (!movementInfo.transport.guid.IsEmpty() && player->GetMapId() == DEEPRUN_TRAM_MAP_ID)
        return;

    uint32 distance2D = (uint32)movementInfo.pos.GetExactDist2d(&data.GetLastMovementInfo().pos);
    uint8 moveType = 0;

    // we need to know HOW is the player moving
    // TO-DO: Should we check the incoming movement flags?
    if (player->HasUnitMovementFlag(MOVEMENTFLAG_SWIMMING))
        moveType = MOVE_SWIM;
    else if (player->IsFlying())
        moveType = MOVE_FLIGHT;
    else if (player->HasUnitMovementFlag(MOVEMENTFLAG_WALKING))
        moveType = MOVE_WALK;
    else
        moveType = MOVE_RUN;

    // how many yards the player can do in one sec.
    uint32 speedRate = (uint32)(player->GetSpeed(UnitMoveType(moveType)) + movementInfo.jump.xyspeed);

    // how long the player took to move to here.
    uint32 timeDiff = getMSTimeDiff(data.GetLastMovementInfo().time, movementInfo.time);

    if (!timeDiff)
        timeDiff = 1;

    // this is the distance doable by the player in 1 sec, using the time done to move to this point.
    uint32 clientSpeedRate = distance2D * IN_MILLISECONDS / timeDiff;

    // we did the (uint32) cast to accept a margin of tolerance
    if (clientSpeedRate > speedRate)
    {
        BuildReport(player, data, SPEED_HACK_REPORT);
        TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Speed-Hack detected player GUID (low) " UI64FMTD, player->GetGUID().GetCounter());
    }
}

void AnticheatMgr::StartScripts()
{
    new AnticheatScripts();
}

void AnticheatMgr::HandlePlayerLogin(Player* player)
{
    AnticheatData& data = player->GetSession()->GetAnticheatData();
    data.Reset();

    ObjectGuid guid = player->GetGUID();

    // we must delete this to prevent errors in case of crash
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ANTICHEAT_REPORT_STATUS);
    stmt->setUInt64(0, guid.GetCounter());
    CharacterDatabase.Execute(stmt);

    // we initialize the pos of lastMovementPosition var.
    data.SetPosition(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ANTICHEAT_DAILY_REPORT);
    stmt->setUInt64(0, guid.GetCounter());

    player->GetSession()->GetQueryProcessor().AddCallback(
        CharacterDatabase.AsyncQuery(stmt).WithPreparedCallback([guid](PreparedQueryResult result)
    {
        if (!result)
            return;

        if (Player* found = ObjectAccessor::FindConnectedPlayer(guid))
            found->GetSession()->GetAnticheatData().SetDailyReportState(true);
    }));
}

void AnticheatMgr::HandlePlayerLogout(Player* player)
{
    // TO-DO Make a table that stores the cheaters of the day, with more detailed information.

    // We must also delete it at logout to prevent have data of offline players in the db when we query the database (IE: The GM Command)
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ANTICHEAT_REPORT_STATUS);
    stmt->setUInt64(0, player->GetGUID().GetCounter());
    CharacterDatabase.Execute(stmt);

    player->GetSession()->GetAnticheatData().Reset();
}

void AnticheatMgr::SavePlayerData(Player* player)
{
    AnticheatData const& data = player->GetSession()->GetAnticheatData();

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_ANTICHEAT_REPORT_STATUS);
    BindReportStatus(stmt, player, data);
    CharacterDatabase.Execute(stmt);
}

void AnticheatMgr::BindReportStatus(CharacterDatabasePreparedStatement* stmt, Player* player, AnticheatData const& data)
{
    stmt->setUInt64(0, player->GetGUID().GetCounter());
    stmt->setFloat(1, data.GetAverage());
    stmt->setUInt32(2, data.GetTotalReports());
    stmt->setUInt32(3, data.GetTypeReports(SPEED_HACK_REPORT));
    stmt->setUInt32(4, data.GetTypeReports(FLY_HACK_REPORT));
    stmt->setUInt32(5, data.GetTypeReports(JUMP_HACK_REPORT));
    stmt->setUInt32(6, data.GetTypeReports(WALK_WATER_HACK_REPORT));
    stmt->setUInt32(7, data.GetTypeReports(TELEPORT_PLANE_HACK_REPORT));
    stmt->setUInt32(8, data.GetTypeReports(CLIMB_HACK_REPORT));
    stmt->setUInt32(9, data.GetCreationTime());
}

uint32 AnticheatMgr::GetTotalReports(Player* player)
{
    return player->GetSession()->GetAnticheatData().GetTotalReports();
}

float AnticheatMgr::GetAverage(Player* player)
{
    return player->GetSession()->GetAnticheatData().GetAverage();
}

uint32 AnticheatMgr::GetTypeReports(Player* player, uint8 type)
{
    return player->GetSession()->GetAnticheatData().GetTypeReports(type);
}

bool AnticheatMgr::MustCheckTempReports(uint8 type)
{
    if (type == JUMP_HACK_REPORT)
        return false;

    return true;
}

void AnticheatMgr::BuildReport(Player* player, AnticheatData& data, uint8 reportType)
{
    if (MustCheckTempReports(reportType))
    {
        uint32 actualTime = getMSTime();

        if (!data.GetTempReportsTimer(reportType))
            data.SetTempReportsTimer(actualTime, reportType);

        if (getMSTimeDiff(data.GetTempReportsTimer(reportType), actualTime) < TEMP_REPORT_WINDOW_MS)
        {
            data.SetTempReports(data.GetTempReports(reportType) + 1, reportType);

            if (data.GetTempReports(reportType) < TEMP_REPORTS_BEFORE_REPORT)
                return;
        }
        else
        {
            data.SetTempReportsTimer(actualTime, reportType);
            data.SetTempReports(1, reportType);
            return;
        }
    }

    // generating creationTime for average calculation
    if (!data.GetTotalReports())
        data.SetCreationTime(getMSTime());

    // increasing total_reports
    data.SetTotalReports(data.GetTotalReports() + 1);
    // increasing specific cheat report
    data.SetTypeReports(reportType, data.GetTypeReports(reportType) + 1);

    // diff time for average calculation
    uint32 diffTime = getMSTimeDiff(data.GetCreationTime(), getMSTime()) / IN_MILLISECONDS;

    if (diffTime > 0)
    {
        // Average == Reports per second
        float average = float(data.GetTotalReports()) / float(diffTime);
        data.SetAverage(average);
    }

    if (sWorld->getIntConfig(CONFIG_ANTICHEAT_MAX_REPORTS_FOR_DAILY_REPORT) < data.GetTotalReports())
    {
        if (!data.GetDailyReportState())
        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_ANTICHEAT_DAILY_REPORT);
            BindReportStatus(stmt, player, data);
            CharacterDatabase.Execute(stmt);
            data.SetDailyReportState(true);
        }
    }

    if (data.GetTotalReports() > sWorld->getIntConfig(CONFIG_ANTICHEAT_REPORTS_INGAME_NOTIFICATION))
    {
        std::string str = "";
        str = "|cFFFFFC00[AC]|cFF00FFFF[|cFF60FF00" + std::string(player->GetName().c_str()) + "|cFF00FFFF] Possible cheater!";
        sWorld->SendGlobalGMMessage(WorldPackets::Chat::PrintNotification(str).Write());
    }
}

void AnticheatMgr::AnticheatGlobalCommand(ChatHandler* handler)
{
    // MySQL will sort all for us, anyway this is not the best way we must only save the anticheat data not whole player's data!.
    ObjectAccessor::SaveAllPlayers();

    PreparedQueryResult resultDB = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_SEL_ANTICHEAT_LOWEST_AVERAGES));
    if (!resultDB)
    {
        handler->PSendSysMessage("No players found.");
        return;
    } else
    {
        handler->SendSysMessage("=============================");
        handler->PSendSysMessage("Players with the lowest averages:");
        do
        {
            Field *fieldsDB = resultDB->Fetch();

            uint32 guid = fieldsDB[0].GetUInt32();
            float average = fieldsDB[1].GetFloat();
            uint32 total_reports = fieldsDB[2].GetUInt32();

            if (Player* player = ObjectAccessor::FindPlayerByLowGUID(guid))
                handler->PSendSysMessage("Player: %s Average: %f Total Reports: %u",player->GetName().c_str(),average,total_reports);

        } while (resultDB->NextRow());
    }

    resultDB = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_SEL_ANTICHEAT_MOST_REPORTS));

    // this should never happen
    if (!resultDB)
    {
        handler->PSendSysMessage("No players found.");
        return;
    } else
    {
        handler->SendSysMessage("=============================");
        handler->PSendSysMessage("Players with the more reports:");
        do
        {
            Field *fieldsDB = resultDB->Fetch();

            uint32 guid = fieldsDB[0].GetUInt32();
            float average = fieldsDB[1].GetFloat();
            uint32 total_reports = fieldsDB[2].GetUInt32();

            if (Player* player = ObjectAccessor::FindPlayerByLowGUID(guid))
                handler->PSendSysMessage("Player: %s Total Reports: %u Average: %f",player->GetName().c_str(),total_reports,average);

        } while (resultDB->NextRow());
    }
}

void AnticheatMgr::AnticheatDeleteCommand(Player* player)
{
    player->GetSession()->GetAnticheatData().ResetReports();

    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ANTICHEAT_REPORT_STATUS);
    stmt->setUInt64(0, player->GetGUID().GetCounter());
    CharacterDatabase.Execute(stmt);
}

void AnticheatMgr::AnticheatDeleteAllCommand()
{
    for (auto const& sessionPair : sWorld->GetAllSessions())
        sessionPair.second->GetAnticheatData().ResetReports();

    CharacterDatabase.Execute(CharacterDatabase.GetPreparedStatement(CHAR_DEL_ANTICHEAT_REPORT_STATUS_ALL));
}

void AnticheatMgr::ResetDailyReportStates()
{
    for (auto const& sessionPair : sWorld->GetAllSessions())
        sessionPair.second->GetAnticheatData().SetDailyReportState(false);
}
