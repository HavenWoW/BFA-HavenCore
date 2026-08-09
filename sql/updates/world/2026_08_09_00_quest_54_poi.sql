-- Quest 54 (Report to Goldshire)

DELETE FROM quest_poi_points
WHERE QuestID = 54;

DELETE FROM quest_poi
WHERE QuestID = 54;

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
(54, 0, 0, -1, 0, 0, 0, 425, 0, 1, 0, 0, 0, 0, 35662);

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
(54, 0, 0, -9463, 74, 35662);