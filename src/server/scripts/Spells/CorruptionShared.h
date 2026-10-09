#ifndef HAVEN_CORRUPTION_SHARED_H
#define HAVEN_CORRUPTION_SHARED_H

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

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "DB2Stores.h"
#include "Log.h"
#include "Item.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Random.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "TemporarySummon.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellHistory.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Timer.h"
#include "Unit.h"
#include "WorldSession.h"
#include <algorithm>
#include <cmath>
#include <memory>
#include <set>
#include <vector>

namespace
{

enum InfiniteStarsSpells
{
    SPELL_CORRUPTION_INFINITE_STARS_1 = 324889,
    SPELL_CORRUPTION_INFINITE_STARS_2 = 324890,
    SPELL_CORRUPTION_INFINITE_STARS_3 = 324891,
    SPELL_INFINITE_STARS_HIDDEN_PROC  = 317257,
    SPELL_INFINITE_STARS_SELECTOR     = 317260,
    SPELL_INFINITE_STARS_MISSILE      = 317262,
    SPELL_INFINITE_STARS_DAMAGE       = 317265,
    SPELL_INFINITE_STARS_RANK_1       = 318274,
    SPELL_INFINITE_STARS_RANK_2       = 318487,
    SPELL_INFINITE_STARS_RANK_3       = 318488
};

enum TwilightDevastationSpells
{
    SPELL_TWILIGHT_DEVASTATION_RANK_1 = 318276,
    SPELL_TWILIGHT_DEVASTATION_RANK_2 = 318477,
    SPELL_TWILIGHT_DEVASTATION_RANK_3 = 318478,
    SPELL_TWILIGHT_PROC               = 317147,
    SPELL_TWILIGHT_BEAM               = 317155,
    SPELL_TWILIGHT_DAMAGE             = 317159
};

enum EchoingVoidSpells
{
    SPELL_ECHOING_VOID_RANK_1   = 318280,
    SPELL_ECHOING_VOID_RANK_2   = 318485,
    SPELL_ECHOING_VOID_RANK_3   = 318486,
    SPELL_ECHOING_VOID_PROC     = 317014,
    SPELL_ECHOING_VOID_STACKS   = 317020,
    SPELL_ECHOING_VOID_COLLAPSE = 317022,
    SPELL_ECHOING_VOID_DAMAGE   = 317029
};

enum TwistedAppendageSpells
{
    SPELL_TWISTED_APPENDAGE_RANK_1 = 318481,
    SPELL_TWISTED_APPENDAGE_RANK_2 = 318482,
    SPELL_TWISTED_APPENDAGE_RANK_3 = 318483,
    SPELL_TWISTED_APPENDAGE_PROC   = 316815,
    SPELL_TWISTED_APPENDAGE_SUMMON = 316818,
    SPELL_TWISTED_APPENDAGE_FLAY   = 316835,
    NPC_TWISTED_APPENDAGE          = 162764
};

enum GushingWoundSpells
{
    SPELL_GUSHING_WOUND_RANK = 318272,
    SPELL_GUSHING_WOUND_PROC = 318179,
    SPELL_GUSHING_WOUND_DOT  = 318187
};

enum GlimpseOfClaritySpells
{
    SPELL_GLIMPSE_ITEM = 318239,
    SPELL_GLIMPSE_PROC = 315574,
    SPELL_GLIMPSE_BUFF = 315573,
    SPELL_FLASH_OF_INSIGHT_ITEM = 318299,
    SPELL_FLASH_OF_INSIGHT_PROC = 316717
};

enum DevourVitalitySpells
{
    SPELL_DEVOUR_VITALITY_ITEM = 318294,
    SPELL_DEVOUR_VITALITY_PROC = 316615,
    SPELL_DEVOUR_VITALITY_LEECH = 316617
};

enum SearingFlamesSpells
{
    SPELL_SEARING_FLAMES_ITEM  = 318293,
    SPELL_SEARING_FLAMES_PROC  = 316698,
    SPELL_SEARING_FLAMES_BUFF  = 316703,
    SPELL_SEARING_FLAMES_BREATH = 316704
};

enum WhisperedTruthsSpells
{
    SPELL_WHISPERED_TRUTHS_PROC = 316780,
    SPELL_WHISPERED_TRUTHS_LOG  = 316782
};

enum FlashOfInsightSpells
{
    // 318299 / 316717 already exist on GlimpseOfClaritySpells as
    // SPELL_FLASH_OF_INSIGHT_ITEM / SPELL_FLASH_OF_INSIGHT_PROC.
    SPELL_FLASH_OF_INSIGHT_BUFF = 316744
};

enum LashOfTheVoidSpells
{
    SPELL_LASH_OF_THE_VOID        = 317290,
    SPELL_LASH_OF_THE_VOID_DAMAGE = 317291,
    SPELL_LASH_OF_THE_VOID_SLOW   = 319241
};

enum ObsidianSkinSpells
{
    SPELL_OBSIDIAN_SKIN           = 316651,
    SPELL_OBSIDIAN_DESTRUCTION    = 317420,
    SPELL_OBSIDIAN_DESTRUCTION_DAMAGE = 316661
};

enum IneffableTruthSpells
{
    SPELL_INEFFABLE_TRUTH_RANK_1 = 318303,
    SPELL_INEFFABLE_TRUTH_RANK_2 = 318484,
    SPELL_INEFFABLE_TRUTH_PROC   = 316799,
    SPELL_INEFFABLE_TRUTH_BUFF   = 316801
};

enum GraspingTendrilsSpells
{
    SPELL_GRASPING_TENDRILS_PROC = 315175,
    SPELL_GRASPING_TENDRILS_SLOW = 315176
};

enum EyeOfCorruptionSpells
{
    SPELL_EYE_OF_CORRUPTION = 315169,
    SPELL_EYE_OF_CORRUPTION_AT = 315154,     // CREATE_AREATRIGGER, SpellMisc 18755
    SPELL_EYE_OF_CORRUPTION_DAMAGE = 315161, // pulse; not on the 315154 trigger chain
    SPELL_EYE_OF_CORRUPTION_PET = 315270     // companion pet, not the combat eye
};

enum GrandDelusionsSpells
{
    SPELL_GRAND_DELUSIONS = 315184,
    // Wowhead same-name summon, 8s, radius 20. Used only when 315184 TriggerSpell is empty.
    SPELL_GRAND_DELUSIONS_SUMMON = 315186,
    SPELL_THING_FROM_BEYOND_AUTOATTACK = 319694, // OVERRIDE_AA -> 315197
    SPELL_GRAND_DELUSIONS_DAMAGE = 315197,       // 35% max HP; 35662 SpellEffect
    SPELL_THING_FROM_BEYOND_CLONE = 318393,      // CLONE_CASTER; 161895 has no model row
    // Named by 318392's SCRIPT_EFFECT base points (35662 dump): CLONE_CASTER
    // (aura 247) plus aura 368 — the retail shadow-tint visual on the clone.
    SPELL_THING_FROM_BEYOND_TINT = 316559,
    // Retail chase mark, 8s = chase duration, hidden, SpellVisual 93005: the
    // dark tether drawn between the Thing (caster) and the player (holder).
    // 315197's TargetAuraSpell also pointed here before the SpellMgr fix.
    SPELL_THING_FROM_BEYOND_TARGET_LOCK = 319695,
    SPELL_THING_FROM_BEYOND_CLOAK = 313301,      // cloak extra, not the 40-tier row
    // Moroes' Shadowform (Karazhan): pure client-side dark translucent shroud,
    // no gameplay effects. Approximation for the retail void-shadow shading —
    // the chain's own visuals (316559 aura 368, 315185, SpellVisual 93528) all
    // render nothing on this client (verified in-game 2026-08-29); deviation
    // registered in the corruption hotfix whitelist.
    SPELL_SHADOWFORM_VISUAL = 29406
};

enum CascadingDisasterSpells
{
    SPELL_CASCADING_DISASTER = 315857
};

enum InevitableDoomSpells
{
    SPELL_INEVITABLE_DOOM = 315179
};

enum InescapableConsequencesSpells
{
    SPELL_INESCAPABLE_CONSEQUENCES = 337612,        // periodic trigger; TriggerSpell is 0
    SPELL_INESCAPABLE_CONSEQUENCES_DAMAGE = 337816  // DAMAGE_FROM_MAX_HEALTH_PCT from SpellEffect
};

// P5: sum Dummy from every worn rank (not highest). CalcValue picks up item-level scaling.
int32 SumCorruptionRankDummy(Unit const* owner, uint32 const* rankIds, uint8 rankCount,
    SpellEffIndex effectIndex, bool useCalc, int32 fallback, char const* /*logKey*/)
{
    if (!owner || !rankIds || !rankCount)
        return 0;

    int32 sum = 0;
    auto addRank = [&](uint32 rankId, Item const* item)
    {
        SpellInfo const* rank = sSpellMgr->GetSpellInfo(rankId);
        SpellEffectInfo const* effect = rank ? rank->GetEffect(effectIndex) : nullptr;
        int32 value = effect ? (useCalc
            ? effect->CalcValue(owner, nullptr, owner, nullptr, item ? item->GetEntry() : 0,
                item ? int32(item->GetItemLevel(owner->ToPlayer())) : -1)
            : effect->BasePoints) : 0;
        sum += value > 0 ? value : std::max(fallback, 0);
    };

    if (Player const* player = owner->ToPlayer())
        for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
        {
            Item const* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
            if (!item || item->IsBroken())
                continue;
            for (uint8 i = 0; i < rankCount; ++i)
            {
                if (!rankIds[i])
                    continue;
                for (ItemEffectEntry const* itemEffect : item->GetEffects())
                    if (itemEffect && itemEffect->SpellID == rankIds[i]
                        && itemEffect->TriggerType == ITEM_SPELLTRIGGER_ON_EQUIP)
                    {
                        addRank(rankIds[i], item);
                        break;
                    }
            }
        }

    // A manually applied rank has no item GUID; stale item-backed auras
    // must not recreate effects after the corresponding item is removed.
    for (auto const& applied : owner->GetAppliedAuras())
    {
        Aura const* aura = applied.second->GetBase();
        if (!aura || !aura->GetCastItemGUID().IsEmpty())
            continue;
        for (uint8 i = 0; i < rankCount; ++i)
            if (rankIds[i] && aura->GetId() == rankIds[i])
            {
                addRank(rankIds[i], nullptr);
                break;
            }
    }
    return sum;
}

// Ignore GCD / current cast / cost so a slam can still drop a star. Not FULL_DEBUG_MASK
// (that includes IGNORE_TARGET_CHECK and CAST_DIRECTLY).
TriggerCastFlags InfiniteStarsCastFlags()
{
    return TriggerCastFlags(
        TRIGGERED_IGNORE_GCD |
        TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD |
        TRIGGERED_IGNORE_POWER_AND_REAGENT_COST |
        TRIGGERED_IGNORE_CAST_IN_PROGRESS |
        TRIGGERED_IGNORE_CASTER_AURASTATE |
        TRIGGERED_IGNORE_SET_FACING |
        TRIGGERED_DONT_REPORT_CAST_ERROR |
        TRIGGERED_DISALLOW_PROC_EVENTS);
}

bool IsGlimpseExcludedSpell(uint32 id)
{
    switch (id)
    {
        case SPELL_INFINITE_STARS_SELECTOR:
        case SPELL_INFINITE_STARS_MISSILE:
        case SPELL_INFINITE_STARS_DAMAGE:
        case SPELL_TWILIGHT_BEAM:
        case SPELL_TWILIGHT_DAMAGE:
        case SPELL_ECHOING_VOID_COLLAPSE:
        case SPELL_ECHOING_VOID_DAMAGE:
        case SPELL_TWISTED_APPENDAGE_SUMMON:
        case SPELL_TWISTED_APPENDAGE_FLAY:
        case SPELL_GUSHING_WOUND_DOT:
        case SPELL_GLIMPSE_ITEM:
        case SPELL_GLIMPSE_PROC:
        case SPELL_GLIMPSE_BUFF:
        case SPELL_FLASH_OF_INSIGHT_ITEM:
        case SPELL_FLASH_OF_INSIGHT_PROC:
        case SPELL_FLASH_OF_INSIGHT_BUFF:
        case SPELL_DEVOUR_VITALITY_ITEM:
        case SPELL_DEVOUR_VITALITY_PROC:
        case SPELL_DEVOUR_VITALITY_LEECH:
        case SPELL_SEARING_FLAMES_ITEM:
        case SPELL_SEARING_FLAMES_PROC:
        case SPELL_SEARING_FLAMES_BUFF:
        case SPELL_SEARING_FLAMES_BREATH:
        case SPELL_WHISPERED_TRUTHS_PROC:
        case SPELL_WHISPERED_TRUTHS_LOG:
        case SPELL_LASH_OF_THE_VOID:
        case SPELL_LASH_OF_THE_VOID_DAMAGE:
        case SPELL_LASH_OF_THE_VOID_SLOW:
        case SPELL_OBSIDIAN_SKIN:
        case SPELL_OBSIDIAN_DESTRUCTION:
        case SPELL_OBSIDIAN_DESTRUCTION_DAMAGE:
        case SPELL_INEFFABLE_TRUTH_RANK_1:
        case SPELL_INEFFABLE_TRUTH_RANK_2:
        case SPELL_INEFFABLE_TRUTH_PROC:
        case SPELL_INEFFABLE_TRUTH_BUFF:
        case SPELL_GRASPING_TENDRILS_PROC:
        case SPELL_GRASPING_TENDRILS_SLOW:
        case SPELL_EYE_OF_CORRUPTION:
        case SPELL_EYE_OF_CORRUPTION_AT:
        case SPELL_EYE_OF_CORRUPTION_DAMAGE:
        case SPELL_EYE_OF_CORRUPTION_PET:
        case SPELL_GRAND_DELUSIONS:
        case SPELL_GRAND_DELUSIONS_SUMMON:
        case SPELL_THING_FROM_BEYOND_CLOAK:
        case SPELL_CASCADING_DISASTER:
        case SPELL_INEVITABLE_DOOM:
        case SPELL_INESCAPABLE_CONSEQUENCES:
        case SPELL_INESCAPABLE_CONSEQUENCES_DAMAGE:
            return true;
        default:
            return false;
    }
}

}
#endif
