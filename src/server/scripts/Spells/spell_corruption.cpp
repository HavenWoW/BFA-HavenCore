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

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"

#include <algorithm>
#include <cmath>

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

void AddSC_corruption_spell_scripts()
{
    RegisterAuraScript(spell_corruption_grasping_tendrils);
}
