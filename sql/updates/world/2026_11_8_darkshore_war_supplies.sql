-- https://github.com/HavenWoW/BFA-HavenCore/issues/442
UPDATE quest_objectives SET Description='Shatterspear Armaments burned' WHERE ID=267385 AND QuestID=13509;

-- idle bunny
UPDATE creature SET MovementType=0 WHERE id=33056;

-- Add bunny flags for visibility and non attackable
UPDATE creature_template SET unit_flags=33555200 WHERE entry=33056;

-- Fix bunny smart scripts
DELETE FROM smart_scripts WHERE entryorguid=33056;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(33056, 0, 0, 1, 8, 0, 100, 0, 62624, 0, 0, 0, 0, '', 33, 33056, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'SpellHit 62624 - Quest Credit to Invoker');

DELETE FROM conditions WHERE SourceTypeOrReferenceId = 13 AND SourceEntry = 62624;
INSERT INTO conditions
(SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup,
 ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3,
 NegativeCondition, ErrorType, ErrorTextId, ScriptName, Comment)
VALUES
(13, 1, 62624, 0, 0, 31, 0, 3, 33056, 0, 0, 0, 0, '', 'Torch Shatterspear Supplies - target creature 33056'),
(13, 1, 62624, 0, 0, 1, 0, 62624, 0, 0, 1, 0, 0, '', 'Torch Shatterspear Supplies - target must not have aura 62624');
