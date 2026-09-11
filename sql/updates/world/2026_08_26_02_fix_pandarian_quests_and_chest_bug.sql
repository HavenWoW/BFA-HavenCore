-- Weapon Rack (210005) - Sets "QuestRequired" to 1, so you cant keep looting the weapon.
DELETE FROM `gameobject_loot_template` WHERE `Entry` = 210005;
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
  (210005, 77278, 0, 100, 1, 1, 0, 1, 1, ''),
  (210005, 77279, 0, 100, 1, 1, 0, 1, 1, '');
  
-- Weapon Rack (210015) - Sets "QuestRequired" to 1, so you cant keep looting the weapon.
DELETE FROM `gameobject_loot_template` WHERE `Entry` = 210015;
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
  (210015, 76390, 0, 100, 1, 1, 0, 1, 1, ''),
  (210015, 76392, 0, 100, 1, 1, 0, 1, 1, '');

-- Weapon Rack (210017) - Sets "QuestRequired" to 1, so you cant keep looting the weapon.
DELETE FROM `gameobject_loot_template` WHERE `Entry` = 210017;
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
  (210017, 73207, 0, 100, 1, 1, 0, 1, 1, ''),
  (210017, 76393, 0, 100, 1, 1, 0, 1, 1, '');


-- Weapon Rack (210018) - Sets "QuestRequired" to 1, so you cant keep looting the weapon.
DELETE FROM `gameobject_loot_template` WHERE `Entry` = 210018;
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
  (210018, 73208, 0, 100, 1, 1, 0, 1, 1, ''),
  (210018, 73212, 0, 100, 1, 1, 0, 1, 1, '');

-- Weapon Rack (210019) - Sets "QuestRequired" to 1, so you cant keep looting the weapon.
DELETE FROM `gameobject_loot_template` WHERE `Entry` = 210019;
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
  (210019, 73213, 0, 100, 1, 1, 0, 1, 1, ''),
  (210019, 76391, 0, 100, 1, 1, 0, 1, 1, '');

-- Weapon Rack (210020) - Sets "QuestRequired" to 1, so you cant keep looting the weapon.
DELETE FROM `gameobject_loot_template` WHERE `Entry` = 210020;
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
  (210020, 73210, 0, 100, 1, 1, 0, 1, 1, '');

-- -------------------------------------------

-- Much to Learn quest fixes --
-- -------------------------------------------

-- Much to Learn (30039) - Sets quest description - Monk
DELETE FROM `quest_offer_reward` WHERE `ID` = 30039;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (30039, 0, 0, 0, 0, 0, 0, 0, 0, 'Today is the day when your fate will be decided. Do you have the strength and cunning to be a $c? Or are you destined to a life working in the fields?\n\nEach has its place, but I can sense that you aspire to greatness.\n\nLet us begin our training, my newest pupil.', 0);
  
-- Much to Learn (30045) - Sets quest description - Warrior
DELETE FROM `quest_offer_reward` WHERE `ID` = 30045;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (30045, 0, 0, 0, 0, 0, 0, 0, 0, 'Today is the day when your fate will be decided. Do you have the strength and hardiness to be a $c? Or are you destined to a life working the fields?\n\nEach has its place, but I can sense that you aspire to greatness.\n\nLet us begin our training, my newest pupil.', 0);

-- Much to Learn (30041) - Sets quest description - Hunter
DELETE FROM `quest_offer_reward` WHERE `ID` = 30041;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (30041, 0, 0, 0, 0, 0, 0, 0, 0, 'Today is the day when your fate will be decided. Do you have the compassion and the keen eye of a $c? Or are you destined to a life tending to the livestock?\n\nEach has its place, but I can sense that you aspire to greatness.\n\nLet us begin our training, my newest pupil.', 0);

-- Much to Learn (30043) - Sets quest description - Rogue
DELETE FROM `quest_offer_reward` WHERE `ID` = 30043;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (30043, 0, 0, 0, 0, 0, 0, 0, 0, 'Today is the day when your fate will be decided. Do you have the deftness and cunning necessary to be a $c? Or are you destined to live the life of a humble merchant?\n\nEach has its place, but I can sense that you aspire to greatness.\n\nLet us begin our training, my newest pupil.', 0);

-- Much to Learn (30042) - Sets quest description - Priest
DELETE FROM `quest_offer_reward` WHERE `ID` = 30042;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (30042, 0, 0, 0, 0, 0, 0, 0, 0, 'Today is the day when your fate will be decided. Do you have the awareness and wisdom necessary to be a $c? Or are you destined to a life scribing in the libraries?\n\nEach has its place, but I can sense that you aspire to greatness.\n\nLet us begin our training, my newest pupil.', 0);

-- Much to Learn (30044) - Sets quest description - Shaman
DELETE FROM `quest_offer_reward` WHERE `ID` = 30044;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (30044, 0, 0, 0, 0, 0, 0, 0, 0, 'Today is the day when your fate will be decided. Do you have the strength of will to be a $c? Or are you destined to a life of tending to the shrines?\n\nEach has its place, but I can sense that you aspire to greatness.\n\nLet us begin our training, my newest pupil.', 0);

-- Much to Learn (30040) - Sets quest description - Mage
DELETE FROM `quest_offer_reward` WHERE `ID` = 30040;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (30040, 0, 0, 0, 0, 0, 0, 0, 0, 'Today is the day when your fate will be decided. Do you have the intellect and force of will necessary to be a $c? Or are you destined to a life scribing in the dusty libraries?\n\nEach has its place, but I can sense that you aspire to greatness.\n\nLet us begin our training, my newest pupil.', 0);
  
  
-- The Lesson of the Sandy Fist (29406) - Sets quest objective description.
DELETE FROM `quest_objectives` WHERE `QuestID` = 29406;
INSERT INTO `quest_objectives` (`ID`, `QuestID`, `Type`, `Order`, `StorageIndex`, `ObjectID`, `Amount`, `Flags`, `Flags2`, `ProgressBarWeight`, `Description`, `VerifiedBuild`)
VALUES
  (252339, 29406, 0, 0, 0, 53714, 5, 0, 0, 0, 'Training Targets Destroyed', 35662);