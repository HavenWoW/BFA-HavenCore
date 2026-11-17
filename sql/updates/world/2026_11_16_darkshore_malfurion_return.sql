-- https://github.com/HavenWoW/BFA-HavenCore/issues/462
DELETE FROM quest_poi WHERE QuestID=13573;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(13573, 0, 0, -1, 0, 0, 1, 62, 0, 1, 0, 0, 0, 0, 35662);

DELETE FROM quest_poi_points WHERE QuestID=13573;
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild) VALUES
(13573, 0, 0, 5919, 194, 66384);
