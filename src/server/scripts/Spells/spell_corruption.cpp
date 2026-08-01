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
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Util.h"

#include <algorithm>
#include <cmath>

enum CorruptionSpells
{
    SPELL_EYE_OF_CORRUPTION_SUMMON = 315154,
    SPELL_EYE_OF_CORRUPTION_DAMAGE = 315161
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

void AddSC_corruption_spell_scripts()
{
    RegisterAuraScript(spell_corruption_grasping_tendrils);
    RegisterSpellScript(spell_corruption_eye_of_corruption);
    RegisterAreaTriggerAI(at_corruption_eye_of_corruption);
}
