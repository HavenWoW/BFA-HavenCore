/*
 * 2026 BFA-HavenCore
 *
 * Ragefire Chasm - Adarogg / Corrupted Houndmaster retail polish
 *
 * Restores the Corrupted Houndmaster intro dialogue used when players
 * approach Adarogg. Combat behavior for Adarogg is intentionally left
 * unchanged from the previously validated HavenCore implementation.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"

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

enum AdaroggMisc
{
    NPC_ADAROGG               = 61408,
    NPC_CORRUPTED_HOUNDMASTER = 61666,

    ACTION_ADAROGG_INTRO_DIALOGUE = 1
};

enum HoundmasterTexts
{
    SAY_HES_CORNERED   = 0,
    SAY_GOT_HIM_NOW    = 1
};

// Retail/reference positions for the two Corrupted Houndmasters that
// deliver the Adarogg intro lines.
static float const HoundmasterIntro1X = -275.0427f;
static float const HoundmasterIntro1Y =  -63.9595f;
static float const HoundmasterIntro1Z =  -60.3566f;

static float const HoundmasterIntro2X = -273.0281f;
static float const HoundmasterIntro2Y =  -50.9456f;
static float const HoundmasterIntro2Z =  -60.7311f;

// AreaTrigger 7904 - Adarogg intro dialogue
class at_rfc_adarogg_intro : public AreaTriggerScript
{
public:
    at_rfc_adarogg_intro() : AreaTriggerScript("at_rfc_adarogg_intro") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/, bool entered) override
    {
        if (!entered || !player)
            return false;

        if (Creature* adarogg = player->FindNearestCreature(NPC_ADAROGG, 100.0f, true))
        {
            if (adarogg->AI())
                adarogg->AI()->DoAction(ACTION_ADAROGG_INTRO_DIALOGUE);

            return true;
        }

        return false;
    }
};

class boss_adarogg : public CreatureScript
{
public:
    boss_adarogg() : CreatureScript("boss_adarogg") { }

    struct boss_adaroggAI : public ScriptedAI
    {
        boss_adaroggAI(Creature* creature)
            : ScriptedAI(creature), _introDialoguePlayed(false)
        {
        }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) override
        {
            events.ScheduleEvent(EVENT_INFERNO, 10000);
            events.ScheduleEvent(EVENT_FLAME_BREATH, 20000);
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_INFERNO_CHARGE)
                me->CastSpell(target, SPELL_INFERNO_CHARGE_TRIGGERED, false);
        }

        void JustDied(Unit* /*killer*/) override
        {
            events.Reset();
        }

        void DoAction(int32 action) override
        {
            if (action != ACTION_ADAROGG_INTRO_DIALOGUE || _introDialoguePlayed)
                return;

            _introDialoguePlayed = true;

            Creature* first = FindIntroHoundmaster(
                HoundmasterIntro1X, HoundmasterIntro1Y, HoundmasterIntro1Z);

            Creature* second = FindIntroHoundmaster(
                HoundmasterIntro2X, HoundmasterIntro2Y, HoundmasterIntro2Z);

            if (first && first->AI())
                first->AI()->Talk(SAY_HES_CORNERED);

            if (second && second != first && second->AI())
                second->AI()->Talk(SAY_GOT_HIM_NOW);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
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

    private:
        Creature* FindIntroHoundmaster(float x, float y, float z)
        {
            std::list<Creature*> houndmasters;
            GetCreatureListWithEntryInGrid(
                houndmasters, me, NPC_CORRUPTED_HOUNDMASTER, 100.0f);

            Creature* closest = nullptr;
            float bestDistance = 5.0f;

            for (Creature* houndmaster : houndmasters)
            {
                if (!houndmaster || !houndmaster->IsAlive())
                    continue;

                float distance = houndmaster->GetDistance(x, y, z);
                if (distance < bestDistance)
                {
                    bestDistance = distance;
                    closest = houndmaster;
                }
            }

            return closest;
        }

        bool _introDialoguePlayed;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_adaroggAI(creature);
    }
};

void AddSC_boss_adarogg()
{
    new at_rfc_adarogg_intro();
    new boss_adarogg();
}
