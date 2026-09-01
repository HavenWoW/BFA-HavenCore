/*
 * 2026 BFA-HavenCore
 *
 * Ragefire Chasm support
 */

#include "ScriptMgr.h"
#include "Creature.h"
#include "GameObject.h"
#include "MotionMaster.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ragefire_chasm.h"

namespace RagefireChasm
{
    enum Quests
    {
        QUEST_NO_ORC_LEFT_BEHIND = 30984,
        QUEST_NO_MAN_LEFT_BEHIND = 30995
    };

    enum Creatures
    {
        NPC_KORKRON_SCOUT = 61680,
        NPC_SI7_RANGER    = 61788,

        NPC_SUSPICIOUS_ROCK     = 61780,
        NPC_INCONSPICUOUS_CRATE = 61790
    };

    enum GameObjects
    {
        GO_RANGER_CAGE_1 = 211883,
        GO_RANGER_CAGE_2 = 211884,
        GO_RANGER_CAGE_3 = 211885
    };

    // Returns the appropriate quest-credit creature for the player.
    uint32 GetRescueCreditEntry(Player* player)
    {
        if (player->GetQuestStatus(QUEST_NO_MAN_LEFT_BEHIND) == QUEST_STATUS_INCOMPLETE)
            return NPC_SI7_RANGER;

        if (player->GetQuestStatus(QUEST_NO_ORC_LEFT_BEHIND) == QUEST_STATUS_INCOMPLETE)
            return NPC_KORKRON_SCOUT;

        return 0;
    }

    bool RescuePrisoner(Player* player, WorldObject* source)
    {
        if (!player || !source)
            return false;

        uint32 creditEntry = GetRescueCreditEntry(player);
        if (!creditEntry)
            return false;

        uint32 questId = creditEntry == NPC_SI7_RANGER
            ? QUEST_NO_MAN_LEFT_BEHIND
            : QUEST_NO_ORC_LEFT_BEHIND;

        // Do not give additional credit once the objective is already complete.
        if (player->GetReqKillOrCastCurrentCount(questId, creditEntry) >= 5)
            return false;

        // Each rescue point already has the appropriate prisoner spawned
        // at or immediately beside it.
        Creature* prisoner = source->FindNearestCreature(creditEntry, 6.0f, true);
        if (!prisoner)
            return false;

        player->KilledMonsterCredit(creditEntry, prisoner->GetGUID());

        // Retail-like rescue presentation:
        // prisoner acknowledges the rescue, becomes passive, follows a
        // location-specific escape route, then despawns.
        prisoner->SetReactState(REACT_PASSIVE);
        prisoner->CombatStop(true);
        prisoner->DeleteThreatList();
        prisoner->SetWalk(false);

        if (prisoner->AI())
            prisoner->AI()->Talk(0);

        prisoner->GetMotionMaster()->Clear();

        // Suspicious Rock rescue point.
        if (source->GetEntry() == NPC_SUSPICIOUS_ROCK)
        {
            prisoner->GetMotionMaster()->MovePoint(1, -306.174f, -23.1592f, -59.0882f);
            prisoner->GetMotionMaster()->MovePoint(2, -297.9861f, -41.88261f, -60.91673f);
            prisoner->GetMotionMaster()->MovePoint(3, -286.8949f, -48.55449f, -60.93217f);
            prisoner->GetMotionMaster()->MovePoint(4, -276.6081f, -47.31757f, -60.93217f);
            prisoner->GetMotionMaster()->MovePoint(5, -253.9539f, -39.25582f, -60.49646f);
            prisoner->GetMotionMaster()->MovePoint(6, -225.0045f, -37.12025f, -55.7888f);
            prisoner->DespawnOrUnsummon(18000);
            return true;
        }

        // Inconspicuous Crate rescue point.
        if (source->GetEntry() == NPC_INCONSPICUOUS_CRATE)
        {
            prisoner->GetMotionMaster()->MovePoint(1, -92.523f, 65.7691f, -18.6579f);
            prisoner->GetMotionMaster()->MovePoint(2, -108.3838f, 23.41842f, -18.44852f);
            prisoner->GetMotionMaster()->MovePoint(3, -129.3571f, 9.659675f, -20.64646f);
            prisoner->GetMotionMaster()->MovePoint(4, -143.5467f, 8.586313f, -21.6209f);
            prisoner->DespawnOrUnsummon(14000);
            return true;
        }

        // Final laboratory cages. Give them a short clean escape from
        // the cage line and despawn outside the immediate encounter area.
        if (source->GetEntry() == GO_RANGER_CAGE_1 ||
            source->GetEntry() == GO_RANGER_CAGE_2 ||
            source->GetEntry() == GO_RANGER_CAGE_3)
        {
            float x = prisoner->GetPositionX();
            float y = prisoner->GetPositionY();
            float z = prisoner->GetPositionZ();

            prisoner->GetMotionMaster()->MovePoint(1, x + 5.0f, y + 1.0f, z);
            prisoner->GetMotionMaster()->MovePoint(2, x + 12.0f, y - 4.0f, z);
            prisoner->GetMotionMaster()->MovePoint(3, x + 20.0f, y - 8.0f, z);
            prisoner->DespawnOrUnsummon(12000);
            return true;
        }

        // Fallback for any other rescue point.
        Position escapePosition = prisoner->GetNearPosition(22.0f, 0.0f);
        prisoner->GetMotionMaster()->MovePoint(1, escapePosition);
        prisoner->DespawnOrUnsummon(8000);

        return true;
    }
}

// Suspicious Rock / Inconspicuous Crate.
//
// Their gossip text is already supplied by the DB through broadcast text
// 61264 ("It's safe to come out now."), preserving client localization.
class npc_rfc_hidden_prisoner : public CreatureScript
{
public:
    npc_rfc_hidden_prisoner() : CreatureScript("npc_rfc_hidden_prisoner") { }

    struct npc_rfc_hidden_prisonerAI : public ScriptedAI
    {
        npc_rfc_hidden_prisonerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            // These are hiding-place props represented as creatures.
            // Keep them stationary instead of allowing continuous turning.
            me->GetMotionMaster()->Clear();
            me->StopMoving();
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(uint32 /*diff*/) override { }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_rfc_hidden_prisonerAI(creature);
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 /*action*/) override
{
    CloseGossipMenuFor(player);

    if (RagefireChasm::RescuePrisoner(player, creature))
    {
        // This rescue point is consumed for the instance.
        creature->RemoveNpcFlag(UNIT_NPC_FLAG_GOSSIP);
        creature->DespawnOrUnsummon(1000);
    }

    return true;
}
};

// Final laboratory prisoner cages.
class go_rfc_prisoner_cage : public GameObjectScript
{
public:
    go_rfc_prisoner_cage() : GameObjectScript("go_rfc_prisoner_cage") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        RagefireChasm::RescuePrisoner(player, go);

        return false;
    }
};

void AddSC_ragefire_chasm()
{
    new npc_rfc_hidden_prisoner();
    new go_rfc_prisoner_cage();
}