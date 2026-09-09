-- Corrupted insignia (82605) - drop only on quest
UPDATE `creature_loot_template`
SET `QuestRequired` = 1
WHERE `Item` = 82605 AND `Entry` IN (42222, 43325, 61666, 61672, 61678, 61705);

-- Add Corrupted insignia (82605) quest requirements to drop
UPDATE `quest_template` SET `ItemDropQuantity1`=5,`ItemDrop1`=82605 WHERE `ID`=30969;
UPDATE `quest_template` SET `ItemDropQuantity1`=5,`ItemDrop1`=82605 WHERE `ID`=30998;
