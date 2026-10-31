-- https://github.com/HavenWoW/BFA-HavenCore/issues/427
DELETE FROM quest_poi WHERE QuestID=25035;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(25035, 0, 0, -1, 0, 0, 1, 463, 0, 2, 0, 0, 0, 0, 35662);

DELETE FROM quest_poi_points WHERE QuestID=25035;
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild) VALUES
(25035, 0, 0, -802, -5555, 35662);
