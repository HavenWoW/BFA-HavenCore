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

#include "CorruptionShared.h"

namespace
{

enum VoidRitualSpells
{
    SPELL_VOID_RITUAL_RANK_1     = 318286,
    SPELL_VOID_RITUAL_RANK_2     = 318479,
    SPELL_VOID_RITUAL_RANK_3     = 318480,
    SPELL_VOID_RITUAL_PROC       = 316814,
    SPELL_VOID_RITUAL_END_COMING = 316823
};

enum StrikethroughSpells
{
    SPELL_STRIKETHROUGH_RANK_1 = 315277,
    SPELL_STRIKETHROUGH_RANK_2 = 315281,
    SPELL_STRIKETHROUGH_RANK_3 = 315282,
    SPELL_STRIKETHROUGH_HIDDEN = 320249
};

enum RacingPulseSpells
{
    SPELL_RACING_PULSE_RANK_1 = 318266,
    SPELL_RACING_PULSE_RANK_2 = 318492,
    SPELL_RACING_PULSE_RANK_3 = 318496,
    SPELL_RACING_PULSE_PROC   = 318220,
    SPELL_RACING_PULSE_BUFF   = 318227
};

enum HonedMindSpells
{
    SPELL_HONED_MIND_RANK_1 = 318269,
    SPELL_HONED_MIND_RANK_2 = 318494,
    SPELL_HONED_MIND_RANK_3 = 318498,
    SPELL_HONED_MIND_PROC   = 318214,
    SPELL_HONED_MIND_BUFF   = 318216
};

enum DeadlyMomentumSpells
{
    SPELL_DEADLY_MOMENTUM_RANK_1 = 318268,
    SPELL_DEADLY_MOMENTUM_RANK_2 = 318493,
    SPELL_DEADLY_MOMENTUM_RANK_3 = 318497,
    SPELL_DEADLY_MOMENTUM_PROC   = 318218,
    SPELL_DEADLY_MOMENTUM_BUFF   = 318219
};

enum SurgingVitalitySpells
{
    SPELL_SURGING_VITALITY_RANK_1 = 318270,
    SPELL_SURGING_VITALITY_RANK_2 = 318495,
    SPELL_SURGING_VITALITY_RANK_3 = 318499,
    SPELL_SURGING_VITALITY_PROC   = 318212,
    SPELL_SURGING_VITALITY_BUFF   = 318211
};

// Wowhead 25-30 yd; DBC 317155 has width 3, no length. TimeToTarget 4000 in spell_areatrigger.
constexpr float TWILIGHT_BEAM_RANGE_YD   = 28.0f;

constexpr uint32 TWILIGHT_BEAM_TRAVEL_MS = 4000;

// 2020-02 hotfix values, not in DBC: beam stops after 10 targets, 6th-10th take 50%.
constexpr uint32 TWILIGHT_BEAM_MAX_TARGETS      = 10;

constexpr uint32 TWILIGHT_BEAM_HALF_DAMAGE_FROM = 6;

// SimC bfa.echoing_void_collapse_chance. Not a DBC field — calibrate in-game if needed.
constexpr float ECHOING_VOID_COLLAPSE_CHANCE = 0.15f;

constexpr int32 ECHOING_VOID_RANK1_BP_FALLBACK = 40;

// tooltip $s1/100 = 0.4% max HP
constexpr uint32 ECHOING_VOID_PERIOD_FALLBACK_MS = 1000;

constexpr int32 ECHOING_VOID_DURATION_SLACK_MS = 400;

// DBC has ally count (2) but no radius. 8 yd is party-range "nearby".
constexpr float VOID_RITUAL_ALLY_RANGE_YD = 8.0f;

// SimC: solo RPPM *= 5/6. Not a DBC field — calibrate if in-game disagrees.
constexpr float VOID_RITUAL_SOLO_RPPM_MULT = 5.0f / 6.0f;

constexpr int32 VOID_RITUAL_RANK1_RATING_FALLBACK = 14;

constexpr uint32 VOID_RITUAL_ALLY_NEED_FALLBACK = 2;

// 320249 EFFECT_0 DBC BP is 0; fill crit-damage Dummy. Do not hardcode 2/3/4.
// Driver EFFECT_1 is a live SPELL_AURA_MOD_CRITICAL_HEALING_AMOUNT — leave 320249 heal at 0.
constexpr int32 STRIKETHROUGH_RANK1_CRIT_FALLBACK = 2;

// 318227 DBC BP is 0; fill rank Dummy. Do not hardcode 546 as the only value.
constexpr int32 RACING_PULSE_RANK1_RATING_FALLBACK = 546;

// 318216 DBC BP is 0; fill rank Dummy. Do not hardcode 392 as the only value.
constexpr int32 HONED_MIND_RANK1_RATING_FALLBACK = 392;

// 318219 DBC BP is 0; fill per-stack Dummy. Engine multiplies by stacks.
constexpr int32 DEADLY_MOMENTUM_RANK1_RATING_FALLBACK = 31;

// Driver Base was hotfixed to 0. Rank-1 dump Scaled is 343. Do not use Icy Veins 312.
constexpr int32 SURGING_VITALITY_RANK1_RATING_FALLBACK = 343;

struct CorruptionDriverFamily
{
    uint32 ranks[3];
    uint8 rankCount;
    uint32 hiddenProc;
};

CorruptionDriverFamily const* FindCorruptionDriverFamily(uint32 spellId)
{
    static CorruptionDriverFamily const families[] =
    {
        { { SPELL_INFINITE_STARS_RANK_1, SPELL_INFINITE_STARS_RANK_2, SPELL_INFINITE_STARS_RANK_3 }, 3, SPELL_INFINITE_STARS_HIDDEN_PROC },
        { { SPELL_TWILIGHT_DEVASTATION_RANK_1, SPELL_TWILIGHT_DEVASTATION_RANK_2, SPELL_TWILIGHT_DEVASTATION_RANK_3 }, 3, SPELL_TWILIGHT_PROC },
        { { SPELL_ECHOING_VOID_RANK_1, SPELL_ECHOING_VOID_RANK_2, SPELL_ECHOING_VOID_RANK_3 }, 3, SPELL_ECHOING_VOID_PROC },
        { { SPELL_TWISTED_APPENDAGE_RANK_1, SPELL_TWISTED_APPENDAGE_RANK_2, SPELL_TWISTED_APPENDAGE_RANK_3 }, 3, SPELL_TWISTED_APPENDAGE_PROC },
        { { SPELL_VOID_RITUAL_RANK_1, SPELL_VOID_RITUAL_RANK_2, SPELL_VOID_RITUAL_RANK_3 }, 3, SPELL_VOID_RITUAL_PROC },
        { { SPELL_RACING_PULSE_RANK_1, SPELL_RACING_PULSE_RANK_2, SPELL_RACING_PULSE_RANK_3 }, 3, SPELL_RACING_PULSE_PROC },
        { { SPELL_HONED_MIND_RANK_1, SPELL_HONED_MIND_RANK_2, SPELL_HONED_MIND_RANK_3 }, 3, SPELL_HONED_MIND_PROC },
        { { SPELL_DEADLY_MOMENTUM_RANK_1, SPELL_DEADLY_MOMENTUM_RANK_2, SPELL_DEADLY_MOMENTUM_RANK_3 }, 3, SPELL_DEADLY_MOMENTUM_PROC },
        { { SPELL_SURGING_VITALITY_RANK_1, SPELL_SURGING_VITALITY_RANK_2, SPELL_SURGING_VITALITY_RANK_3 }, 3, SPELL_SURGING_VITALITY_PROC },
        { { SPELL_GUSHING_WOUND_RANK, 0, 0 }, 1, SPELL_GUSHING_WOUND_PROC },
        { { SPELL_GLIMPSE_ITEM, 0, 0 }, 1, SPELL_GLIMPSE_PROC },
        { { SPELL_INEFFABLE_TRUTH_RANK_1, SPELL_INEFFABLE_TRUTH_RANK_2, 0 }, 2, SPELL_INEFFABLE_TRUTH_PROC },
        { { SPELL_STRIKETHROUGH_RANK_1, SPELL_STRIKETHROUGH_RANK_2, SPELL_STRIKETHROUGH_RANK_3 }, 3, SPELL_STRIKETHROUGH_HIDDEN },
    };

    for (CorruptionDriverFamily const& family : families)
        for (uint8 i = 0; i < family.rankCount; ++i)
            if (family.ranks[i] == spellId)
                return &family;
    return nullptr;
}

void SyncCorruptionHiddenProc(Unit* owner, CorruptionDriverFamily const& family)
{
    if (!owner)
        return;

    bool any = false;
    for (uint8 i = 0; i < family.rankCount; ++i)
        if (family.ranks[i] && owner->HasAura(family.ranks[i]))
            any = true;

    if (any)
    {
        if (!owner->HasAura(family.hiddenProc))
            owner->CastSpell(owner, family.hiddenProc, true);
    }
    else
        owner->RemoveAurasDueToSpell(family.hiddenProc);
}

// 35662 Spell dump fallbacks if GetEffect is missing.
constexpr int32 INFINITE_STARS_VULN_PCT_FALLBACK = 25;

constexpr float INFINITE_STARS_RANGE_FALLBACK    = 50.0f;

int32 InfiniteStarsVulnPct()
{
    if (SpellInfo const* info = sSpellMgr->GetSpellInfo(SPELL_INFINITE_STARS_DAMAGE))
        if (SpellEffectInfo const* effect = info->GetEffect(EFFECT_2))
            return effect->BasePoints;

    static thread_local bool logged = false;
    if (!logged)
    {
        logged = true;
        TC_LOG_ERROR("scripts", "InfiniteStars: 317265 EFFECT_2 missing, using vuln %d", INFINITE_STARS_VULN_PCT_FALLBACK);
    }
    return INFINITE_STARS_VULN_PCT_FALLBACK;
}

float InfiniteStarsSelectRange()
{
    if (SpellInfo const* info = sSpellMgr->GetSpellInfo(SPELL_INFINITE_STARS_SELECTOR))
        if (SpellEffectInfo const* effect = info->GetEffect(EFFECT_0))
        {
            float radius = effect->CalcRadius();
            if (radius > 0.0f)
                return radius;
        }
    return INFINITE_STARS_RANGE_FALLBACK;
}

// 2020-01-23 hotfix: rank EFFECT_0 scales with the worn item's ilvl (Class -9).
// EFFECT_1 Dummy percents are the pre-hotfix tooltip; do not multiply by AP/SP.
int32 InfiniteStarsBaseDamage(Unit const* caster)
{
    uint32 const ranks[] = { SPELL_INFINITE_STARS_RANK_1, SPELL_INFINITE_STARS_RANK_2, SPELL_INFINITE_STARS_RANK_3 };
    int32 damage = SumCorruptionRankDummy(caster, ranks, 3, EFFECT_0, true, 1, "InfiniteStars");
    return damage < 1 ? 1 : damage;
}

TriggerCastFlags TwilightDamageCastFlags()
{
    return TriggerCastFlags(uint32(InfiniteStarsCastFlags()) | uint32(TRIGGERED_IGNORE_TARGET_CHECK));
}

class InfiniteStarImpactEvent : public BasicEvent
{
public:
    InfiniteStarImpactEvent(ObjectGuid casterGuid, ObjectGuid targetGuid)
        : _casterGuid(casterGuid), _targetGuid(targetGuid) { }

    bool Execute(uint64 /*time*/, uint32 /*diff*/) override
    {
        // FindUnit() is world-wide and only reliable for players. The dummy is a
        // Creature: resolve it on the caster's map.
        Unit* caster = ObjectAccessor::FindPlayer(_casterGuid);
        if (!caster)
            caster = ObjectAccessor::FindUnit(_casterGuid);
        if (!caster)
            return true;

        Unit* target = ObjectAccessor::GetUnit(*caster, _targetGuid);
        if (!target || !target->IsAlive())
            return true;

        caster->CastSpell(target, SPELL_INFINITE_STARS_DAMAGE, InfiniteStarsCastFlags());
        return true;
    }

private:
    ObjectGuid _casterGuid;
    ObjectGuid _targetGuid;
};

void CastInfiniteStar(Unit* caster, Unit* target)
{
    if (!caster || !target || !target->IsAlive() || target == caster)
        return;

    if (caster->GetSpellHistory()->HasCooldown(SPELL_INFINITE_STARS_MISSILE))
        return;

    SpellInfo const* missile = sSpellMgr->GetSpellInfo(SPELL_INFINITE_STARS_MISSILE);
    uint32 visual = missile ? missile->GetSpellVisual(caster) : 0;
    float delay = (missile && missile->Speed > 0.0f) ? missile->Speed : 1.0f;

    // One visual path: DEST_DEST (87) CastSpell at the target's feet.
    false;
    if (missile)
        caster->CastSpell(target->GetPosition(), SPELL_INFINITE_STARS_MISSILE, InfiniteStarsCastFlags());

    // Not a DBC value: stops 317257 and 317260 from each dropping a star.
    caster->GetSpellHistory()->AddCooldown(SPELL_INFINITE_STARS_MISSILE, 0, Milliseconds(1500));
    caster->m_Events.AddEvent(new InfiniteStarImpactEvent(caster->GetGUID(), target->GetGUID()),
        caster->m_Events.CalculateTime(uint32(delay * 1000.0f)));
}

Unit* ResolveStarTarget(Unit* caster, ProcEventInfo& eventInfo)
{
    if (Unit* procTarget = eventInfo.GetProcTarget())
        if (procTarget->IsAlive() && procTarget != caster)
            return procTarget;

    if (Unit* actionTarget = eventInfo.GetActionTarget())
        if (actionTarget->IsAlive() && actionTarget != caster)
            return actionTarget;

    if (Unit* victim = caster->GetVictim())
        if (victim->IsAlive() && victim != caster)
            return victim;

    return caster->SelectNearbyTarget(nullptr, InfiniteStarsSelectRange());
}

constexpr int32 TWILIGHT_RANK1_BP_FALLBACK = 60;

float TwilightHealthPct(Unit const* caster)
{
    uint32 const ranks[] = { SPELL_TWILIGHT_DEVASTATION_RANK_1, SPELL_TWILIGHT_DEVASTATION_RANK_2, SPELL_TWILIGHT_DEVASTATION_RANK_3 };
    int32 bp = SumCorruptionRankDummy(caster, ranks, 3, EFFECT_0, true,
        TWILIGHT_RANK1_BP_FALLBACK, "TwilightDevastation");
    // DBC stores 60/120/180; tooltip is $s1/10 percent of health.
    return float(bp) / 10.0f / 100.0f;
}

void CastTwilightDevastation(Unit* caster)
{
    if (!caster || !caster->IsAlive())
        return;

    SpellInfo const* beamInfo = sSpellMgr->GetSpellInfo(SPELL_TWILIGHT_BEAM);
    uint32 visual = beamInfo ? beamInfo->GetSpellVisual(caster) : 0;
    int32 baseDamage = int32(float(caster->GetMaxHealth()) * TwilightHealthPct(caster));
    if (baseDamage < 1)
        baseDamage = 1;

    int32 duration = beamInfo ? beamInfo->CalcDuration(caster) : 0;
    if (duration <= 0)
        duration = int32(TWILIGHT_BEAM_TRAVEL_MS);

    uint32 miscId = 19034;
    if (beamInfo)
        if (SpellEffectInfo const* effect = beamInfo->GetEffect(EFFECT_0))
            if (effect->MiscValue)
                miscId = uint32(effect->MiscValue);

    // SpellGo plays visual 93766. CREATE_AREATRIGGER is prevented in the SpellScript
    // so we spawn exactly one AT (Haven's dest-HIT path often skipped the effect).
    bool beamOk = caster->CastSpell(caster->GetPosition(), SPELL_TWILIGHT_BEAM, InfiniteStarsCastFlags());

    // Damage lives in at_twilight_devastation::OnUnitEnter, driven by the AT's own
    // spline position (spell_areatrigger_splines 19034: 0 -> 28 yd over 4s).
    if (caster->GetAreaTriggers(SPELL_TWILIGHT_BEAM).empty())
        AreaTrigger::CreateAreaTrigger(miscId, caster, nullptr, beamInfo, *caster, duration, visual);

}

TriggerCastFlags EchoingVoidCastFlags()
{
    return InfiniteStarsCastFlags();
}

float EchoingVoidHealthPct(Unit const* caster)
{
    uint32 const ranks[] = { SPELL_ECHOING_VOID_RANK_1, SPELL_ECHOING_VOID_RANK_2, SPELL_ECHOING_VOID_RANK_3 };
    int32 bp = SumCorruptionRankDummy(caster, ranks, 3, EFFECT_0, true,
        ECHOING_VOID_RANK1_BP_FALLBACK, "EchoingVoid");
    // DBC stores 40/60/100; tooltip is $s1/100 percent of health (rank 1 = 0.4%).
    return float(bp) / 100.0f / 100.0f;
}

uint32 EchoingVoidPeriodMs()
{
    if (SpellInfo const* info = sSpellMgr->GetSpellInfo(SPELL_ECHOING_VOID_COLLAPSE))
        if (SpellEffectInfo const* effect = info->GetEffect(EFFECT_0))
            if (effect->ApplyAuraPeriod)
                return effect->ApplyAuraPeriod;

    static thread_local bool logged = false;
    if (!logged)
    {
        logged = true;
        TC_LOG_ERROR("scripts", "EchoingVoid: 317022 period missing, using %u ms",
            ECHOING_VOID_PERIOD_FALLBACK_MS);
    }
    return ECHOING_VOID_PERIOD_FALLBACK_MS;
}

// Player Echoing Void casts 317022 on self. Hivemind puts 317022 on players from the boss.
bool EchoingVoidPlayerOwnsCollapse(Unit const* unit)
{
    if (!unit || !unit->HasAura(SPELL_ECHOING_VOID_PROC))
        return false;
    Aura const* collapse = unit->GetAura(SPELL_ECHOING_VOID_COLLAPSE);
    return collapse && collapse->GetCasterGUID() == unit->GetGUID();
}

void StartEchoingVoidCollapse(Unit* caster)
{
    if (!caster || !caster->IsAlive())
        return;

    uint32 stacks = 1;
    if (Aura const* stackAura = caster->GetAura(SPELL_ECHOING_VOID_STACKS))
        stacks = stackAura->GetStackAmount();
    if (stacks < 1)
        stacks = 1;

    caster->CastSpell(caster, SPELL_ECHOING_VOID_COLLAPSE, EchoingVoidCastFlags());
}

void HandleEchoingVoidProc(Unit* caster, ProcEventInfo& eventInfo)
{
    if (!caster || !caster->IsAlive())
        return;

    // 2020-01-27 hotfix: only spells with a GCD stack. Autos / no-GCD trinkets do not.
    Spell const* procSpell = eventInfo.GetProcSpell();
    if (!procSpell || !procSpell->GetSpellInfo() || procSpell->GetSpellInfo()->StartRecoveryTime == 0)
        return;

    if (caster->HasAura(SPELL_ECHOING_VOID_COLLAPSE))
        return;

    if (caster->GetAura(SPELL_ECHOING_VOID_STACKS) && roll_chance_f(ECHOING_VOID_COLLAPSE_CHANCE * 100.0f))
    {
        StartEchoingVoidCollapse(caster);
        return;
    }

    caster->CastSpell(caster, SPELL_ECHOING_VOID_STACKS, EchoingVoidCastFlags());
    uint32 stacks = 1;
    if (Aura const* stackAura = caster->GetAura(SPELL_ECHOING_VOID_STACKS))
        stacks = stackAura->GetStackAmount();

}

// Same hotfix path as Infinite Stars: EFFECT_0 CalcValue per worn rank, not Dummy% * AP/SP.
int32 TwistedAppendageTickDamage(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_TWISTED_APPENDAGE_RANK_1, SPELL_TWISTED_APPENDAGE_RANK_2, SPELL_TWISTED_APPENDAGE_RANK_3 };
    int32 damage = SumCorruptionRankDummy(owner, ranks, 3, EFFECT_0, true, 1, "TwistedAppendage");
    return damage < 1 ? 1 : damage;
}

// Aura caster is the player (originalCaster) so ticks can crit and CLEU is
// player-to-creature. Fall back to the tentacle's owner if that is missing.
Unit* TwistedAppendageOwner(Unit* caster)
{
    if (!caster)
        return nullptr;
    if (Unit* owner = caster->GetOwner())
        return owner;
    return caster;
}

Unit* ResolveTentacleTarget(Unit* owner)
{
    if (Unit* victim = owner->GetVictim())
        if (victim->IsAlive() && victim != owner)
            return victim;

    if (Player* player = owner->ToPlayer())
        if (Unit* selected = player->GetSelectedUnit())
            if (selected->IsAlive() && selected != owner)
                if (owner->_IsValidAttackTarget(selected, sSpellMgr->GetSpellInfo(SPELL_TWISTED_APPENDAGE_FLAY)))
                    return selected;

    return owner->SelectNearbyTarget(nullptr, 50.0f);
}

void CastTwistedAppendage(Unit* caster)
{
    if (!caster || !caster->IsAlive())
        return;

    caster->CastSpell(caster, SPELL_TWISTED_APPENDAGE_SUMMON, InfiniteStarsCastFlags());
    Unit* target = ResolveTentacleTarget(caster);

}

// Channel 316835: keep CASTING so UpdateAI does not recast every tick.
TriggerCastFlags TwistedAppendageFlayFlags()
{
    return TriggerCastFlags(
        TRIGGERED_IGNORE_GCD |
        TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD |
        TRIGGERED_IGNORE_POWER_AND_REAGENT_COST |
        TRIGGERED_DONT_REPORT_CAST_ERROR |
        TRIGGERED_DISALLOW_PROC_EVENTS);
}

uint32 VoidRitualRankSpell(Unit const* owner)
{
    if (owner->HasAura(SPELL_VOID_RITUAL_RANK_3))
        return SPELL_VOID_RITUAL_RANK_3;
    if (owner->HasAura(SPELL_VOID_RITUAL_RANK_2))
        return SPELL_VOID_RITUAL_RANK_2;
    return SPELL_VOID_RITUAL_RANK_1;
}

int32 VoidRitualRatingPerStack(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_VOID_RITUAL_RANK_1, SPELL_VOID_RITUAL_RANK_2, SPELL_VOID_RITUAL_RANK_3 };
    return SumCorruptionRankDummy(owner, ranks, 3, EFFECT_0, true,
        VOID_RITUAL_RANK1_RATING_FALLBACK, "VoidRitual");
}

uint32 VoidRitualAllyNeed(Unit const* owner)
{
    if (SpellInfo const* proc = sSpellMgr->GetSpellInfo(SPELL_VOID_RITUAL_PROC))
        if (SpellEffectInfo const* effect = proc->GetEffect(EFFECT_2))
            if (effect->BasePoints > 0)
                return uint32(effect->BasePoints);

    if (SpellInfo const* rank = sSpellMgr->GetSpellInfo(VoidRitualRankSpell(owner)))
        if (SpellEffectInfo const* effect = rank->GetEffect(EFFECT_2))
            if (effect->BasePoints > 0)
                return uint32(effect->BasePoints);

    static thread_local bool logged = false;
    if (!logged)
    {
        logged = true;
        TC_LOG_ERROR("scripts", "VoidRitual: ally-need Dummy missing, using %u",
            VOID_RITUAL_ALLY_NEED_FALLBACK);
    }
    return VOID_RITUAL_ALLY_NEED_FALLBACK;
}

uint32 VoidRitualNearbyAllies(Unit* caster)
{
    if (!caster)
        return 0;

    // Grid search already drops other phases; keep an explicit IsInPhase
    // so a later scan helper cannot silently count phased-out allies.
    std::vector<Player*> players;
    caster->GetPlayerListInGrid(players, VOID_RITUAL_ALLY_RANGE_YD);

    uint32 allies = 0;
    for (Player* player : players)
    {
        if (!player || player == caster || !player->IsAlive())
            continue;
        if (!caster->IsInPhase(player))
            continue;
        if (!caster->IsFriendlyTo(player))
            continue;
        if (!player->HasAura(SPELL_VOID_RITUAL_PROC))
            continue;
        ++allies;
    }
    return allies;
}

void CastVoidRitual(Unit* caster)
{
    if (!caster || !caster->IsAlive())
        return;

    if (caster->HasAura(SPELL_VOID_RITUAL_END_COMING))
    {

        return;
    }

    uint32 allies = VoidRitualNearbyAllies(caster);
    bool increased = allies >= VoidRitualAllyNeed(caster);
    if (!increased && !roll_chance_f(VOID_RITUAL_SOLO_RPPM_MULT * 100.0f))
    {

        return;
    }

    caster->CastSpell(caster, SPELL_VOID_RITUAL_END_COMING, InfiniteStarsCastFlags());

}

int32 StrikethroughCritDamagePct(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_STRIKETHROUGH_RANK_1, SPELL_STRIKETHROUGH_RANK_2, SPELL_STRIKETHROUGH_RANK_3 };
    return SumCorruptionRankDummy(owner, ranks, 3, EFFECT_0, true,
        STRIKETHROUGH_RANK1_CRIT_FALLBACK, "Strikethrough");
}

int32 RacingPulseRating(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_RACING_PULSE_RANK_1, SPELL_RACING_PULSE_RANK_2, SPELL_RACING_PULSE_RANK_3 };
    return SumCorruptionRankDummy(owner, ranks, 3, EFFECT_0, true,
        RACING_PULSE_RANK1_RATING_FALLBACK, "RacingPulse");
}

void CastRacingPulse(Unit* caster)
{
    if (!caster || !caster->IsAlive())
        return;

    int32 rating = RacingPulseRating(caster);
    caster->CastSpell(caster, SPELL_RACING_PULSE_BUFF, InfiniteStarsCastFlags());

}

int32 HonedMindRating(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_HONED_MIND_RANK_1, SPELL_HONED_MIND_RANK_2, SPELL_HONED_MIND_RANK_3 };
    return SumCorruptionRankDummy(owner, ranks, 3, EFFECT_0, true,
        HONED_MIND_RANK1_RATING_FALLBACK, "HonedMind");
}

void CastHonedMind(Unit* caster)
{
    if (!caster || !caster->IsAlive())
        return;

    int32 rating = HonedMindRating(caster);
    caster->CastSpell(caster, SPELL_HONED_MIND_BUFF, InfiniteStarsCastFlags());

}

int32 DeadlyMomentumRatingPerStack(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_DEADLY_MOMENTUM_RANK_1, SPELL_DEADLY_MOMENTUM_RANK_2, SPELL_DEADLY_MOMENTUM_RANK_3 };
    return SumCorruptionRankDummy(owner, ranks, 3, EFFECT_0, true,
        DEADLY_MOMENTUM_RANK1_RATING_FALLBACK, "DeadlyMomentum");
}

void CastDeadlyMomentum(Unit* caster)
{
    if (!caster || !caster->IsAlive())
        return;

    caster->CastSpell(caster, SPELL_DEADLY_MOMENTUM_BUFF, InfiniteStarsCastFlags());
    uint32 stacks = 1;
    if (Aura const* aura = caster->GetAura(SPELL_DEADLY_MOMENTUM_BUFF))
        stacks = aura->GetStackAmount();
    if (stacks < 1)
        stacks = 1;
    int32 rating = DeadlyMomentumRatingPerStack(caster) * int32(stacks);

}

int32 SurgingVitalityRating(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_SURGING_VITALITY_RANK_1, SPELL_SURGING_VITALITY_RANK_2, SPELL_SURGING_VITALITY_RANK_3 };
    return SumCorruptionRankDummy(owner, ranks, 3, EFFECT_0, true,
        SURGING_VITALITY_RANK1_RATING_FALLBACK, "SurgingVitality");
}

void CastSurgingVitality(Unit* caster)
{
    if (!caster || !caster->IsAlive())
        return;

    int32 rating = SurgingVitalityRating(caster);
    caster->CastSpell(caster, SPELL_SURGING_VITALITY_BUFF, InfiniteStarsCastFlags());

}

// 318187 Dummy% * max(AP,SP) is the pre-hotfix tooltip. Tick amount is 318272 EFFECT_0.
int32 GushingWoundTickDamage(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_GUSHING_WOUND_RANK };
    return SumCorruptionRankDummy(owner, ranks, 1, EFFECT_0, true, 1, "GushingWound");
}

Unit* ResolveGushingWoundTarget(Unit* caster, ProcEventInfo& eventInfo)
{
    if (Unit* procTarget = eventInfo.GetProcTarget())
        if (procTarget->IsAlive() && procTarget != caster)
            return procTarget;

    if (Unit* actionTarget = eventInfo.GetActionTarget())
        if (actionTarget->IsAlive() && actionTarget != caster)
            return actionTarget;

    if (Unit* victim = caster->GetVictim())
        if (victim->IsAlive() && victim != caster)
            return victim;

    return caster->SelectNearbyTarget(nullptr, 50.0f);
}

void CastGushingWound(Unit* caster, Unit* target)
{
    if (!caster || !caster->IsAlive() || !target || !target->IsAlive() || target == caster)
        return;
    if (!caster->_IsValidAttackTarget(target, sSpellMgr->GetSpellInfo(SPELL_GUSHING_WOUND_DOT)))
        return;

    caster->CastSpell(target, SPELL_GUSHING_WOUND_DOT, InfiniteStarsCastFlags());

}

// 324889/324890/324891 - wrapper has no aura. AfterCast applies the rank driver; family hook syncs 317257.

// Rank drivers (ItemEffect ON_EQUIP). Keep hidden proc up while any rank of the family remains.
class spell_corruption_rank_driver : public AuraScript
{
    PrepareAuraScript(spell_corruption_rank_driver);

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* owner = GetTarget())
            if (CorruptionDriverFamily const* family = FindCorruptionDriverFamily(GetId()))
                SyncCorruptionHiddenProc(owner, *family);
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* owner = GetTarget())
            if (CorruptionDriverFamily const* family = FindCorruptionDriverFamily(GetId()))
                SyncCorruptionHiddenProc(owner, *family);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_corruption_rank_driver::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_corruption_rank_driver::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 317257 - hidden proc aura
class spell_infinite_stars_proc : public AuraScript
{
    PrepareAuraScript(spell_infinite_stars_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_INFINITE_STARS_SELECTOR, SPELL_INFINITE_STARS_MISSILE, SPELL_INFINITE_STARS_DAMAGE });
    }

    void HandleProc(ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        if (Unit* caster = GetTarget())
            CastInfiniteStar(caster, ResolveStarTarget(caster, eventInfo));
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_infinite_stars_proc::HandleProc);
    }
};

// 317260 - dummy selector (core may trigger this from 317257)
class spell_infinite_stars_selector : public SpellScript
{
    PrepareSpellScript(spell_infinite_stars_selector);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_INFINITE_STARS_MISSILE, SPELL_INFINITE_STARS_DAMAGE,
            SPELL_INFINITE_STARS_RANK_1, SPELL_INFINITE_STARS_RANK_2, SPELL_INFINITE_STARS_RANK_3 });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        Unit* target = GetHitUnit();
        if (!target || target == caster)
        {
            if (Unit* expl = GetExplTargetUnit())
                if (expl != caster)
                    target = expl;
        }
        if (!target || target == caster)
            target = caster->GetVictim();
        if (!target)
            target = caster->SelectNearbyTarget(nullptr, InfiniteStarsSelectRange());

        CastInfiniteStar(caster, target);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_infinite_stars_selector::HandleDummy, EFFECT_FIRST_FOUND, SPELL_EFFECT_DUMMY);
    }
};

// 317265 - dummy hit (DBC school damage BP is 0; stack aura still applies from effects)
class spell_infinite_stars_damage : public SpellScript
{
    PrepareSpellScript(spell_infinite_stars_damage);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_INFINITE_STARS_RANK_1, SPELL_INFINITE_STARS_RANK_2, SPELL_INFINITE_STARS_RANK_3 });
    }

    void HandleHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        int32 damage = InfiniteStarsBaseDamage(caster);

        // 317265 applies its stack aura in the same hit. SimC multiplies with stacks *before* this star.
        int32 stacks = 0;
        if (Aura const* aura = target->GetAura(SPELL_INFINITE_STARS_DAMAGE, caster->GetGUID()))
            stacks = int32(aura->GetStackAmount());
        if (stacks > 0)
            --stacks;
        AddPct(damage, InfiniteStarsVulnPct() * stacks);

        // BP is 0; fill hit and let the school-damage effect deal it. DealDamage on top
        // double-dipped HP (twice the tooltip amount per star).
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_infinite_stars_damage::HandleHit);
    }
};

// 317147 - hidden proc (RPPM 1 haste, 4s ICD; DBC includes white hits)
class spell_twilight_devastation_proc : public AuraScript
{
    PrepareAuraScript(spell_twilight_devastation_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_TWILIGHT_BEAM, SPELL_TWILIGHT_DAMAGE,
            SPELL_TWILIGHT_DEVASTATION_RANK_1, SPELL_TWILIGHT_DEVASTATION_RANK_2, SPELL_TWILIGHT_DEVASTATION_RANK_3 });
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        Unit* caster = GetTarget();
        if (!caster)
            return;

        CastTwilightDevastation(caster);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_twilight_devastation_proc::HandleProc);
    }
};

// 317159 - shadow damage BP is 0; script fills maxHP * (rank dummy/10)%
class spell_twilight_devastation_damage : public SpellScript
{
    PrepareSpellScript(spell_twilight_devastation_damage);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_TWILIGHT_DEVASTATION_RANK_1, SPELL_TWILIGHT_DEVASTATION_RANK_2, SPELL_TWILIGHT_DEVASTATION_RANK_3 });
    }

    void HandleHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        // The AT passes the (possibly halved) amount via SPELLVALUE_BASE_POINT0; keep it.
        if (GetHitDamage() > 0)
            return;

        int32 damage = int32(float(caster->GetMaxHealth()) * TwilightHealthPct(caster));
        if (damage < 1)
            damage = 1;

        // BP is 0; fill hit and let the school-damage effect apply (crit/vers). Do not DealDamage
        // on top — that would double-dip HP.
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_twilight_devastation_damage::HandleHit);
    }
};

// 317155 - CREATE_AREATRIGGER 19034. Dest on the caster; the effect itself is spawned in
// CastTwilightDevastation (this core often never runs SPELL_EFFECT_HANDLE_HIT for it).
class spell_twilight_devastation_beam : public SpellScript
{
    PrepareSpellScript(spell_twilight_devastation_beam);

    void SetDest(SpellDestination& dest)
    {
        if (Unit* caster = GetCaster())
            dest.Relocate(*caster);
    }

    void PreventAT(SpellEffIndex /*effIndex*/)
    {
        PreventHitDefaultEffect(EFFECT_0);
    }

    void Register() override
    {
        OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_twilight_devastation_beam::SetDest, EFFECT_0, TARGET_DEST_CASTER);
        OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_twilight_devastation_beam::SetDest, EFFECT_0, TARGET_DEST_DEST_FRONT);
        OnEffectHit += SpellEffectFn(spell_twilight_devastation_beam::PreventAT, EFFECT_0, SPELL_EFFECT_CREATE_AREATRIGGER);
    }
};

// areatrigger_template 23070 (SpellMiscId 19034). Cylinder r=3 h=10 flies 28 yd / 4s; damage on enter.
// Hits anything attackable in the path (retail beam pulls idle mobs). 2020-02 hotfix:
// 6th-10th target take half damage, beam ends after the 10th.
struct at_twilight_devastation : AreaTriggerAI
{
    at_twilight_devastation(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger), _hitCount(0) { }

    void OnCreate() override
    {
        // Spline comes from spell_areatrigger_splines (19034: unique points 0..28 yd,
        // Catmullrom needs no duplicated endpoints on this core). Fallback if the DB row is gone.
        if (!at->HasSplines())
        {
            std::vector<Position> pts =
            {
                { 0.0f, 0.0f, 0.0f },
                { TWILIGHT_BEAM_RANGE_YD * 0.33f, 0.0f, 0.0f },
                { TWILIGHT_BEAM_RANGE_YD * 0.66f, 0.0f, 0.0f },
                { TWILIGHT_BEAM_RANGE_YD, 0.0f, 0.0f }
            };
            at->InitSplineOffsets(pts, TWILIGHT_BEAM_TRAVEL_MS);
        }
    }

    void OnUnitEnter(Unit* unit) override
    {
        Unit* caster = at->GetCaster();
        if (!caster || !unit || unit == caster || !unit->IsAlive())
            return;

        SpellInfo const* damageInfo = sSpellMgr->GetSpellInfo(SPELL_TWILIGHT_DAMAGE);
        if (!caster->_IsValidAttackTarget(unit, damageInfo))
            return;

        // Same-tick enters keep calling OnUnitEnter after Remove(); cap before any settle.
        if (_hitCount >= TWILIGHT_BEAM_MAX_TARGETS)
            return;

        // Each target is hit once per beam even if it re-enters the sphere.
        if (!_hitGuids.insert(unit->GetGUID()).second)
            return;

        ++_hitCount;

        int32 damage = int32(float(caster->GetMaxHealth()) * TwilightHealthPct(caster));
        if (_hitCount >= TWILIGHT_BEAM_HALF_DAMAGE_FROM)
            damage /= 2;
        if (damage < 1)
            damage = 1;

        caster->CastCustomSpell(SPELL_TWILIGHT_DAMAGE, SPELLVALUE_BASE_POINT0, damage, unit, TwilightDamageCastFlags());

        if (_hitCount >= TWILIGHT_BEAM_MAX_TARGETS)
            at->Remove();
    }

private:
    std::set<ObjectGuid> _hitGuids;
    uint32 _hitCount;
};

// 317014 - hidden proc. 35662 ProcFlags were hotfixed to 0; spell_proc restores 69904.
class spell_echoing_void_proc : public AuraScript
{
    PrepareAuraScript(spell_echoing_void_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ECHOING_VOID_STACKS, SPELL_ECHOING_VOID_COLLAPSE, SPELL_ECHOING_VOID_DAMAGE,
            SPELL_ECHOING_VOID_RANK_1, SPELL_ECHOING_VOID_RANK_2, SPELL_ECHOING_VOID_RANK_3 });
    }

    void HandleProc(ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        if (Unit* caster = GetTarget())
            HandleEchoingVoidProc(caster, eventInfo);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_echoing_void_proc::HandleProc);
    }
};

// 317022 - collapse periodic. Shared with Hivemind; only rewrite duration / first tick when the
// caster owns 317014 (player Echoing Void). Otherwise leave the boss aura alone.
class spell_echoing_void_collapse : public AuraScript
{
    PrepareAuraScript(spell_echoing_void_collapse);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ECHOING_VOID_PROC, SPELL_ECHOING_VOID_STACKS, SPELL_ECHOING_VOID_DAMAGE });
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster || !caster->HasAura(SPELL_ECHOING_VOID_PROC))
            return;

        uint32 stacks = 1;
        if (Aura const* stackAura = caster->GetAura(SPELL_ECHOING_VOID_STACKS))
            stacks = stackAura->GetStackAmount();
        if (stacks < 1)
            stacks = 1;

        uint32 period = EchoingVoidPeriodMs();
        int32 duration = int32(stacks * period + uint32(ECHOING_VOID_DURATION_SLACK_MS));
        SetMaxDuration(duration);
        SetDuration(duration);

        // Engine first tick is at +period unless START_PERIODIC_AT_APPLY. SimC pulses immediately.
        if (!GetSpellInfo()->HasAttribute(SPELL_ATTR5_START_PERIODIC_AT_APPLY))
            if (Unit* owner = GetTarget())
                owner->CastSpell(owner, SPELL_ECHOING_VOID_DAMAGE, EchoingVoidCastFlags());
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_echoing_void_collapse::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 317029 - shadow AoE, BP=0. Fill maxHP * (rank dummy/100)%. Decrement stacks once per cast
// (AfterCast, not OnHit — this is an AoE). Shared with Hivemind: only rewrite when the
// player owns both 317014 and a self-cast 317022.
class spell_echoing_void_damage : public SpellScript
{
    PrepareSpellScript(spell_echoing_void_damage);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ECHOING_VOID_PROC, SPELL_ECHOING_VOID_STACKS, SPELL_ECHOING_VOID_COLLAPSE,
            SPELL_ECHOING_VOID_RANK_1, SPELL_ECHOING_VOID_RANK_2, SPELL_ECHOING_VOID_RANK_3 });
    }

    void HandleHit()
    {
        Unit* caster = GetCaster();
        Unit* target = GetHitUnit();
        if (!caster || !target)
            return;

        if (!EchoingVoidPlayerOwnsCollapse(caster))
            return;

        int32 damage = int32(float(caster->GetMaxHealth()) * EchoingVoidHealthPct(caster));
        if (damage < 1)
            damage = 1;

        SetHitDamage(damage);
    }

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();
        if (!EchoingVoidPlayerOwnsCollapse(caster))
            return;

        uint32 remain = 0;
        if (Aura* stacks = caster->GetAura(SPELL_ECHOING_VOID_STACKS))
        {
            if (stacks->GetStackAmount() <= 1)
            {
                caster->RemoveAurasDueToSpell(SPELL_ECHOING_VOID_STACKS);
                caster->RemoveAurasDueToSpell(SPELL_ECHOING_VOID_COLLAPSE);
            }
            else
            {
                stacks->ModStackAmount(-1);
                remain = stacks->GetStackAmount();
            }
        }
        else
            caster->RemoveAurasDueToSpell(SPELL_ECHOING_VOID_COLLAPSE);

    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_echoing_void_damage::HandleHit);
        AfterCast += SpellCastFn(spell_echoing_void_damage::HandleAfterCast);
    }
};

// 316815 - hidden proc. DBC already has RPPM 1 and ProcFlags 69908 (autos+abilities).
class spell_twisted_appendage_proc : public AuraScript
{
    PrepareAuraScript(spell_twisted_appendage_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_TWISTED_APPENDAGE_SUMMON, SPELL_TWISTED_APPENDAGE_FLAY,
            SPELL_TWISTED_APPENDAGE_RANK_1, SPELL_TWISTED_APPENDAGE_RANK_2, SPELL_TWISTED_APPENDAGE_RANK_3 });
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Unit* caster = GetTarget())
            CastTwistedAppendage(caster);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_twisted_appendage_proc::HandleProc);
    }
};

// 316835 - Mind Flay periodic, BP=0. Fill max(AP,SP)*(rank dummy/100) from the owner.
// Shared creature entry: no 316815 on the owner -> leave amount at 0.
// originalCaster is the player: ticks use player crit and player-to-creature CLEU.
class spell_twisted_appendage_flay : public AuraScript
{
    PrepareAuraScript(spell_twisted_appendage_flay);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_TWISTED_APPENDAGE_PROC,
            SPELL_TWISTED_APPENDAGE_RANK_1, SPELL_TWISTED_APPENDAGE_RANK_2, SPELL_TWISTED_APPENDAGE_RANK_3 });
    }

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        // Player originalCaster would haste the period/duration. Official tick is 1s / 10s.
        // DBC has no ATTR5_START_PERIODIC_AT_APPLY; first tick at +1s misses the 10th
        // when the 10s tentacle despawns. Mind Flay ticks on apply.
        if (SpellInfo const* info = GetSpellInfo())
        {
            int32 duration = info->GetDuration();
            if (duration > 0)
            {
                SetMaxDuration(duration);
                SetDuration(duration);
            }
        }

        if (AuraEffect* effect = const_cast<AuraEffect*>(aurEff))
        {
            effect->CalculatePeriodic(nullptr, false, false);
            effect->SetPeriodicTimer(0);
        }
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = false;

        Unit* owner = TwistedAppendageOwner(GetCaster());
        if (!owner || !owner->HasAura(SPELL_TWISTED_APPENDAGE_PROC))
            return;

        amount = TwistedAppendageTickDamage(owner);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_twisted_appendage_flay::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_twisted_appendage_flay::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);

    }
};

// 162764 - Twisted Appendage. Only mind-flays when the owner wears 316815.
struct npc_twisted_appendage : public Scripted_NoMovementAI
{
    npc_twisted_appendage(Creature* creature) : Scripted_NoMovementAI(creature) { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (!summoner || !summoner->HasAura(SPELL_TWISTED_APPENDAGE_PROC))
        {
            me->DespawnOrUnsummon();
            return;
        }

        me->SetFaction(summoner->getFaction());
        me->SetLevel(summoner->getLevel());
        me->SetReactState(REACT_PASSIVE);
        me->AddUnitState(UNIT_STATE_ROOT);

        Unit* target = ResolveTentacleTarget(summoner);
        _flayTarget = target ? target->GetGUID() : ObjectGuid::Empty;
        StartFlay(target);
    }

    void UpdateAI(uint32 /*diff*/) override
    {
        Unit* owner = me->GetOwner();
        if (!owner || !owner->IsAlive() || !owner->HasAura(SPELL_TWISTED_APPENDAGE_PROC))
        {
            if (me->IsSummon())
                me->DespawnOrUnsummon();
            return;
        }

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        Unit* target = ObjectAccessor::GetUnit(*me, _flayTarget);
        if (!target || !target->IsAlive())
        {
            target = ResolveTentacleTarget(owner);
            _flayTarget = target ? target->GetGUID() : ObjectGuid::Empty;
        }

        StartFlay(target);
    }

private:
    ObjectGuid _flayTarget;

    void StartFlay(Unit* target)
    {
        Unit* owner = me->GetOwner();
        if (!target || !owner || me->HasUnitState(UNIT_STATE_CASTING))
            return;
        if (target->HasAura(SPELL_TWISTED_APPENDAGE_FLAY, owner->GetGUID()))
            return;

        // melee=false: pull into combat, do not auto-swing.
        if (me->Attack(target, false))
            DoStartNoMovement(target);

        // originalCaster = owner: crit + player-to-creature CLEU. Channel stays on the tentacle.
        me->CastSpell(target, SPELL_TWISTED_APPENDAGE_FLAY, TwistedAppendageFlayFlags(), nullptr, nullptr, owner->GetGUID());
    }
};

// 316814 - hidden proc. DBC already has RPPM 1 and yellow/heal/hostile/periodic/trap flags.
class spell_void_ritual_proc : public AuraScript
{
    PrepareAuraScript(spell_void_ritual_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_VOID_RITUAL_END_COMING,
            SPELL_VOID_RITUAL_RANK_1, SPELL_VOID_RITUAL_RANK_2, SPELL_VOID_RITUAL_RANK_3 });
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Unit* caster = GetTarget())
            CastVoidRitual(caster);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_void_ritual_proc::HandleProc);
    }
};

// 316823 - The End Is Coming. Fill per-stack Dummy; engine multiplies by stacks.
class spell_void_ritual_end_is_coming : public AuraScript
{
    PrepareAuraScript(spell_void_ritual_end_is_coming);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_VOID_RITUAL_PROC,
            SPELL_VOID_RITUAL_RANK_1, SPELL_VOID_RITUAL_RANK_2, SPELL_VOID_RITUAL_RANK_3 });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* owner = GetUnitOwner();
        if (!owner || !owner->HasAura(SPELL_VOID_RITUAL_PROC))
        {
            amount = 0;
            return;
        }

        amount = VoidRitualRatingPerStack(owner);
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* owner = GetUnitOwner();
        if (!owner || !owner->HasAura(SPELL_VOID_RITUAL_PROC))
            return;

        uint32 stacks = GetAura() ? GetAura()->GetStackAmount() : 1;

    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        Unit* owner = GetUnitOwner();
        Aura* aura = GetAura();
        if (!owner || !aura || !owner->HasAura(SPELL_VOID_RITUAL_PROC))
            return;

        uint32 maxStacks = aura->GetMaxStackAmount();
        if (!maxStacks)
            maxStacks = 20;
        uint32 before = aura->GetStackAmount();
        // refresh=false: official window is 20s total. Default ModStackAmount
        // resets duration, which turned a 20s ramp into ~40s (this log: 15s→56s).
        if (before < maxStacks)
            aura->ModStackAmount(1, AURA_REMOVE_BY_DEFAULT, false, false);

        uint32 stacks = aura->GetStackAmount();
        int32 rating = VoidRitualRatingPerStack(owner) * int32(stacks);
        if (stacks != before)

    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_void_ritual_end_is_coming::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
        AfterEffectApply += AuraEffectApplyFn(spell_void_ritual_end_is_coming::HandleApply, EFFECT_0, SPELL_AURA_MOD_RATING, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_void_ritual_end_is_coming::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 315277/81/82 - driver. Aura 285 Trigger 320249; apply hidden if the trigger did not.
class spell_strikethrough_driver : public AuraScript
{
    PrepareAuraScript(spell_strikethrough_driver);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_STRIKETHROUGH_HIDDEN,
            SPELL_STRIKETHROUGH_RANK_1, SPELL_STRIKETHROUGH_RANK_2, SPELL_STRIKETHROUGH_RANK_3 });
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* owner = GetTarget();
        if (!owner || owner->HasAura(SPELL_STRIKETHROUGH_HIDDEN))
            return;
        owner->CastSpell(owner, SPELL_STRIKETHROUGH_HIDDEN, true);
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* owner = GetTarget();
        if (!owner)
            return;
        if (owner->HasAura(SPELL_STRIKETHROUGH_RANK_1) ||
            owner->HasAura(SPELL_STRIKETHROUGH_RANK_2) ||
            owner->HasAura(SPELL_STRIKETHROUGH_RANK_3))
            return;
        owner->RemoveAurasDueToSpell(SPELL_STRIKETHROUGH_HIDDEN);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_strikethrough_driver::HandleApply, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_strikethrough_driver::HandleRemove, EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 320249 - hidden. EFFECT_0 BP=0; fill crit-damage Dummy. EFFECT_1 heal stays 0
// (driver already has a live crit-heal aura; filling it here would double).
class spell_strikethrough_hidden : public AuraScript
{
    PrepareAuraScript(spell_strikethrough_hidden);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_STRIKETHROUGH_RANK_1, SPELL_STRIKETHROUGH_RANK_2, SPELL_STRIKETHROUGH_RANK_3 });
    }

    void CalculateDamage(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        Unit* owner = GetUnitOwner();
        amount = owner ? StrikethroughCritDamagePct(owner) : 0;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_strikethrough_hidden::CalculateDamage, EFFECT_0, SPELL_AURA_MOD_CRIT_DAMAGE_BONUS);
    }
};

// 318220 - hidden proc. DBC already has RPPM 5 and white+yellow+heal+hostile+periodic+trap.
class spell_racing_pulse_proc : public AuraScript
{
    PrepareAuraScript(spell_racing_pulse_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_RACING_PULSE_BUFF,
            SPELL_RACING_PULSE_RANK_1, SPELL_RACING_PULSE_RANK_2, SPELL_RACING_PULSE_RANK_3 });
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Unit* caster = GetTarget())
            CastRacingPulse(caster);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_racing_pulse_proc::HandleProc);
    }
};

// 318227 - haste rating, 4s, no stacks. DBC BP=0; fill from rank Dummy.
class spell_racing_pulse_buff : public AuraScript
{
    PrepareAuraScript(spell_racing_pulse_buff);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_RACING_PULSE_PROC,
            SPELL_RACING_PULSE_RANK_1, SPELL_RACING_PULSE_RANK_2, SPELL_RACING_PULSE_RANK_3 });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* owner = GetUnitOwner();
        if (!owner || !owner->HasAura(SPELL_RACING_PULSE_PROC))
        {
            amount = 0;
            return;
        }

        amount = RacingPulseRating(owner);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_racing_pulse_buff::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
    }
};

// 318214 - hidden proc. DBC already has RPPM 3 and white+yellow+heal+hostile+periodic+trap.
class spell_honed_mind_proc : public AuraScript
{
    PrepareAuraScript(spell_honed_mind_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_HONED_MIND_BUFF,
            SPELL_HONED_MIND_RANK_1, SPELL_HONED_MIND_RANK_2, SPELL_HONED_MIND_RANK_3 });
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Unit* caster = GetTarget())
            CastHonedMind(caster);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_honed_mind_proc::HandleProc);
    }
};

// 318216 - mastery rating, 10s, no stacks. DBC BP=0; fill from rank Dummy.
class spell_honed_mind_buff : public AuraScript
{
    PrepareAuraScript(spell_honed_mind_buff);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_HONED_MIND_PROC,
            SPELL_HONED_MIND_RANK_1, SPELL_HONED_MIND_RANK_2, SPELL_HONED_MIND_RANK_3 });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* owner = GetUnitOwner();
        if (!owner || !owner->HasAura(SPELL_HONED_MIND_PROC))
        {
            amount = 0;
            return;
        }

        amount = HonedMindRating(owner);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_honed_mind_buff::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
    }
};

// 318218 - hidden proc. DBC mask is the same wide set as Racing Pulse; keep crits only.
class spell_deadly_momentum_proc : public AuraScript
{
    PrepareAuraScript(spell_deadly_momentum_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DEADLY_MOMENTUM_BUFF,
            SPELL_DEADLY_MOMENTUM_RANK_1, SPELL_DEADLY_MOMENTUM_RANK_2, SPELL_DEADLY_MOMENTUM_RANK_3 });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return (eventInfo.GetHitMask() & PROC_HIT_CRITICAL) != 0;
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Unit* caster = GetTarget())
            CastDeadlyMomentum(caster);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_deadly_momentum_proc::CheckProc);
        OnProc += AuraProcFn(spell_deadly_momentum_proc::HandleProc);
    }
};

// 318219 - crit rating, 30s, max 5 stacks. Fill per-stack Dummy; engine multiplies.
class spell_deadly_momentum_buff : public AuraScript
{
    PrepareAuraScript(spell_deadly_momentum_buff);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DEADLY_MOMENTUM_PROC,
            SPELL_DEADLY_MOMENTUM_RANK_1, SPELL_DEADLY_MOMENTUM_RANK_2, SPELL_DEADLY_MOMENTUM_RANK_3 });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* owner = GetUnitOwner();
        if (!owner || !owner->HasAura(SPELL_DEADLY_MOMENTUM_PROC))
        {
            amount = 0;
            return;
        }

        amount = DeadlyMomentumRatingPerStack(owner);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_deadly_momentum_buff::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
    }
};

// 318212 - hidden proc. DBC already has RPPM 2 and TAKEN melee/spell/periodic/heal.
class spell_surging_vitality_proc : public AuraScript
{
    PrepareAuraScript(spell_surging_vitality_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SURGING_VITALITY_BUFF,
            SPELL_SURGING_VITALITY_RANK_1, SPELL_SURGING_VITALITY_RANK_2, SPELL_SURGING_VITALITY_RANK_3 });
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Unit* caster = GetTarget())
            CastSurgingVitality(caster);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_surging_vitality_proc::HandleProc);
    }
};

// 318211 - vers rating, 20s, no stacks. DBC BP=0; fill from rank Scaled via CalcValue.
class spell_surging_vitality_buff : public AuraScript
{
    PrepareAuraScript(spell_surging_vitality_buff);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_SURGING_VITALITY_PROC,
            SPELL_SURGING_VITALITY_RANK_1, SPELL_SURGING_VITALITY_RANK_2, SPELL_SURGING_VITALITY_RANK_3 });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;

        Unit* owner = GetUnitOwner();
        if (!owner || !owner->HasAura(SPELL_SURGING_VITALITY_PROC))
        {
            amount = 0;
            return;
        }

        amount = SurgingVitalityRating(owner);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_surging_vitality_buff::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
    }
};

// 318179 - hidden proc. DBC already has RPPM 4 haste and yellow+hostile flags.
class spell_gushing_wound_proc : public AuraScript
{
    PrepareAuraScript(spell_gushing_wound_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_GUSHING_WOUND_DOT, SPELL_GUSHING_WOUND_RANK });
    }

    void HandleProc(ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        Unit* caster = GetTarget();
        if (!caster)
            return;
        if (Unit* target = ResolveGushingWoundTarget(caster, eventInfo))
            CastGushingWound(caster, target);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_gushing_wound_proc::HandleProc);
    }
};

// 318187 - target bleed. DBC BP=0; fill 318272 EFFECT_0 CalcValue. Do not divide by tick count.
class spell_gushing_wound_dot : public AuraScript
{
    PrepareAuraScript(spell_gushing_wound_dot);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_GUSHING_WOUND_PROC, SPELL_GUSHING_WOUND_RANK });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = false;

        Unit* owner = GetCaster();
        if (!owner || !owner->HasAura(SPELL_GUSHING_WOUND_PROC))
        {
            amount = 0;
            return;
        }

        amount = GushingWoundTickDamage(owner);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gushing_wound_dot::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);

    }
};

}

void AddSC_corruption_procs()
{
    RegisterAuraScript(spell_corruption_rank_driver);
    RegisterAuraScript(spell_infinite_stars_proc);
    RegisterSpellScript(spell_infinite_stars_selector);
    RegisterSpellScript(spell_infinite_stars_damage);
    RegisterAuraScript(spell_twilight_devastation_proc);
    RegisterSpellScript(spell_twilight_devastation_beam);
    RegisterSpellScript(spell_twilight_devastation_damage);
    RegisterAreaTriggerAI(at_twilight_devastation);
    RegisterAuraScript(spell_echoing_void_proc);
    RegisterAuraScript(spell_echoing_void_collapse);
    RegisterSpellScript(spell_echoing_void_damage);
    RegisterAuraScript(spell_twisted_appendage_proc);
    RegisterAuraScript(spell_twisted_appendage_flay);
    RegisterCreatureAI(npc_twisted_appendage);
    RegisterAuraScript(spell_void_ritual_proc);
    RegisterAuraScript(spell_void_ritual_end_is_coming);
    RegisterAuraScript(spell_strikethrough_driver);
    RegisterAuraScript(spell_strikethrough_hidden);
    RegisterAuraScript(spell_racing_pulse_proc);
    RegisterAuraScript(spell_racing_pulse_buff);
    RegisterAuraScript(spell_honed_mind_proc);
    RegisterAuraScript(spell_honed_mind_buff);
    RegisterAuraScript(spell_deadly_momentum_proc);
    RegisterAuraScript(spell_deadly_momentum_buff);
    RegisterAuraScript(spell_surging_vitality_proc);
    RegisterAuraScript(spell_surging_vitality_buff);
    RegisterAuraScript(spell_gushing_wound_proc);
    RegisterAuraScript(spell_gushing_wound_dot);
}
