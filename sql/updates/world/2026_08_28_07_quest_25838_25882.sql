/* Quest: Help from Steelgrill's Depot (25838) */
DELETE FROM `quest_poi` WHERE `QuestID` = 25838;
INSERT INTO `quest_poi` (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES (25838, 0, 0, -1, 0, 0, 0, 27, 0, 1, 0, 0, 0, 0, 23877);

DELETE FROM `quest_poi_points` WHERE `QuestID` = 25838;
INSERT INTO `quest_poi_points` (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES (25838, 0, 0, -5479, -646, 22908);

/* Quest: A Hand at the Ranch (25882) */
DELETE FROM `quest_poi` WHERE `QuestID` = 25882;
INSERT INTO `quest_poi` (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES (25882, 0, 0, -1, 0, 0, 0, 27, 0, 1, 0, 0, 0, 0, 23877);

DELETE FROM `quest_poi_points` WHERE `QuestID` = 25882;
INSERT INTO `quest_poi_points` (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES (25882, 0, 0, -5539, -1311, 22908);