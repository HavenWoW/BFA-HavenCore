-- Quest 26817 - Perfectly Pure
-- Adds the missing objective POI for The Holy Spring.
-- The quest previously displayed no highlighted objective area on the
-- minimap or world map because it had no quest_poi or quest_poi_points data.

DELETE FROM `quest_poi_points`
WHERE `QuestID` = 26817;

DELETE FROM `quest_poi`
WHERE `QuestID` = 26817;

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
    26817,      -- Perfectly Pure
    0,
    0,
    0,          -- First quest objective
    267658,     -- Obtain The Holy Water of Clarity
    60386,      -- The Holy Water of Clarity
    0,          -- Eastern Kingdoms
    210,        -- The Cape of Stranglethorn
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
    (26817, 0, 0, -13828, 348, 35662),
    (26817, 0, 1, -13778, 348, 35662),
    (26817, 0, 2, -13768, 373, 35662),
    (26817, 0, 3, -13778, 398, 35662),
    (26817, 0, 4, -13828, 398, 35662),
    (26817, 0, 5, -13838, 373, 35662);
