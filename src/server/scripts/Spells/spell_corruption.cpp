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

/*
 * Scripts for the 8.3 corruption drawback penalties that Player::UpdateCorruption
 * applies from CorruptionEffects.db2.
 *
 * Every magnitude in this file is a documented approximation. SpellEffect.db2 gives each
 * drawback spell BasePoints 0, Coefficient 0 and RealPointsPerLevel 0, and no world DB
 * override table carries a row for them - retail computed these server-side and shipped
 * the client a placeholder. The sourcing for each curve is in the commit that added it.
 */

#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "Log.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Unit.h"
#include "Util.h"

#include <algorithm>
#include <cmath>

enum CorruptionSpells
{
    SPELL_EYE_OF_CORRUPTION_SUMMON = 315154,
    SPELL_EYE_OF_CORRUPTION_DAMAGE = 315161,
    SPELL_GRAND_DELUSIONS_SUMMON   = 315186
};

// Grasping Tendrils, 1+ Corruption. Container 315175 procs on damage taken and triggers
// this 5 second snare, whose magnitude is Corruption + 10 percent.
namespace GraspingTendrils
{
    constexpr float SlowBasePct     = 10.0f;  // snare at zero effective corruption
    constexpr float SlowPctPerPoint = 1.0f;   // added per point of effective corruption
    constexpr float SlowMaxPct      = 100.0f; // reported to reach a full root around 90
}

// Grasping Tendrils - 315176
class spell_corruption_grasping_tendrils : public AuraScript
{
    PrepareAuraScript(spell_corruption_grasping_tendrils);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        Player* player = GetUnitOwner()->ToPlayer();
        if (!player)
            return;

        float const slow = std::min(GraspingTendrils::SlowBasePct + player->GetEffectiveCorruption() * GraspingTendrils::SlowPctPerPoint,
            GraspingTendrils::SlowMaxPct);

        // Unit::UpdateSpeed feeds the strongest negative modifier straight to AddPct, so a
        // snare has to be stored negative.
        amount = -int32(std::lround(slow));

        // Corruption can change while the 5s debuff is up - an item swap, a zone granting
        // resistance. Snapshot at application rather than retuning mid-flight.
        canBeRecalculated = false;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_corruption_grasping_tendrils::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED);
    }
};

// Eye of Corruption, 20+ Corruption. Container 315169 procs on damage dealt and triggers
// 315154, which creates areatrigger 22815 for eight seconds.
//
// The tooltip promises that "range and damage increase with further Corruption", and those
// are exactly the two quantities the client does not carry: every scaling field on the
// damage effect 315161 is zero, ContentTuningID is 0 and there is no hotfix row. Both
// curves below are Wowhead's 8.3 PTR sampling (news 295810), which its own author calls
// approximate. The timings around them are read from the data instead - see the ctor.
namespace EyeOfCorruption
{
    constexpr float DamagePerCorruption = 875.0f;  // per point of effective corruption
    constexpr float DamageFlatOffset    = 1000.0f; // subtracted from the total
    constexpr float RadiusPerCorruption = 0.2f;    // yards per point, ie corruption / 5
    constexpr float DefaultRadius       = 5.0f;    // yards, if 22815 ever loses its shape data
    constexpr uint32 DefaultPeriod      = 2;       // seconds, if 315154 ever loses its effect 1
}

// Eye of Corruption - areatrigger 22815, created by 315154
struct at_corruption_eye_of_corruption : AreaTriggerAI
{
    at_corruption_eye_of_corruption(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger), _radius(0.0f)
    {
        uint32 period = EyeOfCorruption::DefaultPeriod;

        // The client renders this same field as "every $s2 sec", so reading it keeps the
        // tooltip and the server in step.
        if (SpellInfo const* summon = sSpellMgr->GetSpellInfo(SPELL_EYE_OF_CORRUPTION_SUMMON))
            if (SpellEffectInfo const* periodEffect = summon->GetEffect(EFFECT_1))
                if (periodEffect->BasePoints > 0)
                    period = uint32(periodEffect->BasePoints);

        areatrigger->SetPeriodicProcTimer(period * IN_MILLISECONDS);
    }

    void OnCreate() override
    {
        // Scale the drawn ring and the range test off one division, so they cannot drift.
        // An earlier version drove the graphic with SetObjectScale instead, and the damage
        // then cut off outside the ring the player could see.
        float const templateRadius = at->GetTemplate()->CylinderDatas.Radius > 0.0f
            ? at->GetTemplate()->CylinderDatas.Radius
            : EyeOfCorruption::DefaultRadius;

        _radius = templateRadius;

        if (Player* player = at->GetCaster() ? at->GetCaster()->ToPlayer() : nullptr)
        {
            // A zero reading is possible - a tier aura can outlive the gear that granted
            // it - and must not shrink the zone to nothing.
            float const scaled = player->GetEffectiveCorruption() * EyeOfCorruption::RadiusPerCorruption;
            if (scaled > 0.0f)
                _radius = scaled;
        }

        at->SetOverrideScaleCurve(_radius / templateRadius);

        TC_LOG_DEBUG("scripts.corruption", "Eye of Corruption: spawned, caster %s, radius %.2f (template %.2f)",
            at->GetCasterGuid().ToString().c_str(), _radius, templateRadius);
    }

    void OnPeriodicProc() override
    {
        Unit* caster = at->GetCaster();
        if (!caster)
            return;

        // Measured centre to centre, and deliberately 2d because the zone is a cylinder.
        // Neither IsWithinDist2d nor GetInsideUnits() will do: both reach IsInDist, which
        // adds the player's GetObjectSize() to the radius and so pads the damage zone past
        // the drawn ring by the player's CombatReach.
        if (caster->GetExactDist2d(at) <= _radius)
            caster->CastSpell(caster, SPELL_EYE_OF_CORRUPTION_DAMAGE, true);
    }

private:
    float _radius;
};

// Eye of Corruption - 315161
class spell_corruption_eye_of_corruption : public SpellScript
{
    PrepareSpellScript(spell_corruption_eye_of_corruption);

    void CalculateDamage(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetHitUnit();
        Unit* caster = GetCaster();
        if (!target || !caster)
            return;

        Player* player = caster->ToPlayer();
        if (!player)
            return;

        float damage = EyeOfCorruption::DamagePerCorruption * player->GetEffectiveCorruption() - EyeOfCorruption::DamageFlatOffset;

        // Below about 1.15 corruption the formula is still negative. The tier only unlocks
        // at 20, so this never fires in practice, but a negative SetHitDamage would heal.
        if (damage <= 0.0f)
            return;

        // "Increasing Shadow damage": each tick leaves a stack that raises the next one.
        // Effect 1 holds the per-stack figure and the client renders the tooltip from that
        // same row. The stack this cast applies is not counted - effect 1 has not run yet
        // at effect 0's hook, which is what makes the first tick land unamplified.
        if (SpellEffectInfo const* stackEffect = GetSpellInfo()->GetEffect(EFFECT_1))
            AddPct(damage, float(stackEffect->BasePoints) * float(target->GetAuraCount(GetSpellInfo()->Id)));

        SetHitDamage(int32(damage));
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_corruption_eye_of_corruption::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// Grand Delusions, 40+ Corruption. Container 315184 procs on damage taken and triggers
// 315186, whose single effect summons creature 161895, the Thing From Beyond.
//
// The client carries the summon itself: 315186 has DurationIndex 31 for an 8 second life,
// radius index 9 so the Thing appears up to 20 yards away, and SummonProperties 4793 for
// Control NONE, faction 14 over the template's friendly 35, and a personal spawn. What it
// does not carry is the pursuit speed or the damage - the tooltip promises the speed rises
// with corruption but gives no numbers, and no movement-speed aura exists anywhere in the
// corruption block. Both constants below are approximations, isolated for retuning.
namespace GrandDelusions
{
    constexpr uint32 ContainerSpell   = 315184; // the debuff the player carries
    constexpr uint32 CloneCasterSpell = 60352;  // generic Clone Caster, see IsSummonedBy
    constexpr float Threshold         = 40.0f;  // CorruptionEffects.db2 MinCorruption

    constexpr float SpeedRateAtThreshold = 0.75f;   // 5.25 yd/s against a player's 7.0
    constexpr float SpeedRatePerPoint    = 0.0125f; // reaches parity at 60, overtakes above
    constexpr float SpeedRateMax         = 2.0f;

    // "About your health" is what a whole pursuit costs, not what one swing does, so the
    // total is divided across the strikes the Thing has time to land.
    constexpr float TotalDamagePctOfMaxHealth = 90.0f;
}

// Thing From Beyond - creature 161895, summoned by 315186
struct npc_corruption_thing_from_beyond : ScriptedAI
{
    npc_corruption_thing_from_beyond(Creature* creature) : ScriptedAI(creature),
        _strikeCooldown(0), _damagePctPerStrike(GrandDelusions::TotalDamagePctOfMaxHealth) { }

    void IsSummonedBy(Unit* summoner) override
    {
        Player* player = summoner ? summoner->ToPlayer() : nullptr;
        if (!player)
        {
            me->DespawnOrUnsummon();
            return;
        }

        _summonerGuid = player->GetGUID();

        // Retail's Thing From Beyond is a copy of the player it chases, so its appearance
        // was never meant to come from the template - 161895's only model is the invisible
        // stalker 11686. The clone must be a real aura: HandleMirrorImageDataRequest answers
        // the client's request for the copy's gear only if the unit carries
        // SPELL_AURA_CLONE_CASTER, and reads the appearance off that aura's caster.
        //
        // Applied rather than cast, because Clone Caster is positive and this summon is
        // hostile, so CheckCast answers SPELL_FAILED_BAD_TARGETS. Forgiving that needs
        // TRIGGERED_IGNORE_TARGET_CHECK, which sits outside TRIGGERED_FULL_MASK and so is
        // unreachable from CastSpell(..., true). AddAura skips CheckCast and screens only
        // for immunity, which 161895 has none of.
        player->AddAura(GrandDelusions::CloneCasterSpell, me);

        // GetEffectiveResistChance adds (victim level - attacker level) * 5 resistance, so
        // a Thing below its target's level has much of its damage resisted before it lands.
        // A mirror of the player should be the player's level at any level.
        me->SetLevel(player->getLevel());

        // Both figures are the summon's own: the creature's swing timer, and TempSummon's
        // remaining life, still the full duration here because InitSummon runs immediately
        // after InitStats set it.
        uint32 const swingTime = me->GetBaseAttackTime(BASE_ATTACK);
        uint32 const pursuitTime = me->ToTempSummon() ? me->ToTempSummon()->GetTimer() : 0;
        uint32 const strikes = (swingTime && pursuitTime) ? std::max(1u, pursuitTime / swingTime) : 1;

        _damagePctPerStrike = GrandDelusions::TotalDamagePctOfMaxHealth / float(strikes);

        // The Thing is a movement puzzle, not a fight. Passive keeps the core's own melee
        // out of it, which would otherwise land ordinary swings alongside the scripted ones
        // and drag the Thing into normal combat and evade handling.
        me->SetReactState(REACT_PASSIVE);

        float const corruption = player->GetEffectiveCorruption();
        float const rate = std::min(GrandDelusions::SpeedRateMax,
            GrandDelusions::SpeedRateAtThreshold
                + std::max(0.0f, corruption - GrandDelusions::Threshold) * GrandDelusions::SpeedRatePerPoint);

        TC_LOG_DEBUG("scripts.corruption", "Thing From Beyond: spawned for %s at corruption %.1f, speed rate %.2f, clone %s",
            _summonerGuid.ToString().c_str(), corruption, rate,
            me->HasAuraType(SPELL_AURA_CLONE_CASTER) ? "applied" : "MISSING");

        me->SetSpeedRate(MOVE_RUN, rate);
        me->GetMotionMaster()->MoveChase(player);
    }

    void UpdateAI(uint32 diff) override
    {
        if (_summonerGuid.IsEmpty())
            return;

        Player* player = ObjectAccessor::GetPlayer(*me, _summonerGuid);
        if (!player || !player->IsAlive())
        {
            me->DespawnOrUnsummon();
            return;
        }

        if (_strikeCooldown > diff)
        {
            _strikeCooldown -= diff;
            return;
        }

        // Measure the way the chase generator measures. IsWithinMeleeRange squares the
        // height difference into the distance while ChaseMovementGenerator stops on a 2d
        // test, so on uneven ground the pursuit arrives and then fails its own strike check
        // forever - one observed pursuit ended at 2d 5.72 with dz -1.34, which is 5.88 in
        // three dimensions against a melee range of 5.08.
        if (me->GetExactDist2d(player) > me->GetMeleeRange(player))
            return;

        // No spell exists for this hit: 315186 has one effect, neither creature carries a
        // spell in creature_template.spell1-8 or creature_template_spell, and nothing in
        // the surrounding id range is a plausible contact hit. Retail drove it from creature
        // data the client never shipped, so the magnitude stays a script constant.
        //
        // It is still reported as 315184 rather than dealt anonymously. DealDamage writes
        // health and sends nothing, so the player was killed by an attack that never
        // appeared in their combat log - and it bypassed absorbs, resistances and every
        // damage-taken modifier, making the hit an unconditional execute.
        SpellInfo const* damageSpell = sSpellMgr->GetSpellInfo(GrandDelusions::ContainerSpell);
        if (!damageSpell)
        {
            me->DespawnOrUnsummon();
            return;
        }

        uint32 const damage = CalculatePct(player->GetMaxHealth(), _damagePctPerStrike);

        SpellNonMeleeDamage damageInfo(me, player, damageSpell->Id,
            damageSpell->GetSpellXSpellVisualId(me), SPELL_SCHOOL_MASK_SHADOW);
        me->CalculateSpellDamageTaken(&damageInfo, int32(damage), damageSpell);
        me->SendSpellNonMeleeDamageLog(&damageInfo);
        me->DealSpellDamage(&damageInfo, false);

        // Connecting does not spend the Thing. Cascading Disaster settles it: "if you are
        // struck by the Thing From Beyond, you will be immediately afflicted by Grasping
        // Tendrils and Eye of Corruption" - a snare applied by a pursuer that vanished on
        // contact would do nothing. The 8 seconds are the escape window, not a countdown to
        // one guaranteed hit.
        _strikeCooldown = me->GetBaseAttackTime(BASE_ATTACK);
    }

private:
    ObjectGuid _summonerGuid;
    uint32 _strikeCooldown;
    float _damagePctPerStrike;
};

void AddSC_corruption_spell_scripts()
{
    RegisterAuraScript(spell_corruption_grasping_tendrils);
    RegisterSpellScript(spell_corruption_eye_of_corruption);
    RegisterAreaTriggerAI(at_corruption_eye_of_corruption);
    RegisterCreatureAI(npc_corruption_thing_from_beyond);
}
