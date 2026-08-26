
-- Darkshore: quests that do not have desc part 2 issue quests --
-- --------------------------------------------------------------

-- A Lost Companion (13564) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13564;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13564, 0, 0, 0, 0, 0, 0, 0, 0, 'Greetings, traveler. You seem to have an interest in my friend here. You\'re here on behalf of his friend Volcor then I imagine? If Volcor didn\'t come himself, he must be injured as well. I hope he recovers quickly.\n\nGimclaw is too injured to be moved far and he\'s going to need some immediate treatment. I\'m afraid I can\'t let you take him back just yet, but perhaps you can help me while I tend to him.', 0);

  
-- Ritual Materials (13566) - Quest completion text missing  
DELETE FROM `quest_offer_reward` WHERE `ID` = 13566;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13566, 0, 0, 0, 0, 0, 0, 0, 0, 'These will do perfectly. These are the final ingredients for an incense I\'ve been working on. I will explain in just a moment, let me mix these in first.', 0);


-- Unsavory Remedies (13598) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13598;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13598, 0, 0, 0, 0, 0, 0, 0, 0, 'You have my thanks, young one. We should be able to speed Grimclaw\'s recovery dramatically with this salve.', 0);


-- Twice Removed (13565) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13565;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13565, 0, 0, 0, 0, 0, 0, 0, 0, 'Thank you, ally. We shall keep a more vigilant eye should they return.', 0);


--  Spirit of the Thistle Bear (13597) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13597;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13597, 0, 0, 0, 0, 0, 0, 0, 0, 'For countless ages I have walked these forests and lived in these mountains. Through snow and fire, I have watched civilizations crumble and nature press on. Now, the very earth turns against us, and it becomes very apparent that we must fight if our legacy is to continue.\n\nYour heart is true, and I would choose you as our champion. If you value the thick hide and resilience of my kind, then accept my boon. Know that the other spirits will not accept you once this choice is made, so choose wisely.', 0);


-- Spirit of the Stag (13567) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13567;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13567, 0, 0, 0, 0, 0, 0, 0, 0, 'I am the caretaker for my kind, leading not through force, but through wisdom and swift action. Wisdom is what carries us into the deeper forest when danger beckons and swiftness is what leaves our enemies basking in our absence.\n\nIn this wisdom, I see through to your true intentions, and I know that you are deserving of my blessing, the blessing of my swiftness. It can be yours, but know that the other spirits will not welcome you once you proceed - consider this carefully before accepting my boon.', 0);


-- The Ritual Bond (ID 13569) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13569;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13569, 0, 0, 0, 0, 0, 0, 0, 0, 'So it is done. You have received the blessing of a great animal spirit, and I shall now complete the ritual for you. Please stand still.', 0);


-- A Love Eternal (ID 13563) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13563;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13563, 0, 0, 0, 0, 0, 0, 0, 0, 'Thank you, $n. Perhaps it would have been better... if I had done it myself. But even after these thousands of years, I could not bear to raise my hand against my beloved.\n\nPlease, I would be alone with my grief...', 0);


-- Grimclaw's Return (ID 13599) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13599;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13599, 0, 0, 0, 0, 0, 0, 0, 0, 'You\'ve returned! It is good to see you again. I suppose we have you to thank for Grimclaw\'s return?\n\nI\'m afraid... I have some bad news. I\'ve been unable to purge the toxin... their conditions worsen. Gershalla died shortly after you left, and Volcor... Volcor passed just before you came back.\n\nHe lived long enough to see Grimclaw come back safely, so he died with a smile on his face. I\'m just sorry that I couldn\'t do more.\n\nFor now, I\'m doing everything I can to save Shaldyn, so please excuse me.', 0);

-- --------------------------------------------------------------



--  Darkshore: a few quests that lack description issue quests --
-- --------------------------------------------------------------

-- Breaking Waves of Change (ID 26383) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 26383;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26383, 0, 0, 0, 0, 0, 0, 0, 0, 'Did the sentinels send you? Thank Elune. We need everyone we can get.', 0);

-- Remembrance of Auberdine (ID 13570) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13570;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13570, 0, 0, 0, 0, 0, 0, 0, 0, 'I just want to offer my sincerest thanks for all that you\'ve done for us. In my grief, I\'m afraid that I didn\'t extend the gratitude that you truly deserve. We may not have made it here without you.\n\nKnow that you will always have friends here.\n\nAnd you needn\'t worry about my loneliness. Grimclaw has taken to keeping me company, and I have to say he\'s a more comforting companion than I would have expected.\n\nTake this, and go with all our blessings. Until we meet again, $n.', 0);

-- A Taste for Grouper (ID 13537) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13537;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13537, 0, 0, 0, 0, 0, 0, 0, 0, 'It looks like fishing\'s your thing. My belly appreciates the gift.\n\nI\'m thinking you would get more use out of this pole than I would. Have at it.', 0);

DELETE FROM `quest_request_items` WHERE `ID` = 13537;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (13537, 0, 0, 0, 0, '<Johnathan\'s belly grumbles as you approach.>\n\nCouldn\'t have said it better with words.', 0);

-- Buzzbox 413 (ID 13521) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13521;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13521, 0, 0, 0, 0, 0, 0, 0, 0, '<The machine crackles loudly, and then Wizbang\'s static-ridden voice becomes audible.>\n\nLovely! I hope you didn\'t kill too many of them. We\'re not trying to wipe out the population, we just need to get to the source of this corruption quickly! I\'m proceeding with the analysis now.', 0);

-- No Accounting for Taste (ID 13527) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13527;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13527, 0, 0, 0, 0, 0, 0, 0, 0, 'This definitely looks like the stuff they were eating. Let me take a closer peek...', 0);

-- Buzzbox 723 (ID 13528) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13528;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13528, 0, 0, 0, 0, 0, 0, 0, 0, '<Static precedes Wizbang\'s distorted voice.>\n\nThe results are coming in now. We should have some information on how the bears contracted this in just a moment.', 0);

-- The Corruption's Source (ID 13529) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13529;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13529, 0, 0, 0, 0, 0, 0, 0, 0, 'It is done then. Thank you. I only hope that the wildlife here will see more peace with his passing.', 0);

-- A Troubling Prescription (ID 13831) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13831;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13831, 0, 0, 0, 0, 0, 0, 0, 0, '<Tharnariun Treetender pages through the notes as you relate your story, the lines in his face deepening with concern.>\n\nSo the Forsaken have been studying Foulhoof\'s work? My gut crawls at the thought of what they might have done were they able to weaponize his corruption. You\'ve done well to destroy their work, but this raises deeper concerns. What brought the Forsaken to our doorstep? Are they working with the Shatterspear trolls to our north? Sorrows are heaping upon sorrows, $c. I\'m glad you\'re here.', 0);

-- A Cure In The Dark (ID 13554) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13554;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13554, 0, 0, 0, 0, 0, 0, 0, 0, 'This should be plenty! Thanks so much! I\'ll get working on a cure for this nastiness immediately.', 0);

-- An Ocean Not So Deep (ID 13560) - Quest completion text missing
DELETE FROM `quest_offer_reward` WHERE `ID` = 13560;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (13560, 0, 0, 0, 0, 0, 0, 0, 0, 'The decoys worked then? Great to hear it! Don\'t you worry yourself one bit about the treasures. Me and Gary will take care of the rest this time around.\n\nTruth be told, if something went wrong again and I lost any more of you adventurer types, I\'m pretty sure they\'d kick me out of town.', 0);
  
-- --------------------------------------------------------------

-- Investigate the Wreckage (14066) - Quest completion text missing.
DELETE FROM `quest_offer_reward` WHERE `ID` = 14066;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (14066, 0, 0, 0, 0, 0, 0, 0, 0, 'This Horde caravan was not merely robbed. It was savagely brutalized. Crates of supplies have been ripped apart by sharp claws. The wagon has been demolished. The food has been eaten and the silver is gone, and yet useful supplies have been left behind...', 0);


-- Mine Spider (ID 43) - Fixed red while dead problem.
UPDATE `creature_template`
SET `unit_flags` = 0
WHERE `entry` = 43;