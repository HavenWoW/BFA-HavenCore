DELETE FROM quest_poi_points
WHERE QuestID = 26671;

DELETE FROM quest_poi
WHERE QuestID = 26671;

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
(26671, 0, 0, -1, 0, 0, 0, 47, 0, 1, 0, 0, 0, 0, 35662),
(26671, 0, 1, 32, 0, 0, 0, 47, 0, 0, 0, 0, 11829, 0, 35662);

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
(26671, 0, 0, -10512, -1301, 35662),
(26671, 1, 0, -10561, -1123, 35662);
