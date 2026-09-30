-- Clean up old unused POI and add new ones for quests.
DELETE FROM quest_poi WHERE QuestID IN(997,2518,6341,6343);
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(997, 0, 0, -1, 0, 0, 1, 57, 0, 1, 0, 0, 0, 0, 35662),
(2518, 0, 0, -1, 0, 0, 1, 57, 0, 1, 0, 0, 0, 0, 35662),
(2518, 0, 1, 0, 256358, 8344, 1, 57, 0, 1, 0, 0, 0, 0, 35662),
(6341, 0, 0, -1, 0, 0, 1, 57, 0, 1, 0, 0, 0, 0, 35662),
(6343, 0, 0, -1, 0, 0, 1, 57, 0, 1, 0, 0, 0, 0, 35662);


DELETE FROM quest_poi_points WHERE QuestID IN(997,2518,6341,6343);
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild)
VALUES
(997, 0, 0, 9507, 714, 35662),
(2518, 0, 0, 10677, 1933, 35662),
(2518, 1, 0, 10977, 1846, 35662),
(6341, 0, 0, 9915, 2632, 35662),
(6343, 0, 0, 9752, 903, 35662);
