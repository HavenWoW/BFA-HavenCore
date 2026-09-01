/*
 * 2026 BFA-HavenCore
 *
 * This SourceCode is NOT free a software. Please hold everything Private
 * and read our Terms
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_INFERNO_CHARGE           = 119405,
    SPELL_INFERNO_CHARGE_TRIGGERED = 119299,
    SPELL_FLAME_BREATH             = 119420
};

enum Events
{
    EVENT_FLAME_BREATH = 1,
    EVENT_INFERNO
};

class boss_adarogg : public CreatureScript
{
    public:
        boss_adarogg() : CreatureScript("boss_adarogg") { }

        struct boss_adaroggAI : public ScriptedAI
        {
            boss_adaroggAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() { } 

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_INFERNO, 10000);
                events.ScheduleEvent(EVENT_FLAME_BREATH, 20000);
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell) override
            {
                if (spell->Id == SPELL_INFERNO_CHARGE)
                    me->CastSpell(target, SPELL_INFERNO_CHARGE_TRIGGERED, false);
            }

            void JustDied(Unit* /*killer*/) { }

            void UpdateAI(uint32 const diff)
            {
                if(!UpdateVictim())
                    return;

                events.Update(diff);

                if(me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_FLAME_BREATH:
                            DoCastVictim(SPELL_FLAME_BREATH);
                            events.ScheduleEvent(EVENT_FLAME_BREATH, urand(15000, 20000));
                            break;
                        case EVENT_INFERNO:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            {
                                me->SetFacingToObject(target);

                                // HavenCore lacks the modern TrinityCore
                                // serverside Inferno Charge helper spells.
                                // Move Adarogg physically to the selected
                                // player's location so the charge is visible,
                                // then use the existing spell for the impact.
                                me->GetMotionMaster()->MoveCharge(
                                    target->GetPositionX(),
                                    target->GetPositionY(),
                                    target->GetPositionZ(),
                                    35.0f,
                                    1,
                                    true);

                                DoCast(target, SPELL_INFERNO_CHARGE, true);
                            }

                            events.ScheduleEvent(EVENT_INFERNO, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_adaroggAI(creature);
        }
};

void AddSC_boss_adarogg()
{
    new boss_adarogg();
}
