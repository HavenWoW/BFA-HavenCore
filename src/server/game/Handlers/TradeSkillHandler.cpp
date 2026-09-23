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

// Profession ("trade") chat links: |Htrade:<PlayerGUID>:<SpellID>:<SkillLineID>|h
//
// Wire layout (BFA 8.3.7), per WowPacketParser's only parser for these opcodes
// (V6_0_2 module, not overridden by any later module), TrinityCore's older
// writer, and the field set of a retail 12.1 capture (which adds three uint32
// before the arrays in later builds):
//   CMSG_SHOW_TRADE_SKILL:          PackedGuid PlayerGUID, uint32 SpellID, uint32 SkillLineID
//   SMSG_SHOW_TRADE_SKILL_RESPONSE: PackedGuid PlayerGUID, uint32 SpellID,
//                                   uint32 x4 counts, then int32 arrays:
//                                   SkillLineIDs, SkillRanks, SkillMaxRanks, KnownAbilitySpellIDs
// Content mirrors retail: the root line plus every tier line (rank/max 0 when
// not learned) and every known recipe of that profession.

#include "WorldSession.h"
#include "DB2Stores.h"
#include "Guild.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SpellMgr.h"
#include "WorldPacket.h"
#include <algorithm>
#include <unordered_set>
#include <vector>

void WorldSession::HandleShowTradeSkill(WorldPacket& recvData)
{
    ObjectGuid playerGuid;
    uint32 spellId = 0;
    uint32 skillLineId = 0;
    recvData >> playerGuid >> spellId >> skillLineId;

    uint32 const rootSkillId = Guild::GetRootProfessionSkillLine(skillLineId);
    Player* target = ObjectAccessor::FindConnectedPlayer(playerGuid);
    if (!rootSkillId || !target || !target->HasSkill(rootSkillId))
    {
        return;
    }

    std::vector<uint32> skillLines{ rootSkillId };
    if (std::vector<SkillLineEntry const*> const* children = sDB2Manager.GetSkillLinesForParentSkill(rootSkillId))
    {
        std::vector<uint32> tiers;
        for (SkillLineEntry const* child : *children)
            if (child)
                tiers.push_back(child->ID);

        std::sort(tiers.begin(), tiers.end());
        skillLines.insert(skillLines.end(), tiers.begin(), tiers.end());
    }

    std::vector<int32> ranks;
    std::vector<int32> maxRanks;
    for (uint32 line : skillLines)
    {
        bool const known = target->HasSkill(line);
        ranks.push_back(known ? int32(target->GetPureSkillValue(line)) : 0);
        maxRanks.push_back(known ? int32(target->GetPureMaxSkillValue(line)) : 0);
    }

    std::unordered_set<uint32> knownSpells;
    Guild::AppendLiveKnownSpells(target, knownSpells);

    std::vector<int32> abilitySpells;
    for (uint32 knownSpell : knownSpells)
    {
        SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(knownSpell);
        for (SkillLineAbilityMap::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
        {
            if (itr->second && Guild::GetRootProfessionSkillLine(uint32(itr->second->SkillLine)) == rootSkillId)
            {
                abilitySpells.push_back(int32(knownSpell));
                break;
            }
        }
    }

    WorldPacket data(SMSG_SHOW_TRADE_SKILL_RESPONSE, 16 + 4 + 16 + (skillLines.size() * 3 + abilitySpells.size()) * 4);
    data << playerGuid;
    data << uint32(spellId);
    data << uint32(skillLines.size());
    data << uint32(ranks.size());
    data << uint32(maxRanks.size());
    data << uint32(abilitySpells.size());
    for (uint32 line : skillLines)
        data << int32(line);
    for (int32 rank : ranks)
        data << rank;
    for (int32 maxRank : maxRanks)
        data << maxRank;
    for (int32 ability : abilitySpells)
        data << ability;

    SendPacket(&data);
}
