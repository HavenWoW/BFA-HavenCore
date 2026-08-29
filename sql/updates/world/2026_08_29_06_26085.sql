/* Quest: Rallying the Defenders 26085 */
DELETE FROM `quest_offer_reward` WHERE `ID` = 26085;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`) VALUES
(26085, 0, 0, 0, 0, 0, 0, 0, 0, "The men are finding hidden reserves of strength and courage to deal with the Dark Iron onslaught. The airfield will not fall on our watch! ", 0);

DELETE FROM `quest_offer_reward_locale` WHERE `ID` = 26085 AND `locale` IN ('enUS', 'ruRU');
INSERT INTO `quest_offer_reward_locale` (`ID`, `locale`, `RewardText`, `VerifiedBuild`) VALUES
(26085, 'enUS', 'The men are finding hidden reserves of strength and courage to deal with the Dark Iron onslaught. The airfield will not fall on our watch!', 0),
(26085, 'ruRU', 'Защитники воспряли духом и с новыми силами и смелостью противостоят захватчикам Черного Железа. Аэродром не падет на наших глазах!', 0);
