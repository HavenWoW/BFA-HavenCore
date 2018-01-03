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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "AreaTrigger.h"
#include "AreaTriggerAI.h"
#include "Vehicle.h"

/*######
## npc_hogger
######*/

enum HoggerSpellData
{
    SPELL_SUMMON_MINIONS = 87366,
    SPELL_VICIOUS_SLICE = 87337,
    SPELL_EATING = 87351,
    SPELL_TELEPORT_VISUAL_ONLY_1 = 87459,
    SPELL_TELEPORT_VISUAL_ONLY_2 = 64446,
    SPELL_BLOODY_STRIKE = 87359
};

enum HoggerEventData
{
    EVENT_VICIOUS_SLICE = 1,
    EVENT_HAMMOND_GROUP_START_WALKING = 2,
    EVENT_DISMOUNT_HAMMOND_CLAY = 3,
    EVENT_HOGGER_SAY_GRR = 4,
    EVENT_CLAYS_EXPLAINATION = 5,
    EVENT_CLAY_SAYS_TAKE_HIM = 6,
    EVENT_HOGGER_SAYS_NOO = 7,
    EVENT_CLAY_SPEAKS_TO_ANDROMATH = 8,
    EVENT_TELEPORT_BACK = 9,
    EVENT_CHECK_EAT_RANGE = 10,
    EVENT_BLOODY_STRIKE = 11
};

enum HoggerTextData
{
    SAY_HOGGER_SUMMON_MINIONS = 2,
    SAY_EATING = 3,
    SAY_STUNNED = 4,
    SAY_BEG = 5,
    SAY_RAND = 6,
    SAY_FINAL = 7
};

enum HammondTextData
{
    YELL_OPENING = 0,
    SAY_EXPLAINATION = 1,
    SAY_TAKE_HIM = 2,
    SAY_TO_ANDROMATH = 3
};

enum RagamuffinTextData
{
    SAY_CLAY = 0,
    SAY_WOW = 1,
};

enum HoggerCreatures
{
    NPC_GENERAL_HAMMOND_CLAY = 65153,
    NPC_ANDROMATH = 46941,
    NPC_DUMAS = 46940,
    NPC_HOGGER = 448,
    NPC_EATING_TARGET = 45979,
    NPC_RAGAMUFFIN = 46943,
    NPC_HOGGER_MINION = 46932
};

enum HoggerMiscData
{
    DISPLAYID_GENERAL_HAMMOND_CLAYS_MOUNT = 2410
};

// validated positions
static const Position generalHammondClayPositions[4] =
{
    { -10125.35f, 650.7324f, 36.05776f },
    { -10128.3f,  656.4648f, 36.05776f },
    { -10131.25f, 662.1973f, 36.05776f },
    { -10135.73f, 668.389f,  35.74807f }
};

// validated positions
static const Position andromathPositions[3] =
{
    { -10119.2f, 647.913f, 36.36745f },
    { -10123.0f, 656.875f, 36.05776f },
    { -10126.8f, 665.837f, 35.74807f }
};

// validated positions
static const Position dumasPositions[3] =
{
    { -10130.1f, 647.7671f, 36.04665f },
    { -10132.9f, 653.5605f, 36.05776f },
    { -10135.7f, 659.354f,  36.06887f }
};

// validated positions
static const Position hoggerPositions[1] =
{
    { -10136.9f, 670.009f, 36.03682f }
};

// validated positions
static const Position ragamuffinPositions[2] =
{
    { -10127.00f, 651.0f, 36.05776f },
    { -10123.0f, 651.0f,  36.06887f }
};

struct npc_hogger : public ScriptedAI
{
    npc_hogger(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        _events.Reset();

        _minionsSummoned = false;
        _endingSceneActive = false;
        _alreadyEaten = false;
        _isEating = false;

        me->RemoveUnitFlag(UNIT_FLAG_IMMUNE_TO_PC);
        me->SetReactState(REACT_AGGRESSIVE);
        me->SetWalk(false);
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage) override
    {
        if (_endingSceneActive)
        {
            damage = 0;
            return;
        }

        if (!_minionsSummoned && me->HealthBelowPctDamaged(50, damage))
            SummonMinions();

        if (!_alreadyEaten && me->HealthBelowPctDamaged(30, damage))
            MoveToEatingPosition();


        if (me->GetHealth() <= damage)
        {
            damage = 0;
            me->SetHealth(1);
            StartEndingScene();
            RewardPlayers();
        }

        if (_isEating)
        {
            _isEating = false;
            me->SetReactState(REACT_AGGRESSIVE);
            Talk(SAY_STUNNED);
        }
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_VICIOUS_SLICE, 3000);
    }

    void MoveToEatingPosition()
    {
        _alreadyEaten = true;

        if (Creature* target = me->FindNearestCreature(NPC_EATING_TARGET, 100.0f))
        {
            me->SetReactState(REACT_PASSIVE);
            me->GetMotionMaster()->MovePoint(0, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), true);
            _events.ScheduleEvent(EVENT_CHECK_EAT_RANGE, 200);
        }
    }

    void StartEndingScene()
    {
        _endingSceneActive = true;
        _events.Reset();

        me->SetReactState(REACT_PASSIVE);
        me->AddUnitFlag(UNIT_FLAG_IMMUNE_TO_PC);
        me->StopMoving();
        me->AttackStop();

        Talk(SAY_BEG);

        SummonGeneralHammondClay();
        SummonAndromath();
        SummonDumas();

        _events.ScheduleEvent(EVENT_HAMMOND_GROUP_START_WALKING, 1000);
    }

    void RewardPlayers()
    {
        for (auto itr : me->GetThreatManager().GetModifiableThreatList())
            if (Player* player = ObjectAccessor::GetPlayer(*me, itr->GetVictim()->GetGUID()))
                player->RewardPlayerAndGroupAtEvent(NPC_HOGGER, me);
    }

    void SummonGeneralHammondClay()
    {
        if (TempSummon* hammond = me->SummonCreature(NPC_GENERAL_HAMMOND_CLAY, generalHammondClayPositions[0]))
        {
            _generalHammondGUID = hammond->GetGUID();
            hammond->CastSpell(hammond, SPELL_TELEPORT_VISUAL_ONLY_1, true);
            hammond->Mount(DISPLAYID_GENERAL_HAMMOND_CLAYS_MOUNT);
            hammond->AI()->Talk(YELL_OPENING);
        }
        // summon ragamuffins and do text
        if (TempSummon* ragamuffin1 = me->SummonCreature(NPC_RAGAMUFFIN, ragamuffinPositions[0], TEMPSUMMON_TIMED_DESPAWN, 5000))
        {
            ragamuffin1->AI()->Talk(SAY_CLAY);
        }
        if (TempSummon* ragamuffin2 = me->SummonCreature(NPC_RAGAMUFFIN, ragamuffinPositions[1], TEMPSUMMON_TIMED_DESPAWN, 5000))
        {
            ragamuffin2->AI()->Talk(SAY_WOW);
        }
    }

    void MoveGeneralHammondClay()
    {
        if (GetHammond())
        {
            GetHammond()->SetWalk(true);
            GetHammond()->GetMotionMaster()->MovePoint(0, generalHammondClayPositions[2], true);
            _events.ScheduleEvent(EVENT_DISMOUNT_HAMMOND_CLAY, 8500);
        }
    }

    void SummonAndromath()
    {
        TempSummon* andromath = me->SummonCreature(NPC_ANDROMATH, andromathPositions[0]);
        if (andromath)
        {
            _andromathGUID = andromath->GetGUID();
            andromath->CastSpell(andromath, SPELL_TELEPORT_VISUAL_ONLY_1, true);
        }
    }

    void MoveAndromath()
    {
        if (GetAndromath())
        {
            GetAndromath()->SetWalk(true);
            GetAndromath()->GetMotionMaster()->MovePoint(0, andromathPositions[2], true);
        }
    }

    void SummonDumas()
    {
        TempSummon* dumas = me->SummonCreature(NPC_DUMAS, dumasPositions[0]);
        if (dumas)
        {
            _dumasGUID = dumas->GetGUID();
            dumas->CastSpell(dumas, SPELL_TELEPORT_VISUAL_ONLY_1, true);
        }
    }

    void MoveDumas()
    {
        if (GetDumas())
        {
            GetDumas()->SetWalk(true);
            GetDumas()->GetMotionMaster()->MovePoint(0, dumasPositions[2], true);
        }
    }

    void SummonMinions()
    {
        me->CastStop();
        Talk(SAY_HOGGER_SUMMON_MINIONS);
        //DoCastSelf(SPELL_SUMMON_MINIONS, true); This works, but the minions just sit there, and then despawn
        for (float distance : { 0.5f, 1.5f, 2.5f })
        {
            Position hogPos = me->GetPosition();
            GetPositionWithDistInFront(me, distance, hogPos);
            float z = me->GetMap()->GetHeight(me->GetPhaseShift(), hogPos.GetPositionX(), hogPos.GetPositionY(), hogPos.GetPositionZ());
            hogPos.m_positionZ = z;
            me->SummonCreature(NPC_HOGGER_MINION, hogPos);
        }
        _minionsSummoned = true;
    }

    void MoveHoggerToFinalPosition()
    {
        me->SetWalk(true);
        me->GetMotionMaster()->MovePoint(0, hoggerPositions[0]);
    }

    void TeleportBack()
    {
        if (GetHammond() && GetAndromath() && GetDumas())
        {
            GetHammond()->CastSpell(GetHammond(), SPELL_TELEPORT_VISUAL_ONLY_2, true);
            GetAndromath()->CastSpell(GetAndromath(), SPELL_TELEPORT_VISUAL_ONLY_2, true);
            GetDumas()->CastSpell(GetDumas(), SPELL_TELEPORT_VISUAL_ONLY_2, true);
            DoCastSelf(SPELL_TELEPORT_VISUAL_ONLY_2, true);

            me->DisappearAndDie();
            GetHammond()->DisappearAndDie();
            GetAndromath()->DisappearAndDie();
            GetDumas()->DisappearAndDie();
        }
    }

    Creature* GetHammond()
    {
        return me->GetMap()->GetCreature(_generalHammondGUID);
    }

    Creature* GetAndromath()
    {
        return me->GetMap()->GetCreature(_andromathGUID);
    }

    Creature* GetDumas()
    {
        return me->GetMap()->GetCreature(_dumasGUID);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim() && !_endingSceneActive)
            return;

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_VICIOUS_SLICE:
                DoCastVictim(SPELL_VICIOUS_SLICE);
                _events.Repeat(3s);
                break;

            case EVENT_HAMMOND_GROUP_START_WALKING:
                MoveGeneralHammondClay();
                MoveAndromath();
                MoveDumas();
                MoveHoggerToFinalPosition();
                break;

            case EVENT_DISMOUNT_HAMMOND_CLAY:
                if (GetHammond())
                {
                    me->SetFacingToObject(GetHammond());

                    GetHammond()->Dismount();
                    GetHammond()->GetMotionMaster()->MovePoint(0, generalHammondClayPositions[3], true);

                    _events.ScheduleEvent(EVENT_HOGGER_SAY_GRR, 3000);
                }
                break;

            case EVENT_HOGGER_SAY_GRR:
                Talk(SAY_RAND);
                _events.ScheduleEvent(EVENT_CLAYS_EXPLAINATION, 3000);
                break;

            case EVENT_CLAYS_EXPLAINATION:
                if (GetHammond())
                    GetHammond()->AI()->Talk(SAY_EXPLAINATION);
                _events.ScheduleEvent(EVENT_CLAY_SAYS_TAKE_HIM, 4500);
                break;

            case EVENT_CLAY_SAYS_TAKE_HIM:
                if (GetHammond())
                    GetHammond()->AI()->Talk(SAY_TAKE_HIM);
                _events.ScheduleEvent(EVENT_HOGGER_SAYS_NOO, 2000);
                break;

            case EVENT_HOGGER_SAYS_NOO:
                Talk(SAY_FINAL);
                _events.ScheduleEvent(EVENT_CLAY_SPEAKS_TO_ANDROMATH, 3000);
                break;

            case EVENT_CLAY_SPEAKS_TO_ANDROMATH:
                if (GetHammond() && GetAndromath())
                {
                    GetHammond()->SetFacingToObject(GetAndromath());
                    GetAndromath()->SetFacingToObject(GetHammond());
                    GetHammond()->AI()->Talk(SAY_TO_ANDROMATH);
                }
                _events.ScheduleEvent(EVENT_TELEPORT_BACK, 4000);
                break;

            case EVENT_TELEPORT_BACK:
                TeleportBack();
                break;

            case EVENT_CHECK_EAT_RANGE:
                if (!me->FindNearestCreature(NPC_EATING_TARGET, 3.0f))
                {
                    _events.ScheduleEvent(EVENT_CHECK_EAT_RANGE, 200);
                    break;
                }
                else
                {
                    DoCast(SPELL_EATING);
                    Talk(SAY_EATING);
                    _events.ScheduleEvent(EVENT_BLOODY_STRIKE, 100);
                    _isEating = true;
                }
                break;

            case EVENT_BLOODY_STRIKE:
                if (_isEating)
                {
                    if (Creature* dummy = me->FindNearestCreature(NPC_EATING_TARGET, 10.0f))
                        DoCast(dummy, SPELL_BLOODY_STRIKE, true);
                    _events.ScheduleEvent(EVENT_BLOODY_STRIKE, 1000);
                }

            default:
                break;
            }
        }

        if (!_endingSceneActive)
            DoMeleeAttackIfReady();
    }

private:
    EventMap _events;

    bool _minionsSummoned;
    bool _endingSceneActive;
    bool _alreadyEaten;
    bool _isEating;

    ObjectGuid _generalHammondGUID;
    ObjectGuid _andromathGUID;
    ObjectGuid _dumasGUID;
};

/*######
## npc_hogger_minion
######*/

enum HoggerMinionSpellData
{
    SPELL_ADVENTURERS_RUSH = 87402
};

struct npc_hogger_minion : public ScriptedAI
{
    npc_hogger_minion(Creature* creature) : ScriptedAI(creature){ }

    void Reset() override
    {
        me->SetReactState(REACT_AGGRESSIVE);

        if (Creature* hogger = me->FindNearestCreature(NPC_HOGGER, 35.0f, true))
            me->CastSpell(hogger, SPELL_ADVENTURERS_RUSH, true);
    }
};

/*######
## Quest 35 - Further Concerns
## npc_elwynn_stormwind_charger (42260)
##
## Retail-like route/timing
## Haven-specific adaptation:
## - use DB waypoint path 4226000 so Haven advances the route node-by-node
## - do not root the charger (Haven MotionMaster does not update rooted units)
## - keep scripted AI enabled while the vehicle seat applies CHARM_TYPE_VEHICLE
## - revoke rider control after boarding while keeping the passenger attached
######*/

enum FurtherConcernsData
{
    EVENT_BOARD_PASSENGER = 1,
    EVENT_PLAY_MOUNT_ANIMATION = 2,
    EVENT_START_RIDING = 3,
    EVENT_EJECT_PASSENGER = 4,
    EVENT_FORCE_EJECT = 5,
    EVENT_DESPAWN_CHARGER = 6,
    EVENT_RIDE_TIMEOUT = 7,

    STORMWIND_CHARGER_PATH = 4226000,
    STORMWIND_CHARGER_LAST_WAYPOINT = 38,

    SOUND_ID_MOUNTSPECIAL = 4066,

    SPELL_EJECT_PASSENGER = 77946
};

struct npc_elwynn_stormwind_charger : public ScriptedAI
{
    npc_elwynn_stormwind_charger(Creature* creature) : ScriptedAI(creature) { }

    void Reset() override
    {
        _events.Reset();
        _passengerGuid.Clear();
        _rideStarted = false;
        _finishingRide = false;

        me->SetReactState(REACT_PASSIVE);
        me->SetWalk(false);
        me->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
    }

    // Vehicle 882 uses a controllable seat. Haven normally disables CreatureAI
    // when a player receives CHARM_TYPE_VEHICLE. This taxi must keep its scripted
    // AI active so it can drive the retail route and eject the passenger.
    void OnCharmed(bool /*apply*/) override { }

    void IsSummonedBy(Unit* summoner) override
    {
        if (Player* player = summoner ? summoner->ToPlayer() : nullptr)
        {
            _passengerGuid = player->GetGUID();

            // Spell 78854 normally handles the ride. This delayed fallback makes
            // the script resilient if the client/core summons the charger without
            // completing the vehicle join automatically.
            _events.ScheduleEvent(EVENT_BOARD_PASSENGER, 100);
        }
    }

    void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
    {
        Player* player = passenger ? passenger->ToPlayer() : nullptr;
        if (!player)
            return;

        if (!apply)
        {
            if (!_finishingRide)
                me->DespawnOrUnsummon(1000);
            return;
        }

        _passengerGuid = player->GetGUID();

        // Seat 0 can grant the rider client control of the vehicle. Keep the
        // passenger attached, but remove the vehicle charm so movement remains
        // server/AI authoritative and the player cannot steer the charger.
        if (me->IsCharmed() && me->GetCharmerGUID() == player->GetGUID())
            me->RemoveCharmedBy(player);

        if (_rideStarted)
            return;

        _rideStarted = true;
        me->PlayDirectSound(SOUND_ID_MOUNTSPECIAL, player);
        _events.ScheduleEvent(EVENT_PLAY_MOUNT_ANIMATION, 200);
    }

    void MovementInform(uint32 motionType, uint32 pointId) override
    {
        // WaypointMovementGenerator reports its zero-based node index.
        // Path 4226000 contains 39 nodes, so node 38 is the destination.
        if (motionType == WAYPOINT_MOTION_TYPE && pointId == STORMWIND_CHARGER_LAST_WAYPOINT)
            _events.ScheduleEvent(EVENT_EJECT_PASSENGER, 2000);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_BOARD_PASSENGER:
                {
                    Player* player = ObjectAccessor::GetPlayer(*me, _passengerGuid);
                    if (!player)
                    {
                        me->DespawnOrUnsummon();
                        break;
                    }

                    if (player->IsMounted())
                        player->Dismount();

                    if (!player->GetVehicle())
                        player->EnterVehicle(me, 0);
                    break;
                }
                case EVENT_PLAY_MOUNT_ANIMATION:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_MOUNT_SPECIAL);
                    _events.ScheduleEvent(EVENT_START_RIDING, 1200);
                    break;
                case EVENT_START_RIDING:
                    // Use Haven's waypoint movement generator instead of one long
                    // MoveSmoothPath spline. This forces the charger through every
                    // retail route node and avoids the BFA client collapsing the
                    // scripted ground spline into a direct line to the destination.
                    me->GetMotionMaster()->MovePath(STORMWIND_CHARGER_PATH, false);

                    // Safety only: the retail route should finish long before this.
                    _events.ScheduleEvent(EVENT_RIDE_TIMEOUT, 120000);
                    break;
                case EVENT_EJECT_PASSENGER:
                    BeginRideFinish();
                    break;
                case EVENT_FORCE_EJECT:
                    ForceEjectPassenger();
                    break;
                case EVENT_RIDE_TIMEOUT:
                    BeginRideFinish();
                    break;
                case EVENT_DESPAWN_CHARGER:
                    me->DespawnOrUnsummon();
                    break;
                default:
                    break;
            }
        }
    }

private:
    void BeginRideFinish()
    {
        if (_finishingRide)
            return;

        _finishingRide = true;
        _events.CancelEvent(EVENT_RIDE_TIMEOUT);

        me->HandleEmoteCommand(EMOTE_ONESHOT_MOUNT_SPECIAL);

        // Preserve the donor/retail behavior first.
        DoCastSelf(SPELL_EJECT_PASSENGER, true);

        // If spell 77946 does not detach the rider on this BFA branch, force the
        // vehicle exit shortly afterwards so a failed spell can never trap a player.
        _events.ScheduleEvent(EVENT_FORCE_EJECT, 1000);
        _events.ScheduleEvent(EVENT_DESPAWN_CHARGER, 2000);
    }

    void ForceEjectPassenger()
    {
        if (Vehicle* vehicle = me->GetVehicleKit())
            vehicle->RemoveAllPassengers();
        else if (Player* player = ObjectAccessor::GetPlayer(*me, _passengerGuid))
            player->ExitVehicle();
    }

    EventMap _events;
    ObjectGuid _passengerGuid;
    bool _rideStarted = false;
    bool _finishingRide = false;
};

//88
struct at_fargodeep_mine : public AreaTriggerAI
{
    at_fargodeep_mine(AreaTrigger* at) : AreaTriggerAI(at) { }

    void OnUnitEnter(Unit* unit) override
    {
        if (unit->IsPlayer())
        {
            if (Player* player = unit->ToPlayer())
            {
                if (player->GetQuestStatus(62) == QUEST_STATUS_INCOMPLETE)
                {
                    player->ForceCompleteQuest(62);
                }
            }
        }
    }
};

//87
struct at_jasperlode_mine : public AreaTriggerAI
{
    at_jasperlode_mine(AreaTrigger* at) : AreaTriggerAI(at) { }

    void OnUnitEnter(Unit* unit) override
    {
        if (unit->IsPlayer())
        {
            if (Player* player = unit->ToPlayer())
            {
                if (player->GetQuestStatus(76) == QUEST_STATUS_INCOMPLETE)
                {
                    player->ForceCompleteQuest(76);
                }
            }
        }
    }
};

void AddSC_elwyn_forest()
{
    RegisterCreatureAI(npc_hogger);
    RegisterCreatureAI(npc_hogger_minion);
    RegisterCreatureAI(npc_elwynn_stormwind_charger);
    RegisterAreaTriggerAI(at_fargodeep_mine);
    RegisterAreaTriggerAI(at_jasperlode_mine);
}
