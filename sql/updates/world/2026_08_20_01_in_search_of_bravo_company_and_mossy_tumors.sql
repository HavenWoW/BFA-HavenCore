-- Fixes a few quests (Will tag all issues in PR)

-- Issue 159
-- In Search of Bravo Company (ID 26586) - Now shows correct map when clicked on (Was Burning Steppes)
UPDATE `quest_poi` 
SET `UiMapID`=49 
WHERE `QuestID`=26586 AND `BlobIndex`=0 AND `Idx1`=0;


-- Issue 327
-- Mossy Tumors (ID 923) - Turn in quest was bugged, needed the ID for item(s) choice and quantity. (Before everything was 0).
UPDATE `quest_template` 
SET `RewardChoiceItemID1`=5605, `RewardChoiceItemQuantity1`=1, `RewardChoiceItemID2`=157016, `RewardChoiceItemQuantity2`=1 
WHERE `QuestID` = 923;
