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

constexpr int32 DEVOUR_VITALITY_PCT_FALLBACK = 2;

constexpr int32 SEARING_BREATH_PCT_FALLBACK = 5;

constexpr uint32 SEARING_FLAMES_MAX_STACKS_FALLBACK = 30;

constexpr int32 WHISPERED_TRUTHS_TRIM_MS_FALLBACK = 2000;

constexpr uint32 FLASH_OF_INSIGHT_MAX_STACKS_FALLBACK = 8;

constexpr int32 OBSIDIAN_ARMOR_PCT_FALLBACK = 700;

constexpr uint32 OBSIDIAN_MAX_STACKS_FALLBACK = 30;

constexpr float OBSIDIAN_SPLIT_PER_TARGET = 0.15f;

constexpr uint32 OBSIDIAN_SPLIT_CAP = 6;

void CorruptionRankItemContext(Unit const* owner, uint32 rankId, uint32& itemId, int32& itemLevel)
{
    itemId = 0;
    itemLevel = -1;
    Player const* player = owner ? owner->ToPlayer() : nullptr;
    if (!player)
        return;

    Aura const* aura = player->GetAura(rankId);
    if (!aura || aura->GetCastItemGUID().IsEmpty())
        return;

    if (Item* item = player->GetItemByGuid(aura->GetCastItemGUID()))
    {
        itemId = item->GetEntry();
        itemLevel = int32(item->GetItemLevel(player));
    }
}

int32 DevourVitalityHealthPct(Unit const* owner)
{
    // 318294 is LINKED_2, not a Dummy rank; summing it with 316615 doubles the leech on real weapons.
    uint32 const ranks[] = { SPELL_DEVOUR_VITALITY_PROC };
    int32 pct = SumCorruptionRankDummy(owner, ranks, 1, EFFECT_0, true,
        DEVOUR_VITALITY_PCT_FALLBACK, "DevourVitality");
    if (pct < 1)
        pct = SumCorruptionRankDummy(owner, ranks, 1, EFFECT_1, true,
            DEVOUR_VITALITY_PCT_FALLBACK, "DevourVitality");
    return pct < 1 ? DEVOUR_VITALITY_PCT_FALLBACK : pct;
}

void CastDevourVitality(Unit* caster, Unit* target)
{
    if (!caster || !target || !caster->IsAlive() || !target->IsAlive() || target == caster)
        return;
    if (!caster->_IsValidAttackTarget(target, sSpellMgr->GetSpellInfo(SPELL_DEVOUR_VITALITY_LEECH)))
        return;
    caster->CastSpell(target, SPELL_DEVOUR_VITALITY_LEECH, InfiniteStarsCastFlags());

}

int32 FlashOfInsightPctPerStack(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_FLASH_OF_INSIGHT_ITEM };
    int32 pct = SumCorruptionRankDummy(owner, ranks, 1, EFFECT_0, true, 1, "FlashOfInsight");
    return pct < 1 ? 1 : pct;
}

uint32 FlashOfInsightMaxStacks()
{
    if (SpellInfo const* buff = sSpellMgr->GetSpellInfo(SPELL_FLASH_OF_INSIGHT_BUFF))
        if (buff->StackAmount >= 1)
            return buff->StackAmount;
    static thread_local bool logged = false;
    if (!logged)
    {
        logged = true;
        TC_LOG_ERROR("scripts", "FlashOfInsight: StackAmount missing, using %u",
            FLASH_OF_INSIGHT_MAX_STACKS_FALLBACK);
    }
    return FLASH_OF_INSIGHT_MAX_STACKS_FALLBACK;
}

void RerollFlashOfInsight(Unit* owner)
{
    if (!owner || !owner->IsAlive())
        return;
    uint32 maxStacks = FlashOfInsightMaxStacks();
    uint32 stacks = urand(1, maxStacks);
    if (!owner->HasAura(SPELL_FLASH_OF_INSIGHT_BUFF))
        owner->CastSpell(owner, SPELL_FLASH_OF_INSIGHT_BUFF, InfiniteStarsCastFlags());
    if (Aura* aura = owner->GetAura(SPELL_FLASH_OF_INSIGHT_BUFF))
        aura->SetStackAmount(int32(stacks));

}

// 35662 dump: 316780 EFFECT_0 Dummy BasePoints is 2000 milliseconds, not 2 seconds.
int32 WhisperedTruthsTrimMs(Unit const* /*owner*/)
{
    if (SpellInfo const* proc = sSpellMgr->GetSpellInfo(SPELL_WHISPERED_TRUTHS_PROC))
        if (SpellEffectInfo const* effect = proc->GetEffect(EFFECT_0))
        {
            int32 ms = effect->BasePoints;
            if (ms >= 100)
                return ms;
            if (ms >= 1 && ms <= 10)
                return ms * IN_MILLISECONDS;
        }
    static thread_local bool logged = false;
    if (!logged)
    {
        logged = true;
        TC_LOG_ERROR("scripts", "WhisperedTruths: Dummy missing, using %d ms",
            WHISPERED_TRUTHS_TRIM_MS_FALLBACK);
    }
    return WHISPERED_TRUTHS_TRIM_MS_FALLBACK;
}

// Same class-spell gate as TryGlimpseTrim, but over the spellbook rather than a live cast.
bool IsWhisperedTruthsCandidate(Player const* player, SpellInfo const* info)
{
    if (!player || !info || info->IsPassive())
        return false;
    if (!info->SpellFamilyName)
        return false;
    if (!info->GetRecoveryTime() && !info->ChargeCategoryId)
        return false;
    if (IsGlimpseExcludedSpell(info->Id))
        return false;
    return true;
}

void CastWhisperedTruths(Player* player)
{
    if (!player)
        return;

    SpellHistory* history = player->GetSpellHistory();
    if (!history)
        return;

    int32 trim = WhisperedTruthsTrimMs(player);
    std::vector<uint32> down;
    for (PlayerSpellMap::value_type const& kv : player->GetSpellMap())
    {
        if (!kv.second || kv.second->state == PLAYERSPELL_REMOVED)
            continue;
        SpellInfo const* info = sSpellMgr->GetSpellInfo(kv.first);
        if (!IsWhisperedTruthsCandidate(player, info))
            continue;
        if (!history->GetRemainingCooldown(info))
            continue;
        down.push_back(info->Id);
    }
    if (down.empty())
    {

        return;
    }

    uint32 id = down[urand(0, uint32(down.size() - 1))];
    history->ModifyCooldown(id, -trim);
    if (SpellInfo const* trimmed = sSpellMgr->GetSpellInfo(id))
        if (trimmed->ChargeCategoryId)
            history->ReduceChargeCooldown(trimmed->ChargeCategoryId, uint32(trim));
    if (sSpellMgr->GetSpellInfo(SPELL_WHISPERED_TRUTHS_LOG))
        player->CastSpell(player, SPELL_WHISPERED_TRUTHS_LOG, InfiniteStarsCastFlags());

}

// 35662 dump: 317290 EFFECT_0 Dummy coeff 40.058 is the damage scale.
// EFFECT_1 Dummy BP=30 is slow semantics; EFFECT_2 Dummy BP=6 is duration seconds.
int32 LashOfTheVoidDamage(Unit const* owner)
{
    SpellInfo const* info = sSpellMgr->GetSpellInfo(SPELL_LASH_OF_THE_VOID);
    SpellEffectInfo const* effect = info ? info->GetEffect(EFFECT_0) : nullptr;
    if (!effect)
    {
        static thread_local bool logged = false;
        if (!logged)
        {
            logged = true;
            TC_LOG_ERROR("scripts", "LashOfTheVoid: scaling Dummy missing");
        }
        return 1;
    }
    uint32 itemId = 0;
    int32 itemLevel = -1;
    CorruptionRankItemContext(owner, SPELL_LASH_OF_THE_VOID, itemId, itemLevel);
    int32 value = effect->CalcValue(owner, nullptr, owner, nullptr, itemId, itemLevel);
    return value < 1 ? 1 : value;
}

// 318293 EFFECT_0 LINKED_2 BP=5 is the breath health pct. Do not also list 316698
// (both BP=5); Task 2 devour double-counted when item + hidden proc were summed.
int32 SearingBreathHealthPct(Unit const* owner)
{
    uint32 const ranks[] = { SPELL_SEARING_FLAMES_ITEM };
    int32 pct = SumCorruptionRankDummy(owner, ranks, 1, EFFECT_0, true,
        SEARING_BREATH_PCT_FALLBACK, "SearingFlames");
    return pct < 1 ? SEARING_BREATH_PCT_FALLBACK : pct;
}

uint32 SearingFlamesMaxStacks()
{
    if (SpellInfo const* buff = sSpellMgr->GetSpellInfo(SPELL_SEARING_FLAMES_BUFF))
        if (buff->StackAmount >= 1)
            return buff->StackAmount;
    static thread_local bool logged = false;
    if (!logged)
    {
        logged = true;
        TC_LOG_ERROR("scripts", "SearingFlames: StackAmount missing, using %u",
            SEARING_FLAMES_MAX_STACKS_FALLBACK);
    }
    return SEARING_FLAMES_MAX_STACKS_FALLBACK;
}

void TrySearingBreath(Unit* caster, Unit* facingTarget)
{
    if (!caster)
        return;
    Aura* buff = caster->GetAura(SPELL_SEARING_FLAMES_BUFF);
    uint32 maxStacks = SearingFlamesMaxStacks();
    if (!buff)
    {
        caster->CastSpell(caster, SPELL_SEARING_FLAMES_BUFF, InfiniteStarsCastFlags());
        buff = caster->GetAura(SPELL_SEARING_FLAMES_BUFF);
        if (buff)
            buff->SetStackAmount(1);

        return;
    }

    uint32 stacks = uint32(buff->GetStackAmount()) + 1;
    if (stacks < maxStacks)
    {
        buff->SetStackAmount(int32(stacks));

        return;
    }

    if (facingTarget && facingTarget != caster)
        caster->SetInFront(facingTarget);
    caster->CastSpell(caster, SPELL_SEARING_FLAMES_BREATH, InfiniteStarsCastFlags());
    buff->Remove();

}

// 316651 EFFECT_2 Dummy3 is armor% of the explosion. EFFECT_0 +5% armor is
// live aura 101 — do not script it. Do not also walk 317420.
int32 ObsidianDestructionArmorPct(Unit const* /*owner*/)
{
    if (SpellInfo const* driver = sSpellMgr->GetSpellInfo(SPELL_OBSIDIAN_SKIN))
        if (SpellEffectInfo const* effect = driver->GetEffect(EFFECT_2))
        {
            int32 pct = effect->BasePoints;
            if (pct >= 1)
                return pct;
        }
    static thread_local bool logged = false;
    if (!logged)
    {
        logged = true;
        TC_LOG_ERROR("scripts", "ObsidianSkin: Dummy3 missing, using %d",
            OBSIDIAN_ARMOR_PCT_FALLBACK);
    }
    return OBSIDIAN_ARMOR_PCT_FALLBACK;
}

uint32 ObsidianMaxStacks()
{
    if (SpellInfo const* tick = sSpellMgr->GetSpellInfo(SPELL_OBSIDIAN_DESTRUCTION))
        if (tick->StackAmount >= 1)
            return tick->StackAmount;
    static thread_local bool logged = false;
    if (!logged)
    {
        logged = true;
        TC_LOG_ERROR("scripts", "ObsidianSkin: StackAmount missing, using %u",
            OBSIDIAN_MAX_STACKS_FALLBACK);
    }
    return OBSIDIAN_MAX_STACKS_FALLBACK;
}

// Combat ticks only. Leaving combat must not Remove 317420 — stacks persist.
void TickObsidianDestruction(Unit* owner)
{
    if (!owner || !owner->IsAlive() || !owner->IsInCombat())
        return;

    Aura* buff = owner->GetAura(SPELL_OBSIDIAN_DESTRUCTION);
    if (!buff)
    {
        owner->CastSpell(owner, SPELL_OBSIDIAN_DESTRUCTION, InfiniteStarsCastFlags());
        buff = owner->GetAura(SPELL_OBSIDIAN_DESTRUCTION);
        if (buff)
            buff->SetStackAmount(1);

        return;
    }

    uint32 maxStacks = ObsidianMaxStacks();
    uint32 stacks = uint32(buff->GetStackAmount());
    if (stacks < maxStacks)
    {
        buff->SetStackAmount(int32(stacks + 1));

        return;
    }

    // Already at cap (including pre-stacked out of combat): explode this tick, then back to 1.
    int32 pct = ObsidianDestructionArmorPct(owner);
    int64 damage = int64(owner->GetArmor()) * pct / 100;
    if (damage < 1)
        damage = 1;
    owner->CastCustomSpell(SPELL_OBSIDIAN_DESTRUCTION_DAMAGE, SPELLVALUE_BASE_POINT0,
        int32(damage), owner, InfiniteStarsCastFlags());
    buff->SetStackAmount(1);

}

// 316615 ProcFlags is already melee-auto only (mask 4). CheckProc only
// needs a live action target; re-testing the melee bit would double-filter.
class spell_devour_vitality_proc : public AuraScript
{
    PrepareAuraScript(spell_devour_vitality_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_DEVOUR_VITALITY_LEECH, SPELL_DEVOUR_VITALITY_ITEM });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetActionTarget() != nullptr;
    }

    void HandleProc(ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        if (Unit* caster = GetTarget())
            CastDevourVitality(caster, eventInfo.GetActionTarget());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_devour_vitality_proc::CheckProc);
        OnProc += AuraProcFn(spell_devour_vitality_proc::HandleProc);
    }
};

// 316617 HEALTH_LEECH BasePoints is 0. EffectHealthLeech reads the
// effect-local damage, so fill SetEffectValue from caster max HP * Dummy.
class spell_devour_vitality_leech : public SpellScript
{
    PrepareSpellScript(spell_devour_vitality_leech);

    void HandleLeech(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;
        int32 pct = DevourVitalityHealthPct(caster);
        int64 value = int64(caster->GetMaxHealth()) * pct / 100;
        if (value < 1)
            value = 1;
        SetEffectValue(int32(value));
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_devour_vitality_leech::HandleLeech,
            EFFECT_0, SPELL_EFFECT_HEALTH_LEECH);
    }
};

class spell_flash_of_insight_proc : public AuraScript
{
    PrepareAuraScript(spell_flash_of_insight_proc);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_FLASH_OF_INSIGHT_BUFF, SPELL_FLASH_OF_INSIGHT_ITEM });
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        RerollFlashOfInsight(GetTarget());
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* owner = GetTarget())
            owner->RemoveAurasDueToSpell(SPELL_FLASH_OF_INSIGHT_BUFF);
    }

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        RerollFlashOfInsight(GetTarget());
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_flash_of_insight_proc::HandleApply,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_flash_of_insight_proc::HandleRemove,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
        OnProc += AuraProcFn(spell_flash_of_insight_proc::HandleProc);
    }
};

class spell_flash_of_insight_buff : public AuraScript
{
    PrepareAuraScript(spell_flash_of_insight_buff);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = true;
        amount = FlashOfInsightPctPerStack(GetUnitOwner());
    }

    void Register() override
    {
        // 35662 dump: 316744 EFFECT_0 is aura 137 (total-stat %), hotfix from 80.
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_flash_of_insight_buff::CalculateAmount,
            EFFECT_0, SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE);
    }
};

// 35662 dump: 316780 ProcFlags=64 (ranged auto only). Do not add CheckProc or the
// aura is gated twice and lab AddAura on a non-hunter still must run if it procs.
class spell_whispered_truths : public AuraScript
{
    PrepareAuraScript(spell_whispered_truths);

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        PreventDefaultAction();
        if (Player* player = GetTarget()->ToPlayer())
            CastWhisperedTruths(player);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_whispered_truths::HandleProc);
    }
};

// 35662 dump: 317290 ProcFlags=4 (melee auto only). Do not add CheckProc or the
// aura is gated twice. Hotfix cleared the 317290 trigger; cone damage is 317291.
class spell_lash_of_the_void : public AuraScript
{
    PrepareAuraScript(spell_lash_of_the_void);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_LASH_OF_THE_VOID_DAMAGE, SPELL_LASH_OF_THE_VOID_SLOW });
    }

    void HandleProc(ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        Unit* caster = GetTarget();
        Unit* target = eventInfo.GetActionTarget();
        if (!caster || !target)
            return;
        int32 damage = LashOfTheVoidDamage(caster);
        // 317291 TargetA=104 (caster-front cone). Casting onto the melee target drops the cone.
        caster->CastCustomSpell(SPELL_LASH_OF_THE_VOID_DAMAGE, SPELLVALUE_BASE_POINT0, damage,
            caster, InfiniteStarsCastFlags());

    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_lash_of_the_void::HandleProc);
    }
};

class spell_lash_of_the_void_damage : public SpellScript
{
    PrepareSpellScript(spell_lash_of_the_void_damage);

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;
        int32 damage = LashOfTheVoidDamage(caster);
        SetHitDamage(damage);
    }

    void Register() override
    {
        // 317291 EFFECT_1 triggers 319241 on the caster; OnHit would SetHitDamage on that target too.
        OnEffectHitTarget += SpellEffectFn(spell_lash_of_the_void_damage::HandleHit,
            EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

// White hits are excluded by spell_proc mask 69904. Do NOT also require
// StartRecoveryTime: no-GCD damaging yellows still stack. Do NOT require
// GetDamage()>0: one multi-hit cast is already gated by the 100ms ICD.
class spell_searing_flames_proc : public AuraScript
{
    PrepareAuraScript(spell_searing_flames_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Spell const* procSpell = eventInfo.GetProcSpell();
        return procSpell && procSpell->GetSpellInfo();
    }

    void HandleProc(ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        TrySearingBreath(GetTarget(), eventInfo.GetActionTarget());
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* owner = GetTarget())
            owner->RemoveAurasDueToSpell(SPELL_SEARING_FLAMES_BUFF);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_searing_flames_proc::CheckProc);
        OnProc += AuraProcFn(spell_searing_flames_proc::HandleProc);
        AfterEffectRemove += AuraEffectRemoveFn(spell_searing_flames_proc::HandleRemove,
            EFFECT_0, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 316704 EFFECT_0 SCHOOL_DAMAGE TargetA=104 cone 60deg 12yd. No FilterTargets:
// SelectImplicitConeTargets already does LoS (2020-02-04) and collision (2020-02-05).
// No caster-trigger second effect (unlike 317291), so OnHit + SetHitDamage is OK.
class spell_searing_breath : public SpellScript
{
    PrepareSpellScript(spell_searing_breath);

    void HandleHit()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;
        int64 damage = int64(caster->GetMaxHealth()) * SearingBreathHealthPct(caster) / 100;
        if (damage < 1)
            damage = 1;
        SetHitDamage(int32(damage));
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_searing_breath::HandleHit);
    }
};

class spell_obsidian_destruction : public AuraScript
{
    PrepareAuraScript(spell_obsidian_destruction);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_OBSIDIAN_DESTRUCTION_DAMAGE, SPELL_OBSIDIAN_SKIN });
    }

    void HandlePeriodic(AuraEffect const* /*aurEff*/)
    {
        PreventDefaultAction();
        TickObsidianDestruction(GetTarget());
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_obsidian_destruction::HandlePeriodic,
            EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_obsidian_destruction_damage : public SpellScript
{
    PrepareSpellScript(spell_obsidian_destruction_damage);

    uint32 _targets = 1;

    void CountTargets(std::list<WorldObject*>& targets)
    {
        _targets = targets.empty() ? 1u : uint32(targets.size());
    }

    void HandleHit()
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;
        int32 pct = ObsidianDestructionArmorPct(caster);
        int64 base = int64(caster->GetArmor()) * pct / 100;
        if (base < 1)
            base = 1;
        uint32 n = std::min(_targets, OBSIDIAN_SPLIT_CAP);
        float totalMul = 1.0f + OBSIDIAN_SPLIT_PER_TARGET * float(n - 1);
        int64 damage = int64(float(base) * totalMul / float(_targets));
        if (damage < 1)
            damage = 1;
        SetHitDamage(int32(damage));
    }

    void Register() override
    {
        // 316661 TargetA=22 is DEST_CASTER; area list is TargetB=15.
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(
            spell_obsidian_destruction_damage::CountTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        OnHit += SpellHitFn(spell_obsidian_destruction_damage::HandleHit);
    }
};

}

void AddSC_corruption_weapons()
{
    RegisterAuraScript(spell_devour_vitality_proc);
    RegisterSpellScript(spell_devour_vitality_leech);
    RegisterAuraScript(spell_flash_of_insight_proc);
    RegisterAuraScript(spell_flash_of_insight_buff);
    RegisterAuraScript(spell_whispered_truths);
    RegisterAuraScript(spell_lash_of_the_void);
    RegisterSpellScript(spell_lash_of_the_void_damage);
    RegisterAuraScript(spell_searing_flames_proc);
    RegisterSpellScript(spell_searing_breath);
    RegisterAuraScript(spell_obsidian_destruction);
    RegisterSpellScript(spell_obsidian_destruction_damage);
}
