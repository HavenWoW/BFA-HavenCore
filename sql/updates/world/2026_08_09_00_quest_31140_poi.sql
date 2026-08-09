-- Quest 31140 (Lions for Lambs)

DELETE FROM quest_poi_points
WHERE QuestID = 31140;

DELETE FROM quest_poi
WHERE QuestID = 31140;

INSERT INTO quest_poi
(
    QuestID,
    BlobIndex,
    Idx1,
    ObjectiveIndex,
    QuestObjectiveID,
    QuestObjectID,
    MapID,
    UiMapID,
    Priority,
    Flags,
    WorldEffectID,
    PlayerConditionID,
    SpawnTrackingID,
    AlwaysAllowMergingBlobs,
    VerifiedBuild
)
VALUES
(31140, 0, 0, -1, 0, 0, 0, 425, 0, 1, 0, 0, 0, 0, 35662),
(31140, 0, 1, 0, 268166, 49874, 0, 425, 0, 1, 0, 0, 0, 0, 35662),
(31140, 0, 2, 32, 0, 0, 0, 425, 0, 0, 0, 0, 0, 0, 35662);

INSERT INTO quest_poi_points
(
    QuestID,
    Idx1,
    Idx2,
    X,
    Y,
    VerifiedBuild
)
VALUES
(31140, 2, 0, -8914, -138, 35662),
(31140, 1, 9, -8915, -114, 35662),
(31140, 1, 8, -8961, -96, 35662),
(31140, 1, 7, -8985, -66, 35662),
(31140, 1, 6, -8987, -32, 35662),
(31140, 1, 5, -8969, -22, 35662),
(31140, 1, 4, -8947, -15, 35662),
(31140, 1, 3, -8795, -46, 35662),
(31140, 1, 2, -8784, -100, 35662),
(31140, 1, 1, -8835, -167, 35662),
(31140, 1, 0, -8870, -138, 35662),
(31140, 0, 0, -8913, -137, 35662);