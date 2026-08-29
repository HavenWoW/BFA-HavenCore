/* Quest: Grimaxe's Demise 26102 */
DELETE FROM `quest_poi_points` WHERE `QuestID` = 26102;
INSERT INTO `quest_poi_points` (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES
(26102, 0, 0, -4611, -1695, 22908),
(26102, 1, 0, -5039, -1704, 22908);

DELETE FROM `quest_poi` WHERE `QuestID` = 26102;
INSERT INTO `quest_poi` (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES
(26102, 0, 0, -1, 0, 0, 0, 27, 0, 1, 0, 0, 0, 0, 0),
(26102, 0, 1, 0, 263284, 42010, 0, 27, 0, 7, 0, 0, 0, 0, 0);

DELETE FROM `quest_offer_reward` WHERE `ID` = 26102;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`) VALUES
(26102, 0, 0, 0, 0, 0, 0, 0, 0, "We've done it! Without your help, Ironforge Airfield would surely be occupied by the enemy and the Dark Irons would have a clear road to Ironforge itself. The Council of Three Hammers must be informed of what happened here.", 0);

DELETE FROM `quest_offer_reward_locale` WHERE `ID` = 26102 AND `locale` IN ('enUS', 'ruRU');
INSERT INTO `quest_offer_reward_locale` (`ID`, `locale`, `RewardText`, `VerifiedBuild`) VALUES
(26102, 'enUS', "We've done it! Without your help, Ironforge Airfield would surely be occupied by the enemy and the Dark Irons would have a clear road to Ironforge itself. 

The Council of Three Hammers must be informed of what happened here.", 0),
(26102, 'ruRU', "Мы сделали это! Без твоей помощи аэродром Стальгорна наверняка был бы занят врагом, а клан Черного Железа имел бы прямой доступ к Стальгорну.

Совет Трех Кланов должен узнать о том, что здесь произошло.", 0);
