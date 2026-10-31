-- https://github.com/HavenWoW/BFA-HavenCore/issues/370
DELETE FROM quest_poi WHERE QuestID = 13529;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(13529, 0, 0, -1, 0, 0, 1, 62, 0, 0, 0, 0, 0, 0, 35662),
(13529, 0, 1, 0, 266620, 33020, 1, 62, 0, 0, 0, 0, 0, 0, 35662),
(13529, 0, 2, 1, 266621, 33021, 1, 62, 0, 0, 0, 0, 0, 0, 35662),
(13529, 0, 3, 32, 0, 0, 1, 62, 0, 0, 0, 0, 279465, 0, 35662);

DELETE FROM quest_poi_points WHERE QuestID=13529;
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild) VALUES
(13529, 0, 0, 7373, -288, 35662),
(13529, 2, 0, 6858, -796, 35662),
(13529, 2, 1, 6876, -767, 35662),
(13529, 2, 2, 6898, -636, 35662),
(13529, 2, 3, 6874, -634, 35662),
(13529, 2, 4, 6739, -639, 35662),
(13529, 2, 5, 6717, -646, 35662),
(13529, 2, 6, 6719, -692, 35662),
(13529, 2, 7, 6806, -776, 35662),
(13529, 3, 0, 6926, -654, 35662);
