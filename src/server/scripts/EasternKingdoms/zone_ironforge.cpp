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

#include "ScriptMgr.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "PetDefines.h"
#include "Player.h"
#include "QuestDef.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"
#include "UnitDefines.h"
#include "Vehicle.h"
#include <algorithm>
#include <vector>

enum IronforgeSeizeAmbassador
{
    QUEST_DEMANDING_ANSWERS              = 26112,
    QUEST_SEIZE_AMBASSADOR               = 26118,

    NPC_MOIRA_THAURISSAN                 = 42129,
    NPC_FALSTAD_WILDHAMMER               = 42131,
    NPC_AMBASSADOR_SLAGHAMMER            = 42146,
    NPC_AMBASSADOR_SLAGHAMMER_ESCORT     = 42153,
    NPC_MURADIN_BRONZEBEARD              = 42928,
    NPC_IRONFORGE_GUARD                  = 5595,

    SPELL_ARREST_SLAGHAMMER              = 78628,
    // Dummy aura with chain visual; 78628 has no apply-aura effect, and 125467 is Root in 8.3.7.
    SPELL_COSMETIC_ENSLAVE_CHAINS        = 45631,

    SPAWN_ID_GUARD_ESCORT                = 180741,
    SPAWN_ID_GUARD_DOOR_OTHER            = 180742,

    SAY_MOIRA_COUNCIL_0                  = 0,
    SAY_MOIRA_COUNCIL_1                  = 1,
    SAY_MOIRA_COUNCIL_2                  = 2,
    SAY_MOIRA_COUNCIL_3                  = 3,
    SAY_MOIRA_COUNCIL_4                  = 4,
    SAY_MOIRA_FINALE                     = 5,

    SAY_MURADIN_COUNCIL_0                = 0,
    SAY_MURADIN_COUNCIL_1                = 1,
    SAY_MURADIN_COUNCIL_2                = 2,

    SAY_FALSTAD_COUNCIL                  = 0,

    SAY_SLAGHAMMER_ARREST                = 0,
    SAY_SLAGHAMMER_FINALE                = 1,

    SAY_GUARD_DOOR_0                     = 0,
    SAY_GUARD_DOOR_1                     = 1,
    SAY_GUARD_ESCORT                     = 2,

    ACTION_START_COUNCIL                 = 1,

    POINT_HALT                           = 1
};

// Hall floor in front of the High Seat, not on the dais. Facing Moira on the throne.
static Position const HighSeatHaltPos = { -4848.698730f, -1043.593628f, 502.189972f, 2.544585f };
static Position const GuardEscortHomePos = { -4858.79f, -1056.22f, 502.19f, 1.15192f };

// World 42146 must respawn so a second player can still arrest; dummy of 78628 hides the original.
static Seconds const AmbassadorWorldRespawn = 60s;

static Creature* GetCreatureBySpawnId(WorldObject* searcher, uint32 spawnId)
{
    if (!searcher || !searcher->GetMap())
        return nullptr;

    auto bounds = searcher->GetMap()->GetCreatureBySpawnIdStore().equal_range(spawnId);
    for (auto itr = bounds.first; itr != bounds.second; ++itr)
        if (Creature* creature = itr->second)
            if (creature->IsAlive() && creature->IsInWorld())
                return creature;

    return nullptr;
}

static void TalkIfAI(Creature* creature, uint32 group)
{
    if (!creature || !creature->IsAIEnabled || !creature->IsInWorld())
        return;

    creature->AI()->Talk(group);
}

struct npc_moira_thaurissan : public ScriptedAI
{
    npc_moira_thaurissan(Creature* creature) : ScriptedAI(creature), _eventActive(false) { }

    enum CouncilEvents
    {
        EVENT_COUNCIL_MOIRA_0 = 1,
        EVENT_COUNCIL_MURADIN_0,
        EVENT_COUNCIL_MOIRA_1,
        EVENT_COUNCIL_MURADIN_1,
        EVENT_COUNCIL_MOIRA_2,
        EVENT_COUNCIL_MURADIN_2,
        EVENT_COUNCIL_FALSTAD,
        EVENT_COUNCIL_MOIRA_3,
        EVENT_COUNCIL_MOIRA_4,
        EVENT_COUNCIL_DONE
    };

    void Reset() override
    {
        if (_eventActive)
            return;

        events.Reset();
    }

    void sQuestReward(Player* /*player*/, Quest const* quest, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == QUEST_DEMANDING_ANSWERS)
            DoAction(ACTION_START_COUNCIL);
    }

    void DoAction(int32 action) override
    {
        if (action != ACTION_START_COUNCIL || _eventActive)
            return;

        _eventActive = true;
        events.Reset();
        events.ScheduleEvent(EVENT_COUNCIL_MOIRA_0, 500ms);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!_eventActive)
            return;

        events.Update(diff);
        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_COUNCIL_MOIRA_0:
                    Talk(SAY_MOIRA_COUNCIL_0);
                    events.ScheduleEvent(EVENT_COUNCIL_MURADIN_0, 5500ms);
                    break;
                case EVENT_COUNCIL_MURADIN_0:
                    TalkIfAI(me->FindNearestCreature(NPC_MURADIN_BRONZEBEARD, 25.0f, true), SAY_MURADIN_COUNCIL_0);
                    events.ScheduleEvent(EVENT_COUNCIL_MOIRA_1, 5500ms);
                    break;
                case EVENT_COUNCIL_MOIRA_1:
                    Talk(SAY_MOIRA_COUNCIL_1);
                    events.ScheduleEvent(EVENT_COUNCIL_MURADIN_1, 5500ms);
                    break;
                case EVENT_COUNCIL_MURADIN_1:
                    TalkIfAI(me->FindNearestCreature(NPC_MURADIN_BRONZEBEARD, 25.0f, true), SAY_MURADIN_COUNCIL_1);
                    events.ScheduleEvent(EVENT_COUNCIL_MOIRA_2, 5500ms);
                    break;
                case EVENT_COUNCIL_MOIRA_2:
                    Talk(SAY_MOIRA_COUNCIL_2);
                    events.ScheduleEvent(EVENT_COUNCIL_MURADIN_2, 5500ms);
                    break;
                case EVENT_COUNCIL_MURADIN_2:
                    TalkIfAI(me->FindNearestCreature(NPC_MURADIN_BRONZEBEARD, 25.0f, true), SAY_MURADIN_COUNCIL_2);
                    events.ScheduleEvent(EVENT_COUNCIL_FALSTAD, 5500ms);
                    break;
                case EVENT_COUNCIL_FALSTAD:
                    TalkIfAI(me->FindNearestCreature(NPC_FALSTAD_WILDHAMMER, 25.0f, true), SAY_FALSTAD_COUNCIL);
                    events.ScheduleEvent(EVENT_COUNCIL_MOIRA_3, 5500ms);
                    break;
                case EVENT_COUNCIL_MOIRA_3:
                    Talk(SAY_MOIRA_COUNCIL_3);
                    events.ScheduleEvent(EVENT_COUNCIL_MOIRA_4, 6000ms);
                    break;
                case EVENT_COUNCIL_MOIRA_4:
                    Talk(SAY_MOIRA_COUNCIL_4);
                    events.ScheduleEvent(EVENT_COUNCIL_DONE, 1s);
                    break;
                case EVENT_COUNCIL_DONE:
                    _eventActive = false;
                    events.Reset();
                    break;
                default:
                    break;
            }
        }
    }

private:
    EventMap events;
    bool _eventActive;
};

struct npc_ambassador_slaghammer : public ScriptedAI
{
    npc_ambassador_slaghammer(Creature* creature) : ScriptedAI(creature),
        _started(false), _doorDone(false), _finale(false), _halted(false), _credited(false) { }

    enum EscortEvents
    {
        EVENT_CHECK_OWNER = 1,
        EVENT_CHECK_PROGRESS,
        EVENT_HALT_TIMEOUT,
        EVENT_SAY_MOIRA_FINALE,
        EVENT_SAY_SLAGHAMMER_FINALE,
        EVENT_GUARD_APPROACH,
        EVENT_SAY_GUARD_ESCORT,
        EVENT_CREDIT_QUEST,
        EVENT_LEAVE,
        EVENT_GUARD_HOME
    };

    void Reset() override
    {
        me->SetReactState(REACT_PASSIVE);
        SetCombatMovement(false);

        if (_started)
            return;

        if (Unit* owner = me->GetOwner())
            if (Player* player = owner->ToPlayer())
                StartEscort(player);
    }

    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        me->CombatStop(true);
        if (_finale || !_started)
            return;

        if (Player* player = GetEscortPlayer())
            FollowPlayer(player);
    }

    void IsSummonedBy(Unit* summoner) override
    {
        Player* player = summoner ? summoner->ToPlayer() : nullptr;
        if (!player)
        {
            me->DespawnOrUnsummon();
            return;
        }

        StartEscort(player);
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == POINT_HALT)
            BeginSentencing();
    }

    void UpdateAI(uint32 diff) override
    {
        if (!_started)
            return;

        events.Update(diff);
        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CHECK_OWNER:
                    if (!_finale && !GetEscortPlayer())
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }
                    events.ScheduleEvent(EVENT_CHECK_OWNER, 1s);
                    break;
                case EVENT_CHECK_PROGRESS:
                    UpdateProgress();
                    if (_started && me->IsInWorld())
                        events.ScheduleEvent(EVENT_CHECK_PROGRESS, 1s);
                    break;
                case EVENT_HALT_TIMEOUT:
                    BeginSentencing();
                    break;
                case EVENT_SAY_MOIRA_FINALE:
                    TalkIfAI(me->FindNearestCreature(NPC_MOIRA_THAURISSAN, 40.0f, true), SAY_MOIRA_FINALE);
                    events.ScheduleEvent(EVENT_SAY_SLAGHAMMER_FINALE, 7s);
                    break;
                case EVENT_SAY_SLAGHAMMER_FINALE:
                    Talk(SAY_SLAGHAMMER_FINALE);
                    events.ScheduleEvent(EVENT_GUARD_APPROACH, 1s);
                    break;
                case EVENT_GUARD_APPROACH:
                    StartGuardApproach();
                    events.ScheduleEvent(EVENT_SAY_GUARD_ESCORT, 8s);
                    break;
                case EVENT_SAY_GUARD_ESCORT:
                    TalkIfAI(GetEscortGuard(), SAY_GUARD_ESCORT);
                    events.ScheduleEvent(EVENT_CREDIT_QUEST, 2s);
                    break;
                case EVENT_CREDIT_QUEST:
                    CreditEscortPlayer();
                    events.ScheduleEvent(EVENT_LEAVE, 1s);
                    break;
                case EVENT_LEAVE:
                    BeginGuardEscortAway();
                    events.ScheduleEvent(EVENT_GUARD_HOME, 10s);
                    break;
                case EVENT_GUARD_HOME:
                    RestoreEscortGuard();
                    me->DespawnOrUnsummon();
                    break;
                default:
                    break;
            }
        }
    }

private:
    Player* GetEscortOwner() const
    {
        if (_playerGuid.IsEmpty())
            return nullptr;

        Player* player = ObjectAccessor::GetPlayer(*me, _playerGuid);
        if (!player || !player->IsInWorld() || player->GetMap() != me->GetMap())
            return nullptr;

        return player;
    }

    Player* GetEscortPlayer() const
    {
        Player* player = GetEscortOwner();
        if (!player)
            return nullptr;

        // After ForceCompleteQuest the status is no longer INCOMPLETE; the leave walk still needs the owner.
        if (!_finale && player->GetQuestStatus(QUEST_SEIZE_AMBASSADOR) != QUEST_STATUS_INCOMPLETE)
            return nullptr;

        return player;
    }

    void FollowPlayer(Player* player)
    {
        if (!player)
            return;

        me->GetMotionMaster()->Clear();
        me->SetWalk(true);
        me->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
    }

    void StartEscort(Player* player)
    {
        if (_started || !player)
            return;

        _started = true;
        _playerGuid = player->GetGUID();

        me->setActive(true);
        me->SetReactState(REACT_PASSIVE);
        SetCombatMovement(false);
        me->AddUnitFlag(UnitFlags(UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NON_ATTACKABLE));

        ApplyManacles();
        Talk(SAY_SLAGHAMMER_ARREST);
        FollowPlayer(player);

        events.Reset();
        events.ScheduleEvent(EVENT_CHECK_OWNER, 1s);
        events.ScheduleEvent(EVENT_CHECK_PROGRESS, 1s);
    }

    void UpdateProgress()
    {
        if (_finale)
            return;

        if (!GetEscortPlayer())
            return;

        float const dist = me->GetExactDist(HighSeatHaltPos);
        if (!_doorDone && dist < 28.0f)
        {
            _doorDone = true;
            PlayDoorComments();
        }

        if (dist <= 8.0f)
            StartFinale();
    }

    void PlayDoorComments()
    {
        TalkIfAI(GetCreatureBySpawnId(me, SPAWN_ID_GUARD_ESCORT), SAY_GUARD_DOOR_0);
        TalkIfAI(GetCreatureBySpawnId(me, SPAWN_ID_GUARD_DOOR_OTHER), SAY_GUARD_DOOR_1);
    }

    void StartFinale()
    {
        if (_finale)
            return;

        _finale = true;
        me->GetMotionMaster()->Clear();
        me->SetWalk(true);
        me->GetMotionMaster()->MovePoint(POINT_HALT, HighSeatHaltPos, true);
        events.ScheduleEvent(EVENT_HALT_TIMEOUT, 12s);
    }

    void BeginSentencing()
    {
        if (_halted)
            return;

        _halted = true;
        events.CancelEvent(EVENT_HALT_TIMEOUT);
        me->GetMotionMaster()->MoveIdle();
        me->SetFacingTo(HighSeatHaltPos.GetOrientation());
        me->SetStandState(UNIT_STAND_STATE_KNEEL);
        events.ScheduleEvent(EVENT_SAY_MOIRA_FINALE, 1s);
    }

    Creature* GetEscortGuard()
    {
        return GetCreatureBySpawnId(me, SPAWN_ID_GUARD_ESCORT);
    }

    void ApplyManacles()
    {
        if (me->HasAura(SPELL_COSMETIC_ENSLAVE_CHAINS))
            return;

        me->CastSpell(me, SPELL_COSMETIC_ENSLAVE_CHAINS, true);
    }

    void StartGuardApproach()
    {
        Creature* guard = GetEscortGuard();
        if (!guard)
            return;

        _guardGuid = guard->GetGUID();
        guard->setActive(true);
        guard->SetWalk(true);
        guard->SetReactState(REACT_PASSIVE);
        guard->GetMotionMaster()->Clear();
        guard->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), true);
    }

    void BeginGuardEscortAway()
    {
        me->SetStandState(UNIT_STAND_STATE_STAND);
        me->GetMotionMaster()->Clear();
        me->SetWalk(true);

        Creature* guard = _guardGuid.IsEmpty() ? GetEscortGuard() : ObjectAccessor::GetCreature(*me, _guardGuid);
        if (!guard)
        {
            me->GetMotionMaster()->MovePoint(0, GuardEscortHomePos, true);
            me->DespawnOrUnsummon(10s);
            return;
        }

        guard->SetWalk(true);
        guard->GetMotionMaster()->Clear();
        guard->GetMotionMaster()->MovePoint(0, guard->GetHomePosition(), true);
        me->GetMotionMaster()->MoveFollow(guard, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
    }

    void RestoreEscortGuard()
    {
        Creature* guard = _guardGuid.IsEmpty() ? GetEscortGuard() : ObjectAccessor::GetCreature(*me, _guardGuid);
        if (!guard)
            return;

        Position const& home = guard->GetHomePosition();
        guard->SetReactState(REACT_AGGRESSIVE);
        guard->GetMotionMaster()->Clear();
        guard->StopMoving();
        // MovePoint does not restore spawn facing; home orientation comes from creature.orientation.
        if (guard->GetExactDist(home) > 2.0f)
            guard->NearTeleportTo(home);
        else
            guard->SetFacingTo(home.GetOrientation(), true);
        guard->setActive(false);
    }

    void CreditEscortPlayer()
    {
        if (_credited)
            return;

        _credited = true;

        Player* player = GetEscortOwner();
        Quest const* quest = sObjectMgr->GetQuestTemplate(QUEST_SEIZE_AMBASSADOR);
        if (!player || !quest)
            return;

        // SpecialFlags exploration quests need Explored before CanCompleteQuest; the client
        // also never saw SMSG_QUEST_UPDATE_COMPLETE from ForceCompleteQuest alone.
        player->AreaExploredOrEventHappens(QUEST_SEIZE_AMBASSADOR);
        if (player->GetQuestStatus(QUEST_SEIZE_AMBASSADOR) != QUEST_STATUS_COMPLETE)
            player->ForceCompleteQuest(QUEST_SEIZE_AMBASSADOR);
        player->SendQuestComplete(quest);
    }

    EventMap events;
    ObjectGuid _playerGuid;
    ObjectGuid _guardGuid;
    bool _started;
    bool _doorDone;
    bool _finale;
    bool _halted;
    bool _credited;
};

class spell_ironforge_arrest_slaghammer : public SpellScript
{
    PrepareSpellScript(spell_ironforge_arrest_slaghammer);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_ARREST_SLAGHAMMER, SPELL_COSMETIC_ENSLAVE_CHAINS });
    }

    SpellCastResult CheckCast()
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!player)
            return SPELL_FAILED_BAD_TARGETS;

        if (player->GetQuestStatus(QUEST_SEIZE_AMBASSADOR) != QUEST_STATUS_INCOMPLETE)
            return SPELL_FAILED_BAD_TARGETS;

        for (Unit* controlled : player->m_Controlled)
        {
            if (controlled && controlled->GetEntry() == NPC_AMBASSADOR_SLAGHAMMER_ESCORT && controlled->IsAlive())
            {
                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_SLAGHAMMER_ALREADY_PRISONER);
                return SPELL_FAILED_CUSTOM_ERROR;
            }
        }

        Unit* target = GetExplTargetUnit();
        if (!target || target->GetEntry() != NPC_AMBASSADOR_SLAGHAMMER || !target->IsAlive())
            return SPELL_FAILED_BAD_TARGETS;

        return SPELL_CAST_OK;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        Unit* hit = GetHitUnit();
        Creature* orig = (hit && hit->GetEntry() == NPC_AMBASSADOR_SLAGHAMMER) ? hit->ToCreature() : nullptr;

        Creature* escort = nullptr;
        if (player)
        {
            for (Unit* controlled : player->m_Controlled)
            {
                if (controlled && controlled->GetEntry() == NPC_AMBASSADOR_SLAGHAMMER_ESCORT && controlled->IsAlive())
                {
                    escort = controlled->ToCreature();
                    break;
                }
            }
        }

        if (escort)
            escort->CastSpell(escort, SPELL_COSMETIC_ENSLAVE_CHAINS, true);

        if (orig)
            orig->DespawnOrUnsummon(1s, AmbassadorWorldRespawn);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_ironforge_arrest_slaghammer::CheckCast);
        OnEffectHitTarget += SpellEffectFn(spell_ironforge_arrest_slaghammer::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
    }
};

class npc_mountaineer_watch : public CreatureScript
{
public:
    npc_mountaineer_watch() : CreatureScript("npc_mountaineer_watch") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(313) == QUEST_STATUS_INCOMPLETE)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Captain Tharran wants you to deploy your remote observation bots and withdraw to Kharanos.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        }
        SendGossipMenuFor(player, creature->GetEntry(), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            switch (creature->GetEntry())
            {
            case 40991: // Mountaineer Dunstan
                creature->Whisper("Tell Captain Tharran that I'll be back in Kharanos as soon as I've verified that the bot is working correctly.", LANG_UNIVERSAL, 0);
                CloseGossipMenuFor(player);
                break;
            case 40994: // Mountaineer Lewin
                creature->Whisper("I can't wait to get out there and help in the fight against those trolls.", LANG_UNIVERSAL, 0);
                CloseGossipMenuFor(player);
                break;
            case 41056: // Mountaineer Valgrum
                creature->Whisper("We've been fighting nonstop since the cataclysm. It'll be nice to get a rest, if a brief one.", LANG_UNIVERSAL, 0);
                CloseGossipMenuFor(player);
                break;
            }

            float x, y, z;
            creature->GetClosePoint(x, y, z, creature->GetObjectSize() / 3, 5.0f);
            if (Creature* bot = creature->SummonCreature(41052, x, y, z, 0, TEMPSUMMON_MANUAL_DESPAWN)) // Summon Remote Observation Bot.
            {
                bot->GetMotionMaster()->MoveRandom(10.0f);
                bot->DespawnOrUnsummon(10000);
            }

            player->KilledMonsterCredit(creature->GetEntry());
            creature->DespawnOrUnsummon(6000);
        }

        return true;
    }
};

void AddSC_ironforge()
{
    RegisterCreatureAI(npc_moira_thaurissan);
    RegisterCreatureAI(npc_ambassador_slaghammer);
    RegisterSpellScript(spell_ironforge_arrest_slaghammer);
    new npc_mountaineer_watch();
}
