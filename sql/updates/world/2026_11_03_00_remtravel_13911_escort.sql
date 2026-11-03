-- Quest 13911: The Absent-Minded Prospector (BFA 8.3.7).
-- Restores Remtravel (34343) path/dialogue/SmartAI from locally tested 27-point escort.
-- BFA adjustments: quest-scoped accept, NPC-targeted Ferd (34342) instead of old GUID.
-- Scope: only creature 34343, waypoint entry 34343 and timed script 3434300.
-- Source: recovered MoP data, adjusted and locally tested on BFA 8.3.7.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 34343;

DELETE FROM `waypoints` WHERE `entry` = 34343;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(34343,1,4669.36,585.177,17.8915,'Prospector Remtravel'),
(34343,2,4683.33,605.26,14.1311,'Prospector Remtravel'),
(34343,3,4677.49,625.623,15.974,'Prospector Remtravel'),
(34343,4,4670.31,641.568,14.6007,'Prospector Remtravel'),
(34343,5,4651.58,661.486,14.25,'Prospector Remtravel'),
(34343,6,4642.12,657.763,14.548,'Prospector Remtravel'),
(34343,7,4615.47,677.342,13.0544,'Prospector Remtravel'),
(34343,8,4602.36,668.637,14.2093,'Prospector Remtravel'),
(34343,9,4583.24,649.505,15.8643,'Prospector Remtravel'),
(34343,10,4583.24,649.505,15.8643,'Prospector Remtravel'),
(34343,11,4555.97,636.243,18.5157,'Prospector Remtravel'),
(34343,12,4540.75,618.247,23.8348,'Prospector Remtravel'),
(34343,13,4524.04,591.734,23.607,'Prospector Remtravel'),
(34343,14,4528.84,589.333,24.977,'Prospector Remtravel'),
(34343,15,4527.86,555.729,25.0949,'Prospector Remtravel'),
(34343,16,4552.71,510.944,32.846,'Prospector Remtravel'),
(34343,17,4578.23,519.752,31.72,'Prospector Remtravel'),
(34343,18,4613.59,523.597,29.8953,'Prospector Remtravel'),
(34343,19,4626.1,535.55,30.459,'Prospector Remtravel'),
(34343,20,4639.1,549.26,25.2427,'Prospector Remtravel'),
(34343,21,4639.52,557.623,25.4962,'Prospector Remtravel'),
(34343,22,4633.71,566.42,24.5051,'Prospector Remtravel'),
(34343,23,4629.61,577.656,23.9912,'Prospector Remtravel'),
(34343,24,4625.52,576.866,24.4808,'Prospector Remtravel'),
(34343,25,4647.93,579.998,22.6275,'Prospector Remtravel'),
(34343,26,4647.16,581.092,22.671,'Prospector Remtravel'),
(34343,27,4647.93,579.998,22.6275,'Prospector Remtravel');

DELETE FROM `smart_scripts` WHERE (`entryorguid` = 34343 AND `source_type` = 0) OR (`entryorguid` = 3434300 AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34343, 0, 0, 1, 19, 0, 100, 0, 13911, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Accepted Quest - Store Target List [BFA: quest 13911 only]'),
(34343, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 8000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 0 - Say Line 0'),
(34343, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 6, 13911, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Death - Fail Quest'),
(34343, 0, 3, 0, 52, 0, 100, 0, 0, 34343, 0, 0, 0, 1, 1, 1000, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Text Over 0 - Say Line 1'),
(34343, 0, 4, 5, 52, 0, 100, 0, 1, 34343, 0, 0, 0, 53, 0, 34343, 0, 13911, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Text Over 1 - Start Waypoint'),
(34343, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 231, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 4 - Set Faction 231'),
(34343, 0, 6, 7, 4, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Aggro - Set React Defensive'),
(34343, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 20, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 6 - Auto Attack'),
(34343, 0, 8, 0, 1, 0, 100, 1, 0, 0, 0, 0, 0, 65, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Out of Combat - Waypoint Resume'),
(34343, 0, 9, 10, 40, 0, 100, 0, 5, 34343, 0, 0, 0, 54, 4000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Waypoint Reached - Waypoint Pause'),
(34343, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 9 - Set Orientation'),
(34343, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 10 - Say Line 2'),
(34343, 0, 12, 13, 40, 0, 100, 0, 15, 34343, 0, 0, 0, 54, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Waypoint Reached - Waypoint Pause'),
(34343, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 12 - Say Line 3'),
(34343, 0, 14, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 13 - Set Run 1'),
(34343, 0, 15, 16, 40, 0, 100, 0, 24, 34343, 0, 0, 0, 54, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Waypoint Reached - Waypoint Pause'),
(34343, 0, 16, 17, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 15 - Say Line 4'),
(34343, 0, 17, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 16 - Set Run 0'),
(34343, 0, 18, 19, 40, 0, 100, 0, 25, 34343, 0, 0, 0, 54, 16000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Waypoint Reached - Waypoint Pause'),
(34343, 0, 19, 20, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 18 - Set Orientation'),
(34343, 0, 20, 21, 61, 0, 100, 0, 0, 0, 0, 0, 0, 15, 13911, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 19 - Quest Credit'),
(34343, 0, 21, 22, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 20 - Say Line 5'),
(34343, 0, 22, 23, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 55, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 21 - Set Faction 55'),
(34343, 0, 23, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 34342, 25, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 22 - Set Data 0 1 [BFA: nearby Ferd target instead of fixed MoP GUID]'),
(34343, 0, 24, 25, 40, 0, 100, 0, 26, 34343, 0, 0, 0, 54, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Waypoint Reached - Waypoint Pause'),
(34343, 0, 25, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 6, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - Linked To Id 24 - Say Line 6'),
(34343, 0, 26, 27, 52, 0, 100, 0, 6, 34343, 0, 0, 0, 17, 428, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Waypoint Reached - Set Emote State 428'),
(34343, 0, 27, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3434300, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel Linked To Id 26 - Run Script'),
(34343, 0, 28, 0, 52, 0, 100, 0, 7, 34343, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Text Over 7 - Set Orientation'),
(3434300, 9, 0, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Script - Set Emote State 0'),
(3434300, 9, 1, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 34342, 25, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Script - Set Orientation [BFA: nearby Ferd target instead of fixed MoP GUID]'),
(3434300, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 7, 4000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Remtravel - On Script - Say Line 7');

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

-- No changes to quest rewards, creature spawn rates, factions or quest relations.
