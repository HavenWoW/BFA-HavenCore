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
// Guild profession recipe masks (Guild <Profession> view, member profession
// panel, "View crafters").
//
// [8.3.7] Bit N of a mask = the SkillLineAbility whose UniqueBit == N, keyed by
// the ability's own SkillLine (the root profession line in 8.3). Verified
// in-game; see SetAbilityRecipeBit / AddSpellToGuildRecipeMasks.
//
// NOTE: Guild::GetRootProfessionSkillLine (Guild.cpp) walks the same parent
// chain for the roster; consolidate the two in a later cleanup.
// -----------------------------------------------------------------------------

#ifndef HAVEN_GUILD_RECIPES_H
#define HAVEN_GUILD_RECIPES_H

#include "Define.h"
#include <array>
#include <unordered_map>

struct SkillLineAbilityEntry;

namespace GuildRecipes
{
    constexpr size_t GUILD_RECIPE_MASK_SIZE = 300; // BFA/8.3 wire size
    using GuildRecipeMask = std::array<uint8, GUILD_RECIPE_MASK_SIZE>;

    // Root primary-profession skill line of 'skillId' (0 if not a profession).
    // 'step' optionally receives the tier's ParentTierIndex (legacy callers).
    uint32 GetRootProfessionSkillLine(uint32 skillId, int32* step = nullptr);

    bool SetAbilityRecipeBit(SkillLineAbilityEntry const* ability, GuildRecipeMask& mask);
    bool SetGuildRecipeBit(uint32 skillLineId, uint32 spellId, GuildRecipeMask& mask);
    void AddSpellToGuildRecipeMasks(uint32 spellId, std::unordered_map<uint32, GuildRecipeMask>& masks);

}

#endif // HAVEN_GUILD_RECIPES_H
