-- Quest The Final Flame of Bashal'Aran (ID 13562)
DELETE FROM `quest_poi` WHERE `QuestID` = 13562;
INSERT INTO `quest_poi` (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`)
VALUES
  (13562, 0, 0, -1, 0, 0, 1, 62, 0, 0, 0, 0, 0, 0, 0),
  (13562, 0, 1, 0, 0, 0, 1, 62, 0, 0, 0, 0, 0, 0, 0),
  (13562, 0, 2, 32, 0, 0, 1, 62, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `quest_poi_points` WHERE `QuestID` = 13562;
INSERT INTO `quest_poi_points` (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`)
VALUES
  (13562, 0, 0, 7372, -259, 0),
  (13562, 1, 0, 6748, 48, 0),
  (13562, 3, 0, 7372, -259, 0);


-- Quest (13562) - Fix description
INSERT IGNORE INTO 
`quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`) 
VALUES (13562, 0, 0, 0, 0, 0, 0, 0, 0, 'You have our thanks. It is time that we leave the past behind us, and start looking to our future.', 0);


-- NPC Ranger Glynda Nal'Shea (ID 32971) - Remove Aura ID - 49414 Generic Quest Invisibility 1
UPDATE `creature_template_addon` SET
  `auras` = NULL
WHERE `entry` = 32971;