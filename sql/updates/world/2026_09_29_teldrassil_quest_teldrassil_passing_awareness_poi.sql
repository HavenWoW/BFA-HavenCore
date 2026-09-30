-- Teldrassil: Passing Awareness - fix POI
DELETE FROM quest_poi WHERE QuestID=28731;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(28731, 0, 0, -1, 267468, 5186, 1, 57, 0, 0, 0, 0, 0, 0, 0);
