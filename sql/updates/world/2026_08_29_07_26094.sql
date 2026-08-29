/* Quest: Striking Back 26094 */
DELETE FROM `quest_offer_reward` WHERE `ID` = 26094;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`) VALUES
(26094, 0, 0, 0, 0, 0, 0, 0, 0, "The momentum is starting to shift, $n, largely due to your efforts. Now, it is time to strike at the Dark Iron leadership. ", 0);

DELETE FROM `quest_offer_reward_locale` WHERE `ID` = 26094 AND `locale` IN ('enUS', 'ruRU');
INSERT INTO `quest_offer_reward_locale` (`ID`, `locale`, `RewardText`, `VerifiedBuild`) VALUES
(26094, 'enUS', 'The momentum is starting to shift, $n, largely due to your efforts. Now, it is time to strike at the Dark Iron leadership.', 0),
(26094, 'ruRU', 'Чаша весов начинает склоняться в нашу сторону, $n, и во многом благодаря твоим усилиям. Теперь пришло время нанести удар по предводителям клана Черного Железа.', 0);
