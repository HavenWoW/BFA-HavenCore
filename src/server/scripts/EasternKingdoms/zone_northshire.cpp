/*
* 2026 BFA-HavenCore
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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



/*######
## npc_stormwind_infantry
######*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "World.h"
#include "PetAI.h"
#include "PassiveAI.h"
#include "CombatAI.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Cell.h"
#include "CellImpl.h"
#include "SpellAuras.h"
#include "Vehicle.h"
#include "Player.h"
#include "SpellScript.h"
#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "TemporarySummon.h"

enum NorthshireCreatures
{
    NPC_BROTHER_PAXTON      = 951,
    NPC_STORMWIND_INFANTRY  = 49869,
    NPC_BLACKROCK_WORG      = 49871,
    NPC_BLACKROCK_SPY       = 49874
};

enum NorthshireSettings
{
    AMBIENT_COMBAT_HEALTH_FLOOR_PCT = 85,
    WORG_FIGHTING_FACTION           = 232,
    WORG_RESTORE_FACTION            = 7
};

enum NorthshireSpells
{
    SPELL_WORG_GROWL        = 2649,
    SPELL_PAXTON_FORTITUDE  = 13864,
    SPELL_PAXTON_FLASH_HEAL = 38588,
    SPELL_PAXTON_RENEW      = 8362,
    SPELL_PAXTON_PENANCE    = 66097,
    SPELL_SPYGLASS          = 80676,
    SPELL_SPYING            = 92857,
    SPELL_RENEWED_LIFE      = 93097
};

enum NorthshirePoints
{
    POINT_INJURED_TO_PAXTON = 1
};

static constexpr float INFANTRY_LOCAL_ASSIST_RANGE = 5.0f;
static constexpr float WORG_INITIAL_SPAWN_RADIUS = 5.0f;
static constexpr float WORG_INITIAL_ROAM_RADIUS = 5.0f;
static constexpr float PAXTON_HEAL_RANGE = 20.0f;
static constexpr float PAXTON_HEAL_THRESHOLD_PCT = 95.0f;
static constexpr float INJURED_PAXTON_SEARCH_RANGE = 100.0f;

static void ClampAmbientCombatDamage(Unit* unit, uint32& damage)
{
    uint64 healthFloor = unit->CountPctFromMaxHealth(AMBIENT_COMBAT_HEALTH_FLOOR_PCT);

    if (unit->GetHealth() <= healthFloor)
    {
        damage = 0;
        return;
    }

    uint64 maxDamage = unit->GetHealth() - healthFloor;
    if (damage > maxDamage)
        damage = uint32(maxDamage);
}

enum
{
    QUEST_FEAR_NO_EVIL_WORGEN_WARRIOR = 28813,
    QUEST_FEAR_NO_EVIL_ALLIANCE = 29082,
    QUEST_FEAR_NO_EVIL_ALLIANCE_2 = 28809,
    QUEST_FEAR_NO_EVIL_ALLIANCE_3 = 28808,
    QUEST_FEAR_NO_EVIL_ALLIANCE_4 = 28811,
    QUEST_FEAR_NO_EIVL_ALLIANCE_5 = 28810,
    QUEST_FEAR_NO_EVIL_ALLIANCE_6 = 28806,
    QUEST_FEAR_NO_EVIL_ALLIANCE_NIGHT_ELF_WARLOCK_DK = 28812,
};

class npc_stormwind_infantry : public CreatureScript
{
public:
    npc_stormwind_infantry() : CreatureScript("npc_stormwind_infantry") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_stormwind_infantryAI(creature);
    }

    struct npc_stormwind_infantryAI : public ScriptedAI
    {
        npc_stormwind_infantryAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 waitTime;
        ObjectGuid wolfTarget;

        void Reset() override
        {
            wolfTarget = ObjectGuid::Empty;
            me->SetSheath(SHEATH_STATE_MELEE);
            waitTime = urand(0, 2000);
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (attacker && attacker->GetEntry() == NPC_BLACKROCK_WORG)
                ClampAmbientCombatDamage(me, damage);
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (!who || !wolfTarget.IsEmpty() || me->IsInCombat())
                return;

            Creature* worg = who->ToCreature();
            if (!worg || worg->GetEntry() != NPC_BLACKROCK_WORG)
                return;

            // Retail: Infantry only assists against a normal worg when the
            // player brings it close. It must never acquire another
            // Infantry's staged summon.
            if (me->GetDistance(worg) > INFANTRY_LOCAL_ASSIST_RANGE || !worg->IsInCombat())
                return;

            if (IsStagedWorg(worg))
                return;

            AttackStart(worg);
        }

        Creature* RecoverOwnedWorg()
        {
            std::list<Creature*> wolves;
            me->GetCreatureListWithEntryInGrid(wolves, NPC_BLACKROCK_WORG, 100.0f);

            Creature* ownedWorg = nullptr;

            for (Creature* wolf : wolves)
            {
                TempSummon* summon = wolf->ToTempSummon();
                if (!summon || summon->GetSummonerGUID() != me->GetGUID())
                    continue;

                if (!wolf->IsAlive())
                {
                    wolf->DespawnOrUnsummon();
                    continue;
                }

                if (!ownedWorg)
                {
                    ownedWorg = wolf;
                    wolfTarget = wolf->GetGUID();
                    continue;
                }

                // Grid/AI reloads can lose wolfTarget while a summon remains.
                // Keep one owned worg and remove stale duplicates.
                wolf->DespawnOrUnsummon();
            }

            return ownedWorg;
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) override
        {
            if (summon->GetGUID() == wolfTarget)
                ReturnHomeAfterWorg();
        }

        void SummonedCreatureDespawn(Creature* summon) override
        {
            if (summon->GetGUID() == wolfTarget)
                ReturnHomeAfterWorg();
        }

        void ReturnHomeAfterWorg()
        {
            wolfTarget = ObjectGuid::Empty;
            me->GetThreatManager().ClearAllThreat();
            me->CombatStop(true);
            me->GetMotionMaster()->MoveTargetedHome();
            waitTime = urand(10000, 20000);
        }

        bool IsAtHome() const
        {
            Position const& home = me->GetHomePosition();
            return me->GetDistance2d(home.GetPositionX(), home.GetPositionY()) <= 1.0f;
        }

        bool IsStagedWorg(Creature const* worg) const
        {
            if (!worg)
                return false;

            if (TempSummon const* summon = worg->ToTempSummon())
                return !summon->GetSummonerGUID().IsEmpty();

            return false;
        }

        bool HandleUnassignedCombat()
        {
            if (!wolfTarget.IsEmpty())
                return false;

            if (Unit* victim = me->GetVictim())
            {
                if (victim->IsAlive() && me->IsValidAttackTarget(victim))
                    return true;

                me->AttackStop();
            }

            if (me->IsInCombat())
            {
                me->GetThreatManager().ClearAllThreat();
                me->CombatStop(true);
            }

            // Retail local-assist behavior: once a nearby natural worg has
            // been engaged, Infantry follows it. When combat ends it returns
            // to its own position before resuming the staged cycle.
            if (!IsAtHome())
            {
                me->GetMotionMaster()->MoveTargetedHome();
                return true;
            }

            return false;
        }

        void UpdateAI(uint32 diff) override
        {
            DoMeleeAttackIfReady();

            if (HandleUnassignedCombat())
                return;

            if (waitTime > diff)
            {
                waitTime -= diff;
                return;
            }

            waitTime = urand(10000, 20000);

            if (!wolfTarget.IsEmpty())
            {
                if (Creature* wolf = ObjectAccessor::GetCreature(*me, wolfTarget))
                {
                    if (wolf->IsAlive())
                    {
                        // The owning Infantry stays on its own worg even when a
                        // player takes the worg's threat and drags it away.
                        if (me->GetVictim() != wolf && wolf->IsInCombat())
                        {
                            me->GetThreatManager().AddThreat(wolf, 1000000.0f);
                            AttackStart(wolf);
                        }
                    }
                    else
                    {
                        ReturnHomeAfterWorg();
                        wolf->DespawnOrUnsummon();
                    }
                }
                else
                    ReturnHomeAfterWorg();

                return;
            }

            // Do not create a second summon after a grid/AI reload.
            if (RecoverOwnedWorg())
                return;

            float spawnDistance = frand(2.0f, WORG_INITIAL_SPAWN_RADIUS);
            float spawnAngle = frand(0.0f, float(M_PI * 2.0f));
            Position wolfPos = me->GetNearPosition(spawnDistance, spawnAngle);

            float z = me->GetMap()->GetHeight(
                me->GetPhaseShift(),
                wolfPos.GetPositionX(),
                wolfPos.GetPositionY(),
                wolfPos.GetPositionZ());
            wolfPos.m_positionZ = z;

            if (Creature* wolf = me->SummonCreature(NPC_BLACKROCK_WORG, wolfPos))
            {
                // Retail: the staged worg appears a few yards around its
                // Infantry, wanders locally for several seconds, and only
                // then begins fighting its owning Infantry.
                wolfTarget = wolf->GetGUID();
                wolf->SetFaction(WORG_RESTORE_FACTION);
                wolf->GetMotionMaster()->MoveRandom(WORG_INITIAL_ROAM_RADIUS);
            }
        }
    };
};

/*######
## npc_blackrock_battle_worg
######*/

class npc_blackrock_battle_worg : public CreatureScript
{
public:
    npc_blackrock_battle_worg() : CreatureScript("npc_blackrock_battle_worg") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_blackrock_battle_worgAI(creature);
    }

    struct npc_blackrock_battle_worgAI : public ScriptedAI
    {
        npc_blackrock_battle_worgAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 engageTimer;
        uint32 growlTimer;

        void Reset() override
        {
            engageTimer = urand(3000, 5000);
            growlTimer = urand(8500, 10000);
            me->SetFaction(WORG_RESTORE_FACTION);

            if (GetOwningInfantry())
            {
                me->SetReactState(REACT_PASSIVE);
                me->GetMotionMaster()->MoveRandom(WORG_INITIAL_ROAM_RADIUS);
            }
            else
                me->SetReactState(REACT_AGGRESSIVE);
        }

        Creature* GetOwningInfantry() const
        {
            TempSummon* summon = me->ToTempSummon();
            if (!summon)
                return nullptr;

            Creature* owner = ObjectAccessor::GetCreature(*me, summon->GetSummonerGUID());
            if (!owner || owner->GetEntry() != NPC_STORMWIND_INFANTRY)
                return nullptr;

            return owner;
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (!attacker)
                return;

            if (attacker->GetTypeId() == TYPEID_PLAYER || attacker->IsPet())
            {
                // A player/pet can kill the worg normally. The owning Infantry
                // keeps following and attacking its assigned worg.
                me->SetReactState(REACT_AGGRESSIVE);
                me->GetThreatManager().ResetAllThreat();
                me->GetThreatManager().AddThreat(attacker, 1000000.0f);
                AttackStart(attacker);
                return;
            }

            if (attacker->GetEntry() == NPC_STORMWIND_INFANTRY)
                ClampAmbientCombatDamage(me, damage);
        }

        void UpdateAI(uint32 diff) override
        {
            if (Creature* owner = GetOwningInfantry())
            {
                if (!me->IsInCombat())
                {
                    if (engageTimer > diff)
                    {
                        engageTimer -= diff;
                        return;
                    }

                    // Retail: the staged worg wanders for roughly 3-5 seconds
                    // before its own Infantry engages it. No other Infantry is
                    // allowed to acquire this summon.
                    engageTimer = 0;
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->SetFaction(WORG_FIGHTING_FACTION);
                    me->GetMotionMaster()->Clear();

                    me->GetThreatManager().AddThreat(owner, 1000000.0f);
                    owner->GetThreatManager().AddThreat(me, 1000000.0f);

                    AttackStart(owner);

                    if (owner->IsAIEnabled)
                        owner->AI()->AttackStart(me);
                }
            }

            if (!UpdateVictim())
            {
                if (!GetOwningInfantry())
                    me->SetFaction(WORG_RESTORE_FACTION);

                return;
            }

            if (growlTimer <= diff)
            {
                DoCastVictim(SPELL_WORG_GROWL);
                growlTimer = urand(8500, 10000);
            }
            else
                growlTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_brother_paxton
######*/

// 951 - Brother Paxton
struct npc_brother_paxton : public ScriptedAI
{
    npc_brother_paxton(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        // Retail ambient healing is occasional, not continuous.
        _healTimer = urand(15000, 25000);

        me->GetThreatManager().ClearAllThreat();
        me->CombatStop(true);
        me->SetReactState(REACT_PASSIVE);
        me->SetWalk(false);
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveIdle();

        if (!me->HasAura(SPELL_PAXTON_FORTITUDE))
            DoCastSelf(SPELL_PAXTON_FORTITUDE, true);
    }

    void JustEngagedWith(Unit* /*who*/) override { }
    void AttackStart(Unit* /*who*/) override { }
    void MoveInLineOfSight(Unit* /*who*/) override { }

    void UpdateAI(uint32 diff) override
    {
        if (_healTimer > diff)
        {
            _healTimer -= diff;
            return;
        }

        _healTimer = urand(30000, 50000);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        std::list<Creature*> infantry;
        me->GetCreatureListWithEntryInGrid(infantry, NPC_STORMWIND_INFANTRY, PAXTON_HEAL_RANGE);

        Creature* target = nullptr;
        for (Creature* soldier : infantry)
        {
            if (!soldier->IsAlive() || soldier->GetHealthPct() >= PAXTON_HEAL_THRESHOLD_PCT)
                continue;

            if (!target || soldier->GetHealthPct() < target->GetHealthPct())
                target = soldier;
        }

        if (!target)
            return;

        me->SetFacingToObject(target);
        Talk(0, target);

        switch (urand(0, 2))
        {
            case 0:
                DoCast(target, SPELL_PAXTON_FLASH_HEAL);
                break;
            case 1:
                DoCast(target, SPELL_PAXTON_RENEW, true);
                break;
            case 2:
                // Retail Paxton visibly attempts Penance as part of his ambient
                // healing behavior even though the channel is not reliable.
                DoCast(target, SPELL_PAXTON_PENANCE);
                break;
            default:
                break;
        }
    }

private:
    uint32 _healTimer;
};

/*######
## npc_blackrock_spy
######*/

// 49874 - Blackrock Spy
struct npc_blackrock_spy : public ScriptedAI
{
    npc_blackrock_spy(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        ApplySpyState();
    }

    void JustEngagedWith(Unit* who) override
    {
        Talk(0, who);
        me->RemoveAurasDueToSpell(SPELL_SPYGLASS);
        me->RemoveAurasDueToSpell(SPELL_SPYING);
    }

    void JustReachedHome() override
    {
        ApplySpyState();
    }

    void UpdateAI(uint32 /*diff*/) override
    {
        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }

private:
    void ApplySpyState()
    {
        // creature_addon owns the persistent retail spawn presentation.
        // Reapply only as a fallback if an aura was removed by combat/reset.
        if (!me->HasAura(SPELL_SPYING))
            DoCastSelf(SPELL_SPYING, true);

        if (me->GetDefaultMovementType() == IDLE_MOTION_TYPE)
        {
            if (!me->HasAura(SPELL_SPYGLASS))
                DoCastSelf(SPELL_SPYGLASS, true);
        }
        else
            me->RemoveAurasDueToSpell(SPELL_SPYGLASS);
    }
};

/*######
## npc_stormwind_injured_soldier
######*/

// 50047 - Injured Stormwind Infantry
struct npc_stormwind_injured_soldier : public ScriptedAI
{
    npc_stormwind_injured_soldier(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        ScriptedAI::Reset();

        // A respawn/reset must never inherit delayed revive actions from the
        // previous interaction cycle.
        me->GetScheduler().CancelAll();
        _clickerGuid.Clear();

        // 50047 is a normal DB-spawned NPC, never a player-created minion.
        // Clear any stale creator left by the legacy 93072 quest spell script.
        me->SetCreatorGUID(ObjectGuid::Empty);

        me->NearTeleportTo(me->GetHomePosition());

        // Some historical spawn data can carry an emote state independently
        // from the stand state. Clear it so the injured pose is controlled by
        // UNIT_STAND_STATE_DEAD only.
        me->SetEmoteState(EMOTE_STATE_NONE);
        me->SetStandState(UNIT_STAND_STATE_DEAD);

        // Fear No Evil uses the native spell-click interaction.
        // Do not expose a gossip interaction for this creature.
        me->RemoveNpcFlag(UNIT_NPC_FLAG_GOSSIP);
        me->AddNpcFlag(UNIT_NPC_FLAG_SPELLCLICK);
    }

    void OnSpellClick(Unit* clicker, bool& result) override
    {
        Player* player = clicker ? clicker->ToPlayer() : nullptr;
        if (!player || !HasFearNoEvilQuest(player))
        {
            result = false;
            return;
        }

        // Only one revive sequence may be active for this DB spawn.
        me->GetScheduler().CancelAll();
        _clickerGuid = player->GetGUID();

        // HandleSpellClick casts 93072 before this AI callback. The legacy
        // quest spell script marks 50047 with the player's CreatedBy GUID,
        // which makes the client display "<Player's Minion>". This creature
        // is a normal DB spawn, never a player-created minion.
        me->SetCreatorGUID(ObjectGuid::Empty);

        // 93072 is executed by npc_spellclick_spells and supplies quest credit.
        // The soldier itself owns the revive visual, matching the reference
        // implementation and avoiding player-owned/minion presentation.
        me->CastSpell(me, SPELL_RENEWED_LIFE, true);

        me->RemoveNpcFlag(UNIT_NPC_FLAG_SPELLCLICK);

        // Clear any persistent DB/spawn emote before changing the stand state.
        // Stand state and emote state are independent client presentation fields.
        me->SetEmoteState(EMOTE_STATE_NONE);
        me->SetStandState(UNIT_STAND_STATE_STAND);

        me->GetScheduler().Schedule(1s, [this](TaskContext /*task*/)
        {
            // Reassert the revived presentation after the spell/update cycle.
            // This prevents a late spawn/addon state update from leaving the
            // soldier visually prone while the revive sequence continues.
            me->SetEmoteState(EMOTE_STATE_NONE);
            me->SetStandState(UNIT_STAND_STATE_STAND);

            if (Player* player = ObjectAccessor::GetPlayer(*me, _clickerGuid))
            {
                me->SetFacingToObject(player);

                // Passing the player supplies the context used by $N in
                // creature_text.
                Talk(0, player);
            }
            else
                Talk(0);

            me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
        });

        me->GetScheduler().Schedule(3s, [this](TaskContext /*task*/)
        {
            // Movement should always begin from the revived standing state.
            me->SetEmoteState(EMOTE_STATE_NONE);
            me->SetStandState(UNIT_STAND_STATE_STAND);
            me->SetWalk(false);

            if (Creature* paxton = me->FindNearestCreature(NPC_BROTHER_PAXTON, INJURED_PAXTON_SEARCH_RANGE, true))
            {
                // Retail: after the revive/emote sequence, the soldier only
                // begins running toward Brother Paxton and disappears shortly
                // afterwards rather than completing the run.
                Position destination = paxton->GetNearPosition(1.0f, paxton->GetRelativeAngle(me));
                me->GetMotionMaster()->MovePoint(POINT_INJURED_TO_PAXTON, destination);
                me->ForcedDespawn(1800, 15s);
            }
            else
            {
                // Paxton should normally be present. Preserve the same short
                // visible run even if he cannot be found.
                me->GetMotionMaster()->MoveRandom(10.0f);
                me->ForcedDespawn(1800, 15s);
            }
        });
    }

    void MovementInform(uint32 type, uint32 pointId) override
    {
        if (type != POINT_MOTION_TYPE || pointId != POINT_INJURED_TO_PAXTON)
            return;

        // Normally the timed despawn above happens first. This only handles
        // the unlikely case where the soldier reaches the destination sooner.
        me->ForcedDespawn(1, 15s);
    }

private:
    static bool HasFearNoEvilQuest(Player const* player)
    {
        return player->GetQuestStatus(QUEST_FEAR_NO_EVIL_WORGEN_WARRIOR) == QUEST_STATUS_INCOMPLETE
            || player->GetQuestStatus(QUEST_FEAR_NO_EVIL_ALLIANCE) == QUEST_STATUS_INCOMPLETE
            || player->GetQuestStatus(QUEST_FEAR_NO_EVIL_ALLIANCE_2) == QUEST_STATUS_INCOMPLETE
            || player->GetQuestStatus(QUEST_FEAR_NO_EVIL_ALLIANCE_3) == QUEST_STATUS_INCOMPLETE
            || player->GetQuestStatus(QUEST_FEAR_NO_EVIL_ALLIANCE_4) == QUEST_STATUS_INCOMPLETE
            || player->GetQuestStatus(QUEST_FEAR_NO_EIVL_ALLIANCE_5) == QUEST_STATUS_INCOMPLETE
            || player->GetQuestStatus(QUEST_FEAR_NO_EVIL_ALLIANCE_6) == QUEST_STATUS_INCOMPLETE
            || player->GetQuestStatus(QUEST_FEAR_NO_EVIL_ALLIANCE_NIGHT_ELF_WARLOCK_DK) == QUEST_STATUS_INCOMPLETE;
    }

    ObjectGuid _clickerGuid;
};

/*######
## npc_training_dummy_elwynn
######*/

enum eTrainingDummySpells
{
    SPELL_CHARGE        = 100,
    SPELL_AUTORITE      = 105361, // OnDamage
    SPELL_ASSURE        = 56641,
    SPELL_EVISCERATION  = 2098,
    SPELL_MOT_DOULEUR_1 = 589,
    SPELL_MOT_DOULEUR_2 = 124464, // Je ne sais pas si un des deux est le bon
    SPELL_NOVA          = 122,
    SPELL_CORRUPTION_1  = 172,
    SPELL_CORRUPTION_2  = 87389,
    SPELL_CORRUPTION_3  = 131740,
    SPELL_PAUME_TIGRE   = 100787
};

class npc_training_dummy_start_zones : public CreatureScript
{
public:
    npc_training_dummy_start_zones() : CreatureScript("npc_training_dummy_start_zones") { }

    struct npc_training_dummy_start_zonesAI : Scripted_NoMovementAI
    {
        npc_training_dummy_start_zonesAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {}

        uint32 resetTimer;

        void Reset() override
        {
            me->SetControlled(true, UNIT_STATE_STUNNED);//disable rotate
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);//imune to knock aways like blast wave

            resetTimer = 5000;
        }

        void EnterEvadeMode(EvadeReason /*why*/) override
        {
            if (!_EnterEvadeMode())
                return;

            Reset();
        }

        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (!me->IsWithinDistInMap(p_Who, 25.f) && p_Who->IsInCombat())
            {
                me->RemoveAllAurasByCaster(p_Who->GetGUID());
                p_Who->GetThreatManager().ClearThreat(me);
            }
        }

        void DamageTaken(Unit* doneBy, uint32& damage) override
        {
            resetTimer = 5000;
            damage = 0;

            if (doneBy->HasAura(SPELL_AUTORITE))
            {
                if (Player* player = doneBy->ToPlayer())
                {
                    player->KilledMonsterCredit(44175);
                    player->KilledMonsterCredit(44548);

                }
            }
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            return;
        }

        void SpellHit(Unit* Caster, const SpellInfo* Spell) override
        {
            switch (Spell->Id)
            {
                case SPELL_CHARGE:
                case SPELL_ASSURE:
                case SPELL_EVISCERATION:
                case SPELL_MOT_DOULEUR_1:
                case SPELL_MOT_DOULEUR_2:
                case SPELL_NOVA:
                case SPELL_CORRUPTION_1:
                case SPELL_CORRUPTION_2:
                case SPELL_CORRUPTION_3:
                case SPELL_PAUME_TIGRE:
                {
                    if (Player* player = Caster->ToPlayer())
                    {
                        player->KilledMonsterCredit(44175);
                        player->KilledMonsterCredit(44548);
                    }
                    break;
                }
                default:
                    break;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (!me->HasUnitState(UNIT_STATE_STUNNED))
                me->SetControlled(true, UNIT_STATE_STUNNED);//disable rotate

            if (resetTimer <= diff)
            {
                EnterEvadeMode(EVADE_REASON_OTHER);
                resetTimer = 5000;
            }
            else
                resetTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_training_dummy_start_zonesAI(creature);
    }
};

/*######
## spell_quest_fear_no_evil
######*/

class spell_quest_fear_no_evil : public SpellScriptLoader
{
public:
    spell_quest_fear_no_evil() : SpellScriptLoader("spell_quest_fear_no_evil") { }

    class spell_quest_fear_no_evil_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_quest_fear_no_evil_SpellScript);

        void OnDummy(SpellEffIndex /*effIndex*/)
        {
            if (GetCaster())
                if (GetCaster()->ToPlayer())
                    GetCaster()->ToPlayer()->KilledMonsterCredit(50047);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_quest_fear_no_evil_SpellScript::OnDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_quest_fear_no_evil_SpellScript();
    }

};

/*######
## spell_quest_extincteur
######*/

enum eSpellQuestExtincteur
{
    QUEST_EXTINGUISHING_HOPE = 26391,
    NPC_FIRE                  = 42940,
};

class spell_quest_extincteur : public SpellScriptLoader
{
public:
    spell_quest_extincteur() : SpellScriptLoader("spell_quest_extincteur") { }

    class spell_quest_extincteur_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_quest_extincteur_SpellScript);

        void OnDummy(SpellEffIndex /*effIndex*/)
        {
            Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
            Creature* fire = GetHitCreature();

            if (!player || !fire)
                return;

            // The extinguisher must only reward/consume a fire while
            // Extinguishing Hope is actively in progress.
            if (player->GetQuestStatus(QUEST_EXTINGUISHING_HOPE) != QUEST_STATUS_INCOMPLETE)
                return;

            if (fire->GetEntry() != NPC_FIRE)
                return;

            player->KilledMonsterCredit(NPC_FIRE, fire->GetGUID());
            fire->DespawnOrUnsummon();
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_quest_extincteur_SpellScript::OnDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_quest_extincteur_SpellScript();
    }

};

void AddSC_northshire()
{
    new npc_stormwind_infantry();
    new npc_blackrock_battle_worg();
    RegisterCreatureAI(npc_brother_paxton);
    RegisterCreatureAI(npc_blackrock_spy);
    RegisterCreatureAI(npc_stormwind_injured_soldier);
    new npc_training_dummy_start_zones();
    new spell_quest_fear_no_evil();
    new spell_quest_extincteur();
}

