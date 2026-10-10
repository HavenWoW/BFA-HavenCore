-- https://github.com/HavenWoW/BFA-HavenCore/issues/93

DELETE FROM quest_poi WHERE QuestID=87;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(87, 0, 0, -1, 0, 0, 0, 37, 0, 1, 0, 0, 0,0, 35662),
(87, 0, 1, 0, 252453, 981, 0, 37, 0, 1, 0, 0, 0, 0, 35662),
(87, 0, 2, 32, 0, 0, 0, 37, 0, 0, 0, 0, 11771, 0, 35662);

DELETE FROM quest_poi_points WHERE QuestID=87;
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild) VALUES
(87, 0, 0, -9890, 338, 35662),
(87, 1, 0, -9806, 142, 35662),
(87, 2, 0, -9924, 38, 35662);
