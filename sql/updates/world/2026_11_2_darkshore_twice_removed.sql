-- https://github.com/HavenWoW/BFA-HavenCore/issues/432
UPDATE quest_objectives SET Description='Lady Janira slain' WHERE ID=266153 AND QuestID=13565;
UPDATE quest_objectives SET Description='Withered Ents called' WHERE ID=266154 AND QuestID=13565;

-- Withered Ent - Delete summoned creatures
DELETE FROM creature WHERE id=34009;

-- Remove kill credit hotfix
UPDATE creature_template SET KillCredit2=0 WHERE entry=33206;

DELETE FROM conditions WHERE SourceTypeOrReferenceId = 13 AND SourceEntry = 64306;
INSERT INTO conditions
(SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup,
 ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3,
 NegativeCondition, ErrorType, ErrorTextId, ScriptName, Comment)
VALUES
(13, 3, 64306, 0, 0, 31, 0, 3, 33206, 0, 0, 0, 0, '', 'Spell 64306 - target creature 33206'),
(13, 3, 64306, 0, 0, 36, 0, 0, 0, 0, 1, 0, 0, '', 'Spell 64306 - target must be dead');

-- Withered Ent - cast 11442 on damage
DELETE FROM smart_scripts WHERE entryorguid = 34009 AND source_type = 0;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(34009, 0, 0, 0, 33, 0, 15, 0, 0, 0, 3000, 3000, 0, '', 11, 11442, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Withered Ent - On damaging target (15%) - Cast Withering Touch'),
(34009, 0, 1, 2, 54, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 34010, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Withered Ent - Just summoned - Kill credit to invoker'),
(34009, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 29, 15, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Withered Ent - Linked - Follow invoker');
