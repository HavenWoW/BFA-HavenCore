/*
 * 2026 BFA-HavenCore
 *
 * This SourceCode is NOT free a software. Please hold everything Private
 * and read our Terms
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"

enum Spells
{
    SPELL_SHADOW_STORM     = 119971,
    SPELL_TWISTED_ELEMENTS = 119300
};

enum Events
{
    EVENT_STORM = 1,
    EVENT_ELEMENTS
};

enum Texts
{
    SAY_AGGRO = 0,
    SAY_SHADOW_STORM = 1,
    SAY_DEATH = 2
};

class boss_koranthal : public CreatureScript
{
public:
    boss_koranthal() : CreatureScript("boss_koranthal") { }

    struct boss_koranthalAI : public ScriptedAI
    {
        boss_koranthalAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) override
        {
            Talk(SAY_AGGRO);

            events.ScheduleEvent(EVENT_ELEMENTS, 6000);
            events.ScheduleEvent(EVENT_STORM, 20500);
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(SAY_DEATH);
            events.Reset();
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ELEMENTS:
                    {
                        DoCastVictim(SPELL_TWISTED_ELEMENTS);
                        events.ScheduleEvent(EVENT_ELEMENTS, 7300);
                        break;
                    }

                    case EVENT_STORM:
                    {
                        Talk(SAY_SHADOW_STORM);
                        DoCast(me, SPELL_SHADOW_STORM, false);

                        // Shadow Storm is a phase, not a permanent recast loop.
                        // Delay Twisted Elements until the storm phase ends.
                        events.RescheduleEvent(EVENT_ELEMENTS, 15700);

                        // Reference cadence for the next Shadow Storm.
                        events.ScheduleEvent(EVENT_STORM, 47200);
                        break;
                    }

                    default:
                        break;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_koranthalAI(creature);
    }
};

// 119973 - Shadow Storm
class spell_koranthal_shadow_storm : public SpellScript
{
    PrepareSpellScript(spell_koranthal_shadow_storm);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                caster->CastSpell(target, GetEffectValue(), true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(
            spell_koranthal_shadow_storm::HandleScript,
            EFFECT_0,
            SPELL_EFFECT_DUMMY);
    }
};

void AddSC_boss_koranthal()
{
    new boss_koranthal();
    RegisterSpellScript(spell_koranthal_shadow_storm);
}
