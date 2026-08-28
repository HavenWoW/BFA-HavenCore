/* Quest: Beer Basted Boar Ribs (384) */
DELETE FROM `quest_poi` WHERE `QuestID` = 384;
INSERT INTO `quest_poi` (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES 
(384, 0, 0, -1, 0, 0, 0, 27, 0, 1, 0, 0, 0, 0, 0),
(384, 0, 1, 0, 252712, 60496, 0, 27, 0, 7, 0, 0, 0, 0, 0),
(384, 0, 2, 1, 252713, 2894, 0, 27, 0, 7, 0, 0, 0, 0, 0),
(384, 1, 1, 0, 252712, 60496, 0, 27, 0, 7, 0, 0, 0, 0, 0),
(384, 2, 2, 0, 252713, 2894, 0, 27, 0, 7, 0, 0, 0, 0, 0);

DELETE FROM `quest_poi_points` WHERE `QuestID` = 384;
INSERT INTO `quest_poi_points` (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES 
(384, 0, 0, -5596, -504, 22908),
(384, 1, 0, -5538, -838, 22908),
(384, 1, 1, -5456, -744, 22908),
(384, 1, 2, -5377, -584, 22908),
(384, 1, 3, -5358, -486, 22908),
(384, 1, 4, -5385, -337, 22908),
(384, 1, 5, -5467, -203, 22908),
(384, 1, 6, -5648, -102, 22908),
(384, 1, 7, -5789, -86, 22908),
(384, 1, 8, -5926, -200, 22908),
(384, 1, 9, -5926, -329, 22908),
(384, 1, 10, -5887, -548, 22908),
(384, 1, 11, -5683, -752, 22908),
(384, 2, 0, -5581, -482, 22908);


DELETE FROM `creature_questitem` WHERE `CreatureEntry` IN (1682, 7744, 44006, 100012, 1247);
INSERT INTO `creature_questitem` (`CreatureEntry`, `Idx`, `ItemId`, `VerifiedBuild`) VALUES 
(1682, 0, 2894, 35662),
(7744, 0, 2894, 35662),
(44006, 0, 2894, 35662),
(100012, 0, 2894, 35662),
(1247, 0, 2894, 35662);