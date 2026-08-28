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
#include "MotionMaster.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "SpellInfo.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"
#include "Player.h"
#include "Vehicle.h"
#include "ObjectMgr.h"
#include "Unit.h"
#include "TemporarySummon.h"
#include "GameObject.h"
#include "CombatAI.h"
#include "ObjectAccessor.h"
#include "ScriptedEscortAI.h"
#include "AreaTriggerAI.h"
#include "Map.h"
#include "SmartAI.h"
#include <algorithm>
#include <cmath>
#include <vector>

enum FrozenMountaineer
{
    DATA_SET_ICE_BROKEN      = 1,
    EVENT_RUN_AWAY           = 1,
    SAY_MONSTEREMOTE         = 0,
    SPELL_SUMMON_FROZEN_TOMB = 77906,
    SPELL_FREEZE_ANIM        = 77910
};

/*######
# npc_frozen_mountaineer
######*/

struct npc_frozen_mountaineer : public ScriptedAI
{
    npc_frozen_mountaineer(Creature* creature) : ScriptedAI(creature), _dataOneSet(false) { }

    void Reset() override
    {
        _events.Reset();
        DoCastSelf(SPELL_SUMMON_FROZEN_TOMB, true);
        DoCastSelf(SPELL_FREEZE_ANIM, true);
    }

    void SetData(uint32 /*type*/, uint32 data) override
    {
        if (data == DATA_SET_ICE_BROKEN && !_dataOneSet)
        {
            me->RemoveAllAuras();
            Talk(SAY_MONSTEREMOTE);
            _dataOneSet = true;
            _events.ScheduleEvent(EVENT_RUN_AWAY, Seconds(3));
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (!_dataOneSet)
            return;

        _events.Update(diff);

        if (_events.ExecuteEvent() == EVENT_RUN_AWAY)
        {
            me->GetMotionMaster()->MovePoint(0, me->GetPositionX() + (std::cos(me->GetOrientation()) * 15.0f), me->GetPositionY() + (std::sin(me->GetOrientation()) * 15.0f), me->GetPositionZ());
            me->DespawnOrUnsummon(Seconds(2));
        }
    }
private:
    EventMap _events;
    bool _dataOneSet;
};

enum ConstrictionTotem
{
    NPC_CONSTRICTION_TOTEM  = 41202,
    QUEST_PUSHINGFORWARD    = 25792
};

class spell_burn_constriction_totem : public SpellScript
{
    PrepareSpellScript(spell_burn_constriction_totem);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (!GetHitUnit() || !GetCaster()->IsPlayer())
            return;

        GetCaster()->ToPlayer()->RewardPlayerAndGroupAtEvent(NPC_CONSTRICTION_TOTEM, GetCaster());
        GetHitUnit()->ToCreature()->DisappearAndDie();
    }

    void SelectTarget(WorldObject*& target)
    {
        target = GetCaster()->FindNearestCreature(NPC_CONSTRICTION_TOTEM, 5.0f, true);
    }

    void Register() override
    {
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_burn_constriction_totem::SelectTarget, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
        OnEffectHitTarget += SpellEffectFn(spell_burn_constriction_totem::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

enum FrostmaneBuilder
{
    QUEST_VIEWFROMDOWNHERE = 28868,
    SPELL_EQUALIZEVIEWPOINT = 93773,
    NPC_FROSTMANE_CREDIT = 50606
};

struct npc_frostmane_builder : public ScriptedAI
{
    npc_frostmane_builder(Creature* creature) : ScriptedAI(creature) {}

    void SpellHit(Unit* caster, const SpellInfo* spell) override
    {
        if (spell->Id != SPELL_EQUALIZEVIEWPOINT)
            return;

        Player* player = caster->ToPlayer();
        if (player && player->GetQuestStatus(QUEST_VIEWFROMDOWNHERE) == QUEST_STATUS_INCOMPLETE)
        {
            player->KilledMonsterCredit(NPC_FROSTMANE_CREDIT, ObjectGuid::Empty);
            player->CastSpell(me, SPELL_EQUALIZEVIEWPOINT);
        }
    }

};

enum StrikeFromAbove
{
    NPC_FROSTMANE_RETREAT_NORTH     = 41372,
    NPC_FROSTMANE_RETREAT_SOUTH     = 41373,
    NPC_COVERT_OPS_FLYING_MACHINE   = 41382,
    NPC_BOMBARDMENT_BUNNY           = 41375,
    SPELL_MISSILE_BARRAGE           = 77477,
    ACTION_FLYING_MACHINE_ATTACK    = 1,
    GOBJECT_FIRE                    = 182090
};

static const Position northernFlyingMachinePositions[2] =
{
    { -5705.887f, -997.9028f, 416.8214f },
    { -5699.643f, -995.9340f, 415.1655f }
};

static const Position northernFlyingMachineDestPositions[2] =
{
    { -5718.96f, -1026.789f, 405.8286f },
    { -5706.56f, -1015.849f, 410.3938f }
};

static const Position southernFlyingMachinePositions[2] =
{
    { -5777.16f, -959.2396f, 412.5151f },
    { -5777.124f, -956.1979f, 412.0958f }
};

static const Position southernFlyingMachineDestPositions[2] =
{
    { -5814.012f, -957.7242f, 404.7715f },
    { -5800.190f, -952.4794f, 407.7211f }
};

class spell_signal_flare : public SpellScript
{
    PrepareSpellScript(spell_signal_flare);

    void SelectTarget(WorldObject*& target)
    {
        target = GetCaster()->FindNearestCreature(NPC_FROSTMANE_RETREAT_NORTH, 25.0f, true);
        if (!target)
            target = GetCaster()->FindNearestCreature(NPC_FROSTMANE_RETREAT_SOUTH, 25.0f, true);
    }

    SpellCastResult CheckRequirement()
    {
        Creature* north = GetCaster()->FindNearestCreature(NPC_FROSTMANE_RETREAT_NORTH, 25.0f, true);
        Creature* south = GetCaster()->FindNearestCreature(NPC_FROSTMANE_RETREAT_SOUTH, 25.0f, true);

        if (!north && !south)
            return SPELL_FAILED_INCORRECT_AREA;
        return SPELL_CAST_OK;
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* hitUnit = GetHitUnit();
        if (!hitUnit || !GetCaster()->IsPlayer())
            return;

        if (Creature* target = hitUnit->ToCreature())
        {
            switch (target->GetEntry())
            {
                case NPC_FROSTMANE_RETREAT_NORTH:
                    FlyingMachineAttack(northernFlyingMachineDestPositions[0], GetCaster()->SummonCreature(NPC_COVERT_OPS_FLYING_MACHINE, northernFlyingMachinePositions[0]));
                    FlyingMachineAttack(northernFlyingMachineDestPositions[1], GetCaster()->SummonCreature(NPC_COVERT_OPS_FLYING_MACHINE, northernFlyingMachinePositions[1]));
                    GetCaster()->ToPlayer()->RewardPlayerAndGroupAtEvent(target->GetEntry(), GetCaster());
                    break;

                case NPC_FROSTMANE_RETREAT_SOUTH:
                    FlyingMachineAttack(southernFlyingMachineDestPositions[0], GetCaster()->SummonCreature(NPC_COVERT_OPS_FLYING_MACHINE, southernFlyingMachinePositions[0]));
                    FlyingMachineAttack(southernFlyingMachineDestPositions[1], GetCaster()->SummonCreature(NPC_COVERT_OPS_FLYING_MACHINE, southernFlyingMachinePositions[1]));
                    GetCaster()->ToPlayer()->RewardPlayerAndGroupAtEvent(target->GetEntry(), GetCaster());
                    break;
            }
        }
    }

    void FlyingMachineAttack(Position destination, TempSummon* summon)
    {
        if (!summon)
            return;

        summon->GetMotionMaster()->MovePoint(0, destination);
    }

    void Register() override
    {
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_signal_flare::SelectTarget, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
        OnEffectHitTarget += SpellEffectFn(spell_signal_flare::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        OnCheckCast += SpellCheckCastFn(spell_signal_flare::CheckRequirement);
    }
};

struct npc_flying_machine : public ScriptedAI
{
    npc_flying_machine(Creature* creature) : ScriptedAI(creature) {}

    enum Data
    {
        EVENT_ATTACK = 1
    };

    void Reset() override
    {
        events.ScheduleEvent(EVENT_ATTACK, 3500);
        me->DespawnOrUnsummon(6000);
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_ATTACK:
                DoCast(SPELL_MISSILE_BARRAGE);
                events.ScheduleEvent(EVENT_ATTACK, 1000);
                break;
            }
        }
    }

private:
    EventMap events;
};

class spell_missile_barrage : public SpellScript
{
    PrepareSpellScript(spell_missile_barrage);

    void SelectTarget(WorldObject*& target)
    {
        target = GetCaster()->FindNearestCreature(NPC_BOMBARDMENT_BUNNY, 30.0f, true);
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* hitUnit = GetHitUnit();
        if (!hitUnit)
            return;

        hitUnit->SummonGameObject(GOBJECT_FIRE, hitUnit->GetPosition(), QuaternionData(), 25);
    }

    void Register() override
    {
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_missile_barrage::SelectTarget, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
        OnEffectHitTarget += SpellEffectFn(spell_missile_barrage::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }

};

enum BurningBomber
{
    NPC_BURNING_BOMBER_CREDIT = 42046,
    QUEST_EXTINGUISH_FIRES = 26078
};

class spell_extinguish_fire : public SpellScript
{
    PrepareSpellScript(spell_extinguish_fire);

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        if (!GetHitUnit())
            return;
        GetCaster()->ToPlayer()->RewardPlayerAndGroupAtEvent(NPC_BURNING_BOMBER_CREDIT, GetCaster());
        GetHitUnit()->ToCreature()->DisappearAndDie();
    }

    void SelectTarget(WorldObject*& target)
    {
        target = GetCaster()->FindNearestCreature(NPC_BURNING_BOMBER_CREDIT, 9.0f, true);
    }

    void Register() override
    {
        OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_extinguish_fire::SelectTarget, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
        OnEffectHitTarget += SpellEffectFn(spell_extinguish_fire::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

enum StolenRam
{
    QUEST_RAMS_ON_THE_LAM = 25905,
    SPELL_ON_WAVEEMOTE_TO_STOLEN_RAM = 80464,
    NPC_RAMS_CREDIT = 43064,
    NPC_STOLEN_RAM = 41539
};

class spell_emote_stolen_ram : public SpellScript
{
    PrepareSpellScript(spell_emote_stolen_ram);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* hitUnit = GetHitUnit();
        if (!hitUnit)
            return;

        if (hitUnit->GetTypeId() != TYPEID_PLAYER)
            return;

        if (hitUnit->ToPlayer()->GetQuestStatus(QUEST_RAMS_ON_THE_LAM) != QUEST_STATUS_INCOMPLETE)
            return;

        GetCaster()->GetMotionMaster()->MovePoint(0, hitUnit->GetPosition());

        if (GetCaster()->ToCreature())
            GetCaster()->ToCreature()->DespawnOrUnsummon(3000);

        hitUnit->ToPlayer()->RewardPlayerAndGroupAtEvent(NPC_RAMS_CREDIT, hitUnit);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_emote_stolen_ram::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

struct npc_stolen_ram : public ScriptedAI
{
    npc_stolen_ram(Creature* creature) : ScriptedAI(creature) {}

    void ReceiveEmote(Player* player, uint32 emoteId) override
    {
        if (player->GetDistance(me) > 15.f)
            return;

        switch (emoteId)
        {
            case TEXT_EMOTE_WHISTLE:
            case TEXT_EMOTE_WAVE:
                DoCast(player, SPELL_ON_WAVEEMOTE_TO_STOLEN_RAM);
                break;
            default:
                break;
        }
    }
};

// Fix Start Quest https://www.wowhead.com/quest=25932
enum AmberstillRaid
{
    QUEST_ITS_RAID_NIGHT_EVERY_NIGHT = 25932,
    NPC_SERGEANT_FLINTHAMMER         = 41578,
    NPC_AMBERSTILL_MOUNTAINEER       = 41611,
    NPC_FROSTMANE_RAIDER             = 41603,
    NPC_AMBERSTILL_RAID_CONTROLLER   = 42169,
    SPELL_STEALTH                    = 34189,
    DISPLAY_RAM_MOUNT                = 10001,
    DATA_RAIDER_SLOT                 = 1,
    DATA_RAIDER_READY                = 2,
    ACTION_START_RAID                = 1,
    ACTION_RAIDER_ENGAGE             = 2,
    ACTION_BEGIN_PATH                = 3,
    ACTION_GO_LINE                   = 4,
    ACTION_RAIDER_FLEE               = 5,
    MAX_RAID_RAIDERS                 = 5,
    RAIDERS_TO_KILL                  = 4,
    PATH_TO_STABLE_SIZE              = 7,
    PATH_BACK_SIZE                   = 4,
    PATH_FLEE_SIZE                   = 3,
    // World spawn on the ranch house: corpse visual, full HP, must not join the fight.
    SPAWN_DEAD_AMBERSTILL_MOUNTAINEER = 169414
};

static constexpr float RAIDER_WALK_SPEED_MULT = 1.4f;
static constexpr float RAIDER_RUN_SPEED_MULT = 1.15f;
static constexpr float MOUNTAINEER_MELEE_DISTANCE = 2.5f;
static constexpr float FLEE_GROUND_STEP = 1.5f;

// Spawn and path points from in-game capture (not a sniff).
static const Position raiderSpawnPositions[MAX_RAID_RAIDERS] =
{
    { -5566.773438f, -1317.392456f, 405.538818f, 5.50f },
    { -5565.267578f, -1321.081055f, 406.747467f, 5.50f },
    { -5565.812500f, -1323.508667f, 407.559204f, 5.50f },
    { -5566.226562f, -1326.950439f, 407.263702f, 5.50f },
    { -5563.970904f, -1329.156372f, 405.676941f, 5.50f }
};

static const Position pathToStablePositions[PATH_TO_STABLE_SIZE] =
{
    { -5559.967773f, -1332.055786f, 401.811707f, 0.0f },
    { -5551.619141f, -1329.035522f, 398.782593f, 0.0f },
    { -5544.511719f, -1320.384888f, 398.688934f, 0.0f },
    { -5542.767090f, -1327.131958f, 398.663910f, 0.0f },
    { -5535.750488f, -1331.332275f, 398.663910f, 0.0f },
    { -5520.212891f, -1330.484985f, 398.756592f, 0.0f },
    { -5504.780273f, -1328.047607f, 397.516510f, 0.0f }
};

static const Position pathBackFromStablePositions[PATH_BACK_SIZE] =
{
    { -5519.191406f, -1330.363159f, 398.592590f, 0.0f },
    { -5534.751465f, -1330.661011f, 398.663879f, 0.0f },
    { -5542.462402f, -1325.956543f, 398.663879f, 0.0f },
    { -5547.369141f, -1310.798340f, 398.098175f, 0.0f }
};

static const Position raiderLinePositions[MAX_RAID_RAIDERS] =
{
    { -5550.504395f, -1293.393799f, 400.405334f, 5.50f },
    { -5554.273438f, -1298.032837f, 398.850342f, 5.50f },
    { -5557.594727f, -1302.698975f, 398.675537f, 5.50f },
    { -5560.039551f, -1305.670166f, 398.665283f, 5.50f },
    { -5562.562012f, -1309.101562f, 399.225311f, 5.50f }
};

static const Position raiderFleePositions[PATH_FLEE_SIZE] =
{
    { -5565.224609f, -1266.871094f, 400.927765f, 0.0f },
    { -5573.417969f, -1272.611572f, 401.576050f, 0.0f },
    { -5587.217285f, -1264.947021f, 399.643646f, 0.0f }
};

static Position OppositeLinePos(uint32 slot, float distance)
{
    Position const& line = raiderLinePositions[slot];
    float ang = line.GetOrientation();
    return Position(line.GetPositionX() + std::cos(ang) * distance,
        line.GetPositionY() + std::sin(ang) * distance,
        line.GetPositionZ(), ang + static_cast<float>(M_PI));
}

static Position GroundPos(WorldObject const* obj, Position pos)
{
    float x = pos.GetPositionX();
    float y = pos.GetPositionY();
    float z = pos.GetPositionZ();
    float const capturedZ = z;
    // Keep captured GPS height: UpdateAllowedPositionZ / a wide GetHeight search
    // can pick an undercroft under the ranch snow and sink the model.
    float const ground = obj->GetMap()->GetHeight(obj->GetPhaseShift(), x, y, capturedZ + 2.0f, true, 4.0f);
    if (ground > INVALID_HEIGHT && ground >= capturedZ - 0.15f && ground <= capturedZ + 2.0f)
        z = ground;
    else
        z = capturedZ;
    pos.Relocate(x, y, z, pos.GetOrientation());
    return pos;
}

// Search from above the GPS corridor so a snowbank/WMO surface is found instead
// of the walkable mmap poly that cuts through it.
static float SurfaceZ(WorldObject const* obj, float x, float y, float hintZ)
{
    float const ground = obj->GetMap()->GetHeight(obj->GetPhaseShift(), x, y, hintZ + 8.0f, true, 12.0f);
    if (ground > INVALID_HEIGHT && ground >= hintZ - 1.0f)
        return ground;
    return hintZ;
}

static void AppendGroundedSegment(WorldObject const* obj, Position const& from, Position const& to, std::vector<Position>& out)
{
    float const dx = to.GetPositionX() - from.GetPositionX();
    float const dy = to.GetPositionY() - from.GetPositionY();
    float const dz = to.GetPositionZ() - from.GetPositionZ();
    float const dist = std::sqrt(dx * dx + dy * dy);
    uint32 steps = uint32(std::ceil(dist / FLEE_GROUND_STEP));
    if (!steps)
        steps = 1;
    for (uint32 i = 1; i <= steps; ++i)
    {
        float const t = float(i) / float(steps);
        float const x = from.GetPositionX() + dx * t;
        float const y = from.GetPositionY() + dy * t;
        float const hint = from.GetPositionZ() + dz * t;
        out.emplace_back(x, y, SurfaceZ(obj, x, y, hint));
    }
}

struct npc_frostmane_raider : public ScriptedAI
{
    npc_frostmane_raider(Creature* creature) : ScriptedAI(creature), _slot(0), _fleeing(false) { }

    enum Data
    {
        EVENT_START_PATH = 1,
        POINT_TO_STABLE  = 100,
        POINT_BACK       = 200,
        POINT_FINAL      = 300,
        POINT_FLEE       = 400
    };

    void SetData(uint32 type, uint32 data) override
    {
        if (type != DATA_RAIDER_SLOT)
            return;

        _slot = data < MAX_RAID_RAIDERS ? data : 0;
    }

    void SetGUID(ObjectGuid guid, int32 /*id*/) override
    {
        _attackTarget = guid;
    }

    void IsSummonedBy(Unit* /*summoner*/) override
    {
        me->SetReactState(REACT_PASSIVE);
        me->AddUnitFlag(UnitFlags(UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NON_ATTACKABLE));
        me->SetWalk(true);
        DoCastSelf(SPELL_STEALTH, true);
    }

    void DoAction(int32 action) override
    {
        switch (action)
        {
            case ACTION_BEGIN_PATH:
                events.ScheduleEvent(EVENT_START_PATH, 1);
                break;
            case ACTION_GO_LINE:
                if (!_fleeing)
                    GoToLine();
                break;
            case ACTION_RAIDER_ENGAGE:
                if (_fleeing)
                    break;
                ClearRaiderStealth();
                me->RemoveUnitFlag(UnitFlags(UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NON_ATTACKABLE));
                me->SetReactState(REACT_AGGRESSIVE);
                if (Creature* target = ObjectAccessor::GetCreature(*me, _attackTarget))
                    AttackStart(target);
                break;
            case ACTION_RAIDER_FLEE:
                BeginFlee();
                break;
            default:
                break;
        }
    }

    void AttackStart(Unit* who) override
    {
        if (!who || _fleeing)
            return;

        // Hold the ram line vs the paired mountaineer; chase a player who takes aggro.
        SetCombatMovement(who->GetGUID() != _attackTarget);
        ScriptedAI::AttackStart(who);
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage) override
    {
        if (!_fleeing)
            return;

        damage = 0;
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != EFFECT_MOTION_TYPE && type != POINT_MOTION_TYPE)
            return;

        if (_fleeing)
        {
            if (id == POINT_FLEE)
                me->DespawnOrUnsummon();
            return;
        }

        if (id == POINT_TO_STABLE)
        {
            StealRamAndReturn();
            return;
        }

        if (id == POINT_BACK)
        {
            GoToLine();
            return;
        }

        if (id == POINT_FINAL)
            ReportReady();
    }

    void UpdateAI(uint32 diff) override
    {
        events.Update(diff);
        while (uint32 eventId = events.ExecuteEvent())
        {
            if (eventId == EVENT_START_PATH && !_fleeing)
                StartSpline(POINT_TO_STABLE, pathToStablePositions, PATH_TO_STABLE_SIZE, true);
        }

        if (_fleeing || !UpdateVictim())
            return;

        UpdateRaiderChase();
        DoMeleeAttackIfReady();
    }

private:
    void UpdateRaiderChase()
    {
        Unit* victim = me->GetVictim();
        if (!victim)
            return;

        bool const chase = victim->GetGUID() != _attackTarget;
        bool const alreadyChasing = me->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE;
        if (chase == IsCombatMovementAllowed() && chase == alreadyChasing)
            return;

        SetCombatMovement(chase);
        if (chase)
            me->GetMotionMaster()->MoveChase(victim);
        else
            me->GetMotionMaster()->MoveIdle();
    }

    void StartSpline(uint32 pointId, Position const* points, uint32 count, bool walk)
    {
        Position path[1 + PATH_TO_STABLE_SIZE];
        path[0] = me->GetPosition();
        for (uint32 i = 0; i < count; ++i)
            path[i + 1] = GroundPos(me, points[i]);

        me->SetWalk(walk);
        float const speed = me->GetSpeed(walk ? MOVE_WALK : MOVE_RUN) * (walk ? RAIDER_WALK_SPEED_MULT : RAIDER_RUN_SPEED_MULT);
        me->GetMotionMaster()->MoveSmoothPath(pointId, path, count + 1, walk, false, speed);
    }

    void ClearRaiderStealth()
    {
        // Spell 34189 sets UNIT_VIS_FLAGS_CREEP. If the aura-remove ran while the
        // unit was out of client visibility (stables), the creep flag can stick
        // and the last man in the ram line still looks stealthed.
        me->RemoveAurasDueToSpell(SPELL_STEALTH);
        me->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
        me->RemoveVisFlags(UNIT_VIS_FLAGS_CREEP);
        me->SetVisible(true);
        me->UpdateObjectVisibility();
    }

    void StealRamAndReturn()
    {
        ClearRaiderStealth();
        me->Mount(DISPLAY_RAM_MOUNT);
        ClearRaiderStealth();
        me->SetWalk(false);
        StartSpline(POINT_BACK, pathBackFromStablePositions, PATH_BACK_SIZE, false);
    }

    void GoToLine()
    {
        ClearRaiderStealth();
        Position line = GroundPos(me, raiderLinePositions[_slot]);
        StartSpline(POINT_FINAL, &line, 1, false);
    }

    void ReportReady()
    {
        Position line = GroundPos(me, raiderLinePositions[_slot]);
        ClearRaiderStealth();
        me->StopMoving();
        me->GetMotionMaster()->MoveIdle();
        me->SetHomePosition(line);
        me->Relocate(line);
        me->SetFacingTo(line.GetOrientation());
        SetCombatMovement(false);
        if (Creature* sergeant = me->FindNearestCreature(NPC_SERGEANT_FLINTHAMMER, 80.0f))
            sergeant->AI()->SetData(DATA_RAIDER_READY, _slot);
    }

    void BeginFlee()
    {
        if (_fleeing)
            return;

        _fleeing = true;
        me->AttackStop();
        me->CombatStop(true);
        me->SetReactState(REACT_PASSIVE);
        SetCombatMovement(false);
        me->AddUnitFlag(UnitFlags(UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NON_ATTACKABLE));
        me->SetWalk(false);
        ClearRaiderStealth();
        me->ClearUnitState(UNIT_STATE_IGNORE_PATHFINDING);
        me->GetMotionMaster()->Clear();

        // Mmaps here are one walkable poly through the ranch snow; PathGenerator
        // returns a 2-point shortcut and MoveSmoothPath compresses it, so the
        // model cuts the drift. Follow the captured GPS in short steps, snap Z
        // to the visual surface, send an uncompressed linear spline.
        std::vector<Position> path;
        path.push_back(me->GetPosition());
        Position from = me->GetPosition();
        for (uint32 i = 0; i < PATH_FLEE_SIZE; ++i)
        {
            AppendGroundedSegment(me, from, raiderFleePositions[i], path);
            from = raiderFleePositions[i];
        }

        float const speed = me->GetSpeed(MOVE_RUN) * RAIDER_RUN_SPEED_MULT;
        me->GetMotionMaster()->MoveSmoothPath(POINT_FLEE, path.data(), path.size(), false, false, speed);
        me->DespawnOrUnsummon(15s);
    }

    uint32 _slot;
    bool _fleeing;
    ObjectGuid _attackTarget;
};

struct npc_sergeant_flinthammer : public ScriptedAI
{
    npc_sergeant_flinthammer(Creature* creature) : ScriptedAI(creature), _eventActive(false), _nextRaiderSlot(0), _raidersDead(0), _raidersReady(0), _credited(false) { }

    enum Data
    {
        EVENT_SAY_0 = 1,
        EVENT_SAY_1,
        EVENT_SAY_2,
        EVENT_SAY_3,
        EVENT_SAY_4,
        EVENT_SAY_5,
        EVENT_SPAWN_RAIDERS,
        EVENT_RAIDERS_MOVE,
        EVENT_ALARM,
        EVENT_TAUNT,
        EVENT_GATHER,
        EVENT_ENGAGE,
        EVENT_RESTORE_DEFENDERS,
        POINT_HOME = 399,
        POINT_GATHER = 400
    };

    void InitializeAI() override
    {
        _homePos = me->GetHomePosition();
        me->SetReactState(REACT_PASSIVE);
        SetCombatMovement(false);
        ScriptedAI::InitializeAI();
    }

    void sQuestAccept(Player* /*player*/, Quest const* quest) override
    {
        if (quest->GetQuestId() != QUEST_ITS_RAID_NIGHT_EVERY_NIGHT)
            return;

        DoAction(ACTION_START_RAID);
    }

    void Reset() override
    {
        if (_eventActive)
            return;

        events.Reset();
        summons.DespawnAll();
        _nextRaiderSlot = 0;
        _raidersDead = 0;
        _raidersReady = 0;
        _credited = false;
        HoldHome();
    }

    void EnterEvadeMode(EvadeReason /*why*/) override
    {
        me->CombatStop(true);
        HoldHome();
    }

    void JustSummoned(Creature* summon) override
    {
        summons.Summon(summon);
        if (summon->GetEntry() != NPC_FROSTMANE_RAIDER)
            return;

        uint32 slot = _nextRaiderSlot++;
        if (slot >= MAX_RAID_RAIDERS)
            return;

        summon->AI()->SetData(DATA_RAIDER_SLOT, slot);
    }

    void SetData(uint32 type, uint32 /*data*/) override
    {
        if (type != DATA_RAIDER_READY || !_eventActive)
            return;

        ++_raidersReady;
        if (_raidersReady >= MAX_RAID_RAIDERS)
        {
            events.CancelEvent(EVENT_GATHER);
            events.CancelEvent(EVENT_ALARM);
            events.ScheduleEvent(EVENT_ALARM, 500ms);
        }
    }

    void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) override
    {
        if (summon->GetEntry() != NPC_FROSTMANE_RAIDER)
            return;

        ++_raidersDead;
        if (_raidersDead < RAIDERS_TO_KILL)
        {
            AssistRemainingRaiders();
            return;
        }

        FinishRanchFight();
    }

    void SummonedCreatureDespawn(Creature* summon) override
    {
        summons.Despawn(summon);
        if (!_eventActive || summon->GetEntry() != NPC_FROSTMANE_RAIDER)
            return;

        if (_credited)
            return;

        if (CountLivingRaiders())
            return;

        EndEvent();
    }

    void DoAction(int32 action) override
    {
        if (action != ACTION_START_RAID)
            return;

        if (_eventActive)
            return;

        _eventActive = true;
        _nextRaiderSlot = 0;
        _raidersDead = 0;
        _raidersReady = 0;
        _credited = false;
        RestoreDefenders();
        summons.DespawnAll();
        events.Reset();
        me->SetReactState(REACT_PASSIVE);
        SetCombatMovement(false);
        me->CombatStop(true);
        HoldHome();
        events.ScheduleEvent(EVENT_SAY_0, 2s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!_eventActive)
            return;

        events.Update(diff);
        while (uint32 eventId = events.ExecuteEvent())
        {
            Creature* mountaineer = FindTalkMountaineer();
            switch (eventId)
            {
                case EVENT_SAY_0:
                    Talk(0);
                    events.ScheduleEvent(EVENT_SAY_1, 3500ms);
                    break;
                case EVENT_SAY_1:
                    if (mountaineer && mountaineer->IsAIEnabled)
                        mountaineer->AI()->Talk(0);
                    events.ScheduleEvent(EVENT_SAY_2, 4500ms);
                    break;
                case EVENT_SAY_2:
                    Talk(1);
                    events.ScheduleEvent(EVENT_SAY_3, 5500ms);
                    break;
                case EVENT_SAY_3:
                    Talk(2);
                    events.ScheduleEvent(EVENT_SAY_4, 6500ms);
                    break;
                case EVENT_SAY_4:
                    if (mountaineer && mountaineer->IsAIEnabled)
                        mountaineer->AI()->Talk(1);
                    events.ScheduleEvent(EVENT_SAY_5, 7500ms);
                    break;
                case EVENT_SAY_5:
                    Talk(3);
                    events.ScheduleEvent(EVENT_SPAWN_RAIDERS, 2s);
                    break;
                case EVENT_SPAWN_RAIDERS:
                    for (uint8 i = 0; i < MAX_RAID_RAIDERS; ++i)
                        me->SummonCreature(NPC_FROSTMANE_RAIDER, GroundPos(me, raiderSpawnPositions[i]), TEMPSUMMON_TIMED_DESPAWN, 180000);
                    events.ScheduleEvent(EVENT_RAIDERS_MOVE, 2s);
                    events.ScheduleEvent(EVENT_GATHER, 120s);
                    break;
                case EVENT_RAIDERS_MOVE:
                    for (ObjectGuid guid : summons)
                    {
                        Creature* raider = ObjectAccessor::GetCreature(*me, guid);
                        if (!raider || !raider->IsAlive() || raider->GetEntry() != NPC_FROSTMANE_RAIDER)
                            continue;
                        raider->AI()->DoAction(ACTION_BEGIN_PATH);
                    }
                    break;
                case EVENT_ALARM:
                    if (mountaineer && mountaineer->IsAIEnabled)
                        mountaineer->AI()->Talk(2);
                    events.ScheduleEvent(EVENT_TAUNT, 2s);
                    break;
                case EVENT_TAUNT:
                    if (mountaineer && mountaineer->IsAIEnabled)
                        mountaineer->AI()->Talk(3);
                    if (Creature* raider = me->FindNearestCreature(NPC_FROSTMANE_RAIDER, 60.0f, true))
                        if (raider->IsAIEnabled)
                            raider->AI()->Talk(0);
                    GatherMountaineers();
                    events.ScheduleEvent(EVENT_ENGAGE, 8s);
                    break;
                case EVENT_GATHER:
                    for (ObjectGuid guid : summons)
                    {
                        Creature* raider = ObjectAccessor::GetCreature(*me, guid);
                        if (!raider || !raider->IsAlive() || raider->GetEntry() != NPC_FROSTMANE_RAIDER)
                            continue;
                        raider->AI()->DoAction(ACTION_GO_LINE);
                    }
                    events.ScheduleEvent(EVENT_ALARM, 8s);
                    break;
                case EVENT_ENGAGE:
                    StartRanchFight();
                    break;
                case EVENT_RESTORE_DEFENDERS:
                    RestoreDefenders();
                    me->CombatStop(true);
                    HoldHome();
                    _eventActive = false;
                    break;
                default:
                    break;
            }
        }
    }

private:
    void GatherMountaineers()
    {
        RestoreDefenders();
        std::vector<Creature*> defenders;
        CollectWorldMountaineers(defenders);
        std::sort(defenders.begin(), defenders.end(), [this](Creature const* a, Creature const* b)
        {
            return me->GetExactDist(a) < me->GetExactDist(b);
        });
        if (defenders.size() > MAX_RAID_RAIDERS)
            defenders.resize(MAX_RAID_RAIDERS);

        for (uint8 i = 0; i < defenders.size(); ++i)
        {
            Position const gather = GroundPos(defenders[i], OppositeLinePos(i, MOUNTAINEER_MELEE_DISTANCE));
            DefenderState state;
            state.guid = defenders[i]->GetGUID();
            state.home = defenders[i]->GetHomePosition();
            state.gather = gather;
            state.movementType = defenders[i]->GetDefaultMovementType();
            state.wander = defenders[i]->GetRespawnRadius();
            _defenders.push_back(state);
            defenders[i]->SetDefaultMovementType(IDLE_MOTION_TYPE);
            defenders[i]->SetRespawnRadius(0.0f);
            defenders[i]->SetWalk(false);
            defenders[i]->SetHomePosition(gather);
            defenders[i]->SetReactState(REACT_PASSIVE);
            defenders[i]->SetStandState(UNIT_STAND_STATE_STAND);
            defenders[i]->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);
            if (SmartAI* ai = dynamic_cast<SmartAI*>(defenders[i]->AI()))
            {
                ai->SetEvadeDisabled(true);
                ai->SetCombatMove(false);
            }
            defenders[i]->GetMotionMaster()->Clear();
            Position path[2];
            path[0] = defenders[i]->GetPosition();
            path[1] = gather;
            defenders[i]->GetMotionMaster()->MoveSmoothPath(POINT_GATHER, path, 2, false, false, defenders[i]->GetSpeed(MOVE_RUN));
        }
    }

    void StartRanchFight()
    {
        uint8 i = 0;
        for (ObjectGuid guid : summons)
        {
            Creature* raider = ObjectAccessor::GetCreature(*me, guid);
            if (!raider || !raider->IsAlive() || raider->GetEntry() != NPC_FROSTMANE_RAIDER)
                continue;

            DefenderState const* state = i < _defenders.size() ? &_defenders[i] : nullptr;
            Creature* defender = state ? ObjectAccessor::GetCreature(*me, state->guid) : nullptr;
            uint8 const slot = i;
            ++i;
            if (!defender || !defender->IsAlive() || !state)
                continue;

            defender->GetMotionMaster()->Clear();
            defender->Relocate(state->gather);
            defender->SetFacingTo(state->gather.GetOrientation());
            defender->SetHomePosition(state->gather);
            defender->SetStandState(UNIT_STAND_STATE_STAND);
            defender->ClearUnitState(UNIT_STATE_IGNORE_PATHFINDING);
            if (SmartAI* ai = dynamic_cast<SmartAI*>(defender->AI()))
            {
                ai->SetEvadeDisabled(true);
                ai->SetCombatMove(true);
            }
            defender->SetReactState(REACT_AGGRESSIVE);
            raider->Relocate(GroundPos(raider, raiderLinePositions[slot]));
            raider->SetFacingTo(raiderLinePositions[slot].GetOrientation());
            raider->AI()->SetGUID(defender->GetGUID());
            raider->AI()->DoAction(ACTION_RAIDER_ENGAGE);
            defender->AI()->AttackStart(raider);
        }
        LinkDefenderThreat();
    }

    void LinkDefenderThreat()
    {
        std::vector<Creature*> raiders;
        for (ObjectGuid guid : summons)
        {
            Creature* raider = ObjectAccessor::GetCreature(*me, guid);
            if (raider && raider->IsAlive() && raider->GetEntry() == NPC_FROSTMANE_RAIDER)
                raiders.push_back(raider);
        }

        for (DefenderState const& state : _defenders)
        {
            Creature* defender = ObjectAccessor::GetCreature(*me, state.guid);
            if (!defender || !defender->IsAlive())
                continue;

            for (Creature* raider : raiders)
            {
                defender->SetInCombatWith(raider);
                raider->SetInCombatWith(defender);
                defender->AddThreat(raider, 1.0f);
            }
        }
    }

    void AssistRemainingRaiders()
    {
        std::vector<Creature*> raiders;
        for (ObjectGuid guid : summons)
        {
            Creature* raider = ObjectAccessor::GetCreature(*me, guid);
            if (raider && raider->IsAlive() && raider->GetEntry() == NPC_FROSTMANE_RAIDER)
                raiders.push_back(raider);
        }
        if (raiders.empty())
            return;

        for (DefenderState const& state : _defenders)
        {
            Creature* defender = ObjectAccessor::GetCreature(*me, state.guid);
            if (!defender || !defender->IsAlive())
                continue;
            if (defender->GetVictim() && defender->GetVictim()->IsAlive())
                continue;

            Creature* target = raiders.front();
            float best = defender->GetExactDist(target);
            for (Creature* raider : raiders)
            {
                float const dist = defender->GetExactDist(raider);
                if (dist < best)
                {
                    best = dist;
                    target = raider;
                }
            }
            defender->AI()->AttackStart(target);
        }
    }

    bool IsUsableRanchMountaineer(Creature const* candidate) const
    {
        if (!candidate || !candidate->IsAlive() || candidate->IsSummon())
            return false;
        if (candidate->GetSpawnId() == SPAWN_DEAD_AMBERSTILL_MOUNTAINEER)
            return false;
        if (candidate->GetStandState() != UNIT_STAND_STATE_STAND)
            return false;
        if (candidate->HasUnitFlag2(UNIT_FLAG2_FEIGN_DEATH) || candidate->HasDynamicFlag(UNIT_DYNFLAG_DEAD))
            return false;
        if (candidate->HasUnitFlag(UNIT_FLAG_NOT_SELECTABLE) || candidate->HasUnitFlag(UNIT_FLAG_IMMUNE_TO_NPC))
            return false;
        if (std::fabs(candidate->GetPositionZ() - me->GetPositionZ()) > 4.0f)
            return false;
        return true;
    }

    Creature* FindTalkMountaineer()
    {
        std::vector<Creature*> defenders;
        CollectWorldMountaineers(defenders);
        std::sort(defenders.begin(), defenders.end(), [this](Creature const* a, Creature const* b)
        {
            return me->GetExactDist(a) < me->GetExactDist(b);
        });
        return defenders.empty() ? nullptr : defenders.front();
    }

    void CollectWorldMountaineers(std::vector<Creature*>& defenders)
    {
        me->GetCreatureListWithEntryInGrid(defenders, NPC_AMBERSTILL_MOUNTAINEER, 80.0f);
        defenders.erase(std::remove_if(defenders.begin(), defenders.end(), [this](Creature* c)
        {
            return !IsUsableRanchMountaineer(c);
        }), defenders.end());
    }

    void FinishRanchFight()
    {
        if (_credited)
            return;

        _credited = true;
        for (ObjectGuid guid : summons)
        {
            Creature* raider = ObjectAccessor::GetCreature(*me, guid);
            if (!raider || !raider->IsAlive() || raider->GetEntry() != NPC_FROSTMANE_RAIDER)
                continue;

            raider->AI()->DoAction(ACTION_RAIDER_FLEE);
        }

        CreditNearbyPlayers();
        events.Reset();
        events.ScheduleEvent(EVENT_RESTORE_DEFENDERS, 6s);
    }

    void CreditNearbyPlayers()
    {
        std::list<Player*> players;
        me->GetPlayerListInGrid(players, 80.0f);
        for (Player* player : players)
        {
            if (player->GetQuestStatus(QUEST_ITS_RAID_NIGHT_EVERY_NIGHT) != QUEST_STATUS_INCOMPLETE)
                continue;

            player->KilledMonsterCredit(NPC_AMBERSTILL_RAID_CONTROLLER);
        }
    }

    void EndEvent()
    {
        _eventActive = false;
        events.Reset();
        RestoreDefenders();
        me->CombatStop(true);
        HoldHome();
    }

    void HoldHome()
    {
        me->SetReactState(REACT_PASSIVE);
        SetCombatMovement(false);
        if (!_homePos.GetPositionX() && !_homePos.GetPositionY())
            _homePos = me->GetHomePosition();
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MovePoint(POINT_HOME, _homePos, false);
        me->SetFacingTo(_homePos.GetOrientation());
    }

    void RestoreDefenders()
    {
        for (DefenderState const& state : _defenders)
        {
            Creature* defender = ObjectAccessor::GetCreature(*me, state.guid);
            if (!defender || !defender->IsAlive())
                continue;

            defender->CombatStop(true);
            defender->SetControlled(false, UNIT_STATE_ROOT);
            defender->ClearUnitState(UNIT_STATE_IGNORE_PATHFINDING);
            if (SmartAI* ai = dynamic_cast<SmartAI*>(defender->AI()))
            {
                ai->SetEvadeDisabled(false);
                ai->SetCombatMove(true);
            }
            defender->SetReactState(REACT_AGGRESSIVE);
            defender->SetDefaultMovementType(state.movementType);
            defender->SetRespawnRadius(state.wander);
            defender->SetHomePosition(state.home);
            defender->GetMotionMaster()->Clear();
            Position path[2];
            path[0] = defender->GetPosition();
            path[1] = GroundPos(defender, state.home);
            defender->GetMotionMaster()->MoveSmoothPath(0, path, 2, false, false, defender->GetSpeed(MOVE_RUN));
        }
        _defenders.clear();
    }

    uint32 CountLivingRaiders() const
    {
        uint32 alive = 0;
        for (ObjectGuid guid : summons)
        {
            Creature* raider = ObjectAccessor::GetCreature(*me, guid);
            if (raider && raider->IsAlive() && raider->GetEntry() == NPC_FROSTMANE_RAIDER)
                ++alive;
        }
        return alive;
    }

    bool _eventActive;
    uint32 _nextRaiderSlot;
    uint32 _raidersDead;
    uint32 _raidersReady;
    bool _credited;
    Position _homePos;
    struct DefenderState
    {
        ObjectGuid guid;
        Position home;
        Position gather;
        MovementGeneratorType movementType;
        float wander;
    };
    std::vector<DefenderState> _defenders;
};

// Fix End Quest https://www.wowhead.com/quest=25932

// Fix DailyQuest https://www.wowhead.com/quest=29356/
struct at_i_need_to_cask_a_favor : public AreaTriggerAI
{
    at_i_need_to_cask_a_favor(AreaTrigger* at) : AreaTriggerAI(at) { }

    void OnUnitEnter(Unit* unit) override
    {
        Player* player = unit->ToPlayer();

        if (!player)
            return;

        if (player->GetQuestStatus(29356) == QUEST_STATUS_INCOMPLETE)
        {
            if (player->HasAura(99491))
            {
                player->ForceCompleteQuest(29356);
            }
        }
    }
};

void AddSC_dun_morogh()
{
    RegisterCreatureAI(npc_frozen_mountaineer);
    RegisterSpellScript(spell_burn_constriction_totem);
    RegisterCreatureAI(npc_frostmane_builder);
    RegisterSpellScript(spell_signal_flare);
    RegisterCreatureAI(npc_flying_machine);
    RegisterSpellScript(spell_missile_barrage);
    RegisterSpellScript(spell_extinguish_fire);
    RegisterSpellScript(spell_emote_stolen_ram);
    RegisterCreatureAI(npc_stolen_ram);
    RegisterCreatureAI(npc_frostmane_raider);
    RegisterCreatureAI(npc_sergeant_flinthammer);
    RegisterAreaTriggerAI(at_i_need_to_cask_a_favor);
}
