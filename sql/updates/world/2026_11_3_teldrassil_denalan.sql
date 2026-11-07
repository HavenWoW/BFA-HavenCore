-- https://github.com/HavenWoW/BFA-HavenCore/issues/297
-- Create new smart script for quest 930 and improve existing one for quest 997
DELETE FROM smart_scripts WHERE entryorguid = 2080 AND source_type = 0 AND id IN (0, 1, 2);
DELETE FROM smart_scripts WHERE entryorguid IN (208000, 208002) AND source_type = 9;

INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(2080, 0, 0, 0, 20, 0, 100, 512, 997, 0, 0, 0, 0, '', 80, 208000, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - On quest 997 rewarded - Start action list'),
(2080, 0, 1, 0, 20, 0, 100, 512, 931, 0, 0, 0, 0, '', 80, 208001, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - On quest 931 rewarded - Start action list 1'),
(2080, 0, 2, 0, 20, 0, 100, 512, 930, 0, 0, 0, 0, '', 80, 208002, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - On quest 930 rewarded - Start action list 2'),
-- Action list 208000
(208000, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 83, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Remove questgiver flag'),
(208000, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 18, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Set uninteractible'),
(208000, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Set Run off'),
(208000, 9, 3, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Say Text 0'),
(208000, 9, 4, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, '', 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 9506.54, 720.7, 1256.13, 2.63, 'Denalan - Move to position'),
(208000, 9, 5, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0.0, 0.0, 0.0, 2.63, 'Denalan - Set Orientation to Position'),
(208000, 9, 6, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, '', 5, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Emote kneel'),
(208000, 9, 7, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Set Orientation to Invoker'),
(208000, 9, 8, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Say Text 1'),
(208000, 9, 9, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, '', 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 9507.29, 714.583, 1255.89, 0.279253, 'Denalan - Move to position'),
(208000, 9, 10, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0.0, 0.0, 0.0, 0.279, 'Denalan - Set Orientation to Position'),
(208000, 9, 11, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, '', 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Remove uninteractible'),
(208000, 9, 12, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 82, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Add questgiver flag'),
-- Action list 208002
(208002, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 83, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Remove questgiver flag'),
(208002, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 18, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Set uninteractible'),
(208002, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Set Run off'),
(208002, 9, 3, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Say Text 5'),
(208002, 9, 4, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, '', 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 9506.54, 720.7, 1256.13, 2.63, 'Denalan - Move to position'),
(208002, 9, 5, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0.0, 0.0, 0.0, 2.63, 'Denalan - Set Orientation to Position'),
(208002, 9, 6, 0, 0, 0, 100, 0, 2500, 2500, 0, 0, 0, '', 5, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Emote kneel'),
(208002, 9, 7, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, '', 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 9507.29, 714.583, 1255.89, 0.279253, 'Denalan - Move to last position'),
(208002, 9, 8, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0.0, 0.0, 0.0, 0.279, 'Denalan - Set Orientation at last position'),
(208002, 9, 9, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, '', 12, 3569, 1, 300000, 0, 0, 0, 8, 0, 0, 0, 9504.75, 722.048, 1256.47, 0.314159, 'Denalan - Summon creature 1'),
(208002, 9, 10, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 12, 3569, 1, 300000, 0, 0, 0, 8, 0, 0, 0, 9503.99, 721.516, 1256.47, 4.04916, 'Denalan - Summon creature 2'),
(208002, 9, 11, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 12, 3569, 1, 300000, 0, 0, 0, 8, 0, 0, 0, 9504.07, 720.209, 1256.47, 0.890118, 'Denalan - Summon creature 3'),
(208002, 9, 12, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, '', 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Remove uninteractible'),
(208002, 9, 13, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 82, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Add questgiver flag');

-- Bogling - set Bogling Root loot to 100%
UPDATE creature_loot_template SET Chance=100.0 WHERE Entry=3569 AND Item=5206;

-- Remove existing summon
DELETE FROM creature WHERE guid=302864 AND id=3569;
DELETE FROM creature WHERE guid=302865 AND id=3569;
DELETE FROM creature WHERE guid=302866 AND id=3569;
