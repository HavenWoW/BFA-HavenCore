-- Fix quest 14321 POI
DELETE FROM `quest_poi` WHERE `QuestID` = 14321 AND `BlobIndex` = 0 AND `Idx1` = 0;
INSERT INTO `quest_poi` (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES (14321, 0, 0, -1, 0, 0, 654, 545, 0, 1, 0, 0, 0, 0, 0);

DELETE FROM `quest_poi_points` WHERE `QuestID` = 14321 AND `Idx1` = 0 AND `Idx2` = 0;
INSERT INTO `quest_poi_points` (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES (14321, 0, 0, -1865, 2267, 0);
