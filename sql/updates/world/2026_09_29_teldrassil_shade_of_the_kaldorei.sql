-- make bunny idle
UPDATE creature SET  MovementType=0 WHERE id=34575;

-- Slow down Shade of the Kaldorei
UPDATE creature_template SET speed_walk=0.8 WHERE entry=34575;

DELETE FROM smart_scripts WHERE entryorguid=34575;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(34575, 0, 0, 0, 10, 0, 100, 0, 1, 10, 10000, 10000, 0, '', 11, 65656, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Doranel Amberleaf - ooc los - Cast Summon Shade of the Kaldorei');

DELETE FROM smart_scripts WHERE entryorguid IN(34574,3457400,3457401,3457402,3457403);
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(34574, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, '', 80, 3457400, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On just summoned  - Action list'),
(34574, 0, 1, 0, 40, 0, 100, 0, 1, 34574, 0, 0, 0, '', 80, 3457401, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On wp1 reached  - Action list'),
(34574, 0, 2, 0, 40, 0, 100, 0, 3, 34574, 0, 0, 0, '', 80, 3457402, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On wp3 reached  - Action list'),
(34574, 0, 3, 0, 40, 0, 100, 0, 5, 34574, 0, 0, 0, '', 80, 3457403, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On wp5 reached  - Action list'),
(3457400, 9, 0, 1, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, '', 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On just summoned  - Say text 1'),
(3457400, 9, 1, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, '', 53, 0, 34574, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On just summoned  - Start Waypoint'),
(3457401, 9, 0, 1, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 54, 11000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On wp1 reached  - Pause wp'),
(3457401, 9, 1, 2, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0.0, 0.0, 0.0, 2.91, 'Shade of the Kaldorei - On wp1 reached  - Set Orientation'),
(3457401, 9, 2, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, '', 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On wp1 reached  - Say text 2'),
(3457402, 9, 0, 1, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 54, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On wp3 reached  - Pause wp'),
(3457402, 9, 1, 2, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0.0, 0.0, 0.0, 4.91, 'Shade of the Kaldorei - On wp3 reached  - Set Orientation'),
(3457402, 9, 2, 3, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On wp3 reached  - Say text 3'),
(3457402, 9, 3, 0, 0, 0, 100, 0, 7000, 7000, 0, 0, 0, '', 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On wp3 reached  - Say text 4'),
(3457403, 9, 0, 1, 0, 0, 100, 0, 100, 100, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0.0, 0.0, 0.0, 2.81, 'Shade of the Kaldorei - On wp5 reached  - Set Orientation'),
(3457403, 9, 1, 2, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, '', 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On wp5 reached  - Say text 5'),
(3457403, 9, 2, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, '', 41, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Shade of the Kaldorei - On wp5 reached  - Despawn');

DELETE FROM waypoints WHERE entry=34574;
INSERT INTO waypoints (entry, pointid, position_x, position_y, position_z, point_comment) VALUES
(34574, 1, 10702.9, 761.374, 1322.91, 'Shade of the Kaldorei'),
(34574, 2, 10704.4, 769.148, 1322.6, 'Shade of the Kaldorei'),
(34574, 3, 10705.9, 769.14, 1322.83, 'Shade of the Kaldorei'),
(34574, 4, 10707.03, 767.418, 1321.95, 'Shade of the Kaldorei'),
(34574, 5, 10712.1, 762.04, 1321.39, 'Shade of the Kaldorei');
