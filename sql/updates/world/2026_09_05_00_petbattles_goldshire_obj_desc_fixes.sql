-- GOLDSHIRE

-- Updated quest completion text for quest 31308
DELETE FROM `quest_offer_reward` WHERE `ID` = 31308;

INSERT INTO `quest_offer_reward` (`ID`, `RewardText`, `VerifiedBuild`) 
VALUES (31308, 'Nice work, $n! You''re better with those pets than I thought!', 0);

-- Fix Objective 31309
UPDATE `quest_objectives` 
SET `Description` = 'Battle Pets Healed' 
WHERE `QuestID` = 31309;

-- Fix Completion Description 31309
DELETE FROM `quest_offer_reward` WHERE `ID` = 31309;

INSERT INTO `quest_offer_reward` (`ID`, `RewardText`, `VerifiedBuild`) 
VALUES (31309, 'Now those look like some healthy pets, $n. Remember, you can heal and revive your entire collection of pets at any time by visiting a stable master.', 0);

-- Clean up old entry and insert completion text for quest 31785
DELETE FROM `quest_offer_reward` WHERE `ID` = 31785;

INSERT INTO `quest_offer_reward` (`ID`, `RewardText`, `VerifiedBuild`) 
VALUES (31785, 'Great job $n. Next up, catching your own pets!', 0);

-- Update Quest Objective Description for 31550
UPDATE `quest_objectives` 
SET `Description` = 'Catch a Pet' 
WHERE `QuestID` = 31550;

-- Delete old completion text and insert new completion text for 31550
DELETE FROM `quest_offer_reward` WHERE `ID` = 31550;

INSERT INTO `quest_offer_reward` (`ID`, `RewardText`, `VerifiedBuild`) 
VALUES (31550, 'I see... an excellent choice. You''re on your way to building up a great team!\r\n\r\nRemember, you have the opportunity to catch any pet you can fight, so go ahead and try to get as many as you can during your adventures.', 0);