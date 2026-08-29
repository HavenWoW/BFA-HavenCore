/* Quest: Extinguish the Fires 26078 */
DELETE FROM `quest_offer_reward` WHERE `ID` = 26078;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`) VALUES
(26078, 0, 0, 0, 0, 0, 0, 0, 0, "Well done! My men are working hard on getting our flying machines and steam tanks back into the fight.", 0);

DELETE FROM `quest_offer_reward_locale` WHERE `ID` = 26078 AND `locale` IN ('enUS', 'ruRU');
INSERT INTO `quest_offer_reward_locale` (`ID`, `locale`, `RewardText`, `VerifiedBuild`) VALUES
(26078, 'enUS', 'Well done! My men are working hard on getting our flying machines and steam tanks back into the fight.', 0),
(26078, 'ruRU', 'Отличная работа! Мои люди работают над тем, чтобы наши летающие машины и паровые танки вернулись в бой.', 0);
