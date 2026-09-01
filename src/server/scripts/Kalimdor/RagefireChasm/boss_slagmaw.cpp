/*
 * 2026 BFA-HavenCore
 *
 * Ragefire Chasm - Slagmaw (61463)
 * Retail-style encounter behavior adapted for HavenCore 8.3.7.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "InstanceScript.h"

enum SlagmawSpells
{
    SPELL_LAVA_SPIT = 119434,
    SPELL_SUBMERGE  = 120384
};

enum SlagmawPositions
{
    POSITION_NORTH = 0,
    POSITION_EAST,
    POSITION_SOUTH,
    POSITION_WEST,
    POSITION_MAX
};

// Retail/TrinityCore emergence positions for Slagmaw in map 389.
Position const SlagmawTeleportPositions[POSITION_MAX] =
{
    { -222.940f, 165.703f, -19.721f, 3.797819f },
    { -226.477f, 135.704f, -19.721f, 2.330294f },
    { -263.212f, 136.244f, -19.721f, 0.7556769f },
    { -256.389f, 172.884f, -19.721f, 5.577933f }
};

enum SlagmawEvents
{
    EVENT_LAVA_SPIT = 1,
    EVENT_TELEPORT,
    EVENT_EMERGE,
    EVENT_BOUNDARY_CHECK
};

class boss_slagmaw : public CreatureScript
{
public:
    boss_slagmaw() : CreatureScript("boss_slagmaw") { }

    struct boss_slagmawAI : public ScriptedAI
    {
        boss_slagmawAI(Creature* creature)
            : ScriptedAI(creature),
              instance(creature->GetInstanceScript()),
              lavaSpitCount(0),
              lastTeleport(POSITION_WEST)
        {
        }

        InstanceScript* instance;
        uint8 lavaSpitCount;
        uint8 lastTeleport;

        void Reset() override
        {
            events.Reset();

            lavaSpitCount = 0;
            lastTeleport = POSITION_WEST;

            me->RemoveAurasDueToSpell(SPELL_SUBMERGE);
            me->RemoveUnitFlag(UnitFlags(
                UNIT_FLAG_NON_ATTACKABLE |
                UNIT_FLAG_NOT_SELECTABLE));

            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        }

        void AttackStart(Unit* target) override
        {
            if (!target)
                return;

            if (me->Attack(target, true))
                DoStartNoMovement(target);
        }

        void EnterCombat(Unit* who) override
        {
            DoStartNoMovement(who);

            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

            events.ScheduleEvent(EVENT_LAVA_SPIT, 1000);
            events.ScheduleEvent(EVENT_BOUNDARY_CHECK, 2500);
        }

        void JustReachedHome() override
        {
            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        }

        void JustDied(Unit* /*killer*/) override
        {
            events.Reset();
            me->RemoveAurasDueToSpell(SPELL_SUBMERGE);

            if (instance)
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
        }

        uint8 GetNextTeleport()
        {
            uint8 selected;

            do
            {
                selected = urand(0, POSITION_MAX - 1);
            }
            while (selected == lastTeleport);

            lastTeleport = selected;
            return selected;
        }

        void StartSubmerge()
        {
            DoCast(me, SPELL_SUBMERGE);
            lavaSpitCount = 0;
            events.ScheduleEvent(EVENT_TELEPORT, 3000);
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
                    case EVENT_LAVA_SPIT:
                    {
                        if (lavaSpitCount < 5)
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            {
                                DoCast(target, SPELL_LAVA_SPIT);
                                ++lavaSpitCount;
                            }

                            events.ScheduleEvent(EVENT_LAVA_SPIT, 1000);
                        }
                        else
                        {
                            StartSubmerge();
                        }

                        break;
                    }

                    case EVENT_TELEPORT:
                    {
                        uint8 positionIndex = GetNextTeleport();
                        Position const& pos = SlagmawTeleportPositions[positionIndex];

                        me->NearTeleportTo(
                            pos.GetPositionX(),
                            pos.GetPositionY(),
                            pos.GetPositionZ(),
                            pos.GetOrientation());

                        events.ScheduleEvent(EVENT_EMERGE, 1000);
                        break;
                    }

                    case EVENT_EMERGE:
                    {
                        me->RemoveAurasDueToSpell(SPELL_SUBMERGE);

                        if (Unit* victim = me->GetVictim())
                            DoStartNoMovement(victim);

                        events.ScheduleEvent(EVENT_LAVA_SPIT, 1000);
                        break;
                    }

                    case EVENT_BOUNDARY_CHECK:
                    {
                        if (Unit* victim = me->GetVictim())
                        {
                            if (me->GetDistance(victim) > 50.0f)
                            {
                                EnterEvadeMode(EVADE_REASON_OTHER);
                                return;
                            }
                        }

                        events.ScheduleEvent(EVENT_BOUNDARY_CHECK, 2500);
                        break;
                    }

                    default:
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_slagmawAI(creature);
    }
};

void AddSC_boss_slagmaw()
{
    new boss_slagmaw();
}
