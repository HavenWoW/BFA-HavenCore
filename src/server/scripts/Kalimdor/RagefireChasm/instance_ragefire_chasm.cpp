/*
 * 2026 BFA-HavenCore
 *
 * Ragefire Chasm instance support.
 *
 * Keeps the existing LFG/instance-script behavior and filters the duplicated
 * faction-specific rescue NPCs so Alliance and Horde prisoners are not
 * rendered on top of each other in the same instance.
 */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Player.h"
#include "Creature.h"

namespace
{
    enum RfcCreatures
    {
        NPC_KORKRON_ELITE          = 61404,
        NPC_KORKRON_SCOUT          = 61680,
        NPC_INVOKER_XORENTH        = 61716,
        NPC_COMMANDER_BAGRAN       = 61724,

        NPC_SI7_RANGER             = 61788,
        NPC_SI7_COMMANDO           = 61821,
        NPC_SI7_FIELD_COMMANDER    = 61822,
        NPC_HIGH_SORCERESS_ARYNA   = 61823
    };

    uint32 const HordeRescueSpawns[] =
    {
        10615981,
        10615985,
        10615984,
        10615969,
        10615970,
        10615942,
        10615916,
        10615895
    };

    uint32 const AllianceRescueSpawns[] =
    {
        10642794,
        10642797,
        10642796,
        10642793,
        10642795,
        10642790,
        10642791,
        10642792
    };
}

class instance_ragefire_chasm : public InstanceMapScript
{
public:
    instance_ragefire_chasm() : InstanceMapScript("instance_ragefire_chasm", 389) { }

    struct instance_ragefire_chasm_InstanceMapScript : public InstanceScript
    {
        instance_ragefire_chasm_InstanceMapScript(InstanceMap* map)
            : InstanceScript(map), _teamInitialized(false), _teamId(TEAM_NEUTRAL)
        {
        }

        void OnPlayerEnter(Player* player) override
        {
            if (!player || _teamInitialized)
                return;

            _teamId = player->GetTeamId();
            _teamInitialized = true;

            // Creatures in unloaded grids may not exist yet. Remove any that
            // are already loaded now; OnCreatureCreate below handles the rest.
            if (_teamId == TEAM_ALLIANCE)
            {
                for (uint32 spawnId : HordeRescueSpawns)
                    DespawnSpawn(spawnId);
            }
            else if (_teamId == TEAM_HORDE)
            {
                for (uint32 spawnId : AllianceRescueSpawns)
                    DespawnSpawn(spawnId);
            }
        }

        void OnCreatureCreate(Creature* creature) override
        {
            if (!creature || !_teamInitialized)
                return;

            if (_teamId == TEAM_ALLIANCE)
            {
                switch (creature->GetEntry())
                {
                    case NPC_KORKRON_ELITE:
                    case NPC_KORKRON_SCOUT:
                    case NPC_INVOKER_XORENTH:
                    case NPC_COMMANDER_BAGRAN:
                        creature->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }
            else if (_teamId == TEAM_HORDE)
            {
                switch (creature->GetEntry())
                {
                    case NPC_SI7_RANGER:
                    case NPC_SI7_COMMANDO:
                    case NPC_SI7_FIELD_COMMANDER:
                    case NPC_HIGH_SORCERESS_ARYNA:
                        creature->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        void DespawnSpawn(uint32 spawnId)
        {
            auto bounds = instance->GetCreatureBySpawnIdStore().equal_range(spawnId);
            for (auto itr = bounds.first; itr != bounds.second; ++itr)
                if (Creature* creature = itr->second)
                    if (creature->IsInWorld())
                        creature->DespawnOrUnsummon();
        }

        bool _teamInitialized;
        TeamId _teamId;
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_ragefire_chasm_InstanceMapScript(map);
    }
};

void AddSC_instance_ragefire_chasm()
{
    new instance_ragefire_chasm();
}
