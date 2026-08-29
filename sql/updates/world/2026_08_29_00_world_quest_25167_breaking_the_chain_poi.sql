-- Quest 25167 - Breaking the Chain
-- Retail POI data absent from the 8.3.7 dump; every other Sen'jin questline
-- entry (25151-25185) carries quest_poi rows, so the tracker arrow and map
-- blob are missing only for this quest.

DELETE FROM `quest_poi_points` WHERE `QuestID` = 25167;
DELETE FROM `quest_poi` WHERE `QuestID` = 25167;

INSERT INTO `quest_poi`
    (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`,
     `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`,
     `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`)
VALUES
    (25167, 0, 0, -1, 0,      0, 1, 1, 0, 0, 0, 0, 0, 0, 35662),  -- overall / quest giver
    (25167, 0, 1,  0, 266185, 0, 1, 1, 0, 0, 0, 0, 0, 0, 35662),  -- Northwatch Supply Crate
    (25167, 0, 2,  1, 266186, 0, 1, 1, 0, 0, 0, 0, 0, 0, 35662),  -- Northwatch Lug
    (25167, 0, 3, 32, 0,      0, 1, 1, 0, 0, 0, 0, 0, 0, 35662);  -- turn in

INSERT INTO `quest_poi_points`
    (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`)
VALUES
    (25167, 0, 0,  -826, -4921, 35662),
    (25167, 1, 0, -1085, -4745, 35662),
    (25167, 1, 1, -1085, -4405, 35662),
    (25167, 1, 2,  -965, -4405, 35662),
    (25167, 1, 3,  -965, -4745, 35662),
    (25167, 2, 0, -1085, -4745, 35662),
    (25167, 2, 1, -1085, -4405, 35662),
    (25167, 2, 2,  -965, -4405, 35662),
    (25167, 2, 3,  -965, -4745, 35662),
    (25167, 3, 0,  -826, -4921, 35662);
