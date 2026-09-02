-- Quest 24782 - The Rise of the Darkspear
-- Restore the missing route from Jin'thala to Tunari.

DELETE FROM `quest_poi_points` WHERE `QuestID` = 24782;
DELETE FROM `quest_poi` WHERE `QuestID` = 24782;

INSERT INTO `quest_poi`
    (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`,
     `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`,
     `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`)
VALUES
    (24782, 0, 0, -1, 0, 0, 1, 463, 0, 0, 0, 0,      0, 0, 35662),
    (24782, 0, 1, 32, 0, 0, 1, 463, 0, 0, 0, 0, 345555, 0, 35662);

INSERT INTO `quest_poi_points`
    (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`)
VALUES
    (24782, 0, 0, -1318, -5604, 35662),
    (24782, 1, 0, -1118, -5540, 35662);
