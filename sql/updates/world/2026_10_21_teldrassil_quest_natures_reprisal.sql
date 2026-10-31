-- https://github.com/HavenWoW/BFA-HavenCore/issues/322
-- Add quest description
UPDATE quest_objectives SET Description='Fel Rock grellkin killed with Ireroot Seeds' WHERE ID=265120 AND QuestID = 13946;

-- Remove hack kill credit
--  Auto-generated SQL script #202610192140
UPDATE creature_template SET KillCredit1=0 WHERE entry IN(2002,2003,2004,2005);

-- Allow spell 65455 to only work in this creatures
DELETE FROM conditions WHERE SourceTypeOrReferenceId = 13 AND SourceEntry = 65455;
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, ErrorType, ErrorTextId, ScriptName, Comment) VALUES
(13, 2, 65455, 0, 0, 31, 0, 3, 2002, 0, 0, 0, 0, '', 'Nature''s Fury can hit only Rascal Sprite'),
(13, 2, 65455, 0, 1, 31, 0, 3, 2003, 0, 0, 0, 0, '', 'Nature''s Fury can hit only Shadow Sprite'),
(13, 2, 65455, 0, 2, 31, 0, 3, 2004, 0, 0, 0, 0, '', 'Nature''s Fury can hit only Dark Sprite'),
(13, 2, 65455, 0, 3, 31, 0, 3, 2005, 0, 0, 0, 0, '', 'Nature''s Fury can hit only Vicious Grell');

-- Rascal Sprite - refactor to add spellhit behavior
DELETE FROM smart_scripts WHERE entryorguid IN(2002,200200);
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(2002, 0, 0, 0, 0, 0, 100, 0, 5000, 12000, 30000, 40000, 0, '', 11, 6950, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Cast Faerie Fire'),
(2002, 0, 1, 2, 8, 0, 100, 0, 65455, 0, 0, 0, 0, '', 11, 65462, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65455 - Cast Explode Demon Shrink Countdown on self'),
(2002, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 34440, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65455 - kill credit');

-- Shadow Sprite - refactor to add spellhit behavior
DELETE FROM smart_scripts WHERE entryorguid IN(2003,200300);
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(2003, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Set Phase 1 on Aggro'),
(2003, 0, 1, 0, 4, 1, 100, 1, 0, 0, 0, 0, 0, '', 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Stop Moving on Aggro'),
(2003, 0, 1, 2, 2, 0, 100, 1, 0, 15, 0, 0, 0, '', 25, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Flee at 15% HP'),
(2003, 0, 2, 0, 4, 1, 100, 1, 0, 0, 0, 0, 0, '', 11, 9613, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Cast bolt on Aggro'),
(2003, 0, 3, 0, 9, 1, 100, 0, 0, 40, 3400, 4700, 0, '', 11, 9613, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Cast bolt'),
(2003, 0, 4, 0, 9, 1, 100, 0, 40, 100, 1000, 1000, 0, '', 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Start Moving when not in bolt Range'),
(2003, 0, 5, 0, 9, 1, 100, 0, 10, 15, 1000, 1000, 0, '', 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Stop Moving at 15 Yards'),
(2003, 0, 6, 0, 9, 1, 100, 0, 0, 40, 1000, 1000, 0, '', 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Stop Moving when in bolt Range'),
(2003, 0, 7, 0, 3, 1, 100, 0, 0, 15, 1000, 1000, 0, '', 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Set Phase 2 at 15% Mana'),
(2003, 0, 8, 0, 3, 2, 100, 0, 0, 15, 1000, 1000, 0, '', 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Start Moving at 15% Mana'),
(2003, 0, 9, 0, 3, 2, 100, 0, 30, 100, 100, 100, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Set Phase 1 When Mana is above 30%'),
(2003, 0, 10, 0, 2, 2, 100, 1, 0, 15, 1000, 1000, 0, '', 22, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Set Phase 3 at 15% HP'),
(2003, 0, 11, 0, 2, 3, 100, 1, 0, 15, 1000, 1000, 0, '', 25, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Flee at 15% HP'),
(2003, 0, 12, 0, 7, 3, 100, 1, 0, 0, 0, 0, 0, '', 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Reset on Evade'),
(2003, 0, 13, 0, 2, 3, 100, 1, 0, 15, 1000, 1000, 0, '', 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Say Text at 15% HP'),
(2003, 0, 14, 15, 8, 0, 100, 0, 65455, 0, 0, 0, 0, '', 11, 65462, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65455 - Cast Explode Demon Shrink Countdown on self'),
(2003, 0, 15, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 34440, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65455 - kill credit');

-- Dark Sprite - refactor to add spellhit behavior
DELETE FROM smart_scripts WHERE entryorguid=2004;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(2004, 0, 0, 0, 0, 0, 100, 0, 4000, 8000, 15000, 21000, 0, '', 11, 5514, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Cast Darken Vision'),
(2004, 0, 1, 2, 8, 0, 100, 0, 65455, 0, 0, 0, 0, '', 11, 65462, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65455 - Cast Explode Demon Shrink Countdown on self'),
(2004, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 34440, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65455 - kill credit');

-- Vicious Grell - refactor to add spellhit behavior
DELETE FROM smart_scripts WHERE entryorguid=2005;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(2005, 0, 0, 0, 2, 0, 100, 0, 0, 30, 10000, 16000, 0, '', 11, 5515, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Cast Savagery at 30% HP'),
(2005, 0, 1, 2, 8, 0, 100, 0, 65455, 0, 0, 0, 0, '', 11, 65462, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65455 - Cast Explode Demon Shrink Countdown on self'),
(2005, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 34440, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65455 - kill credit');
