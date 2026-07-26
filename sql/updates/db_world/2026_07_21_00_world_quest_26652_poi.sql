-- Quest 26652 - Ghost Hair Thread
-- Add the missing quest POI for Blind Mary.
-- Fixes the quest location not appearing on the minimap/world map.

DELETE FROM `quest_poi_points`
WHERE `QuestID` = 26652;

DELETE FROM `quest_poi`
WHERE `QuestID` = 26652;

INSERT INTO `quest_poi`
(
    `QuestID`,
    `BlobIndex`,
    `Idx1`,
    `ObjectiveIndex`,
    `QuestObjectiveID`,
    `QuestObjectID`,
    `MapID`,
    `UiMapID`,
    `Priority`,
    `Flags`,
    `WorldEffectID`,
    `PlayerConditionID`,
    `SpawnTrackingID`,
    `AlwaysAllowMergingBlobs`,
    `VerifiedBuild`
)
VALUES
(
    26652,  -- Ghost Hair Thread
    0,
    0,
    -1,     -- General quest-location POI
    0,
    0,
    0,      -- Eastern Kingdoms
    42,     -- Duskwood
    0,
    1,
    0,
    0,
    0,
    0,
    35662
);

INSERT INTO `quest_poi_points`
(
    `QuestID`,
    `Idx1`,
    `Idx2`,
    `X`,
    `Y`,
    `VerifiedBuild`
)
VALUES
(
    26652,
    0,
    0,
    -10778,
    -1378,
    35662
);
