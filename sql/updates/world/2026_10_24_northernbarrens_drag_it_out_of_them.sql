-- https://github.com/HavenWoW/BFA-HavenCore/issues/542
-- Fix the quest objective to default area explored
DELETE FROM quest_objectives WHERE ID=264308 and QuestID=13961;
INSERT INTO quest_objectives (ID, QuestID, `Type`, `Order`, StorageIndex, ObjectID, Amount, Flags, Flags2, ProgressBarWeight, Description, VerifiedBuild) VALUES
(264308, 13961, 10, 0, 0, -1, 0, 0, 0, 0.0, 'Razormane Prisoner Delivered', 35662);

-- SmartAI only works when special flag is setup
UPDATE quest_template_addon SET SpecialFlags=2 WHERE ID=13961;

-- Remove kill credit hack
DELETE FROM smart_scripts WHERE entryorguid=34513;

-- Hogtied Razormane - refactor script
DELETE FROM smart_scripts WHERE entryorguid=34514;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(34514, 0, 0, 1, 25, 0, 100, 0, 0, 0, 0, 0, 0, '', 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On Reset - Enable Run'),
(34514, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 29, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Link - Follow Owner'),
(34514, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 11, 58951, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Link - Feign Death'),
(34514, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 11, 202496, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Link - Burrow'),
(34514, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 11, 65596, 2, 0, 0, 0, 0, 23, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Link - Rope to Owner'),
(34514, 0, 5, 6, 75, 0, 100, 0, 0, 34513, 7, 2000, 0, '', 15, 13961, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Distance to Togrik - Quest Update'),
(34514, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 28, 202496, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Link - Remove Aura burrow'),
(34514, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Link - Despawn');

-- Razormane Pillager - refactor to add quest behavior
DELETE FROM smart_scripts WHERE entryorguid IN(34503);
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(34503, 0, 0, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, '', 25, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Flee at 15% HP'),
(34503, 0, 1, 0, 2, 0, 100, 1, 0, 15, 1000, 1000, 0, '', 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'Say Text at 15% HP'),
(34503, 0, 2, 3, 8, 0, 100, 0, 65580, 0, 0, 0, 0, '', 2, 35, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65580 - change faction 35'),
(34503, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 11, 65581, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65580 - Cast 65581 to himself'),
(34503, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On spellhit 65580 - Despawn in 30s'),
(34503, 0, 5, 6, 62, 0, 100, 0, 10519, 0, 0, 0, 0, '', 85, 65595, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On Gossip option 0 selected - Cast spell 65595 to player'),
(34503, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On Gossip option 0 selected - Despawn');

DELETE FROM quest_offer_reward WHERE ID=13961;
INSERT INTO quest_offer_reward (ID, Emote1, Emote2, Emote3, Emote4, EmoteDelay1, EmoteDelay2, EmoteDelay3, EmoteDelay4, RewardText, VerifiedBuild) VALUES
(13961, 0, 0, 0, 0, 0, 0, 0, 0, 'Don''t be too harsh with our captive, he will still need his head attached to answer our questions.', 35662);

-- Set bunny idle
UPDATE creature SET MovementType=0 WHERE id IN(34527,34346);

-- Captured Razormane - Add cosmetic aura (chain)
DELETE FROM smart_scripts WHERE entryorguid=34523 and id=24;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(34523, 0, 24, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, '', 11, 65612, 16, 0, 0, 0, 0, 19, 34527, 16, 0, 0.0, 0.0, 0.0, 0.0, 'On respawn - Cast 65612 on dummy');

-- Create phase for quest 13961
DELETE FROM phase_area WHERE AreaId=1704 AND PhaseId=3000;
INSERT INTO phase_area (AreaId, PhaseId, Comment) VALUES
(1704, 3000, 'Northern Barrens - Captured Razormane');

DELETE FROM conditions WHERE SourceTypeOrReferenceId=26 AND SourceGroup=3000 AND SourceEntry=1704;
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, ErrorType, ErrorTextId, ScriptName, Comment) VALUES
(26, 3000, 1704, 0, 0, 8, 0, 13961, 0, 0, 0, 0, 0, '', 'Northern Barrens - new phase after quest 13961 is rewarded');

-- Set phase on creature
UPDATE creature SET PhaseId=3000 WHERE guid=303126 AND id=34523;
