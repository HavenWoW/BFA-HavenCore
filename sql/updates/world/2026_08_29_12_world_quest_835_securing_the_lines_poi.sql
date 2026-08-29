-- Quest 835 - Securing the Lines
-- Retail POI data absent from the 8.3.7 dump: quest_poi/quest_poi_points carry
-- no rows, so the Durotar map blob and the objective tracker arrow never appear.
-- Sibling quest 834 (Winds in the Desert), same giver (Rezlak) and same zone,
-- carries full POI data and is the model here.
-- Objective creature 41007 (Durotar Harpy) has no spawns of its own; it is a
-- kill-credit bucket satisfied by Dustwind Harpy/Pillager/Savage/Storm Witch
-- (3115-3118), which cluster in the Dustwind Dunes area southeast of Razor Hill.

DELETE FROM `quest_poi_points` WHERE `QuestID` = 835;
DELETE FROM `quest_poi` WHERE `QuestID` = 835;

INSERT INTO `quest_poi`
    (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`,
     `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`,
     `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`)
VALUES
    (835, 0, 0, -1, 0,      0, 1, 1, 0, 0, 0, 0, 0, 0, 35662),  -- overall / quest giver
    (835, 0, 1,  0, 253951, 0, 1, 1, 0, 0, 0, 0, 0, 0, 35662),  -- Durotar Harpy kill area
    (835, 0, 2, 32, 0,      0, 1, 1, 0, 0, 0, 0, 0, 0, 35662);  -- turn in

INSERT INTO `quest_poi_points`
    (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`)
VALUES
    (835, 0, 0, 1000, -4414, 35662),  -- Rezlak (quest giver)
    (835, 1, 0,  610, -4410, 35662),  -- Dustwind spawn area blob
    (835, 1, 1,  610, -4810, 35662),
    (835, 1, 2, 1130, -4810, 35662),
    (835, 1, 3, 1130, -4410, 35662),
    (835, 2, 0, 1000, -4414, 35662);  -- Rezlak (turn in)
