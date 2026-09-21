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

#include "Creature.h"
#include "ScriptMgr.h"
#include "MotionMaster.h"
#include "ScriptedCreature.h"
#include "WaypointMovementGenerator.h"

enum eAgitatedEarthSpirit
{
    SPELL_SOOTHE_EARTH_SPIRIT       = 69453,
    SPELL_ROCK_BARRAGE              = 81305,
    NPC_EARTH_SPIRIT_CREDIT_BUNNY   = 36872
};

// 36845 - Agitated Earth Spirit
struct npc_agitated_earth_spirit : public ScriptedAI
{
    npc_agitated_earth_spirit(Creature* creature) : ScriptedAI(creature) { }

    void SpellHit(Unit* caster, SpellInfo const* spell) override
    {
        if (spell->Id == SPELL_SOOTHE_EARTH_SPIRIT)
        {
            Position pos;
            caster->GetNearPoint(caster, pos.m_positionX, pos.m_positionY, pos.m_positionZ, 0.0f, MIN_MELEE_REACH, caster->GetAngle(me));
            me->GetMotionMaster()->MovePoint(1, pos);
            _playerGUID = caster->GetGUID();
        }
    }

    void MovementInform(uint32 type, uint32 pointId) override
    {
        if (type == POINT_MOTION_TYPE && pointId == 1)
        {
            switch (urand(0, 1))
            {
                case 0:
                {
                    me->SetFaction(35);
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        player->KilledMonsterCredit(NPC_EARTH_SPIRIT_CREDIT_BUNNY);

                    me->GetScheduler().Schedule(1s, [](TaskContext context)
                    {
                        GetContextCreature()->DisappearAndDie();
                    });

                    break;
                }
                case 1:
                    me->SetFaction(14);
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _playerGUID))
                        AttackStart(player);
                    break;
            }
        }
    }

    void JustEngagedWith(Unit* /*victim*/) override
    {
        me->GetScheduler().Schedule(4s, 5s, [this](TaskContext context)
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
            {
                GetContextUnit()->CastSpell(target, SPELL_ROCK_BARRAGE, false);
                context.Repeat(18s, 21s);
            }
        });
    }

private:
    ObjectGuid _playerGUID;
};

enum eKyleTheFrenzied
{
    SPELL_LUNCH_FOR_KYLE    = 42222,
    NPC_KYLE_THE_FRENZIED   = 23616,
    NPC_KYLE_THE_FRIENDLY   = 23622
};

// 23616
struct npc_kyle_the_frenzied : public ScriptedAI
{
    npc_kyle_the_frenzied(Creature* creature) : ScriptedAI(creature) { }
    void SpellHit(Unit* caster, SpellInfo const* spell) override
    {
        if (spell->Id == SPELL_LUNCH_FOR_KYLE)
        {
            Position pos;
            caster->GetNearPoint(caster, pos.m_positionX, pos.m_positionY, pos.m_positionZ, 0.0f, 0.5f, caster->GetAngle(me));
            me->GetMotionMaster()->MovePoint(1, pos);
            _playerGUID = caster->GetGUID();
        }
    }
    void MovementInform(uint32 type, uint32 pointId) override
    {
        if (type == POINT_MOTION_TYPE && pointId == 1)
        {
            // Wait 15 seconds then resume path
            if (WaypointMovementGenerator<Creature>* move = dynamic_cast<WaypointMovementGenerator<Creature>*>(me->GetMotionMaster()->top()))
                move->GetTrackerTimer().Reset(15000);
            Talk(0);
            me->GetScheduler().Schedule(4s, [this](TaskContext /*context*/)
            {
                Talk(1);
            });
            me->GetScheduler().Schedule(9s, [this](TaskContext context)
            {
                Creature* ctxCrea = GetContextCreature();
                Talk(2);
                ctxCrea->UpdateEntry(NPC_KYLE_THE_FRIENDLY);
                ctxCrea->HandleEmoteCommand(EMOTE_STATE_DANCE);
                if (Player* player = ObjectAccessor::GetPlayer(*ctxCrea, _playerGUID))
                    player->KilledMonsterCredit(NPC_KYLE_THE_FRENZIED);
            });
            me->GetScheduler().Schedule(15s, [](TaskContext context)
            {
                Creature* ctxCrea = GetContextCreature();
                ctxCrea->UpdateEntry(NPC_KYLE_THE_FRENZIED);
                ctxCrea->HandleEmoteCommand(EMOTE_STATE_NONE);
            });
        }
    }
private:
    ObjectGuid _playerGUID;
};

enum EagleSpirit
{
    SPELL_EJECT_ALL_PASSENGERS = 50630,
    SPELL_SPIRIT_FORM = 69324
};

Position const EagleSpiritflightPath[] =
{
    { -2884.155f, -71.08681f, 242.0678f },
    { -2720.592f, -111.0035f, 242.5955f },
    { -2683.951f, -382.9010f, 231.1792f },
    { -2619.148f, -484.9288f, 231.1792f },
    { -2543.868f, -525.3333f, 231.1792f },
    { -2465.321f, -502.4896f, 190.7347f },
    { -2343.872f, -401.8281f, -8.320873f }
};
size_t const EagleSpiritflightPathSize = std::extent<decltype(EagleSpiritflightPath)>::value;

class npc_eagle_spirit : public CreatureScript
{
public:
    npc_eagle_spirit() : CreatureScript("npc_eagle_spirit") { }

    struct npc_eagle_spirit_AI : public ScriptedAI
    {
        npc_eagle_spirit_AI(Creature* creature) : ScriptedAI(creature) { }

        void PassengerBoarded(Unit* /*who*/, int8 /*seatId*/, bool apply) override
        {
            if (!apply)
                return;

            me->GetMotionMaster()->MoveSmoothPath(uint32(EagleSpiritflightPathSize), EagleSpiritflightPath, EagleSpiritflightPathSize, false, true);
            me->CastSpell(me, SPELL_SPIRIT_FORM);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == EFFECT_MOTION_TYPE && pointId == EagleSpiritflightPathSize)
            {
                DoCast(SPELL_EJECT_ALL_PASSENGERS);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_eagle_spirit_AI(creature);
    }
};

// 71898 Funeral Offering
class spell_mulgore_funeral_offering : public SpellScript
{
    PrepareSpellScript(spell_mulgore_funeral_offering);

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        if (Creature* target = GetHitCreature())
            if (GetCaster()->IsPlayer())
                GetCaster()->ToPlayer()->KilledMonsterCredit(target->GetEntry());
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_mulgore_funeral_offering::HandleHitTarget, EFFECT_1, SPELL_EFFECT_DUMMY);
    }
};

// NPC Fledgling Brave 36942
class npc_fledgling_brave : public CreatureScript
{
public:
    npc_fledgling_brave() : CreatureScript("npc_fledgling_brave") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_fledgling_brave_AI(creature);
    }

    struct npc_fledgling_brave_AI : public ScriptedAI
    {
        npc_fledgling_brave_AI(Creature* creature) : ScriptedAI(creature) {}

        enum Spells
        {
            SPELL_SHOOT     = 70092,
            SPELL_CLEAVE    = 81502,
            SPELL_WAR_STOMP = 81500
        };

        enum Events
        {
            EVENT_SHOOT       = 1,
            EVENT_CLEAVE      = 2,
            EVENT_WAR_STOMP   = 3,
            EVENT_RANGE_CHECK = 4
        };

        enum Weapons
        {
            WEAPON_MELEE = 2023,
            WEAPON_RANGE = 49462
        };

        void Reset() override
        {
            events.Reset();
            me->SetVirtualItem(0, WEAPON_MELEE);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            me->StopMoving();
            me->AttackStop();
            ScheduleCombatEvents();
        }

        void ScheduleCombatEvents()
        {
            events.Reset();
            events.ScheduleEvent(EVENT_SHOOT, urand(2300, 3900));
            events.ScheduleEvent(EVENT_CLEAVE, urand(7000, 9000));
            events.ScheduleEvent(EVENT_WAR_STOMP, urand(17000, 22000));
        }

        void CheckDistanceBehavior(Unit* victim){
            if(!victim){
                return;
            }

            float distance = me->GetDistance(victim);

            me->StopMoving();
            // 0 - 10 yards: move toward target and use melee weapon
            if (distance <= 10.0f)
            {
                me->SetVirtualItem(0, WEAPON_MELEE);
            }
            else{
                me->SetVirtualItem(0, WEAPON_RANGE);
            }
        }

        void ExecuteEvent(uint32 eventId) override
        {
            switch (eventId)
            {
                case EVENT_SHOOT:
                {
                    Unit* victim = me->GetVictim();

                    if (victim && me->GetDistance(victim) > 10.0f)
                        DoCastVictim(SPELL_SHOOT);

                    events.ScheduleEvent(EVENT_SHOOT, urand(2300, 3900));
                    break;
                }
                case EVENT_CLEAVE:
                    DoCastVictim(SPELL_CLEAVE);
                    events.ScheduleEvent(EVENT_CLEAVE, urand(7000, 9000));
                    break;

                case EVENT_WAR_STOMP:
                    DoCastSelf(SPELL_WAR_STOMP);
                    events.ScheduleEvent(EVENT_WAR_STOMP, urand(17000, 22000));
                    break;
            }
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            // Players are allowed to damage the creature below 80%.
            if (attacker && attacker->IsPlayer())
                return;

            uint64 minimumHealth = me->CountPctFromMaxHealth(80);

            //If the creature is below 80% set the HP to 80%
            if (me->GetHealth() <= minimumHealth ||
                me->GetHealth() - damage <= minimumHealth)
            {
                damage = 0;
                me->SetHealth(minimumHealth);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
            {
                if (Unit* nearestTarget = me->SelectNearestTarget(5.0f))
                {
                    AttackStart(nearestTarget);
                }
                return;
            }

            Unit* victim = me->GetVictim();

            if (!victim || !victim->IsAlive())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            CheckDistanceBehavior(victim);
            while (uint32 eventId = events.ExecuteEvent())
                ExecuteEvent(eventId);


            DoMeleeAttackIfReady();
        }
    };
};

// NPC Bristleback Invader 36943
class npc_bristleback_invader: public CreatureScript
{
public:
    npc_bristleback_invader() : CreatureScript("npc_bristleback_invader") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_bristleback_invader_AI(creature);
    }

    struct npc_bristleback_invader_AI : public ScriptedAI
    {
        npc_bristleback_invader_AI(Creature* creature) : ScriptedAI(creature) { }

        enum Spells
        {
            SPELL_BRISTLEBACK = 81653,
            SPELL_REND        = 11977
        };

        enum Events
        {
            EVENT_REND = 1
        };

        void Reset() override
        {
            events.Reset();
            DoCastSelf(SPELL_BRISTLEBACK);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            events.ScheduleEvent(EVENT_REND, urand(2000, 4000));
        }

        void ExecuteEvent(uint32 eventId) override
        {
            switch (eventId)
            {
                case EVENT_REND:
                    DoCastVictim(SPELL_REND);
                    events.ScheduleEvent(EVENT_REND, urand(25000, 28000));
                    break;
            }
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            // Players are allowed to damage the creature below 80%.
            if (attacker && attacker->IsPlayer())
                return;

            uint64 minimumHealth = me->CountPctFromMaxHealth(80);

            //If the creature is below 80% set the HP to 80%
            if (me->GetHealth() <= minimumHealth ||
                me->GetHealth() - damage <= minimumHealth)
            {
                damage = 0;
                me->SetHealth(minimumHealth);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
                ExecuteEvent(eventId);

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_mulgore()
{
    RegisterCreatureAI(npc_agitated_earth_spirit);
    RegisterCreatureAI(npc_kyle_the_frenzied);
    new npc_eagle_spirit();
    RegisterSpellScript(spell_mulgore_funeral_offering);
    new npc_fledgling_brave();
    new npc_bristleback_invader();
}
