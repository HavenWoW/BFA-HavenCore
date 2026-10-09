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

// 315573 / 315574 Dummy is 3 seconds. Do not hardcode 3000 as the only value.
constexpr int32 GLIMPSE_TRIM_MS_FALLBACK = 3000;

// 316801 DBC BP is 50 on both effects. Strength is the driver Dummy (30/50).
constexpr int32 INEFFABLE_TRUTH_RANK1_PCT_FALLBACK = 30;

int32 GlimpseTrimMs(Unit const* /*owner*/)
{
    if (SpellInfo const* buff = sSpellMgr->GetSpellInfo(SPELL_GLIMPSE_BUFF))
        if (SpellEffectInfo const* effect = buff->GetEffect(EFFECT_0))
            if (effect->BasePoints >= 1)
                return effect->BasePoints * IN_MILLISECONDS;

    if (SpellInfo const* proc = sSpellMgr->GetSpellInfo(SPELL_GLIMPSE_PROC))
        if (SpellEffectInfo const* effect = proc->GetEffect(EFFECT_0))
            if (effect->BasePoints >= 1)
                return effect->BasePoints * IN_MILLISECONDS;

    static thread_local bool logged = false;
    if (!logged)
    {
        logged = true;
        TC_LOG_ERROR("scripts", "GlimpseOfClarity: Dummy missing, using trim %d ms",
            GLIMPSE_TRIM_MS_FALLBACK);
    }
    return GLIMPSE_TRIM_MS_FALLBACK;
}

bool IsIneffableTruthOwnSpell(uint32 id)
{
    switch (id)
    {
        case SPELL_INEFFABLE_TRUTH_RANK_1:
        case SPELL_INEFFABLE_TRUTH_RANK_2:
        case SPELL_INEFFABLE_TRUTH_PROC:
        case SPELL_INEFFABLE_TRUTH_BUFF:
            return true;
        default:
            return false;
    }
}

int32 IneffableTruthPct(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_INEFFABLE_TRUTH_RANK_1, SPELL_INEFFABLE_TRUTH_RANK_2 };
    return SumCorruptionRankDummy(owner, ranks, 2, EFFECT_0, true,
        INEFFABLE_TRUTH_RANK1_PCT_FALLBACK, "IneffableTruth");
}

void ApplyIneffableTruthRate(int32& ms, int32 pct)
{
    if (ms > 0 && pct > 0)
        ms = int32(int64(ms) * 100 / (100 + pct));
}

void ScaleExistingCharges(Player* player, int32 pct, bool apply);

void ScaleExistingCooldowns(Player* player, int32 pct, bool apply)
{
    if (!player || pct <= 0)
        return;

    SpellHistory* history = player->GetSpellHistory();
    if (!history)
        return;

    for (PlayerSpellMap::value_type const& kv : player->GetSpellMap())
    {
        if (!kv.second || kv.second->state == PLAYERSPELL_REMOVED)
            continue;

        SpellInfo const* info = sSpellMgr->GetSpellInfo(kv.first);
        if (!info || IsIneffableTruthOwnSpell(info->Id) || IsGlimpseExcludedSpell(info->Id))
            continue;

        uint32 remain = history->GetRemainingCooldown(info);
        if (!remain || remain > uint32(DAY * IN_MILLISECONDS))
            continue;

        int64 scaled = apply
            ? (int64(remain) * 100) / (100 + pct)
            : (int64(remain) * (100 + pct)) / 100;
        int32 delta = int32(scaled - int64(remain));
        if (delta)
            history->ModifyCooldown(info->Id, delta);
    }

    ScaleExistingCharges(player, pct, apply);
}

bool IneffableTruthScalesChargeCategory(Player const* player, uint32 chargeCategoryId)
{
    if (!player || !chargeCategoryId)
        return false;

    for (PlayerSpellMap::value_type const& kv : player->GetSpellMap())
    {
        if (!kv.second || kv.second->state == PLAYERSPELL_REMOVED)
            continue;

        SpellInfo const* info = sSpellMgr->GetSpellInfo(kv.first);
        if (!info || info->ChargeCategoryId != chargeCategoryId)
            continue;
        if (info->IsPassive() || IsIneffableTruthOwnSpell(info->Id) || IsGlimpseExcludedSpell(info->Id))
            continue;
        return true;
    }

    return false;
}

void ScaleExistingCharges(Player* player, int32 pct, bool apply)
{
    if (!player || pct <= 0)
        return;

    SpellHistory* history = player->GetSpellHistory();
    if (!history)
        return;

    std::set<uint32> seen;
    for (PlayerSpellMap::value_type const& kv : player->GetSpellMap())
    {
        if (!kv.second || kv.second->state == PLAYERSPELL_REMOVED)
            continue;

        SpellInfo const* info = sSpellMgr->GetSpellInfo(kv.first);
        if (!info || !info->ChargeCategoryId || !seen.insert(info->ChargeCategoryId).second)
            continue;
        if (!IneffableTruthScalesChargeCategory(player, info->ChargeCategoryId))
            continue;

        history->ScaleChargeRecovery(info->ChargeCategoryId, pct, apply);
    }
}

void ConsumeGlimpseStack(Player* player)
{
    Aura* aura = player->GetAura(SPELL_GLIMPSE_BUFF);
    if (!aura)
        return;

    if (aura->GetStackAmount() <= 1)
        player->RemoveAurasDueToSpell(SPELL_GLIMPSE_BUFF);
    else
        aura->ModStackAmount(-1);
}

void TryGlimpseTrim(Player* player, Spell* spell)
{
    if (!player || !spell || !player->HasAura(SPELL_GLIMPSE_BUFF))
        return;

    SpellInfo const* info = spell->GetSpellInfo();
    if (!info || info->IsPassive() || spell->IsTriggered())
        return;
    if (!info->SpellFamilyName)
        return;
    if (!info->GetRecoveryTime() && !info->ChargeCategoryId)
        return;
    if (spell->m_CastItem || spell->m_castItemEntry)
        return;
    if (IsGlimpseExcludedSpell(info->Id))
        return;

    SpellHistory* history = player->GetSpellHistory();
    if (!history)
        return;

    int32 trim = GlimpseTrimMs(player);
    uint32 before = history->GetRemainingCooldown(info);
    if (info->ChargeCategoryId)
        history->ReduceChargeCooldown(info->ChargeCategoryId, uint32(trim));
    if (info->GetRecoveryTime() > 0 || before > 0)
        history->ModifyCooldown(info->Id, -trim);

    uint32 after = history->GetRemainingCooldown(info);
    ConsumeGlimpseStack(player);

}

// 315573 - Glimpse buff. Dummy only; trim is PlayerScript OnSuccessfulSpellCast.
class spell_glimpse_of_clarity : public AuraScript
{
    PrepareAuraScript(spell_glimpse_of_clarity);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_GLIMPSE_PROC, SPELL_GLIMPSE_ITEM });
    }

    void Register() override { }
};

class player_glimpse_of_clarity : public PlayerScript
{
public:
    player_glimpse_of_clarity() : PlayerScript("player_glimpse_of_clarity") { }

    void OnSuccessfulSpellCast(Player* player, Spell* spell) override
    {
        TryGlimpseTrim(player, spell);
    }
};

// 316801 - 10s recharge buff. Aura 143/173 are NYI; Dummy comes from the driver.
class spell_ineffable_truth : public AuraScript
{
    PrepareAuraScript(spell_ineffable_truth);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_INEFFABLE_TRUTH_PROC,
            SPELL_INEFFABLE_TRUTH_RANK_1, SPELL_INEFFABLE_TRUTH_RANK_2 });
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        amount = IneffableTruthPct(GetUnitOwner());
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* owner = GetUnitOwner();
        int32 pct = IneffableTruthPct(owner);
        if (!_scaled)
        {
            if (Player* player = owner ? owner->ToPlayer() : nullptr)
            {
                ScaleExistingCooldowns(player, pct, true);
                _pct = pct;
                _scaled = true;
            }
        }

        int32 mult = pct > 0 ? (100 * 100 / (100 + pct)) : 100;

    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (!_scaled)
            return;

        Unit* owner = GetUnitOwner();
        if (Player* player = owner ? owner->ToPlayer() : nullptr)
            ScaleExistingCooldowns(player, _pct, false);
        _scaled = false;
        _pct = 0;
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_ineffable_truth::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RECOVERY_RATE);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_ineffable_truth::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_RECOVERY_RATE_2);
        AfterEffectApply += AuraEffectApplyFn(spell_ineffable_truth::HandleApply, EFFECT_0, SPELL_AURA_MOD_RECOVERY_RATE, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_ineffable_truth::HandleRemove, EFFECT_0, SPELL_AURA_MOD_RECOVERY_RATE, AURA_EFFECT_HANDLE_REAL);
    }

private:
    bool _scaled = false;
    int32 _pct = 0;
};

class player_ineffable_truth : public PlayerScript
{
public:
    player_ineffable_truth() : PlayerScript("player_ineffable_truth") { }

    void OnCooldownStart(Player* player, SpellInfo const* spellInfo, uint32 itemId, int32& cooldown, uint32& /*categoryId*/, int32& categoryCooldown) override
    {
        if (!player || !spellInfo || !player->HasAura(SPELL_INEFFABLE_TRUTH_BUFF))
            return;
        if (itemId || spellInfo->IsPassive())
            return;
        if (IsIneffableTruthOwnSpell(spellInfo->Id) || IsGlimpseExcludedSpell(spellInfo->Id))
            return;

        int32 pct = IneffableTruthPct(player);
        ApplyIneffableTruthRate(cooldown, pct);
        ApplyIneffableTruthRate(categoryCooldown, pct);
    }

    void OnChargeRecoveryTimeStart(Player* player, uint32 chargeCategoryId, int32& chargeRecoveryTime) override
    {
        if (!player || !player->HasAura(SPELL_INEFFABLE_TRUTH_BUFF))
            return;
        if (!IneffableTruthScalesChargeCategory(player, chargeCategoryId))
            return;

        ApplyIneffableTruthRate(chargeRecoveryTime, IneffableTruthPct(player));
    }

    // Logout saves SpellHistory before auras drop. Strip 316801 first so HandleRemove
    // restores CDs, then the save writes the unscaled remaining times.
    void OnSave(Player* player) override
    {
        if (!player || !player->HasAura(SPELL_INEFFABLE_TRUTH_BUFF))
            return;
        WorldSession* session = player->GetSession();
        if (!session || !session->PlayerLogout())
            return;
        player->RemoveAurasDueToSpell(SPELL_INEFFABLE_TRUTH_BUFF);
    }
};

}

void AddSC_corruption_cooldowns()
{
    RegisterAuraScript(spell_glimpse_of_clarity);
    RegisterPlayerScript(player_glimpse_of_clarity);
    RegisterAuraScript(spell_ineffable_truth);
    RegisterPlayerScript(player_ineffable_truth);
}
