-- Fix https://github.com/HavenWoW/BFA-HavenCore/issues/149

/* Fix quest 9133 objective */
UPDATE `quest_poi` SET `QuestObjectiveID` = 262444, `QuestObjectID` = 22549 WHERE `QuestID` = 9133 AND `BlobIndex` = 0 AND `Idx1` = 0;

/* Fix quest 8491 objective */
DELETE FROM `quest_poi` WHERE `QuestID` = 8491;
INSERT INTO `quest_poi` (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES 
(8491, 0, 0, 0, 256671, 20772, 530, 94, 0, 1, 0, 0, 0, 0, 0),
(8491, 0, 1, 0, 256671, 20772, 530, 94, 0, 1, 0, 0, 0, 0, 0),
(8491, 0, 2, 0, 256671, 20772, 530, 94, 0, 1, 0, 0, 0, 0, 0),
(8491, 0, 3, 0, 256671, 20772, 530, 94, 0, 1, 0, 0, 0, 0, 0),
(8491, 0, 4, -1, 0, 0, 530, 94, 0, 1, 0, 0, 0, 0, 0);

DELETE FROM `quest_poi_points` WHERE `QuestID` = 8491;
INSERT INTO `quest_poi_points` (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES 
(8491, 0, 0, 8718, -6284, 0),
(8491, 0, 1, 8682, -6184, 0),
(8491, 0, 2, 8648, -6150, 0),
(8491, 0, 3, 8587, -6212, 0),
(8491, 1, 0, 8715, -6888, 0),
(8491, 1, 1, 8783, -6884, 0),
(8491, 1, 2, 9018, -6851, 0),
(8491, 1, 3, 8985, -6481, 0),
(8491, 1, 4, 8853, -6351, 0),
(8491, 1, 5, 8814, -6392, 0),
(8491, 2, 0, 8416, -6050, 0),
(8491, 2, 1, 8486, -6048, 0),
(8491, 2, 2, 8680, -5921, 0),
(8491, 2, 3, 8684, -5785, 0),
(8491, 2, 4, 8482, -5651, 0),
(8491, 2, 5, 8350, -5716, 0),
(8491, 2, 6, 8320, -5748, 0),
(8491, 2, 7, 8377, -6015, 0),
(8491, 3, 0, 8514, -6621, 0),
(8491, 3, 1, 8583, -6617, 0),
(8491, 3, 2, 8720, -6551, 0),
(8491, 3, 3, 8717, -6484, 0),
(8491, 3, 4, 8617, -6447, 0),
(8491, 4, 0, 8756, -6690, 0);