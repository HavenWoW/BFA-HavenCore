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

// Wowhead / 清单: slow = effective corruption + 10, cap 99 (2020-02 hotfix).
constexpr int32 GRASPING_TENDRILS_BONUS_PCT = 10;

constexpr int32 GRASPING_TENDRILS_CAP_PCT = 99;

// 315179 DBC BP is 0 on all three live effects. X = Dummy(EFFECT_3) *
// (effectiveCorruption - 50), floored at 0. The -50 offset is the Wowhead
// 2019-10 measured table (hotfix whitelist, approved 2026-08-29).
constexpr int32 INEVITABLE_DOOM_CORRUPTION_OFFSET = 50;

constexpr int32 INEVITABLE_DOOM_PER_POINT_FALLBACK = 1;

// Retail: "Its speed increases with further Corruption." No public numbers —
// approved approximation (2026-08-29 hotfix whitelist): 85% of standard run
// speed (rate 1.0 = 7 yd/s, an unbuffed player's 100%) at the 40-corruption
// threshold, +0.5% per point, capped at 130%.
// 65 + 0.5*corr is that same line without embedding the threshold constant.
constexpr float THING_SPEED_BASE_PCT = 65.0f;

constexpr float THING_SPEED_PER_CORRUPTION_PCT = 0.5f;

constexpr float THING_SPEED_MIN_PCT = 85.0f;

constexpr float THING_SPEED_CAP_PCT = 130.0f;

// Dummy 2 on 315169 is the pulse period. Duration 8s is Wowhead; prefer DBC duration.
constexpr uint32 EYE_PULSE_MS_FALLBACK = 2000;

constexpr uint32 EYE_DURATION_MS_FALLBACK = 8000;

constexpr float EYE_RADIUS_FALLBACK = 10.0f;

// not DBC — log once if radius missing

// 315161 school-damage BP is 0 (8.3.0–8.3.7). Retail is a server script.
// Wowhead 2020-02 measured table; 875*corr-1000 misses the table above 40.
// Interpolate these points. Dummy 15 (taken amp) stays on DBC EFFECT_1.
struct EyePulseSample
{
    int32 corruption;
    int32 damage;
};

constexpr EyePulseSample EYE_PULSE_SAMPLES[] =
{
    { 20, 17157 },
    { 30, 24899 },
    { 40, 34776 },
    { 50, 50940 },
    { 60, 60811 },
    { 70, 68601 },
    { 80, 78431 }
};

// 315197 is max-health % on the player. Guardian-as-caster fails CheckCast
// (same faction / originalCaster=owner / LOS). CAST_DIRECTLY so despawn
// does not cancel the spell event. Also used for the 318393 clone cast onto
// the Thing-from-Beyond body: 161895 is hostile + PC/NPC-immune, so the
// positive cast needs IGNORE_TARGET_CHECK to land.
TriggerCastFlags DelusionHitCastFlags()
{
    return TriggerCastFlags(uint32(InfiniteStarsCastFlags()) |
        uint32(TRIGGERED_IGNORE_TARGET_CHECK) |
        uint32(TRIGGERED_CAST_DIRECTLY));
}

float EffectiveCorruptionRating(Player const* player)
{
    if (!player)
        return 0.0f;
    return player->GetRatingBonusValue(CR_CORRUPTION) - player->GetRatingBonusValue(CR_CORRUPTION_RESISTANCE);
}

int32 GraspingTendrilsBonusPct()
{
    if (SpellInfo const* info = sSpellMgr->GetSpellInfo(SPELL_GRASPING_TENDRILS_PROC))
        if (SpellEffectInfo const* effect = info->GetEffect(EFFECT_0))
            if (effect->BasePoints >= GRASPING_TENDRILS_BONUS_PCT)
                return effect->BasePoints;

    return GRASPING_TENDRILS_BONUS_PCT;
}

int32 GraspingTendrilsSlowPct(Player const* player)
{
    int32 pct = int32(EffectiveCorruptionRating(player)) + GraspingTendrilsBonusPct();
    if (pct < 0)
        pct = 0;
    if (pct > GRASPING_TENDRILS_CAP_PCT)
        pct = GRASPING_TENDRILS_CAP_PCT;
    return pct;
}

int32 InevitableDoomPerPoint()
{
    if (SpellInfo const* info = sSpellMgr->GetSpellInfo(SPELL_INEVITABLE_DOOM))
        if (SpellEffectInfo const* effect = info->GetEffect(EFFECT_3))
            if (effect->BasePoints >= 1)
                return effect->BasePoints;

    static thread_local bool logged = false;
    if (!logged)
    {
        logged = true;
        TC_LOG_ERROR("scripts", "InevitableDoom: 315179 EFFECT_3 Dummy missing, using %d",
            INEVITABLE_DOOM_PER_POINT_FALLBACK);
    }
    return INEVITABLE_DOOM_PER_POINT_FALLBACK;
}

int32 InevitableDoomPct(Player const* player)
{
    int32 pct = InevitableDoomPerPoint()
        * (int32(EffectiveCorruptionRating(player)) - INEVITABLE_DOOM_CORRUPTION_OFFSET);
    return pct > 0 ? pct : 0;
}

float ThingFromBeyondSpeedPct(Player const* player)
{
    float pct = THING_SPEED_BASE_PCT + THING_SPEED_PER_CORRUPTION_PCT * EffectiveCorruptionRating(player);
    if (pct < THING_SPEED_MIN_PCT)
        pct = THING_SPEED_MIN_PCT;
    if (pct > THING_SPEED_CAP_PCT)
        pct = THING_SPEED_CAP_PCT;
    return pct;
}

int32 EyeOfCorruptionVulnPct()
{
    if (SpellInfo const* info = sSpellMgr->GetSpellInfo(SPELL_EYE_OF_CORRUPTION_DAMAGE))
        if (SpellEffectInfo const* effect = info->GetEffect(EFFECT_1))
            return effect->BasePoints;
    return 0;
}

int32 EyeOfCorruptionPulseDamage(Player const* player)
{
    float corr = EffectiveCorruptionRating(player);
    uint32 const count = uint32(sizeof(EYE_PULSE_SAMPLES) / sizeof(EYE_PULSE_SAMPLES[0]));
    if (count < 2)
        return 1;

    auto lerp = [](EyePulseSample const& a, EyePulseSample const& b, float x) -> int32
    {
        float span = float(b.corruption - a.corruption);
        float t = span != 0.0f ? (x - float(a.corruption)) / span : 0.0f;
        int32 damage = int32(float(a.damage) + t * float(b.damage - a.damage));
        return damage < 1 ? 1 : damage;
    };

    if (corr <= float(EYE_PULSE_SAMPLES[0].corruption))
        return lerp(EYE_PULSE_SAMPLES[0], EYE_PULSE_SAMPLES[1], corr);
    if (corr >= float(EYE_PULSE_SAMPLES[count - 1].corruption))
        return lerp(EYE_PULSE_SAMPLES[count - 2], EYE_PULSE_SAMPLES[count - 1], corr);

    for (uint32 i = 0; i + 1 < count; ++i)
        if (corr <= float(EYE_PULSE_SAMPLES[i + 1].corruption))
            return lerp(EYE_PULSE_SAMPLES[i], EYE_PULSE_SAMPLES[i + 1], corr);

    return 1;
}

bool SpellDealsDirectCorruptionHit(SpellInfo const* info)
{
    if (!info)
        return false;
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        SpellEffectInfo const* effect = info->GetEffect(SpellEffIndex(i));
        if (!effect)
            continue;
        if (effect->Effect == SPELL_EFFECT_SCHOOL_DAMAGE ||
            effect->Effect == SPELL_EFFECT_DAMAGE_FROM_MAX_HEALTH_PCT)
            return true;
    }
    return false;
}

// Wowhead 315175 / 315184: "Taking damage has a chance". 35662 ProcFlags also
// set taken-heal / taken-buff bits, which fire out of combat with no HP loss.
bool IsCorruptionTakenDamageProc(ProcEventInfo const& eventInfo)
{
    DamageInfo const* damage = eventInfo.GetDamageInfo();
    if (!damage)
        return false;
    return damage->GetDamage() > 0 || damage->GetAbsorb() > 0;
}

void CastGraspingTendrils(Unit* owner)
{
    Player* player = owner ? owner->ToPlayer() : nullptr;
    if (!player || !player->IsAlive())
        return;

    int32 pct = GraspingTendrilsSlowPct(player);
    player->CastCustomSpell(SPELL_GRASPING_TENDRILS_SLOW, SPELLVALUE_BASE_POINT0, pct, player, InfiniteStarsCastFlags());

}

struct EyeChain
{
    uint32 triggerSpell = 0;
    uint32 summonEntry = 0;
    uint32 damageSpell = 0;
    uint32 areaTriggerMisc = 0;
    float radius = 0.0f;
    uint32 pulseMs = EYE_PULSE_MS_FALLBACK;
    uint32 durationMs = EYE_DURATION_MS_FALLBACK;
};

void InspectSpellForEye(SpellInfo const* info, EyeChain& chain, uint8 depth = 0)
{
    if (!info || depth > 3)
        return;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        SpellEffectInfo const* effect = info->GetEffect(SpellEffIndex(i));
        if (!effect)
            continue;

        if (effect->Effect == SPELL_EFFECT_SUMMON && effect->MiscValue > 0)
            chain.summonEntry = uint32(effect->MiscValue);

        if (effect->Effect == SPELL_EFFECT_CREATE_AREATRIGGER && effect->MiscValue > 0)
            chain.areaTriggerMisc = uint32(effect->MiscValue);

        if (effect->Effect == SPELL_EFFECT_SCHOOL_DAMAGE)
            chain.damageSpell = info->Id;
        if (effect->Effect == SPELL_EFFECT_APPLY_AURA && effect->ApplyAuraName == SPELL_AURA_PERIODIC_DAMAGE)
            chain.damageSpell = info->Id;

        float radius = effect->CalcRadius();
        if (radius > chain.radius)
            chain.radius = radius;

        if (effect->TriggerSpell && effect->TriggerSpell != SPELL_EYE_OF_CORRUPTION_PET
            && effect->TriggerSpell != SPELL_EYE_OF_CORRUPTION)
            InspectSpellForEye(sSpellMgr->GetSpellInfo(effect->TriggerSpell), chain, depth + 1);
    }
}

EyeChain const& ResolveEyeChain()
{
    static thread_local EyeChain chain;
    static thread_local bool loaded = false;
    if (loaded)
        return chain;
    loaded = true;

    SpellInfo const* driver = sSpellMgr->GetSpellInfo(SPELL_EYE_OF_CORRUPTION);
    if (!driver)
    {
        static thread_local bool logged = false;
        if (!logged)
        {
            logged = true;
            TC_LOG_ERROR("scripts", "EyeOfCorruption: 315169 missing");
        }
        return chain;
    }

    if (SpellEffectInfo const* triggerEff = driver->GetEffect(EFFECT_0))
        if (triggerEff->TriggerSpell && triggerEff->TriggerSpell != SPELL_EYE_OF_CORRUPTION_PET)
            chain.triggerSpell = triggerEff->TriggerSpell;

    if (SpellEffectInfo const* dummy = driver->GetEffect(EFFECT_1))
        if (dummy->BasePoints >= 1)
            chain.pulseMs = uint32(dummy->BasePoints) * IN_MILLISECONDS;

    if (chain.triggerSpell)
    {
        if (SpellInfo const* trigger = sSpellMgr->GetSpellInfo(chain.triggerSpell))
        {
            int32 duration = trigger->GetDuration();
            if (duration > 0)
                chain.durationMs = uint32(duration);
            InspectSpellForEye(trigger, chain);
        }
    }

    // 315161 is the pulse; it is not linked from 315154's TriggerSpell chain.
    if (!chain.damageSpell)
        chain.damageSpell = SPELL_EYE_OF_CORRUPTION_DAMAGE;

    if (chain.radius <= 0.0f)
    {
        static thread_local bool logged = false;
        if (!logged)
        {
            logged = true;
            TC_LOG_ERROR("scripts", "EyeOfCorruption: DBC radius missing, using %.1f yd", EYE_RADIUS_FALLBACK);
        }
        chain.radius = EYE_RADIUS_FALLBACK;
    }

    return chain;
}

// areatrigger_template 22815 (SpellMisc 18755). Cylinder search already includes the caster.
// Pulse while the owner is inside; lifetime follows 315154 (8s). Do not require 315169
// — UpdateCorruption() strips the driver at 0 effective corruption and would kill the pulse.
struct at_eye_of_corruption : AreaTriggerAI
{
    at_eye_of_corruption(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

    void OnCreate() override
    {
        EyeChain const& chain = ResolveEyeChain();
        at->SetPeriodicProcTimer(chain.pulseMs ? chain.pulseMs : EYE_PULSE_MS_FALLBACK);
    }

    void OnPeriodicProc() override
    {
        Unit* caster = at->GetCaster();
        if (!caster || !caster->IsAlive())
            return;

        EyeChain const& chain = ResolveEyeChain();
        uint32 damageSpell = chain.damageSpell ? chain.damageSpell : SPELL_EYE_OF_CORRUPTION_DAMAGE;

        bool inRange = false;
        for (ObjectGuid const& guid : at->GetInsideUnits())
        {
            if (guid == caster->GetGUID())
            {
                inRange = true;
                break;
            }
        }

        int32 table = 0;
        int32 stacks = 0;
        if (Player* player = caster->ToPlayer())
        {
            table = EyeOfCorruptionPulseDamage(player);
            if (Aura const* aura = caster->GetAura(damageSpell))
                stacks = int32(aura->GetStackAmount());
        }

        if (inRange)
            caster->CastSpell(caster, damageSpell, DelusionHitCastFlags());

    }
};

Creature* FindOwnedCorruptionSummon(Unit* owner, uint32 entry, float range)
{
    if (!owner || !entry)
        return nullptr;

    for (Unit* unit : owner->m_Controlled)
    {
        if (!unit || unit->GetEntry() != entry)
            continue;
        if (Creature* creature = unit->ToCreature())
            return creature;
    }

    for (Creature* found : owner->FindNearestCreatures(entry, range))
    {
        if (!found)
            continue;
        if (found->GetOwnerGUID() == owner->GetGUID()
            || (found->ToTempSummon() && found->ToTempSummon()->GetSummonerGUID() == owner->GetGUID()))
            return found;
    }

    return nullptr;
}

void CastEyeOfCorruption(Unit* owner)
{
    Player* player = owner ? owner->ToPlayer() : nullptr;
    if (!player || !player->IsAlive())
        return;

    EyeChain const& chain = ResolveEyeChain();
    if (!chain.triggerSpell)
    {
        static thread_local bool logged = false;
        if (!logged)
        {
            logged = true;
            TC_LOG_ERROR("scripts", "EyeOfCorruption: 315169 TriggerSpell missing");
        }

        return;
    }

    // Keep the default CREATE_AREATRIGGER. Do not PreventHitDefaultEffect, and do not
    // hand-create a second eye if GetAreaTriggers is briefly empty after a successful cast.
    player->CastSpell(player, chain.triggerSpell, InfiniteStarsCastFlags());

}

void TryCascadingDisaster(Unit* owner)
{
    if (!owner || !owner->HasAura(SPELL_CASCADING_DISASTER))
        return;

    CastGraspingTendrils(owner);
    CastEyeOfCorruption(owner);

}

constexpr uint32 DELUSION_DURATION_MS_FALLBACK = 8000;

// First UpdateAI can already be in melee (315186 radius includes 0). Wait so the
// clone is visible before the 35% hit despawns the NPC.
constexpr uint32 DELUSION_CONTACT_GRACE_MS = 1000;

struct DelusionChain
{
    uint32 triggerSpell = 0;
    uint32 summonEntry = 0;
    uint32 damageSpell = 0;
    uint32 durationMs = DELUSION_DURATION_MS_FALLBACK;
};

void InspectDelusionSpell(SpellInfo const* info, DelusionChain& chain, uint8 depth = 0)
{
    if (!info || depth > 3 || info->Id == SPELL_THING_FROM_BEYOND_CLOAK)
        return;

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        SpellEffectInfo const* effect = info->GetEffect(SpellEffIndex(i));
        if (!effect)
            continue;
        if (effect->Effect == SPELL_EFFECT_SUMMON && effect->MiscValue > 0)
            chain.summonEntry = uint32(effect->MiscValue);
        if (effect->Effect == SPELL_EFFECT_SCHOOL_DAMAGE ||
            effect->Effect == SPELL_EFFECT_DAMAGE_FROM_MAX_HEALTH_PCT)
            chain.damageSpell = info->Id;
        if (effect->TriggerSpell && effect->TriggerSpell != SPELL_THING_FROM_BEYOND_CLOAK)
            InspectDelusionSpell(sSpellMgr->GetSpellInfo(effect->TriggerSpell), chain, depth + 1);
    }
}

bool SpellHasSummonEffect(SpellInfo const* info)
{
    if (!info)
        return false;
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        SpellEffectInfo const* effect = info->GetEffect(SpellEffIndex(i));
        if (effect && effect->Effect == SPELL_EFFECT_SUMMON && effect->MiscValue > 0)
            return true;
    }
    return false;
}

DelusionChain const& ResolveDelusionChain()
{
    static thread_local DelusionChain chain;
    static thread_local bool loaded = false;
    if (loaded)
        return chain;
    loaded = true;

    // 318392 / 316559 are the remaining same-name rows on the Wowhead chain —
    // dumped to identify the retail shadow-tint visual the clone still lacks
    // (318392 SCRIPT_EFFECT carries 316559 in its base points).

    SpellInfo const* driver = sSpellMgr->GetSpellInfo(SPELL_GRAND_DELUSIONS);
    if (!driver)
    {
        TC_LOG_ERROR("scripts", "GrandDelusions: 315184 missing");
        return chain;
    }

    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
    {
        SpellEffectInfo const* effect = driver->GetEffect(SpellEffIndex(i));
        if (!effect || !effect->TriggerSpell || effect->TriggerSpell == SPELL_THING_FROM_BEYOND_CLOAK)
            continue;
        chain.triggerSpell = effect->TriggerSpell;
        break;
    }

    // Wowhead 315186 is the 8s Thing-from-Beyond summon. Only when DBC TriggerSpell is empty.
    if (!chain.triggerSpell && SpellHasSummonEffect(sSpellMgr->GetSpellInfo(SPELL_GRAND_DELUSIONS_SUMMON)))
        chain.triggerSpell = SPELL_GRAND_DELUSIONS_SUMMON;

    if (chain.triggerSpell)
    {
        if (SpellInfo const* trigger = sSpellMgr->GetSpellInfo(chain.triggerSpell))
        {
            int32 duration = trigger->GetDuration();
            if (duration > 0)
                chain.durationMs = uint32(duration);
            else
                TC_LOG_ERROR("scripts", "GrandDelusions: trigger %u duration missing, using %u ms",
                    chain.triggerSpell, DELUSION_DURATION_MS_FALLBACK);
            InspectDelusionSpell(trigger, chain);
        }
    }

    if (!chain.damageSpell)
    {
        for (uint32 id = 315185; id <= 315190; ++id)
        {
            SpellInfo const* nearby = sSpellMgr->GetSpellInfo(id);
            if (!nearby)
                continue;
            for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                SpellEffectInfo const* effect = nearby->GetEffect(SpellEffIndex(i));
                if (effect && (effect->Effect == SPELL_EFFECT_SCHOOL_DAMAGE ||
                    effect->Effect == SPELL_EFFECT_DAMAGE_FROM_MAX_HEALTH_PCT))
                    chain.damageSpell = id;
            }
        }
    }

    if (!chain.damageSpell)
    {
        if (SpellInfo const* overrideInfo = sSpellMgr->GetSpellInfo(SPELL_THING_FROM_BEYOND_AUTOATTACK))
            if (SpellEffectInfo const* overrideEffect = overrideInfo->GetEffect(EFFECT_1))
                if (overrideEffect->TriggerSpell &&
                    SpellDealsDirectCorruptionHit(sSpellMgr->GetSpellInfo(overrideEffect->TriggerSpell)))
                    chain.damageSpell = overrideEffect->TriggerSpell;
    }

    if (!chain.damageSpell && SpellDealsDirectCorruptionHit(sSpellMgr->GetSpellInfo(SPELL_GRAND_DELUSIONS_DAMAGE)))
        chain.damageSpell = SPELL_GRAND_DELUSIONS_DAMAGE;

    if (!chain.triggerSpell)
        TC_LOG_ERROR("scripts", "GrandDelusions: 315184 TriggerSpell missing");
    if (!chain.summonEntry)
        TC_LOG_ERROR("scripts", "GrandDelusions: summon MiscValue missing");
    if (!chain.damageSpell)
        TC_LOG_ERROR("scripts", "GrandDelusions: school damage spell missing");

    return chain;
}

struct npc_thing_from_beyond : public ScriptedAI
{
    explicit npc_thing_from_beyond(Creature* creature) : ScriptedAI(creature) { }

    void BindOwner(Unit* owner)
    {
        if (!owner)
        {
            me->DespawnOrUnsummon();
            return;
        }

        _ownerGuid = owner->GetGUID();
        _hit = false;
        _elapsed = 0;
        // SummonGuardian copies the owner's faction (green name). Restore the
        // template faction (14 + PC/NPC-immune, 2026_08_29_03): red like retail,
        // but nobody — owner AoE or mobs — can fight the personal delusion.
        if (CreatureTemplate const* creatureTemplate = me->GetCreatureTemplate())
            me->SetFaction(creatureTemplate->faction);
        me->SetLevel(owner->getLevel());
        me->SetReactState(REACT_PASSIVE);
        // 161895 has no creature_template_model; a CLONE_CASTER aura provides
        // the body. Retail 318392's script effect applies 316559 (clone + aura
        // 368 void-shadow tint) — the client keys the dark tint to the clone
        // aura's own spell, so 316559 must be the ONLY clone aura: with plain
        // 318393 applied first the client rendered a full-color copy (verified
        // in-game 2026-08-29, clone=1 tint=1 yet untinted). AddAura instead of
        // a cast: 316559 evaluates hostile and a player cast onto the
        // PC-immune body is filtered out.
        owner->AddAura(SPELL_THING_FROM_BEYOND_TINT, me);
        // Fallback body if the tinted clone could not be applied at all.
        if (!me->HasAura(SPELL_THING_FROM_BEYOND_TINT))
            owner->CastSpell(me, SPELL_THING_FROM_BEYOND_CLONE, DelusionHitCastFlags());
        if (!me->GetDisplayId())
            me->SetDisplayId(owner->GetDisplayId());
        // Retail sniff (creature_template_model 2026_08_29_10) keeps native
        // display 11686 (invisible) on the wire, but this client renders that
        // as a fully invisible unit (verified in-game 2026-08-29) — it does not
        // rebuild the reflection from UNIT_FLAG2_MIRROR_IMAGE alone, so the
        // clone aura's display copy has to stand.
        me->CastSpell(me, SPELL_SHADOWFORM_VISUAL, true);
        // Retail chase tether: 319695 on the victim with the Thing as caster
        // (SpellVisual 93005 links caster and holder, 8s = chase duration).
        // AddAura: the hidden negative self-targeted cast is rejected by
        // CheckCast (verified 2026-08-29), and the caster must be the Thing,
        // not the owner, for the line to anchor on the right unit.
        me->AddAura(SPELL_THING_FROM_BEYOND_TARGET_LOCK, owner);
        // Retail: chase speed rises with corruption. Rate 1.0 is the standard
        // 7 yd/s run speed (an unbuffed player's 100%). Never multiply by the
        // owner's current rate: the 40-tier tendril slow procs off the same
        // damage event, and a 95%-slow snapshot leaves the clone crawling too
        // slowly to ever reach melee range before the 8s despawn.
        if (Player const* player = owner->ToPlayer())
            me->SetSpeedRate(MOVE_RUN, ThingFromBeyondSpeedPct(player) / 100.0f);
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveChase(owner);
    }

    void IsSummonedBy(Unit* summoner) override
    {
        BindOwner(summoner);
    }

    void UpdateAI(uint32 diff) override
    {
        _elapsed += diff;

        Unit* owner = ObjectAccessor::GetUnit(*me, _ownerGuid);
        if (!owner)
            owner = me->GetOwner();
        if (!owner)
            if (TempSummon* summon = me->ToTempSummon())
                owner = summon->GetSummoner();

        if (!owner)
        {
            if (_elapsed >= 2000)
                me->DespawnOrUnsummon();
            return;
        }

        if (!owner->IsAlive() || !owner->HasAura(SPELL_GRAND_DELUSIONS))
        {
            me->DespawnOrUnsummon();
            return;
        }

        DelusionChain const& chain = ResolveDelusionChain();
        if (_elapsed >= chain.durationMs)
        {
            me->DespawnOrUnsummon();
            return;
        }

        if (_hit)
            return;

        if (!me->isMoving())
            me->GetMotionMaster()->MoveChase(owner);

        if (_elapsed < DELUSION_CONTACT_GRACE_MS)
            return;

        // Contact = unit body / melee reach. No raycast, no auto-swing.
        if (!me->IsWithinMeleeRange(owner))
            return;

        _hit = true;

        if (chain.damageSpell)
            owner->CastSpell(owner, chain.damageSpell, DelusionHitCastFlags());

        // Chase ended — drop the tether early instead of letting it run out
        // its full 8s on the victim.
        owner->RemoveAurasDueToSpell(SPELL_THING_FROM_BEYOND_TARGET_LOCK);
        TryCascadingDisaster(owner);
        me->DespawnOrUnsummon();
    }

private:
    ObjectGuid _ownerGuid;
    bool _hit = false;
    uint32 _elapsed = 0;
};

void BindDelusionAI(Creature* thing, Unit* owner)
{
    if (!thing || !owner)
        return;
    if (npc_thing_from_beyond* ai = dynamic_cast<npc_thing_from_beyond*>(thing->AI()))
    {
        ai->BindOwner(owner);
        return;
    }
    thing->AIM_Initialize(new npc_thing_from_beyond(thing));
    if (npc_thing_from_beyond* ai = dynamic_cast<npc_thing_from_beyond*>(thing->AI()))
        ai->BindOwner(owner);
}

void CastGrandDelusions(Unit* owner)
{
    Player* player = owner ? owner->ToPlayer() : nullptr;
    if (!player || !player->IsAlive())
        return;

    DelusionChain const& chain = ResolveDelusionChain();
    if (!chain.triggerSpell)
    {
        static thread_local bool logged = false;
        if (!logged)
        {
            logged = true;
            TC_LOG_ERROR("scripts", "GrandDelusions: 315184 TriggerSpell missing");
        }

        return;
    }

    player->CastSpell(player, chain.triggerSpell, InfiniteStarsCastFlags());
    Creature* thing = FindOwnedCorruptionSummon(player, chain.summonEntry, 30.0f);
    if (thing)
        BindDelusionAI(thing, player);
    else
    {
        static thread_local bool logged = false;
        if (!logged)
        {
            logged = true;
            TC_LOG_ERROR("scripts", "GrandDelusions: summon not bound entry=%u trigger=%u",
                chain.summonEntry, chain.triggerSpell);
        }
    }

    // tint=1 clone=0 is the expected state: 316559 is the retail tinted clone
    // and must be the only clone aura on the body; 318393 (clone=1) is the
    // untinted fallback, only cast when 316559 failed to apply.

}

// 315175 - CorruptionEffects Grasping Tendrils. Taken proc; do not compare thresholds.
class spell_grasping_tendrils_proc : public AuraScript
{
    PrepareAuraScript(spell_grasping_tendrils_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_GRASPING_TENDRILS_SLOW });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return IsCorruptionTakenDamageProc(eventInfo);
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Unit* owner = GetTarget())
            CastGraspingTendrils(owner);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_grasping_tendrils_proc::CheckProc);
        OnProc += AuraProcFn(spell_grasping_tendrils_proc::HandleProc);
    }
};

// 315176 - 5s snare. Amount = min(effectiveCorruption+10, 99).
class spell_grasping_tendrils_slow : public AuraScript
{
    PrepareAuraScript(spell_grasping_tendrils_slow);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_GRASPING_TENDRILS_PROC });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        Player* player = GetUnitOwner() ? GetUnitOwner()->ToPlayer() : nullptr;
        if (!player)
        {
            amount = 0;
            return;
        }
        amount = GraspingTendrilsSlowPct(player);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_grasping_tendrils_slow::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED);
    }
};

// 315169 - CorruptionEffects Eye of Corruption. Class abilities; do not compare thresholds.
class spell_eye_of_corruption : public AuraScript
{
    PrepareAuraScript(spell_eye_of_corruption);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_EYE_OF_CORRUPTION, SPELL_EYE_OF_CORRUPTION_DAMAGE });
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Unit* owner = GetTarget())
            CastEyeOfCorruption(owner);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_eye_of_corruption::HandleProc);
    }
};

// 315161 - DBC school damage BP is 0. Fill the Wowhead 2020 measured table.
// EFFECT_1 Dummy 15 stacks (CumulativeAura 99); same-hit stack is not yet vuln.
class spell_eye_of_corruption_damage : public SpellScript
{
    PrepareSpellScript(spell_eye_of_corruption_damage);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_EYE_OF_CORRUPTION_DAMAGE });
    }

    void HandleHit()
    {
        Unit* target = GetHitUnit();
        if (!target)
            return;

        Player* player = target->ToPlayer();
        if (!player)
            return;

        if (GetHitDamage() > 0)
            return;

        int32 damage = EyeOfCorruptionPulseDamage(player);
        int32 stacks = 0;
        if (Aura const* aura = target->GetAura(SPELL_EYE_OF_CORRUPTION_DAMAGE))
            stacks = int32(aura->GetStackAmount());
        if (stacks > 0)
            --stacks;
        AddPct(damage, EyeOfCorruptionVulnPct() * stacks);
        if (damage < 1)
            damage = 1;

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_eye_of_corruption_damage::HandleHit);
    }
};

// 315184 - CorruptionEffects Grand Delusions. Taken proc. Do not use cloak 313301.
class spell_grand_delusions : public AuraScript
{
    PrepareAuraScript(spell_grand_delusions);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_GRAND_DELUSIONS, SPELL_GRAND_DELUSIONS_SUMMON });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return IsCorruptionTakenDamageProc(eventInfo);
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Unit* owner = GetTarget())
            CastGrandDelusions(owner);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_grand_delusions::CheckProc);
        OnProc += AuraProcFn(spell_grand_delusions::HandleProc);
    }
};

// 315179 - all three live effects have BP 0. Fill Dummy * (corr - 50), floor 0.
// Damage taken is positive; healing and absorb taken are negative (reductions).
// UpdateCorruption() re-casts on every rating change; ModStackAmount recalcs.
class spell_inevitable_doom : public AuraScript
{
    PrepareAuraScript(spell_inevitable_doom);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_INEVITABLE_DOOM });
    }

    int32 DoomPct() const
    {
        Player const* player = GetUnitOwner() ? GetUnitOwner()->ToPlayer() : nullptr;
        return player ? InevitableDoomPct(player) : 0;
    }

    void CalculateDamageTaken(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        amount = DoomPct();

    }

    void CalculateHealingTaken(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        amount = -DoomPct();
    }

    void CalculateAbsorbTaken(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        amount = -DoomPct();
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_inevitable_doom::CalculateDamageTaken,
            EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_inevitable_doom::CalculateHealingTaken,
            EFFECT_1, SPELL_AURA_MOD_HEALING_PCT);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_inevitable_doom::CalculateAbsorbTaken,
            EFFECT_2, SPELL_AURA_MOD_ABSORB_EFFECTS_TAKEN_PCT);
    }

};

// 337612 - TriggerSpell is 0. Combat ticks cast 337816 (DBC max-health %).
class spell_inescapable_consequences : public AuraScript
{
    PrepareAuraScript(spell_inescapable_consequences);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_INESCAPABLE_CONSEQUENCES,
            SPELL_INESCAPABLE_CONSEQUENCES_DAMAGE });
    }

    uint32 TickPeriod() const
    {
        if (AuraEffect const* effect = GetEffect(EFFECT_0))
            if (effect->GetPeriod() > 0)
                return uint32(effect->GetPeriod());
        return GetSpellInfo()->GetEffect(EFFECT_0)
            ? uint32(GetSpellInfo()->GetEffect(EFFECT_0)->ApplyAuraPeriod)
            : 1000;
    }

    void TryTick()
    {
        Unit* owner = GetTarget();
        if (!owner || !owner->IsAlive() || !owner->IsInCombat())
            return;

        uint32 now = getMSTime();
        if (_lastTick && getMSTimeDiff(_lastTick, now) < TickPeriod())
            return;
        _lastTick = now;

        owner->CastSpell(owner, SPELL_INESCAPABLE_CONSEQUENCES_DAMAGE, InfiniteStarsCastFlags());

    }

    void HandleUpdate(uint32 /*diff*/)
    {
        Unit* owner = GetTarget();
        bool combat = owner && owner->IsInCombat();
        if (combat && !_wasInCombat)
            TryTick();
        _wasInCombat = combat;
    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        PreventDefaultAction();
        TryTick();
    }

    void Register() override
    {
        OnAuraUpdate += AuraUpdateFn(spell_inescapable_consequences::HandleUpdate);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_inescapable_consequences::HandlePeriodic,
            EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }

private:
    uint32 _lastTick = 0;
    bool _wasInCombat = false;
};

}

void AddSC_corruption_drawbacks()
{
    RegisterAuraScript(spell_grasping_tendrils_proc);
    RegisterAuraScript(spell_grasping_tendrils_slow);
    RegisterAuraScript(spell_eye_of_corruption);
    RegisterSpellScript(spell_eye_of_corruption_damage);
    RegisterAuraScript(spell_grand_delusions);
    RegisterAuraScript(spell_inevitable_doom);
    RegisterAuraScript(spell_inescapable_consequences);
    RegisterAreaTriggerAI(at_eye_of_corruption);
    RegisterCreatureAI(npc_thing_from_beyond);
}
