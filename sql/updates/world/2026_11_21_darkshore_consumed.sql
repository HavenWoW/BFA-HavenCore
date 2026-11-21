-- https://github.com/HavenWoW/BFA-HavenCore/issues/557
DELETE FROM quest_poi WHERE QuestID=13881;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(13881, 0, 0, -1, 0, 0, 1, 62, 0, 1, 0, 0, 0, 0, 35662),
(13881, 0, 1, 0, 266216, 34302, 1, 62, 0, 1, 0, 0, 0, 0, 35662),
(13881, 0, 2, 1, 266217, -1, 1, 62, 0, 1, 0, 0, 0, 0, 35662);

DELETE FROM quest_poi_points WHERE QuestID=13881;
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild) VALUES
(13881, 0, 0, 5007, 95, 35662),
(13881, 1, 0, 4845, 21, 35662),
(13881, 1, 1, 4891, 85, 35662),
(13881, 1, 2, 4911, 170, 35662),
(13881, 1, 3, 4940, 320, 35662),
(13881, 1, 4, 4898, 342, 35662),
(13881, 1, 5, 4763, 130, 35662),
(13881, 1, 6, 4709, 37, 35662),
(13881, 2, 0, 4806, 37, 35662),
(13881, 2, 1, 4873, 104, 35662),
(13881, 2, 2, 4806, 171, 35662),
(13881, 2, 3, 4739, 104, 35662);

DELETE FROM areatrigger_involvedrelation WHERE id=5449 AND quest=13881;
INSERT INTO areatrigger_involvedrelation (id, quest) VALUES (5449, 13881);
