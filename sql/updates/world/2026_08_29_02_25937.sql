/* Quest: Priceless Treasures 25937 */
DELETE FROM `quest_poi_points` WHERE `QuestID` = 25937;
INSERT INTO `quest_poi_points` (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES
(25937, 1, 0, -5673.39, -1594.03, 0),
(25937, 0, 0, -5570, -1802, 0),
(25937, 0, 1, -5510, -1743, 0),
(25937, 0, 2, -5539, -1695, 0),
(25937, 0, 3, -5786, -1514, 0),
(25937, 0, 4, -5833, -1492, 0),
(25937, 0, 5, -5839, -1611, 0),
(25937, 0, 6, -5819, -1695, 0),
(25937, 0, 7, -5635, -1792, 0);

DELETE FROM `quest_poi` WHERE `QuestID` = 25937;
INSERT INTO `quest_poi` (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES
(25937, 0, 0, 0, 265327, 56225, 0, 27, 0, 1, 0, 0, 0, 0, 0),
(25937, 0, 1, 0, 265328, 56226, 0, 27, 0, 1, 0, 0, 0, 0, 0);

DELETE FROM `quest_offer_reward_locale` WHERE `ID` = 25937 AND `locale` IN ('enUS', 'ruRU');
INSERT INTO `quest_offer_reward_locale` (`ID`, `locale`, `RewardText`, `VerifiedBuild`) VALUES
(25937, 'ruRU', 'Слава богу, ты здесь, Стентс. Клан Черного Железа атакует аэродром, и мы держимся с трудом.', 0),
(25937, 'enUS', "Thank goodness you're here, $n. The airfield's been attacked by the Dark Irons and I don't think we can hold out much longer.", 0);
