-- https://github.com/HavenWoW/BFA-HavenCore/issues/577
DELETE FROM conditions WHERE SourceTypeOrReferenceId = 13 AND SourceEntry = 65300;
INSERT INTO conditions
(SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup,
 ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, Comment)
VALUES
(13, 1, 65300, 0, 0, 31, 0, 3, 34356, 0, 'Ping for Artifact - target artifact bunnies');

-- Bunnies should be idle
UPDATE creature SET MovementType=0 WHERE id=34356;

-- Remove gameobjects that should be summoned
DELETE FROM gameobject WHERE id=195055;

-- Summon gameobject
DELETE FROM smart_scripts WHERE entryorguid = 34356 AND source_type = 0;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(34356, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Reset - Set Event Phase 1'),
(34356, 0, 1, 2, 8, 1, 25, 0, 65300, 0, 0, 0, 0, '', 50, 195055, 60, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Spellhit Ping for Artifact (Phase 1) - Summon gameobject 195055'),
(34356, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Link - Set Event Phase 2'),
(34356, 0, 3, 0, 1, 2, 100, 0, 60000, 60000, 60000, 60000, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'OOC Update 60s (Phase 2) - Set Event Phase 1');

-- Add gameobject requirement to cast spell
DELETE FROM gameobject WHERE id=195085;
INSERT INTO gameobject (guid, id, `map`, zoneId, areaId, spawnDifficulties, phaseUseFlags, PhaseId, PhaseGroup, terrainSwapMap, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, isActive, ScriptName, VerifiedBuild) VALUES
(3440000001591, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4878.53, 560.016, 3.49972, 1.38223, -0.0, -0.0, -0.637398, -0.770534, 300, 255, 1, 0, '', 0),
(3440000001590, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4857.51, 515.339, 2.75259, 5.77261, -0.0, -0.0, -0.252524, 0.967591, 300, 255, 1, 0, '', 0),
(3440000001589, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4817.71, 529.705, 4.60765, 5.86293, -0.0, -0.0, -0.208583, 0.978005, 300, 255, 1, 0, '', 0),
(3440000001588, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4780.07, 536.664, 3.87344, 5.80795, -0.0, -0.0, -0.235386, 0.971902, 300, 255, 1, 0, '', 0),
(3440000001587, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4746.02, 554.929, 3.25364, 4.97544, -0.0, -0.0, -0.608266, 0.793733, 300, 255, 1, 0, '', 0),
(3440000001586, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4736.13, 586.678, 5.83519, 4.90867, -0.0, -0.0, -0.634419, 0.772989, 300, 255, 1, 0, '', 0),
(3440000001585, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4718.58, 622.378, 3.32848, 5.30137, -0.0, -0.0, -0.471426, 0.881906, 300, 255, 1, 0, '', 0),
(3440000001584, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4705.41, 668.258, 2.58975, 5.48987, -0.0, -0.0, -0.386337, 0.922358, 300, 255, 1, 0, '', 0),
(3440000001583, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4675.77, 693.6, 2.1374, 5.61946, -0.0, -0.0, -0.325803, 0.945438, 300, 255, 1, 0, '', 0),
(3440000001582, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4641.95, 694.442, 7.10942, 6.1771, -0.0, -0.0, -0.0530192, 0.998594, 300, 255, 1, 0, '', 0),
(3440000001581, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4607.03, 690.928, 12.1337, 0.160946, -0.0, -0.0, -0.0803863, -0.996764, 300, 255, 1, 0, '', 0),
(3440000001580, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4550.75, 689.117, 3.8895, 0.506517, -0.0, -0.0, -0.25056, -0.968101, 300, 255, 1, 0, '', 0),
(3440000001579, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4529.95, 669.275, 5.35318, 0.808892, -0.0, -0.0, -0.393509, -0.919321, 300, 255, 1, 0, '', 0),
(3440000001578, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4496.4, 651.555, 7.40602, 0.290526, -0.0, -0.0, -0.144753, -0.989468, 300, 255, 1, 0, '', 0),
(3440000001577, 195085, 1, 0, 0, '0', 0, 0, 0, -1, 4465.82, 670.57, 4.47169, 4.55131, -0.0, -0.0, -0.761703, 0.647927, 300, 255, 1, 0, '', 0);
