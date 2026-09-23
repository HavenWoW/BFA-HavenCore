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

#include "GuildRecipes.h"
#include "DB2Stores.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include <algorithm>

namespace GuildRecipes
{
    uint32 GetRootProfessionSkillLine(uint32 skillId, int32* step /*= nullptr*/)
    {
        SkillLineEntry const* skillLine = sSkillLineStore.LookupEntry(skillId);
        if (!skillLine)
            return 0;

        if (step)
            *step = std::max<int32>(1, skillLine->ParentTierIndex);

        uint32 rootSkillId = skillId;
        SkillLineEntry const* rootSkill = skillLine;
        while (rootSkill && rootSkill->ParentSkillLineID)
        {
            rootSkillId = rootSkill->ParentSkillLineID;
            rootSkill = sSkillLineStore.LookupEntry(rootSkillId);
        }

        return rootSkill && rootSkill->CategoryID == SKILL_CATEGORY_PROFESSION ? rootSkillId : 0;
    }

    // The guild recipe bit arrays are indexed by SkillLineAbility.UniqueBit: the
    // client decodes bit N as the ability whose UniqueBit == N. Verified in-game
    // (8.3.7): bits sent at 55,58,588,596,603,604,611 rendered exactly the seven
    // recipes whose UniqueBit have those values. CMSG_GUILD_QUERY_MEMBERS_FOR_RECIPE
    // also carries UniqueBit, which the client takes from these masks.
    bool SetAbilityRecipeBit(SkillLineAbilityEntry const* ability, GuildRecipeMask& mask)
    {
        if (!ability || ability->UniqueBit <= 0)
            return false;

        uint32 bit = uint32(ability->UniqueBit);
        if (bit / 8 >= mask.size())
            return false;

        mask[bit / 8] |= uint8(1u << (bit % 8));
        return true;
    }

    bool SetGuildRecipeBit(uint32 skillLineId, uint32 spellId, GuildRecipeMask& mask)
    {
        SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
        for (SkillLineAbilityMap::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
        {
            SkillLineAbilityEntry const* ability = itr->second;
            if (!ability)
                continue;

            uint32 abilityLine = ability->SkillupSkillLineID ? uint32(ability->SkillupSkillLineID) : uint32(ability->SkillLine);
            if (abilityLine == skillLineId || uint32(ability->SkillLine) == skillLineId)
                return SetAbilityRecipeBit(ability, mask);
        }

        return false;
    }


    void AddSpellToGuildRecipeMasks(uint32 spellId, std::unordered_map<uint32, GuildRecipeMask>& masks)
    {
        SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
        for (SkillLineAbilityMap::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
        {
            SkillLineAbilityEntry const* ability = itr->second;
            if (!ability)
                continue;

            // Key by the ability's own SkillLine: that is the line whose UniqueBit
            // numbering the client decodes against. In 8.3 every recipe sits on the
            // root line (e.g. 197 Tailoring) with a per-tier SkillupSkillLineID;
            // keying by SkillupSkillLineID split a profession into ~9 masks and the
            // client, reading the root mask, saw ~4% of recipes as learned.
            uint32 skillLineId = uint32(ability->SkillLine);
            if (!GetRootProfessionSkillLine(skillLineId))
                continue;

            SetGuildRecipeBit(skillLineId, spellId, masks[skillLineId]);
        }
    }
}
