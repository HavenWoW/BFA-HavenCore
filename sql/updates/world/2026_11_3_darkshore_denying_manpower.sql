-- https://github.com/HavenWoW/BFA-HavenCore/issues/441
DELETE FROM quest_poi WHERE QuestID=13507;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(13507, 0, 0, -1, 0, 0, 1, 62, 0, 1, 0, 0, 0, 0, 35662),
(13507, 0, 1, 0, 253227, 32859, 1, 62, 0, 1, 0, 0, 0, 0, 35662),
(13507, 0, 2, 1, 253228, 34248, 1, 62, 0, 1, 0, 0, 0, 0, 35662);


DELETE FROM quest_poi_points WHERE QuestID=13507;
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild) VALUES
(13507, 0, 0, 7386, -791, 35662),
(13507, 1, 0, 7760, -1087, 35662),
(13507, 1, 1, 7884, -1073, 35662),
(13507, 1, 2, 7910, -1033, 35662),
(13507, 1, 3, 7915, -901, 35662),
(13507, 1, 4, 7743, -894, 35662),
(13507, 1, 5, 7703, -957, 35662),
(13507, 1, 6, 7715, -1046, 35662),
(13507, 2, 0, 7869, -1124, 35662),
(13507, 2, 1, 7918, -1116, 35662),
(13507, 2, 2, 7954, -1050, 35662),
(13507, 2, 3, 7875, -950, 35662),
(13507, 2, 4, 7827, -936, 35662),
(13507, 2, 5, 7727, -1028, 35662);
