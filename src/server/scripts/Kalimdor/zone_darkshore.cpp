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

 //Missing scripts

#include "ScriptMgr.h"
#include "Creature.h"
#include "MotionMaster.h"
#include "Player.h"
#include "ScriptedEscortAI.h"

namespace RemtravelQuest13911
{
    enum RemtravelData : uint32
    {
        QUEST_ABSENT_MINDED_PROSPECTOR = 13911,
        NPC_JR_ARCHAEOLOGIST_FERD      = 34342,
        REMTRAVEL_ESCORT_FACTION       = 231,
        REMTRAVEL_FINISH_FACTION       = 55,
        REMTRAVEL_RETURN_POINT         = 0xFFFFFF,
        REMTRAVEL_LAST_WAYPOINT        = 27
    };

    // A quest-specific escort, not a modification of the generic SmartAI engine.
    class npc_prospector_remtravel : public CreatureScript
    {
    public:
        npc_prospector_remtravel() : CreatureScript("npc_prospector_remtravel") { }

        struct npc_prospector_remtravelAI : public npc_escortAI
        {
            explicit npc_prospector_remtravelAI(Creature* creature) : npc_escortAI(creature)
            {
                Reset();
            }

            uint32 _introTimer;
            uint32 _pauseTimer;
            uint32 _sceneTimer;
            uint32 _postEvadeTimer;
            uint32 _combatLeashTimer;
            uint32 _combatStallTimer;
            uint32 _returnWatchTimer;
            uint32 _routeWatchTimer;
            uint32 _expectedWaypoint;
            bool _runSegment; // Only waypoints 16-24 use the historical running pace.
            uint8 _introPhase;
            uint8 _scenePhase;
            Position _safeEscortPosition;
            Position _combatStallPosition;
            Position _returnWatchPosition;
            Position _routeWatchPosition;

            void Reset() override
            {
                if (HasEscortState(STATE_ESCORT_ESCORTING))
                    return;

                _introTimer = 0;
                _pauseTimer = 0;
                _sceneTimer = 0;
                _postEvadeTimer = 0;
                _combatLeashTimer = 1500;
                _combatStallTimer = 12000;
                _returnWatchTimer = 10000;
                _routeWatchTimer = 15000;
                _expectedWaypoint = 0;
                _runSegment = false;
                _introPhase = 0;
                _scenePhase = 0;
                _safeEscortPosition = me->GetPosition();
                _combatStallPosition = me->GetPosition();
                _returnWatchPosition = me->GetPosition();
                _routeWatchPosition = me->GetPosition();
                me->SetEmoteState(EMOTE_STATE_NONE);
            }

            void JustRespawned() override
            {
                // The escort temporarily moves the home/return point along the route.
                // Restore the spawn home before the normal escort respawn handling.
                float x, y, z;
                me->GetRespawnPosition(x, y, z);
                me->SetHomePosition(x, y, z, me->GetOrientation());
                npc_escortAI::JustRespawned();
            }

            void sQuestAccept(Player* player, Quest const* quest) override
            {
                if (!player || !quest || quest->GetQuestId() != QUEST_ABSENT_MINDED_PROSPECTOR
                    || HasEscortState(STATE_ESCORT_ESCORTING) || me->IsInCombat())
                    return;

                // The base escort reads the 27-point path from script_waypoint.
                npc_escortAI::Start(true, false, player->GetGUID(), quest);
                if (!HasEscortState(STATE_ESCORT_ESCORTING))
                    return;

                _safeEscortPosition = me->GetPosition();
                me->SetHomePosition(_safeEscortPosition);
                SetMaxPlayerDistance(60.0f); // Match SmartAI escort's existing 60-yard range.
                SetEscortPaused(true);
                Talk(0, player);
                _introPhase = 1;
                _introTimer = 8000;
            }

            void WaypointStart(uint32 waypointId) override
            {
                _expectedWaypoint = waypointId;
                _routeWatchPosition = me->GetPosition();
                _routeWatchTimer = 15000;
                RestoreRoutePace(); // Ensure new MovePoint segments use the correct gait.
            }

            void WaypointReached(uint32 waypointId) override
            {
                // Called only by the escort base for its current, ordered waypoint.
                _expectedWaypoint = 0;
                _safeEscortPosition = me->GetPosition();
                switch (waypointId)
                {
                    case 5:
                        PauseFor(4000);
                        if (Player* player = GetPlayerForEscort())
                            me->SetFacingToObject(player);
                        Talk(2);
                        break;
                    case 15:
                        PauseFor(5000);
                        Talk(3);
                        _runSegment = true;
                        SetRun(true);
                        break;
                    case 24:
                        PauseFor(5000);
                        Talk(4);
                        _runSegment = false;
                        SetRun(false);
                        break;
                    case 25:
                        PauseFor(16000);
                        if (Player* player = GetPlayerForEscort())
                            player->GroupEventHappens(QUEST_ABSENT_MINDED_PROSPECTOR, me);
                        Talk(5);
                        me->SetFaction(REMTRAVEL_FINISH_FACTION);
                        if (Creature* ferd = me->FindNearestCreature(NPC_JR_ARCHAEOLOGIST_FERD, 25.0f))
                            ferd->AI()->SetData(0, 1);
                        break;
                    case 26:
                        // Preserve the MoP dialogue/emote sequence (3s + 4s + 4s).
                        // Wait long enough to say the last line before waypoint 27 ends.
                        PauseFor(15000);
                        Talk(6);
                        _scenePhase = 1;
                        _sceneTimer = 3000;
                        break;
                    case REMTRAVEL_LAST_WAYPOINT:
                        // npc_escortAI will naturally end and despawn this escort.
                        break;
                    default:
                        break;
                }
            }

            void AttackStart(Unit* who) override
            {
                if (who && HasEscortState(STATE_ESCORT_ESCORTING))
                {
                    float maxDistance = HasEscortState(STATE_ESCORT_RETURNING) ? 6.0f : 14.0f;
                    if (who->GetVictim() != me && me->GetDistance(who) > maxDistance)
                        return;
                }
                npc_escortAI::AttackStart(who);
            }

            void MoveInLineOfSight(Unit* who) override
            {
                if (who && HasEscortState(STATE_ESCORT_ESCORTING))
                {
                    float maxDistance = HasEscortState(STATE_ESCORT_RETURNING) ? 6.0f : 14.0f;
                    if (who->GetVictim() != me && me->GetDistance(who) > maxDistance)
                        return;
                }
                npc_escortAI::MoveInLineOfSight(who);
            }

            void EnterEvadeMode(EvadeReason why = EVADE_REASON_OTHER) override
            {
                // Avoid the redundant out-of-combat evade that made the NPC
                // turn around on an otherwise healthy waypoint segment.
                // This guard is local to Remtravel; SmartAI is untouched.
                if (HasEscortState(STATE_ESCORT_ESCORTING)
                    && !HasEscortState(STATE_ESCORT_RETURNING | STATE_ESCORT_PAUSED)
                    && !me->IsInCombat() && !me->IsEngaged() && !me->GetVictim()
                    && !me->HasUnitState(UNIT_STATE_EVADE)
                    && me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                    return;

                bool escorting = HasEscortState(STATE_ESCORT_ESCORTING);
                if (escorting)
                {
                    // This home position is only for this NPC's current escort.
                    // The base escort uses it for its return-to-last-point move.
                    me->SetHomePosition(_safeEscortPosition);
                    _postEvadeTimer = 6000;
                    _returnWatchTimer = 10000;
                    _returnWatchPosition = me->GetPosition();
                    _combatLeashTimer = 1500;
                }
                npc_escortAI::EnterEvadeMode(why);
            }

            void MovementInform(uint32 movementType, uint32 pointId) override
            {
                if (movementType == POINT_MOTION_TYPE && pointId == REMTRAVEL_RETURN_POINT)
                {
                    _returnWatchTimer = 10000;
                    _returnWatchPosition = me->GetPosition();
                }
                npc_escortAI::MovementInform(movementType, pointId);
                RestoreRoutePace(); // Combat return should not leave the next leg running.
            }

            void UpdateAI(uint32 diff) override
            {
                // Initial quest dialogue. Escort is paused for the first 9 seconds.
                if (_introPhase && Tick(_introTimer, diff))
                {
                    if (_introPhase == 1)
                    {
                        Talk(1);
                        _introPhase = 2;
                        _introTimer = 1000;
                    }
                    else
                    {
                        _introPhase = 0;
                        me->SetFaction(REMTRAVEL_ESCORT_FACTION);
                        SetEscortPaused(false);
                    }
                }

                if (_pauseTimer && Tick(_pauseTimer, diff))
                    SetEscortPaused(false);

                if (_scenePhase && Tick(_sceneTimer, diff))
                {
                    if (_scenePhase == 1)
                    {
                        me->SetEmoteState(Emote(428));
                        _scenePhase = 2;
                        _sceneTimer = 4000;
                    }
                    else if (_scenePhase == 2)
                    {
                        me->SetEmoteState(EMOTE_STATE_NONE);
                        _scenePhase = 3;
                        _sceneTimer = 4000;
                    }
                    else
                    {
                        _scenePhase = 0;
                        if (Creature* ferd = me->FindNearestCreature(NPC_JR_ARCHAEOLOGIST_FERD, 25.0f))
                            me->SetFacingToObject(ferd);
                        Talk(7);
                    }
                }

                if (HasEscortState(STATE_ESCORT_ESCORTING))
                {
                    if (_postEvadeTimer)
                        Tick(_postEvadeTimer, diff);

                    if (!me->IsInCombat() && _expectedWaypoint
                        && !HasEscortState(STATE_ESCORT_RETURNING | STATE_ESCORT_PAUSED)
                        && me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                        _safeEscortPosition = me->GetPosition();

                    RecoverCombat(diff);
                    RecoverReturn(diff);
                    RecoverWaypoint(diff);
                }

                // Combat chase movement can clear the walk flag. Repair it before
                // and after the base escort issues its next MovePoint.
                RestoreRoutePace();
                npc_escortAI::UpdateAI(diff);
                RestoreRoutePace();
            }

        private:
            void RestoreRoutePace()
            {
                if (!HasEscortState(STATE_ESCORT_ESCORTING)
                    || me->IsInCombat() || me->GetVictim())
                    return;

                bool shouldWalk = !_runSegment;
                if (me->IsWalking() != shouldWalk)
                    me->SetWalk(shouldWalk);
            }

            static bool Tick(uint32& timer, uint32 diff)
            {
                if (timer > diff)
                {
                    timer -= diff;
                    return false;
                }
                timer = 0;
                return true;
            }

            void PauseFor(uint32 time)
            {
                SetEscortPaused(true);
                _pauseTimer = time;
            }

            void RecoverCombat(uint32 diff)
            {
                if (!me->IsInCombat())
                {
                    _combatLeashTimer = 1500;
                    _combatStallTimer = 12000;
                    _combatStallPosition = me->GetPosition();
                    return;
                }

                Unit* victim = me->GetVictim();
                bool orphaned = !victim || !victim->IsAlive();
                float leashX = me->GetPositionX() - _safeEscortPosition.GetPositionX();
                float leashY = me->GetPositionY() - _safeEscortPosition.GetPositionY();
                bool longChase = leashX * leashX + leashY * leashY > 196.0f;
                bool stationaryChase = false;

                if (victim && victim->IsAlive() && !me->IsWithinMeleeRange(victim))
                {
                    float dx = me->GetPositionX() - _combatStallPosition.GetPositionX();
                    float dy = me->GetPositionY() - _combatStallPosition.GetPositionY();
                    if (dx * dx + dy * dy > 1.0f)
                    {
                        _combatStallPosition = me->GetPosition();
                        _combatStallTimer = 12000;
                    }
                    else if (Tick(_combatStallTimer, diff))
                    {
                        stationaryChase = true;
                        _combatStallTimer = 12000;
                    }
                }
                else
                {
                    _combatStallPosition = me->GetPosition();
                    _combatStallTimer = 12000;
                }

                if ((orphaned && !_postEvadeTimer) || longChase || stationaryChase)
                {
                    if (stationaryChase || Tick(_combatLeashTimer, diff))
                        EnterEvadeMode(EVADE_REASON_OTHER);
                }
                else
                    _combatLeashTimer = 1500;
            }

            void RecoverReturn(uint32 diff)
            {
                if (!HasEscortState(STATE_ESCORT_RETURNING) || me->IsInCombat())
                {
                    _returnWatchTimer = 10000;
                    _returnWatchPosition = me->GetPosition();
                    return;
                }

                if (!Tick(_returnWatchTimer, diff))
                    return;

                float dx = me->GetPositionX() - _returnWatchPosition.GetPositionX();
                float dy = me->GetPositionY() - _returnWatchPosition.GetPositionY();
                if (dx * dx + dy * dy < 0.25f)
                    ReturnToLastPoint(); // Let the movement generator report actual arrival.
                _returnWatchPosition = me->GetPosition();
                _returnWatchTimer = 10000;
            }

            void RecoverWaypoint(uint32 diff)
            {
                if (!_expectedWaypoint || me->IsInCombat()
                    || HasEscortState(STATE_ESCORT_PAUSED | STATE_ESCORT_RETURNING))
                {
                    _routeWatchTimer = 15000;
                    _routeWatchPosition = me->GetPosition();
                    return;
                }

                float dx = me->GetPositionX() - _routeWatchPosition.GetPositionX();
                float dy = me->GetPositionY() - _routeWatchPosition.GetPositionY();
                if (dx * dx + dy * dy > 1.0f)
                {
                    _routeWatchPosition = me->GetPosition();
                    _routeWatchTimer = 15000;
                    return;
                }

                if (!Tick(_routeWatchTimer, diff))
                    return;

                float x, y, z;
                if (GetWaypointPosition(_expectedWaypoint, x, y, z))
                    me->GetMotionMaster()->MovePoint(_expectedWaypoint, x, y, z);

                _routeWatchPosition = me->GetPosition();
                _routeWatchTimer = 15000;
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_prospector_remtravelAI(creature);
        }
    };
}

void AddSC_darkshore()
{
    new RemtravelQuest13911::npc_prospector_remtravel();
}
