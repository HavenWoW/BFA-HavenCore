-- Feed of Evil (14461) - create custom quest objectives
DELETE FROM quest_objectives WHERE QuestID = 14461 AND ID IN(5000000,5000001,5000002);
INSERT INTO quest_objectives
(ID, QuestID, `Type`, `Order`, StorageIndex, ObjectID, Amount, Flags, Flags2, ProgressBarWeight, Description, VerifiedBuild)
VALUES
(5000000, 14461, 0, 0, 0, 36727, 1, 0, 1, 0.0, 'First Trough', 35662),
(5000001, 14461, 0, 0, 1, 37155, 1, 0, 1, 0.0, 'Second Trough', 35662),
(5000002, 14461, 0, 0, 2, 37156, 1, 0, 1, 0.0, 'Third Trough', 35662);

--  NPC bunny are untargetable on respawn
UPDATE creature_template SET unit_flags=33554432 WHERE entry IN(36727, 37155, 37156);

-- NPC bunny should have bigger auras
UPDATE creature_template SET `scale`=3.0 WHERE entry IN(36727, 37155, 37156);

-- NPC bunny cast aura to nearby Armored Battleboar (36696)
DELETE FROM smart_scripts WHERE entryorguid IN(36727, 37155, 37156);
INSERT INTO smart_scripts
(entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5,
event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2,
target_param3, target_x, target_y, target_z, target_o, comment)
VALUES
(36727, 0, 0, 0, 8, 0, 100, 1, 69228, 0, 0, 0, 0, '', 33, 36727, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'First Trough - Kill credit 36727'),
(36727, 0, 1, 0, 8, 0, 100, 0, 69228, 0, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'First Trough - Fuego visual (Cambio de fase)'),
(36727, 0, 2, 0, 8, 0, 100, 0, 69228, 0, 0, 0, 0, '', 75, 98950, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'First Trough - Fuego visual'),
(36727, 0, 3, 0, 8, 0, 100, 0, 69228, 0, 0, 0, 0, '', 75, 98950, 0, 0, 0, 0, 0, 11, 36696, 6, 0, 0.0, 0.0, 0.0, 0.0, 'Cast 98950 to nearby creatures'),
(36727, 0, 4, 0, 1, 1, 100, 0, 10000, 10000, 1000, 1000, 0, '', 28, 98950, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'First Trough - Fuego visual (Retire l''aura)'),
(36727, 0, 5, 0, 1, 1, 100, 0, 10020, 10020, 1000, 1000, 0, '', 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'First Trough - Muere'),
(36727, 0, 6, 0, 6, 1, 100, 0, 0, 0, 0, 0, 0, '', 70, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'First Trough - Respawn'),
(37155, 0, 0, 0, 8, 0, 100, 1, 69228, 0, 0, 0, 0, '', 33, 37155, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Second Trough - Kill credit 37155'),
(37155, 0, 1, 0, 8, 0, 100, 0, 69228, 0, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Second Trough - Fuego visual (Cambio de fase)'),
(37155, 0, 2, 0, 8, 0, 100, 0, 69228, 0, 0, 0, 0, '', 75, 98950, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Second Trough - Fuego visual'),
(37155, 0, 3, 0, 8, 0, 100, 0, 69228, 0, 0, 0, 0, '', 75, 98950, 0, 0, 0, 0, 0, 11, 36696, 6, 0, 0.0, 0.0, 0.0, 0.0, 'Cast 98950 to nearby creatures'),
(37155, 0, 4, 0, 1, 1, 100, 0, 10000, 10000, 1000, 1000, 0, '', 28, 98950, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Second Trough - Fuego visual (Retire l''aura)'),
(37155, 0, 5, 0, 1, 1, 100, 0, 10020, 10020, 1000, 1000, 0, '', 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Second Trough - Muere'),
(37155, 0, 6, 0, 6, 1, 100, 0, 0, 0, 0, 0, 0, '', 70, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Second Trough - Respawn'),
(37156, 0, 0, 0, 8, 0, 100, 0, 69228, 0, 0, 0, 0, '', 33, 37156, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Third Trough - Kill credit 37156'),
(37156, 0, 1, 0, 8, 0, 100, 0, 69228, 0, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Third Trough - Fuego visual (Cambio de fase)'),
(37156, 0, 2, 0, 8, 0, 100, 0, 69228, 0, 0, 0, 0, '', 75, 98950, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Third Trough - Fuego visual'),
(37156, 0, 3, 0, 8, 0, 100, 0, 69228, 0, 0, 0, 0, '', 75, 98950, 0, 0, 0, 0, 0, 11, 36696, 6, 0, 0.0, 0.0, 0.0, 0.0, 'Cast 98950 to nearby creatures'),
(37156, 0, 4, 0, 1, 1, 100, 0, 10000, 10000, 1000, 1000, 0, '', 28, 98950, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Third Trough - Fuego visual (Retire l''aura)'),
(37156, 0, 5, 0, 1, 1, 100, 0, 10020, 10020, 1000, 1000, 0, '', 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Third Trough - Muere'),
(37156, 0, 6, 0, 6, 1, 100, 0, 0, 0, 0, 0, 0, '', 70, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Third Trough - Respawn');

-- Armored Battleboar (36696) - behavior after starting burning
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (36696, 3669601);
INSERT INTO `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
(36696, 0, 0, 0, 1, 0, 100, 0, 3000, 5000, 3000, 4500, 0, '', 5, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Armored Battleboar - On OOC - Play Eat Emote'),
(36696, 0, 1, 2, 23, 0, 100, 1, 98950, 1, 0, 0, 0, '', 122, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Armored Battleboar - On Aura Burning - Flee for 10s'),
(36696, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 80, 3669601, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Armored Battleboar - On Aura - Call Delayed Death List'),
(36696, 0, 3, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, '', 28, 98950, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Armored Battleboar - On Reset/Despawn - Remove Aura 98950'),
(3669601, 9, 0, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, '', 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Delayed Death - Wait 8000ms then Kill Self');
