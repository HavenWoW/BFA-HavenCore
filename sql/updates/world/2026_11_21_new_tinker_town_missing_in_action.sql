-- https://github.com/HavenWoW/BFA-HavenCore/issues/178
UPDATE quest_objectives SET Description='Captured Demolitionists rescued' WHERE ID=266073;

DELETE FROM smart_scripts WHERE entryorguid IN(42645,4264500);
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(42645, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, '', 32, 0, 0, 0, 0, 0, 0, 20, 204019, 5, 0, 0.0, 0.0, 0.0, 0.0, 'Captured Demolitionist - On Respawn - Reset Makeshift Cage 204019'),
(42645, 0, 1, 0, 38, 0, 100, 1, 0, 1, 0, 0, 0, '', 80, 4264500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Captured Demolitionist - On Data Set 0 1 - Run Timed Actionlist'),
(4264500, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, '', 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Say Random Text'),
(4264500, 9, 1, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Set run off'),
(4264500, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, '', 114, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 8.0, 0.0, 0.0, 'Move Forward 8 yards'),
(4264500, 9, 3, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, '', 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Despawn');

DELETE FROM smart_scripts WHERE entryorguid = 204019;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(204019, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, '', 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Makeshift Cage - On Gameobject State Changed - Store Targetlist'),
(204019, 1, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 42645, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Link - Kill Credit'),
(204019, 1, 2, 0, 61, 0, 100, 0, 2, 0, 0, 0, 0, '', 45, 0, 1, 0, 0, 0, 0, 19, 42645, 5, 0, 0.0, 0.0, 0.0, 0.0, 'Makeshift Cage - On Gameobject State Changed - Set Data 0 1');

-- Reduce respawn timer for better early game
UPDATE creature SET spawntimesecs=120 WHERE id=42645;
