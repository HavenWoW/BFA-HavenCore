-- Area Trigger issues for quests, multiple issues will be closed.
-- --------------------------------------------------------------

-- A Lost Companion (13564) - Fixed area trigger not triggering. (Issue #392)
DELETE FROM `quest_objectives` WHERE `QuestID` = 13564;
INSERT INTO `quest_objectives` (`ID`, `QuestID`, `Type`, `Order`, `StorageIndex`, `ObjectID`, `Amount`, `Flags`, `Flags2`, `ProgressBarWeight`, `Description`, `VerifiedBuild`)
VALUES
  (267649, 13564, 10, 0, 0, 5972, 0, 0, 0, 0, NULL, 35662);

  
-- The Fargodeep Mine (62) - Fixed area trigger not triggering. (Issue #92)
DELETE FROM `quest_objectives` WHERE `QuestID` = 62;
INSERT INTO `quest_objectives` (`ID`, `QuestID`, `Type`, `Order`, `StorageIndex`, `ObjectID`, `Amount`, `Flags`, `Flags2`, `ProgressBarWeight`, `Description`, `VerifiedBuild`)
VALUES
  (252033, 62, 10, 0, 0, 88, 0, 0, 0, 0, NULL, 35662);
  
  
-- The Jasperlode Mine (76) - Fixed area trigger not triggering. (Issue #117)
DELETE FROM `quest_objectives` WHERE `QuestID` = 76;
INSERT INTO `quest_objectives` (`ID`, `QuestID`, `Type`, `Order`, `StorageIndex`, `ObjectID`, `Amount`, `Flags`, `Flags2`, `ProgressBarWeight`, `Description`, `VerifiedBuild`)
VALUES
  (252154, 76, 10, 0, 0, 87, 0, 0, 0, 0, NULL, 35662);
  
  
-- Investigate the Wreckage (14066) - Fixed area trigger not triggering.
DELETE FROM `quest_objectives` WHERE `QuestID` = 14066;
INSERT INTO `quest_objectives` (`ID`, `QuestID`, `Type`, `Order`, `StorageIndex`, `ObjectID`, `Amount`, `Flags`, `Flags2`, `ProgressBarWeight`, `Description`, `VerifiedBuild`)
VALUES
  (264415, 14066, 10, 0, 0, 5482, 0, 0, 0, 0, NULL, 35662);

UPDATE `quest_template_addon` SET
  `SpecialFlags` = 2
WHERE `ID` = 14066;


-- The Forgotten Pools (870) - Fixed area trigger not triggering. (#98)
DELETE FROM `quest_objectives` WHERE `QuestID` = 870;
INSERT INTO `quest_objectives` (`ID`, `QuestID`, `Type`, `Order`, `StorageIndex`, `ObjectID`, `Amount`, `Flags`, `Flags2`, `ProgressBarWeight`, `Description`, `VerifiedBuild`)
VALUES
  (254343, 870, 10, 0, 0, 216, 0, 0, 0, 0, NULL, 35662);