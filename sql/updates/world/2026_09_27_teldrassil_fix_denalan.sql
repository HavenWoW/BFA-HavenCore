DELETE FROM smart_scripts WHERE entryorguid IN(2080,208000,208001,208002);
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(2080, 0, 0, 0, 20, 0, 100, 512, 997, 0, 0, 0, 0, '', 80, 208000, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - On quest 997 rewarded - Start action list'),
(2080, 0, 1, 0, 20, 0, 100, 512, 931, 0, 0, 0, 0, '', 80, 208001, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - On quest 931 rewarded - Start action list 1'),
(2080, 0, 2, 0, 20, 0, 100, 512, 930, 0, 0, 0, 0, '', 80, 208002, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - On quest 930 rewarded - Start action list 2'),
(208000, 9, 0, 1, 0, 0, 100, 0, 0, 0, 0, 0, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Set Run off'),
(208000, 9, 1, 2, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Say Text 0'),
(208000, 9, 2, 3, 0, 0, 100, 0, 500, 500, 0, 0, 0, '', 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 9506.54, 720.7, 1256.13, 2.63, 'Denalan - Move to position'),
(208000, 9, 3, 4, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0.0, 0.0, 0.0, 2.63, 'Denalan - Set Orientation to Position'),
(208000, 9, 4, 5, 0, 0, 100, 0, 500, 500, 0, 0, 0, '', 5, 16, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Emote kneel'),
(208000, 9, 5, 6, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Set Orientation to Invoker'),
(208000, 9, 6, 7, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Denalan - Say Text 1'),
(208000, 9, 7, 8, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, '', 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 9507.29, 714.583, 1255.89, 0.279253, 'Denalan - Move to position'),
(208000, 9, 8, 9, 0, 0, 100, 0, 4000,4000, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0.0, 0.0, 0.0, 0.279, 'Denalan - Set Orientation to Position');
