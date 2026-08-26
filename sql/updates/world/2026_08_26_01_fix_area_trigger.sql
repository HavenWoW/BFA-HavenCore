-- Area Trigger issues for quests, multiple issues will be closed.
-- --------------------------------------------------------------

-- A Lost Companion (13564) - Fixed area trigger not triggering. (Issue #392)
DELETE FROM `smart_scripts` WHERE `entryorguid` = 5972 AND `source_type` = 2;
INSERT INTO `smart_scripts` (
  `entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
  `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`,
  `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
  `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`
) VALUES
  (5972, 2, 0, 0, 46, 0, 100, 0, 5972, 0, 0, 0, 0, '', 15, 13564, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'A Lost Companion - Area Trigger');
  
UPDATE `quest_template_addon` SET
  `SpecialFlags` = 2
WHERE `ID` = 13564;

-- Only done because it doesnt work.
DELETE FROM `quest_objectives` WHERE `QuestID` = 13564;
INSERT INTO `quest_objectives` (`ID`, `QuestID`, `Type`, `Order`, `StorageIndex`, `ObjectID`, `Amount`, `Flags`, `Flags2`, `ProgressBarWeight`, `Description`, `VerifiedBuild`)
VALUES
  (267649, 13564, 10, 0, 0, 5972, 0, 0, 0, 0, NULL, 35662);

  
-- The Fargodeep Mine (62) - Added the last area trigger that was missing.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 197 AND `source_type` = 2;
INSERT INTO `smart_scripts` (
  `entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
  `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`,
  `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
  `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`
) VALUES
  (197, 2, 0, 0, 46, 0, 100, 0, 197, 0, 0, 0, 0, '', 15, 62, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Fargodeep Mine - Area Trigger - ID 197'),
  (197, 2, 1, 0, 46, 0, 100, 0, 88, 0, 0, 0, 0, '', 15, 62, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Fargodeep Mine - Area Trigger - ID 88');
  
  
-- Investigate the Wreckage (14066) - Updated the SmartAI comment field + Add the area trigger to ObjectID
DELETE FROM `smart_scripts` WHERE `entryorguid` = 5482 AND `source_type` = 2;
INSERT INTO `smart_scripts` (
  `entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
  `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`,
  `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
  `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`
) VALUES
  (5482, 2, 0, 0, 46, 0, 100, 0, 5482, 0, 0, 0, 0, '', 15, 14066, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Investigate the Wreckage - Area Trigger');

UPDATE `quest_template_addon` SET
  `SpecialFlags` = 2
WHERE `ID` = 14066;

DELETE FROM `quest_objectives` WHERE `QuestID` = 14066;
INSERT INTO `quest_objectives` (`ID`, `QuestID`, `Type`, `Order`, `StorageIndex`, `ObjectID`, `Amount`, `Flags`, `Flags2`, `ProgressBarWeight`, `Description`, `VerifiedBuild`)
VALUES
  (264415, 14066, 10, 0, 0, 5482, 0, 0, 0, 0, NULL, 35662);



-- The Forgotten Pools (870) - Updated SmartAI comment field.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 216 AND `source_type` = 2;
INSERT INTO `smart_scripts` (
  `entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
  `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`,
  `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
  `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`
) VALUES
  (216, 2, 0, 0, 46, 0, 100, 0, 216, 0, 0, 0, 0, '', 15, 870, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'The Forgotten Pools - Area Trigger');