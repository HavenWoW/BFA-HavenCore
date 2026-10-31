-- https://github.com/HavenWoW/BFA-HavenCore/issues/372
-- Remove quest requirement for loot
UPDATE creature_loot_template SET QuestRequired=0 WHERE Entry=33022 AND Item=44927;
UPDATE creature_loot_template SET QuestRequired=0 WHERE Entry=33020 AND Item=44927;

-- Add quest description
UPDATE quest_objectives SET Description='Uncorrupted animals freed' WHERE ID=256238 AND QuestId=13557;

-- Fix poi index
UPDATE quest_poi SET BlobIndex=0 WHERE QuestID=13557 AND Idx1=1;

-- Set 45 seconds reset
UPDATE gameobject_template SET Data3=45000 WHERE entry=194124;
UPDATE gameobject_template SET Data3=45000 WHERE entry=194133;

-- Match gameobject reset
UPDATE creature SET spawntimesecs=40 WHERE id IN(33023,33024);

-- Secure Bear Cage
DELETE FROM smart_scripts WHERE entryorguid = 194124;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(194124, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, '', 45, 0, 1, 0, 0, 0, 0, 11, 33024, 2, 0, 0.0, 0.0, 0.0, 0.0, 'open - set data'),
(194124, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 33024, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'link - killcredit');

-- Secure Duskrat Cage
DELETE FROM smart_scripts WHERE entryorguid = 194133;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(194133, 1, 0, 1, 70, 0, 100, 0, 2, 0, 0, 0, 0, '', 45, 0, 1, 0, 0, 0, 0, 11, 33023, 2, 0, 0.0, 0.0, 0.0, 0.0, 'open - set data'),
(194133, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 33024, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'link - killcreditt');

-- Uncorrupted Thistle Bear
DELETE FROM smart_scripts WHERE entryorguid IN(33023,33024);
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(33023, 0, 0, 1, 38, 0, 100, 0, 0, 1, 60000, 60000, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'ActionList 0ms - Set Run Mode'),
(33023, 0, 1, 2, 61, 0, 100, 0, 0, 1, 0, 0, 0, '', 114, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 8.0, 0.0, 0.0, 'data - move forward'),
(33023, 0, 2, 0, 61, 0, 100, 0, 0, 1, 0, 0, 0, '', 41, 4500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'link - despawn'),
(33024, 0, 0, 1, 38, 0, 100, 0, 0, 1, 60000, 60000, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'ActionList 0ms - Set Run Mode'),
(33024, 0, 1, 2, 61, 0, 100, 0, 0, 1, 0, 0, 0, '', 114, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 8.0, 0.0, 0.0, 'data - move forward'),
(33024, 0, 2, 0, 61, 0, 100, 0, 0, 1, 0, 0, 0, '', 41, 4500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'link - despawn');
