-- https://github.com/HavenWoW/BFA-HavenCore/issues/364
-- https://github.com/HavenWoW/BFA-HavenCore/issues/365
-- https://github.com/HavenWoW/BFA-HavenCore/issues/389
DELETE FROM quest_poi WHERE QuestID=13518;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(13518, 0, 0, -1, 0, 0, 1, 62, 0, 2, 0, 0, 0, 0, 35662),
(13518, 0, 1, 0, 267127, 33093, 1, 62, 0, 0, 0, 0, 0, 0, 35662),
(13518, 0, 2, 1, 267128, 32911, 1, 62, 0, 0, 0, 0, 0, 0, 35662),
(13518, 0, 3, 2, 267129, 33095, 1, 62, 0, 0, 0, 0, 0, 0, 35662),
(13518, 0, 4, 3, 267130, 33094, 1, 62, 0, 0, 0, 0, 0, 0, 35662);

DELETE FROM quest_poi_points WHERE QuestID=13518;
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild) VALUES
(13518, 0, 0, 7420, -283, 35662),
(13518, 1, 0, 7366, 135, 35662),
(13518, 2, 0, 7456, 164, 35662),
(13518, 3, 0, 7296, 243, 35662),
(13518, 4, 0, 7439, 106, 35662);

-- Set hp at 10%
UPDATE creature SET curhealth=16 WHERE guid=303076;
UPDATE creature SET curhealth=16 WHERE guid=303075;
UPDATE creature SET curhealth=16 WHERE guid=308258;
UPDATE creature SET curhealth=16 WHERE guid=303078;
UPDATE creature_template SET RegenHealth = 0 WHERE entry IN (32911,33093,33094,33095);

-- Set proper level
UPDATE creature_template SET maxlevel=60,minlevel=10 WHERE entry=43440;

-- Add missing sentinels
DELETE FROM creature WHERE guid IN(100000000,100000001,100000002,100000003) AND id=32969;
INSERT INTO creature (guid, id, `map`, zoneId, areaId, spawnDifficulties, phaseUseFlags, PhaseId, PhaseGroup, terrainSwapMap, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, MovementType, npcflag, unit_flags, unit_flags2, unit_flags3, dynamicflags, ScriptName, VerifiedBuild) VALUES
(100000003, 32969, 1, 0, 0, '0', 0, 2000, 0, -1, 0, 1, 7430.35, 150.339, 0.779603, 1.45299, 300, 0.0, 0, 4767, 0, 0, 0, 768, 0, 0, 0, '', 35662),
(100000002, 32969, 1, 0, 0, '0', 0, 2000, 0, -1, 0, 1, 7390.69, 108.088, 7.5072, 3.20836, 300, 0.0, 0, 4767, 0, 0, 0, 768, 0, 0, 0, '', 35662),
(100000001, 32969, 1, 0, 0, '0', 0, 2000, 0, -1, 0, 1, 7422.75, 39.3669, 2.39565, 1.24879, 300, 0.0, 0, 4767, 0, 0, 0, 768, 0, 0, 0, '', 35662),
(100000000, 32969, 1, 0, 0, '0', 0, 2000, 0, -1, 0, 1, 7418.63, 34.9982, 2.28486, 1.24879, 300, 0.0, 0, 4767, 0, 0, 0, 768, 0, 0, 0, '', 35662);


DELETE FROM creature_addon WHERE guid IN(100000001,100000003);
INSERT INTO creature_addon (guid, path_id, mount, bytes1, bytes2, emote, aiAnimKit, movementAnimKit, meleeAnimKit, visibilityDistanceType, auras) VALUES
(100000001, 0, 9991, 1, 0, 0, 0, 0, 0, 0, NULL),
(100000003, 0, 0, 8, 0, 0, 0, 0, 0, 0, NULL);

-- hardcoded creature text by checking Broadcast_text values
DELETE FROM creature_text WHERE CreatureID=32969;
INSERT INTO creature_text (CreatureID, GroupID, ID, `Text`, `Type`, `Language`, Probability, Emote, Duration, Sound, BroadcastTextId, TextRange, comment) VALUES
(32969, 0, 0, 'Cerellean! You made it!', 12, 0, 100.0, 0, 0, 0, 33248, 0, 'Lor''danel Sentinel'),
(32969, 1, 0, 'Gershala... Dentaria will be so happy to see you alive.', 12, 0, 100.0, 0, 0, 0, 33258, 0, 'Lor''danel Sentinel'),
(32969, 2, 0, 'I''ll take him back to the priestess. Thank you for your kindness.', 12, 0, 100.0, 0, 0, 0, 33259, 0, 'Lor''danel Sentinel'),
(32969, 3, 0, 'I will see to it that Shaldyn is returned safely. Thank you, hero.', 12, 0, 100.0, 0, 0, 0, 33260, 0, 'Lor''danel Sentinel'),
(32969, 4, 0, 'Shaldyn!', 12, 0, 100.0, 0, 0, 0, 33261, 0, 'Lor''danel Sentinel'),
(32969, 5, 0, 'Volcor... I''m so glad to see that you made it. But where is Grimclaw?', 12, 0, 100.0, 0, 0, 0, 33262, 0, 'Lor''danel Sentinel');

-- Volcor
DELETE FROM smart_scripts WHERE entryorguid = 33094;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(33094, 0, 0, 1, 64, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 33094, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, ''),
(33094, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 45, 1, 10, 0, 0, 0, 0, 10, 100000001, 0, 0, 0.0, 0.0, 0.0, 0.0, '');

-- Set behavior for Volcor sentinel
DELETE FROM smart_scripts WHERE entryorguid=32969 AND source_type=0 AND id IN (30,31);
DELETE FROM smart_scripts WHERE entryorguid IN (3296910,3296911) AND source_type=9;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(32969, 0, 30, 0, 38, 0, 100, 0, 1, 10, 60000, 60000, 0, '', 80, 3296910, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Data 1=10 - call go list'),
(32969, 0, 31, 0, 34, 0, 100, 0, 8, 1, 0, 0, 0, '', 80, 3296911, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Point 1 reached - call arrive list'),
(3296910, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Set run'),
(3296910, 9, 1, 0, 0, 0, 100, 0, 200, 200, 0, 0, 0, '', 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, 7439.04, 102.05, 0.56, 1.29, 'Move to position (point 1)'),
(3296911, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 7439.04, 102.05, 0.56, 1.29, 'Set orientation'),
(3296911, 9, 1, 0, 0, 0, 100, 0, 200, 200, 0, 0, 0, '', 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Talk group 5'),
(3296911, 9, 2, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, '30s - set walk'),
(3296911, 9, 3, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, '', 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, '30s - evade (return home)');

-- Gelshala
DELETE FROM smart_scripts WHERE entryorguid = 32911;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(32911, 0, 0, 1, 64, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 32911, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, ''),
(32911, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 45, 1, 11, 0, 0, 0, 0, 10, 100000003, 0, 0, 0.0, 0.0, 0.0, 0.0, '');

-- Set behavior for Gelshala sentinel
DELETE FROM smart_scripts WHERE entryorguid=32969 AND source_type=0 AND id IN (32,33);
DELETE FROM smart_scripts WHERE entryorguid IN (3296920,3296921) AND source_type=9;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(32969, 0, 32, 0, 38, 0, 100, 0, 1, 11, 60000, 60000, 0, '', 80, 3296920, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Data 1=11 - call go list'),
(32969, 0, 33, 0, 34, 0, 100, 0, 8, 2, 0, 0, 0, '', 80, 3296921, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Point 2 reached - call arrive list'),
(3296920, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Set run'),
(3296920, 9, 1, 0, 0, 0, 100, 0, 200, 200, 0, 0, 0, '', 69, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, 7453.65, 162.69, 2.22, 0.59, 'Move to position (point 2)'),
(3296921, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 7453.65, 162.69, 2.22, 0.59, 'Set orientation'),
(3296921, 9, 1, 0, 0, 0, 100, 0, 200, 200, 0, 0, 0, '', 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Talk group 1'),
(3296921, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, '', 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Talk group 2'),
(3296921, 9, 3, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, '30s - set walk'),
(3296921, 9, 4, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, '', 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, '30s - evade (return home)');

-- Cerellean
DELETE FROM smart_scripts WHERE entryorguid = 33093;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(33093, 0, 0, 1, 64, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 33093, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, ''),
(33093, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 45, 1, 12, 0, 0, 0, 0, 10, 100000002, 0, 0, 0.0, 0.0, 0.0, 0.0, '');

-- Set behavior for Cerellean sentinel
DELETE FROM smart_scripts WHERE entryorguid=32969 AND source_type=0 AND id IN (34,35);
DELETE FROM smart_scripts WHERE entryorguid IN (3296930,3296931) AND source_type=9;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(32969, 0, 34, 0, 38, 0, 100, 0, 1, 12, 60000, 60000, 0, '', 80, 3296930, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Data 1=10 - call go list'),
(32969, 0, 35, 0, 34, 0, 100, 0, 8, 3, 0, 0, 0, '', 80, 3296931, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Point 3 reached - call arrive list'),
(3296930, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Set run'),
(3296930, 9, 1, 0, 0, 0, 100, 0, 200, 200, 0, 0, 0, '', 69, 3, 0, 0, 0, 0, 0, 8, 0, 0, 0, 7370.18, 132.09, 12.08, 2.55, 'Move to position (point 3)'),
(3296931, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 7370.18, 132.09, 12.08, 2.55, 'Set orientation'),
(3296931, 9, 1, 0, 0, 0, 100, 0, 200, 200, 0, 0, 0, '', 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Talk group 0'),
(3296931, 9, 2, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, '30s - set walk'),
(3296931, 9, 3, 0, 0, 0, 100, 0, 30000, 30000, 0, 0, 0, '', 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, '30s - evade (return home)');

-- Shaldyn
DELETE FROM smart_scripts WHERE entryorguid = 33095 AND source_type = 0;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(33095, 0, 0, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'AI init - Set phase 1 (ready)'),
(33095, 0, 1, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 33095, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On Gossip Hello (any phase) - Kill credit'),
(33095, 0, 2, 3, 64, 1, 100, 0, 0, 0, 0, 0, 0, '', 45, 1, 1, 0, 0, 0, 0, 11, 33115, 60, 0, 0.0, 0.0, 0.0, 0.0, 'On Gossip Hello (phase 1 only) - Set data 1 1 on 33115'),
(33095, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 12, 33115, 3, 60000, 0, 0, 0, 8, 0, 0, 0, 7273.81, 256.58, 10.13, 6.01, 'Summon 32969'),
(33095, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Set phase 2 (cooldown)'),
(33095, 0, 5, 0, 1, 2, 100, 0, 90000, 90000, 90000, 90000, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Cooldown over - Back to phase 1');

-- Main script: only starts the timed action list
DELETE FROM smart_scripts WHERE entryorguid = 33115 AND source_type = 0 AND id IN (10,11);
DELETE FROM smart_scripts WHERE entryorguid IN (3311500,3311501) AND source_type = 9;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(33115, 0, 10, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, '', 80, 3311500, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On summoned - Call timed action list 3296940'),
(33115, 0, 11, 0, 34, 0, 100, 0, 8, 6, 0, 0, 0, '', 80, 3311501, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Point 5 reached - call arrive list'),
(3311500, 9, 0, 1, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 43, 0, 22474, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Timed list - Mount model 22474'),
(3311500, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 208, 6, 12, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Timed list - Flight speed 12'),
(3311500, 9, 4, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, '', 69, 6, 0, 0, 0, 0, 0, 8, 0, 0, 0, 7291.64, 244.72, 2.38, 6.01, 'Timed list - Move to point (point 6)'),
(3311501, 9, 0, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 7291.64, 244.72, 2.38, 6.01, 'Set orientation'),
(3311501, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, '', 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Timed list - Talk 4'),
(3311501, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, '', 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Timed list - Talk 4 to Talk 3'),
(3311501, 9, 3, 0, 0, 0, 100, 0, 15000, 15000, 0, 0, 0, '', 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Timed list - Despawn');

-- Yes, I realize I have another creature with the same text but fuck it i'm not fixing things at this point
DELETE FROM creature_text WHERE CreatureID=33115;
INSERT INTO creature_text (CreatureID, GroupID, ID, `Text`, `Type`, `Language`, Probability, Emote, Duration, Sound, BroadcastTextId, TextRange, comment) VALUES
(33115, 0, 0, 'Cerellean! You made it!', 12, 0, 100.0, 0, 0, 0, 33248, 0, 'Lor''danel Sentinel'),
(33115, 1, 0, 'Gershala... Dentaria will be so happy to see you alive.', 12, 0, 100.0, 0, 0, 0, 33258, 0, 'Lor''danel Sentinel'),
(33115, 2, 0, 'I''ll take him back to the priestess. Thank you for your kindness.', 12, 0, 100.0, 0, 0, 0, 33259, 0, 'Lor''danel Sentinel'),
(33115, 3, 0, 'I will see to it that Shaldyn is returned safely. Thank you, hero.', 12, 0, 100.0, 0, 0, 0, 33260, 0, 'Lor''danel Sentinel'),
(33115, 4, 0, 'Shaldyn!', 12, 0, 100.0, 0, 0, 0, 33261, 0, 'Lor''danel Sentinel'),
(33115, 5, 0, 'Volcor... I''m so glad to see that you made it. But where is Grimclaw?', 12, 0, 100.0, 0, 0, 0, 33262, 0, 'Lor''danel Sentinel');

-- Lor'danel Sentinel - add flying mount with path
UPDATE creature SET MovementType=2 WHERE guid=245733;

DELETE FROM creature_addon WHERE guid=245733;
INSERT INTO creature_addon (guid, path_id, mount, bytes1, bytes2, emote, aiAnimKit, movementAnimKit, meleeAnimKit, visibilityDistanceType, auras) VALUES
(245733, 245733, 22474, 0, 1, 0, 0, 0, 0, 0, NULL);

DELETE FROM waypoint_data WHERE id = 245733;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, orientation, delay, move_type, action, action_chance, wpguid) VALUES
(245733,  1, 7468.480000, -158.105000, 35.151500, 0, 0, 1, 0, 100, 0),
(245733,  2, 7436.713379,  -68.385773, 12.345327, 0, 0, 1, 0, 100, 0),
(245733,  3, 7417.770996,  -15.810356,  8.738731, 0, 0, 1, 0, 100, 0),
(245733,  4, 7408.901367,   15.125710,  7.704081, 0, 0, 1, 0, 100, 0),
(245733,  5, 7410.651855,   34.646446,  7.515902, 0, 0, 1, 0, 100, 0),
(245733,  6, 7431.226562,   79.746925, 16.613977, 0, 0, 1, 0, 100, 0),
(245733,  7, 7427.709961,  134.105011, 19.497040, 0, 0, 1, 0, 100, 0),
(245733,  8, 7422.034180,  193.950958, 22.077559, 0, 0, 1, 0, 100, 0),
(245733,  9, 7425.671387,  217.359024, 23.186438, 0, 0, 1, 0, 100, 0),
(245733, 10, 7455.323730,  240.397980, 22.834530, 0, 0, 1, 0, 100, 0),
(245733, 11, 7484.232422,  242.894501, 20.015265, 0, 0, 1, 0, 100, 0),
(245733, 12, 7511.179688,  211.865860, 17.747576, 0, 0, 1, 0, 100, 0),
(245733, 13, 7500.928711,  163.427307, 16.340353, 0, 0, 1, 0, 100, 0),
(245733, 14, 7432.782715,   78.180115, 18.359528, 0, 0, 1, 0, 100, 0),
(245733, 15, 7392.311035,    3.058084, 25.162676, 0, 0, 1, 0, 100, 0),
(245733, 16, 7388.667480,  -37.322018, 24.926807, 0, 0, 1, 0, 100, 0),
(245733, 17, 7406.145508, -119.528915, 23.575014, 0, 0, 1, 0, 100, 0),
(245733, 18, 7425.953125, -156.382675, 24.230701, 0, 0, 1, 0, 100, 0),
(245733, 19, 7456.786621, -168.279922, 30.616329, 0, 0, 1, 0, 100, 0);


-- Make it faster
DELETE FROM smart_scripts WHERE entryorguid = -245733 AND source_type = 0;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(-245733, 0, 0, 1, 37, 0, 100, 0, 0, 0, 0, 0, 0, '', 208, 6, 21, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flight speed 21'),
(-245733, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 208, 1, 21, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Run speed 21'),
(-245733, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 208, 0, 21, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Walk speed 21'),
(-245733, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 48, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Set active 1');

-- Delete useless creature that was created by waypoint_data
DELETE FROM creature WHERE guid=245616;

-- Set waypoint_data movement
UPDATE creature SET MovementType=2 WHERE guid=247681;
UPDATE creature_addon SET path_id=247681 WHERE guid=247681;

DELETE FROM waypoint_data WHERE id = 247681;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, orientation, delay, move_type, action, action_chance, wpguid) VALUES
(247681,  1, 7487.644531, -276.285370, 23.822990, 0, 0, 1, 0, 100, 0),
(247681,  2, 7501.711426, -235.282196, 25.905167, 0, 0, 1, 0, 100, 0),
(247681,  3, 7515.030762, -206.206024, 28.904974, 0, 0, 1, 0, 100, 0),
(247681,  4, 7500.110352, -164.364563, 34.933090, 0, 0, 1, 0, 100, 0),
(247681,  5, 7466.956055, -146.766541, 38.358910, 0, 0, 1, 0, 100, 0),
(247681,  6, 7421.120605, -162.118637, 41.339069, 0, 0, 1, 0, 100, 0),
(247681,  7, 7400.892578, -190.051514, 39.369392, 0, 0, 1, 0, 100, 0),
(247681,  8, 7409.957520, -214.676071, 36.969566, 0, 0, 1, 0, 100, 0),
(247681,  9, 7445.355957, -237.024796, 38.248409, 0, 0, 1, 0, 100, 0),
(247681, 10, 7460.704102, -249.795395, 38.127449, 0, 0, 1, 0, 100, 0),
(247681, 11, 7534.323730, -240.689133, 36.542854, 0, 0, 1, 0, 100, 0),
(247681, 12, 7595.150879, -245.403702, 29.388683, 0, 0, 1, 0, 100, 0),
(247681, 13, 7616.934082, -280.472137, 28.406517, 0, 0, 1, 0, 100, 0),
(247681, 14, 7598.979980, -333.927002, 24.869902, 0, 0, 1, 0, 100, 0),
(247681, 15, 7547.240723, -351.997711, 20.777073, 0, 0, 1, 0, 100, 0),
(247681, 16, 7495.719238, -341.883484, 19.588335, 0, 0, 1, 0, 100, 0);

-- Make it faster
DELETE FROM smart_scripts WHERE entryorguid = -247681 AND source_type = 0;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(-247681, 0, 0, 1, 37, 0, 100, 0, 0, 0, 0, 0, 0, '', 208, 6, 21, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flight speed 21'),
(-247681, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 208, 1, 21, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Run speed 21'),
(-247681, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 208, 0, 21, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Walk speed 21'),
(-247681, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 48, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Set active 1');


-- Lor'danel sentinel - set idle movement
UPDATE creature SET MovementType=0 WHERE guid=245617;
UPDATE creature SET MovementType=0 WHERE guid=245618;
UPDATE creature SET MovementType=0 WHERE guid=245731;
UPDATE creature SET MovementType=0 WHERE guid=245734;
UPDATE creature SET MovementType=0 WHERE guid=245737;
UPDATE creature SET MovementType=0 WHERE guid=245738;
UPDATE creature SET MovementType=0 WHERE guid=245545;
UPDATE creature SET MovementType=0 WHERE guid=245546;
UPDATE creature SET MovementType=0 WHERE guid=245547;
UPDATE creature SET MovementType=0 WHERE guid=245548;
UPDATE creature SET MovementType=0 WHERE guid=245549;
UPDATE creature SET MovementType=0 WHERE guid=245699;
UPDATE creature SET MovementType=0 WHERE guid=245700;
UPDATE creature SET MovementType=0 WHERE guid=245701;
UPDATE creature SET MovementType=0 WHERE guid=245614;
UPDATE creature SET MovementType=0 WHERE guid=245619;
UPDATE creature SET MovementType=0 WHERE guid=245736;


UPDATE creature SET MovementType=2 WHERE guid=245739;

DELETE FROM creature_addon WHERE guid=245739;
INSERT INTO creature_addon (guid, path_id, mount, bytes1, bytes2, emote, aiAnimKit, movementAnimKit, meleeAnimKit, visibilityDistanceType, auras) VALUES
(245739, 245739, 22474, 0, 1, 0, 0, 0, 0, 0, NULL);

DELETE FROM waypoint_data WHERE id = 245739;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, orientation, delay, move_type, action, action_chance, wpguid) VALUES
(245739,  1, 7276.761230,  -182.360443, 28.054508, 0, 0, 1, 0, 100, 0),
(245739,  2, 7249.088867,  -185.402222, 27.494860, 0, 0, 1, 0, 100, 0),
(245739,  3, 7242.467773,  -198.279144, 28.110798, 0, 0, 1, 0, 100, 0),
(245739,  4, 7250.017090,  -237.838959, 28.985332, 0, 0, 1, 0, 100, 0),
(245739,  5, 7250.588867,  -303.440887, 38.118958, 0, 0, 1, 0, 100, 0),
(245739,  6, 7253.547852,  -353.739258, 36.445538, 0, 0, 1, 0, 100, 0),
(245739,  7, 7255.160645,  -422.874878, 38.196716, 0, 0, 1, 0, 100, 0),
(245739,  8, 7281.867676,  -472.379242, 37.561108, 0, 0, 1, 0, 100, 0),
(245739,  9, 7306.163086,  -487.869720, 35.888290, 0, 0, 1, 0, 100, 0),
(245739, 10, 7338.982422,  -484.496887, 34.874283, 0, 0, 1, 0, 100, 0),
(245739, 11, 7358.229980,  -461.798645, 33.443752, 0, 0, 1, 0, 100, 0),
(245739, 12, 7368.480957,  -419.558258, 32.325283, 0, 0, 1, 0, 100, 0),
(245739, 13, 7381.164551,  -343.336121, 31.734077, 0, 0, 1, 0, 100, 0),
(245739, 14, 7386.222656,  -276.432983, 31.187714, 0, 0, 1, 0, 100, 0),
(245739, 15, 7372.833496,  -238.987228, 30.158724, 0, 0, 1, 0, 100, 0),
(245739, 16, 7346.731445,  -203.421417, 25.953705, 0, 0, 1, 0, 100, 0),
(245739, 17, 7335.477051,  -196.268661, 23.648476, 0, 0, 1, 0, 100, 0);

-- Make it faster
DELETE FROM smart_scripts WHERE entryorguid = -245739 AND source_type = 0;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(-245739, 0, 0, 1, 37, 0, 100, 0, 0, 0, 0, 0, 0, '', 208, 6, 21, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flight speed 21'),
(-245739, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 208, 1, 21, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Run speed 21'),
(-245739, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 208, 0, 21, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Walk speed 21'),
(-245739, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 48, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Set active 1');
