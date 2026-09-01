/*
 * Ragefire Chasm - Lava Guard Gordoth
 * BFA-HavenCore 8.3.7
 *
 * Retail-like intro adapted from modern TrinityCore reference behavior:
 * - AreaTrigger 7899 starts the breakout
 * - Gordoth breaks the cage and jumps into the laboratory
 * - Lab vials are destroyed
 * - Dark Shaman Researchers are killed by the experiment
 * - Combat rotation: Ground Rupture, Seismic Slam, Enrage at 30%
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "GameObject.h"
#include "MotionMaster.h"

enum GordothSpells
{
    // Intro
    SPELL_JAIL_BREAK        = 120025,
    SPELL_GROUND_SLAM       = 120023,
    SPELL_MEAT_EXPLOSION_1  = 111778,
    SPELL_MEAT_EXPLOSION_2  = 111779,
    SPELL_MEAT_EXPLOSION_3  = 111780,
    SPELL_BLOODY_SUICIDE    = 120022,

    // Combat
    SPELL_ENRAGE            = 50420,
    SPELL_GROUND_RUPTURE    = 119999,
    SPELL_SEISMIC_SLAM      = 120024
};

enum GordothEvents
{
    EVENT_RUPTURE = 1,
    EVENT_SLAM
};

enum GordothMisc
{
    NPC_GORDOTH                 = 61528,
    NPC_DARK_SHAMAN_RESEARCHER  = 61644,

    GO_GORDOTH_CAGE             = 211792,
    GO_LAB_VIAL                 = 211784,

    ACTION_GORDOTH_BREAKOUT     = 1,
    POINT_GORDOTH_JUMP          = 1
};

static Position const GordothJumpPos =
{
    -363.5392f,
    203.36604f,
    -22.005634f,
    0.30812347f
};

// AreaTrigger 7899 - Gordoth laboratory intro
class at_rfc_gordoth_intro : public AreaTriggerScript
{
public:
    at_rfc_gordoth_intro() : AreaTriggerScript("at_rfc_gordoth_intro") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/, bool entered) override
    {
        if (!entered || !player)
            return false;

        if (Creature* gordoth = player->FindNearestCreature(NPC_GORDOTH, 100.0f, true))
        {
            if (gordoth->AI())
                gordoth->AI()->DoAction(ACTION_GORDOTH_BREAKOUT);

            return true;
        }

        return false;
    }
};

class boss_gordoth : public CreatureScript
{
public:
    boss_gordoth() : CreatureScript("boss_gordoth") { }

    struct boss_gordothAI : public ScriptedAI
    {
        boss_gordothAI(Creature* creature)
            : ScriptedAI(creature), _enraged(false), _introStarted(false)
        {
        }

        void Reset() override
        {
            events.Reset();
            _enraged = false;
        }

        void EnterCombat(Unit* /*who*/) override
        {
            events.ScheduleEvent(EVENT_RUPTURE, 7300);
            events.ScheduleEvent(EVENT_SLAM, 13300);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            if (!_enraged && me->HealthBelowPctDamaged(30, damage))
            {
                _enraged = true;
                DoCast(me, SPELL_ENRAGE, true);
            }
        }

        void DoAction(int32 action) override
        {
            if (action != ACTION_GORDOTH_BREAKOUT || _introStarted)
                return;

            _introStarted = true;

            // Open/break the cage visually.
            if (GameObject* cage = GetClosestGameObjectWithEntry(me, GO_GORDOTH_CAGE, 50.0f))
                cage->SetGoState(GO_STATE_ACTIVE);

            // Retail breakout visual.
            DoCast(me, SPELL_JAIL_BREAK, true);

            me->SetReactState(REACT_PASSIVE);
            me->CombatStop(true);
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveJump(GordothJumpPos, 50.0f, 4.0f, POINT_GORDOTH_JUMP);
        }

        void MovementInform(uint32 /*type*/, uint32 id) override
        {
            if (id != POINT_GORDOTH_JUMP)
                return;

            me->SetHomePosition(GordothJumpPos);
            me->SetFacingTo(GordothJumpPos.GetOrientation());

            // Landing slam.
            DoCast(me, SPELL_GROUND_SLAM, true);

            // Smash/change the laboratory vials as part of the breakout.
            std::list<GameObject*> vials;
            GetGameObjectListWithEntryInGrid(vials, me, GO_LAB_VIAL, 30.0f);
            for (GameObject* vial : vials)
                vial->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);

            // The researchers are killed by the failed experiment.
            std::list<Creature*> researchers;
            GetCreatureListWithEntryInGrid(researchers, me, NPC_DARK_SHAMAN_RESEARCHER, 30.0f);
            for (Creature* researcher : researchers)
            {
                if (!researcher->IsAlive())
                    continue;

                researcher->CastSpell(researcher, SPELL_MEAT_EXPLOSION_1, true);
                researcher->CastSpell(researcher, SPELL_MEAT_EXPLOSION_2, true);
                researcher->CastSpell(researcher, SPELL_MEAT_EXPLOSION_3, true);
                researcher->CastSpell(researcher, SPELL_BLOODY_SUICIDE, true);
                researcher->KillSelf();
            }

            // Gordoth is now active and may engage nearby players normally.
            me->SetReactState(REACT_AGGRESSIVE);
            DoZoneInCombat();
        }

        void UpdateAI(uint32 diff) override
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
                    case EVENT_RUPTURE:
                        DoCastRandom(SPELL_GROUND_RUPTURE, 30.0f);
                        events.ScheduleEvent(EVENT_RUPTURE, 12100);
                        break;

                    case EVENT_SLAM:
                        DoCastVictim(SPELL_SEISMIC_SLAM);
                        events.ScheduleEvent(EVENT_SLAM, 36300);
                        break;

                    default:
                        break;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }

    private:
        bool _enraged;
        bool _introStarted;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_gordothAI(creature);
    }
};

void AddSC_boss_gordoth()
{
    new at_rfc_gordoth_intro();
    new boss_gordoth();
}
