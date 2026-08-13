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

#include "AnticheatData.h"

AnticheatData::AnticheatData()
{
    Reset();
}

AnticheatData::~AnticheatData()
{
}

void AnticheatData::Reset()
{
    ResetReports();
    _lastOpcode = 0;
    _lastMovementInfo = MovementInfo();
    _hasDailyReport = false;
}

void AnticheatData::ResetReports()
{
    _totalReports = 0;
    _average = 0;
    _creationTime = 0;
    for (uint8 i = 0; i < MAX_REPORT_TYPES; i++)
    {
        _typeReports[i] = 0;
        _tempReports[i] = 0;
        _tempReportsTimer[i] = 0;
    }
}

void AnticheatData::SetDailyReportState(bool b)
{
    _hasDailyReport = b;
}

bool AnticheatData::GetDailyReportState() const
{
    return _hasDailyReport;
}

void AnticheatData::SetLastOpcode(uint32 opcode)
{
    _lastOpcode = opcode;
}

void AnticheatData::SetPosition(float x, float y, float z, float o)
{
    _lastMovementInfo.pos = { x, y, z, o };
}

uint32 AnticheatData::GetLastOpcode() const
{
    return _lastOpcode;
}

MovementInfo const& AnticheatData::GetLastMovementInfo() const
{
    return _lastMovementInfo;
}

void AnticheatData::SetLastMovementInfo(MovementInfo const& moveInfo)
{
    _lastMovementInfo = moveInfo;
}

uint32 AnticheatData::GetTotalReports() const
{
    return _totalReports;
}

void AnticheatData::SetTotalReports(uint32 totalReports)
{
    _totalReports = totalReports;
}

void AnticheatData::SetTypeReports(uint32 type, uint32 amount)
{
    _typeReports[type] = amount;
}

uint32 AnticheatData::GetTypeReports(uint32 type) const
{
    return _typeReports[type];
}

float AnticheatData::GetAverage() const
{
    return _average;
}

void AnticheatData::SetAverage(float average)
{
    _average = average;
}

uint32 AnticheatData::GetCreationTime() const
{
    return _creationTime;
}

void AnticheatData::SetCreationTime(uint32 creationTime)
{
    _creationTime = creationTime;
}

void AnticheatData::SetTempReports(uint32 amount, uint8 type)
{
    _tempReports[type] = amount;
}

uint32 AnticheatData::GetTempReports(uint8 type) const
{
    return _tempReports[type];
}

void AnticheatData::SetTempReportsTimer(uint32 time, uint8 type)
{
    _tempReportsTimer[type] = time;
}

uint32 AnticheatData::GetTempReportsTimer(uint8 type) const
{
    return _tempReportsTimer[type];
}
