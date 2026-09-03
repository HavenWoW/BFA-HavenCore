-- ============================================================
-- Deadmines - Consolidated World Database Fixes
-- HavenCore BFA 8.3.7
-- Map: 36
--
-- Consolidates the Deadmines SQL fixes from:
--   2023_09_03_01_DM_Creatures_Clipping_Fix.sql
--   2026_09_02_00_DM_Fix_Heroic_Loot_Mix.sql
--   2026_09_02_01_DM_Fix Ripsnarl_Quest_Offer_Issue.sql
--   2026_09_02_02_DM_Goblin_Teleport_Fix.sql
--   2026_09_02_03_DM_Defias_Overseer_Fix.sql
--   2026_09_03_00_DM_Drink Bunny Fixes.sql
--
-- Also includes the verified Ripsnarl fog-trigger visibility fix
-- from the same Deadmines repair pass.
-- ============================================================

START TRANSACTION;

-- ============================================================
-- 1. Ambient Rat clipping fix
-- ============================================================
UPDATE `creature`
SET `spawndist` = 3
WHERE `map` = 36
  AND `id` = 4075
  AND `guid` IN
  (
      318999,
      319000,
      319001,
      319002,
      319003,
      319004,
      319005,
      319378
  );


-- ============================================================
-- 2. Separate Heroic boss loot from Normal loot
-- ============================================================
UPDATE `creature_loot_template`
SET `LootMode` = 2
WHERE `Entry` = 47162
  AND `Item` IN (63467,63468,63470,63471,65163);

UPDATE `creature_loot_template`
SET `LootMode` = 2
WHERE `Entry` = 47296
  AND `Item` IN (63473,63474,63475,63476,65164);

UPDATE `creature_loot_template`
SET `LootMode` = 2
WHERE `Entry` = 43778
  AND `Item` IN (65157,65165,65166);

UPDATE `creature_loot_template`
SET `LootMode` = 2
WHERE `Entry` = 47626
  AND `Item` IN (65168,65169,65170);

UPDATE `creature_loot_template`
SET `LootMode` = 2
WHERE `Entry` = 47739
  AND `Item` IN (65171,65172,65173,65174,65177);


-- ============================================================
-- 3. Admiral Ripsnarl quest-offer cleanup
--    Ripsnarl must not directly offer these dungeon quests.
-- ============================================================
DELETE FROM `creature_queststarter`
WHERE `id` = 47626
  AND `quest` IN (27785,27848);


-- ============================================================
-- 4. Vanessa note is Heroic-only
-- ============================================================
UPDATE `creature`
SET `spawnDifficulties` = '2'
WHERE `guid` = 326853
  AND `id` = 49564;


-- ============================================================
-- 5. Goblin teleport GameObject script
-- ============================================================
UPDATE `gameobject_template`
SET `ScriptName` = 'go_deadmines_tp'
WHERE `entry` = 208002;


-- ============================================================
-- 6. Defias Overseer dialogue / AI registration
-- ============================================================
DELETE FROM `creature_text`
WHERE `CreatureID` = 48421;

INSERT INTO `creature_text`
(
    `CreatureID`,
    `GroupID`,
    `ID`,
    `Text`,
    `Type`,
    `Language`,
    `Probability`,
    `Emote`,
    `Duration`,
    `Sound`,
    `BroadcastTextId`,
    `TextRange`,
    `comment`
)
VALUES
(48421, 0, 0, 'It''s broken.', 12, 0, 100, 0, 0, 0, 0, 0, 'Defias Overseer - pipe dialogue 1'),
(48421, 1, 0, 'It''s not broken!', 12, 0, 100, 0, 0, 0, 0, 0, 'Defias Overseer - pipe dialogue 2'),
(48421, 2, 0, 'Why''s it shooting steam out of the side there, then?', 12, 0, 100, 0, 0, 0, 0, 0, 'Defias Overseer - pipe dialogue 3'),
(48421, 3, 0, 'That''s the ... pressure release valve.', 12, 0, 100, 0, 0, 0, 0, 0, 'Defias Overseer - pipe dialogue 4'),
(48421, 4, 0, 'In the middle of the pipe?', 12, 0, 100, 0, 0, 0, 0, 0, 'Defias Overseer - pipe dialogue 5'),
(48421, 5, 0, 'Er ... backup release valve?', 12, 0, 100, 0, 0, 0, 0, 0, 'Defias Overseer - pipe dialogue 6'),
(48421, 6, 0, 'We should tell the Admiral.', 12, 0, 100, 0, 0, 0, 0, 0, 'Defias Overseer - pipe dialogue 7'),
(48421, 7, 0, 'You tell him.', 12, 0, 100, 0, 0, 0, 0, 0, 'Defias Overseer - pipe dialogue 8'),
(48421, 8, 0, 'No way! He gives me the creeps!', 12, 0, 100, 0, 0, 0, 0, 0, 'Defias Overseer - pipe dialogue 9'),
(48421, 9, 0, 'I know! The way he looks at you with those hungry eyes. I''m afraid to even turn my back!', 12, 0, 100, 0, 0, 0, 0, 0, 'Defias Overseer - pipe dialogue 10');

UPDATE `creature_template`
SET `ScriptName` = 'npc_defias_overseer'
WHERE `entry` = 48421;


-- ============================================================
-- 7. Mine Bunny combat AI - Drunken Haze
-- ============================================================
UPDATE `creature_template`
SET `AIName` = 'SmartAI'
WHERE `entry` IN (48338, 48351);

DELETE FROM `smart_scripts`
WHERE `source_type` = 0
  AND `entryorguid` IN (48338, 48351);

INSERT INTO `smart_scripts`
(
    `entryorguid`,
    `source_type`,
    `id`,
    `link`,
    `event_type`,
    `event_phase_mask`,
    `event_chance`,
    `event_flags`,
    `event_param1`,
    `event_param2`,
    `event_param3`,
    `event_param4`,
    `event_param5`,
    `event_param_string`,
    `action_type`,
    `action_param1`,
    `action_param2`,
    `action_param3`,
    `action_param4`,
    `action_param5`,
    `action_param6`,
    `target_type`,
    `target_param1`,
    `target_param2`,
    `target_param3`,
    `target_x`,
    `target_y`,
    `target_z`,
    `target_o`,
    `comment`
)
VALUES
(48338,0,0,0,0,0,100,0,2000,4000,7000,10000,0,'',
 11,91032,0,0,0,0,0,
 2,0,0,0,0,0,0,0,
 'Mine Bunny - Cast Drunken Haze'),

(48351,0,0,0,0,0,100,0,2000,4000,7000,10000,0,'',
 11,91032,0,0,0,0,0,
 2,0,0,0,0,0,0,0,
 'Mine Bunny - Cast Drunken Haze');


-- ============================================================
-- 8. Mine Bunny patrols
--    48338 spawn 319274 = Bunny 1
--    48351 spawn 319125 = Bunny 2
-- ============================================================
UPDATE `creature`
SET `MovementType` = 2,
    `spawndist` = 0
WHERE `guid` IN (319274, 319125);

-- Bunny 2: correct starting height
UPDATE `creature`
SET `position_x` = -217.922409,
    `position_y` = -495.439697,
    `position_z` = 48.990154,
    `orientation` = 2.281233
WHERE `guid` = 319125
  AND `id` = 48351;

-- Bunny 1: corrected first ground point
UPDATE `creature`
SET `position_x` = -195.947937,
    `position_y` = -501.173187,
    `position_z` = 53.127834,
    `orientation` = 3.515090
WHERE `guid` = 319274
  AND `id` = 48338;

DELETE FROM `creature_addon`
WHERE `guid` IN (319274, 319125);

INSERT INTO `creature_addon`
(
    `guid`,
    `path_id`,
    `mount`,
    `bytes1`,
    `bytes2`,
    `emote`,
    `aiAnimKit`,
    `movementAnimKit`,
    `meleeAnimKit`,
    `visibilityDistanceType`,
    `auras`
)
VALUES
(319274, 3192740, 0, 0, 1, 0, 0, 0, 0, 0, '89842'),
(319125, 3191250, 0, 0, 1, 0, 0, 0, 0, 0, '89842');

DELETE FROM `waypoint_data`
WHERE `id` IN (3192740, 3191250);

-- Bunny 1 patrol
INSERT INTO `waypoint_data`
(
    `id`,
    `point`,
    `position_x`,
    `position_y`,
    `position_z`,
    `orientation`,
    `delay`,
    `move_type`,
    `action`,
    `action_chance`,
    `wpguid`
)
VALUES
(3192740, 1, -189.765167, -499.350647, 53.012089, 3.742078, 0, 0, 0, 100, 0),
(3192740, 2, -194.662170, -506.416382, 53.155178, 0.530578, 0, 0, 0, 100, 0),
(3192740, 3, -179.990860, -504.032562, 53.719868, 6.213718, 0, 0, 0, 100, 0),
(3192740, 4, -181.730530, -490.224915, 53.923595, 1.875180, 0, 0, 0, 100, 0);

-- Bunny 2 patrol
INSERT INTO `waypoint_data`
(
    `id`,
    `point`,
    `position_x`,
    `position_y`,
    `position_z`,
    `orientation`,
    `delay`,
    `move_type`,
    `action`,
    `action_chance`,
    `wpguid`
)
VALUES
(3191250, 1, -217.922409, -495.439697, 48.990154, 2.281233, 0, 0, 0, 100, 0),
(3191250, 2, -220.670029, -500.620239, 49.584881, 4.794508, 0, 0, 0, 100, 0),
(3191250, 3, -218.647263, -510.027893, 51.129749, 4.763093, 0, 0, 0, 100, 0),
(3191250, 4, -204.912201, -509.287567, 51.824902, 0.053846, 0, 0, 0, 100, 0);


-- ============================================================
-- 9. Ripsnarl fog helper visibility
--    General Purpose Bunny trigger NPCs must be invisible to
--    normal players.
--    CREATURE_FLAG_EXTRA_TRIGGER = 128
-- ============================================================
UPDATE `creature_template`
SET `flags_extra` = (`flags_extra` | 128)
WHERE `entry` IN (47242, 47677);


COMMIT;
