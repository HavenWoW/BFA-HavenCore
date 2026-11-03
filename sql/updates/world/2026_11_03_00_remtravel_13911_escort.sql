-- Quest 13911: upstream data for the v76.14.4 tested Remtravel escort.
START TRANSACTION;

-- Quest 13911: The Absent-Minded Prospector (BFA 8.3.7).
-- NPC-specific npc_escortAI implementation in zone_darkshore.cpp.
-- No changes to shared SmartAI.cpp / SmartAI.h, global escort settings or other NPCs.
-- 27 waypoints retained from the locally validated MoP-derived route.
-- C++ owns the quest dialogue, timed pauses, combat leash and recovery.

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_prospector_remtravel'
WHERE `entry` = 34343;

-- Remove only the NPC's own inactive SmartAI records, not its old timed-action
-- list (3434300) or generic waypoints path (34343), which might be referenced
-- by another SmartAI script. These legacy records are inert for Remtravel after
-- the AI binding below switches to its C++ script.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 34343 AND `source_type` = 0;

-- The scripted NPC escort uses `script_waypoint`, not the generic `waypoints` table.
-- Leave old generic waypoint rows intact to protect any external path references.
DELETE FROM `script_waypoint` WHERE `entry` = 34343;
INSERT INTO `script_waypoint`
(`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES
(34343, 1, 4669.36, 585.177, 17.8915, 0, 'Prospector Remtravel'),
(34343, 2, 4683.33, 605.26, 14.1311, 0, 'Prospector Remtravel'),
(34343, 3, 4677.49, 625.623, 15.974, 0, 'Prospector Remtravel'),
(34343, 4, 4670.31, 641.568, 14.6007, 0, 'Prospector Remtravel'),
(34343, 5, 4651.58, 661.486, 14.25, 0, 'Prospector Remtravel'),
(34343, 6, 4642.12, 657.763, 14.548, 0, 'Prospector Remtravel'),
(34343, 7, 4615.47, 677.342, 13.0544, 0, 'Prospector Remtravel'),
(34343, 8, 4602.36, 668.637, 14.2093, 0, 'Prospector Remtravel'),
(34343, 9, 4583.24, 649.505, 15.8643, 0, 'Prospector Remtravel'),
(34343, 10, 4583.24, 649.505, 15.8643, 0, 'Prospector Remtravel'),
(34343, 11, 4555.97, 636.243, 18.5157, 0, 'Prospector Remtravel'),
(34343, 12, 4540.75, 618.247, 23.8348, 0, 'Prospector Remtravel'),
(34343, 13, 4524.04, 591.734, 23.607, 0, 'Prospector Remtravel'),
(34343, 14, 4528.84, 589.333, 24.977, 0, 'Prospector Remtravel'),
(34343, 15, 4527.86, 555.729, 25.0949, 0, 'Prospector Remtravel'),
(34343, 16, 4552.71, 510.944, 32.846, 0, 'Prospector Remtravel'),
(34343, 17, 4578.23, 519.752, 31.72, 0, 'Prospector Remtravel'),
(34343, 18, 4613.59, 523.597, 29.8953, 0, 'Prospector Remtravel'),
(34343, 19, 4626.1, 535.55, 30.459, 0, 'Prospector Remtravel'),
(34343, 20, 4639.1, 549.26, 25.2427, 0, 'Prospector Remtravel'),
(34343, 21, 4639.52, 557.623, 25.4962, 0, 'Prospector Remtravel'),
(34343, 22, 4633.71, 566.42, 24.5051, 0, 'Prospector Remtravel'),
(34343, 23, 4629.61, 577.656, 23.9912, 0, 'Prospector Remtravel'),
(34343, 24, 4625.52, 576.866, 24.4808, 0, 'Prospector Remtravel'),
(34343, 25, 4647.93, 579.998, 22.6275, 0, 'Prospector Remtravel'),
(34343, 26, 4647.16, 581.092, 22.671, 0, 'Prospector Remtravel'),
(34343, 27, 4647.93, 579.998, 22.6275, 0, 'Prospector Remtravel');

DELETE FROM `creature_text` WHERE `CreatureID` = 34343 AND `GroupID` BETWEEN 0 AND 7;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `comment`) VALUES
(34343, 0, 0, 'Superb! Gather your strength, whats-your-face. We''ve got a long way to travel.', 12, 0, 100, 4, 0, 0, 34524, 'Prospector Remtravel'),
(34343, 1, 0, 'Ready? Let''s move!', 12, 0, 100, 0, 0, 0, 34525, 'Prospector Remtravel'),
(34343, 2, 0, 'The ruins you see here are identical to ones I dug up a few months ago, but they''re flooded! Curious, eh?', 12, 0, 100, 1, 0, 0, 34526, 'Prospector Remtravel'),
(34343, 3, 0, 'Ah, we''re almost there, whos-yer-face! The camp where I left my notes is just around the corner!', 12, 0, 100, 5, 0, 0, 34527, 'Prospector Remtravel'),
(34343, 4, 0, 'By the three hammers, will you look at that! More ruins!', 12, 0, 100, 25, 0, 0, 34528, 'Prospector Remtravel'),
(34343, 5, 0, 'These ruins I''ve found are IDENTICAL to the ones I just left! It''s the find of a lifetime!', 12, 0, 100, 0, 0, 0, 34529, 'Prospector Remtravel'),
(34343, 6, 0, 'Nonsense. See? Here''s my journal, right here. ', 12, 0, 100, 1, 0, 0, 34531, 'Prospector Remtravel'),
(34343, 7, 0, 'What do you make of this, Ferd?', 12, 0, 100, 1, 0, 0, 34532, 'Prospector Remtravel');


-- Quest rewards, starter/ender relations, creature spawns and all other SmartAI scripts unchanged.

COMMIT;
