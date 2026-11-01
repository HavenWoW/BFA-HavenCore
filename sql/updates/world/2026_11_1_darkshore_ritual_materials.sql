-- https://github.com/HavenWoW/BFA-HavenCore/issues/430
-- Set neutral faction
UPDATE creature_template SET faction=190 WHERE entry=33127;

DELETE FROM smart_scripts WHERE entryorguid = 33127 AND source_type = 0;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(33127, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, '', 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellclick - Remove spellclick flag'),
(33127, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Linked - Remove emote state'),
(33127, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 90, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Linked - Set stand state to stand'),
(33127, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 2, 14, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Linked - Set faction 14'),
(33127, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Linked - Set event phase 1 (awake)'),
(33127, 0, 5, 6, 1, 1, 100, 0, 60000, 60000, 60000, 60000, 0, '', 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Out of combat 60s (awake) - Restore template faction'),
(33127, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 90, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Linked - Set stand state to sleep'),
(33127, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 82, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Linked - Add spellclick flag'),
(33127, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Linked - Set event phase 0 (asleep)'),
(33127, 0, 9, 0, 0, 0, 100, 0, 3000, 6000, 3000, 6000, 0, '', 11, 75008, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'In combat - Cast Pounce every 3-6 seconds'),
(33127, 0, 10, 11, 37, 0, 100, 0, 0, 0, 0, 0, 0, '', 90, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On AI init - Sleep'),
(33127, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 82, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Linked - Add spellclick flag');

-- change spell target to player
UPDATE npc_spellclick_spells SET cast_flags=3 WHERE npc_entry=33978 AND spell_id=64116;

DELETE FROM smart_scripts WHERE entryorguid = 33978 AND source_type = 0;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(33978, 0, 0, 0, 0, 0, 100, 0, 6000, 8000, 18000, 22000, 0, '', 11, 5422, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Cast Lacerate'),
(33978, 0, 1, 2, 73, 0, 100, 1, 0, 0, 0, 0, 0, '', 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'SpellClick - Remove spellclick flag'),
(33978, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'SpellClick - Despawn');

-- Mottled Doe - they should move
UPDATE creature SET spawndist=5.0,MovementType=1 WHERE id=33313;

DELETE FROM smart_scripts WHERE entryorguid=33313;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(33313, 0, 0, 1, 73, 0, 100, 1, 0, 0, 0, 0, 0, '', 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'SpellClick - Remove spellclick flag'),
(33313, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'SpellClick - Despawn');
