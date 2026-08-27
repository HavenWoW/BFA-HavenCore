-- Nidas Necklace (26508) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26508;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26508, 0, 0, 0, 0, 0, 0, 0, 0, 'Thank you for finding my necklace mister $c... you are very kind!  My kitty thanks you too - isn\'t that right Effsee?', 0);

DELETE FROM `quest_request_items` WHERE `ID` = 26508;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26508, 0, 0, 0, 0, 'Hi.  I miss my necklace.  My daddy got it for me.  Daddy says that there are monsters in the lake.  Did you beat up any monsters?\nCompletion', 0);



-- An Unwelcome Guest (26509) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26509;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26509, 0, 0, 0, 0, 0, 0, 0, 0, 'Finally the menace is laid to rest!  Thank you, <name>, you have done me a great service.  The garden shall be in full bloom this season!', 0);

DELETE FROM `quest_request_items` WHERE `ID` = 26509;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26509, 0, 0, 0, 0, 'Is Bellygrub still at it or were you able to rid Lakeshire of the pest once and for all?', 0);


-- Lake Everstill Clean Up (26511) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26511;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26511, 0, 0, 0, 0, 0, 0, 0, 0, 'That ought to teach those murlocs a lesson. Hopefully the next time they decide to raid our town they\'ll think twice.\n\nWe both know that won\'t happen.', 0);

DELETE FROM `quest_request_items` WHERE `ID` = 26511;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26511, 0, 0, 0, 0, 'Have you killed all the murlocs?', 0);


-- WANTED: Redridge Gnolls (26504) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26504;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26504, 0, 0, 0, 0, 0, 0, 0, 0, 'A job well done deserves a reward, wouldn\'t you say?\n\nDon\'t spend this all in one place, $c. Better to spread it around, if you catch my drift.', 0);

DELETE FROM `quest_request_items` WHERE `ID` = 26504;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26504, 0, 0, 0, 0, 'Come to collect on a bounty?', 0);


-- Franks and Beans (26506) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26506;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26506, 0, 0, 0, 0, 0, 0, 0, 0, 'PERFECT! I\'ll put these in the pot right away. Dinner should be ready in a few hours.\n\nThank you, darling!', 0);

DELETE FROM `quest_request_items` WHERE `ID` = 26506;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26506, 0, 0, 0, 0, 'Have you gotten my ingredients?', 0);


-- Hero's Call: Redridge Mountains! (26365) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26365;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26365, 0, 0, 0, 0, 0, 0, 0, 0, 'Heard Westfall was in some deep doody.\n\n<Parker doesn\'t even turn to face you.>\n\nWell, we got our own set of problems here. See them gnolls out there? Looks like they\'re actin\' up again. I don\'t like it. Don\'t like it one bit.', 0);

-- Hero's Call: Redridge Mountains! (28563) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 28563;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (28563, 0, 0, 0, 0, 0, 0, 0, 0, 'Heard Westfall was in some deep doody.\n\n<Parker doesn\'t even turn to face you.>\n\nWell, we got our own set of problems here. See them gnolls out there? Looks like they\'re actin\' up again. I don\'t like it. Don\'t like it one bit.', 0);

-- Threat to the Kingdom (26761) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26761;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26761, 0, 0, 0, 0, 0, 0, 0, 0, 'I heard about the Defias making a return. Terrible news. I wish I had better news, but it would appear that Lakeshire is under attack! We are losing citizens left and right. Our own guards are spread far too thin to handle the situation. We need a hero to step up!', 0);

-- Still Assessing the Threat (26503) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26503;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26503, 0, 0, 0, 0, 0, 0, 0, 0, 'I knew it! Looks like Yowler is behind this uprising - which is incredible, because we keep killing gnolls named Yowler. I don\'t know how many sons the original Yowler had, but it\'s got to be close to a hundred.\n\nWell, looks like we got ourselves another Yowler to kill.\n\nMagistrate Solomon must be notified.', 0);
  
  DELETE FROM `quest_request_items` WHERE `ID` = 26503;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26503, 0, 0, 0, 0, 'Did you find the gnoll plans?', 0);

-- We Must Prepare! (26510) - Sets quest completion description.
DELETE FROM `quest_request_items` WHERE `ID` = 26510;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26510, 0, 0, 0, 0, 'Have you recovered the gnomecorder?', 0);
  
DELETE FROM `quest_offer_reward` WHERE `ID` = 26510;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26510, 0, 0, 0, 0, 0, 0, 0, 0, 'Excellent! Let me make a few adjustments here and we should be good to go.', 0);

-- Like a Fart in the Wind (26513) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26513;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26513, 0, 0, 0, 0, 0, 0, 0, 0, 'Thank you, citizen. Without you, we would have starved or worse, been forced to go out and get food for ourselves.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26513;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26513, 0, 0, 0, 0, 'Did you recover the supplies?', 0);

-- Tuning the Gnomecorder (26512) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26512;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26512, 0, 0, 0, 0, 0, 0, 0, 0, '<The gnomecorder crackles and pops.>\n\nCan you hear me, $n? Is this thing on? Ah, yes, I see you there now.\n\nLet\'s get to work!', 0);

-- Canyon Romp (26514) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26514;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26514, 0, 0, 0, 0, 0, 0, 0, 0, '<The gnomecorder crackles and pops.>\n\nGood work, $n. I\'ve just been handed some important information. It would appear that our most hated enemy, the Blackrock orcs, have wised up...\n\n\n\nI\'ve transferred a few silver over to you for the gnome, erm, gnoll kills.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26514;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26514, 0, 0, 0, 0, 'Did you recover those collars?', 0);

-- He Who Controls the Ettins (26519) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26519;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26519, 0, 0, 0, 0, 0, 0, 0, 0, '<The orb hisses as you touch it.>', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26519;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26519, 0, 0, 0, 0, '<The orb emanates powerful magic.>', 0);

-- Saving Foreman Oslow (26520) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26520;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26520, 0, 0, 0, 0, 0, 0, 0, 0, 'You used an ettin to remove the boulder that was crushing Foreman Oslow? Incredible work, $n! Oslow will be grateful once he regains his composure.', 0);
  

-- They've Wised Up (26544) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26544;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26544, 0, 0, 0, 0, 0, 0, 0, 0, '<The gnomecorder buzzes and whirs.>\n\nJust put the missive in the little compartment and I\'ll read it. I am fluent in orcish.\n\n<You hear a distraught yell in the background.>\n\nIt looks like these orcs are here under the direct command of Yowler. That means Yowler must have the orc invasion plan.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26544;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26544, 0, 0, 0, 0, 'What have you found out?', 0);


-- Yowler Must Die! (26545) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26545;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26545, 0, 0, 0, 0, 0, 0, 0, 0, '<Magistrate Solomon takes the plans from you and begins reading.>\n\nShadowhide army!? Damn those orcs to hell! These invasion plans speak of a massive build up of orcish and gnoll forces in the east. Looks like Gath\'Ilzogg, the Blackrock general, is preparing to march his armies across Lakeshire and attack Stormwind directly!', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26545;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26545, 0, 0, 0, 0, 'Do you have the Blackrock invasion plans?', 0);


-- Weapons of War (26571) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26571;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26571, 0, 0, 0, 0, 0, 0, 0, 0, '<The gnomecorder buzzes with energy.>\n\nCome in, $n. Do you copy? Troteman here! Great job on getting Keeshan\'s weapons back! Now there\'s one final mission you have to run. Are you up to the challenge?', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26571;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26571, 0, 0, 0, 0, 'Did you terminate Murdunk and Homurk?', 0);


-- His Heart Must Be In It (26573) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26573;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26573, 0, 0, 0, 0, 0, 0, 0, 0, 'I hope with these items and with his crew all rescued he\'ll have a change of heart. We can\'t do this without Keeshan.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26573;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26573, 0, 0, 0, 0, 'Did you locate the headband and amulet?', 0);


-- Surveying Equipment (26569) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26569;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26569, 0, 0, 0, 0, 0, 0, 0, 0, 'Fantastic work, $n! These will come in handy for our next project, the Lakeshire SUPER BRIDGE, meant to traverse the length of Lake Everstill. It should be done in 20 or so years. Give or take a decade or two.', 0);

DELETE FROM `quest_request_items` WHERE `ID` = 26569;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26569, 0, 0, 0, 0, 'Did you recover the spyglasses?', 0);


-- Render's Army (26570) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26570;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26570, 0, 0, 0, 0, 0, 0, 0, 0, 'You\'ve probably bought us another day, maybe two. Nice work, $n.', 0);


-- In Search of Bravo Company (26586) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26586;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26586, 0, 0, 0, 0, 0, 0, 0, 0, 'Keeshan? You mean JOHNNY\'S ALIVE? You bet your sweet behind I\'ll help, but first you gotta get me out of this damned cage.', 0);


-- Breaking Out is Hard to Do (26587) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26587;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26587, 0, 0, 0, 0, 0, 0, 0, 0, 'IT\'S PAYBACK TIME!', 0);

DELETE FROM `quest_request_items` WHERE `ID` = 26587;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26587, 0, 0, 0, 0, 'Did you get the cage key?', 0);

-- Jorgensen (26560) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26560;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26560, 0, 0, 0, 0, 0, 0, 0, 0, 'I know where they\'re holding Krakauer and Danforth. We gotta hurry. They were prepping those two for a sacrifice!', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26560;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26560, 0, 0, 0, 0, 'Messner! Damn, it\'s good to see a friendly face. Get me out of here!', 0);


-- Krakauer (26561) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26561;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26561, 0, 0, 0, 0, 0, 0, 0, 0, 'Wha... Where am I? What happened? Messner? Jorgensen?\n\nOH MY GOSH! We have to get to Danforth! It might already be too late!', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26561;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26561, 0, 0, 0, 0, '<Krakauer groans.>', 0);


-- And Last But Not Least... Danforth (2656) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26562;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26562, 0, 0, 0, 0, 0, 0, 0, 0, 'About damn time you sissies showed up. My arms were gettin\' tired.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26562;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26562, 0, 0, 0, 0, 'Well look at that! The team\'s all here... everyone but Keeshan.', 0);


-- Return of the Bravo Company (26563) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26563;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26563, 0, 0, 0, 0, 0, 0, 0, 0, '<Colonel Troteman salutes.>\n\nUnbelievable work, $n. You\'ve managed to rescue the entire Bravo Company from the clutches of the Blackrock orcs? With Keeshan\'s team rescued and Keeshan\'s belongings returned, there\'s no way Keeshan could refuse us.', 0);


-- They Drew First Blood (26607) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26607;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26607, 0, 0, 0, 0, 0, 0, 0, 0, '<Keeshan takes the bundle from you and opens it.>\n\nMy bow... and knife! Where did you...\n\nJade\'s amulet... My darling Jade.\n\n<Keeshan picks up his red headband.>\n\nWe got orcs to kill.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26607;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26607, 0, 0, 0, 0, 'You again?', 0);
  

-- It's Never Over (26616) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26616;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26616, 0, 0, 0, 0, 0, 0, 0, 0, 'It\'s orc killin\' time.', 0);
  
  
-- Point of Contact: Brubaker (26639) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26639;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26639, 0, 0, 0, 0, 0, 0, 0, 0, 'Brubaker is dead. It looks like the orcs are attempting to send a message with this grisly display. Upon closer examination you notice something in Brubaker\'s pouch.', 0);


-- Unspeakable Atrocities (26640) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26640;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26640, 0, 0, 0, 0, 0, 0, 0, 0, '<Keeshan reads the report.>\n\nDAMN IT! This just got complicated. We\'ve got prisoners of war to rescue before we can blow up the valley. Not to mention the orcs have black dragons on their side.\n\nAre you ready, $n?', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26640;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26640, 0, 0, 0, 0, 'They got Brubaker?', 0);


-- Hunting the Hunters (26638) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26638;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26638, 0, 0, 0, 0, 0, 0, 0, 0, 'Great work, $n. We need to move quickly before they send for reinforcements.', 0);


-- Bravo Company Field Kit: Camouflage (26636) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26636;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26636, 0, 0, 0, 0, 0, 0, 0, 0, 'Nice job, rookie. I\'m gonna go ahead and put the camo in the box. We should be ready soon.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26636;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26636, 0, 0, 0, 0, 'Leaves and poop - that\'s what I need.', 0);


-- Bravo Company Field Kit: Chloroform (26637) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26637;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26637, 0, 0, 0, 0, 0, 0, 0, 0, 'Perfect. Those orcs won\'t know what hit \'em - literally. I\'ll need a few minutes to get these in working order and then I\'ll put them in the Bravo Company field kit.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26637;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26637, 0, 0, 0, 0, 'Did you recover the glands?', 0);


-- Prisoners of War (26646) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26646;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26646, 0, 0, 0, 0, 0, 0, 0, 0, '<The radio on the Bravo Company field kit crackles.>\n\nLooks like the P.O.W.s are free. Time to initiate Operation: End All.', 0);


-- To Win a War, You Gotta Become War (26651) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26651;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26651, 0, 0, 0, 0, 0, 0, 0, 0, 'And now, we blow the whole damned thing up.', 0);


-- Detonation (26668) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26668;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26668, 0, 0, 0, 0, 0, 0, 0, 0, 'Did you see those orcs explode? THAT\'S HOW WE DO IT IN BRAVO COMPANY! Now to clean up the rest of these jackasses.', 0);


-- Shadowhide Extinction (26692) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26692;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26692, 0, 0, 0, 0, 0, 0, 0, 0, 'We\'re closing in on the Blackrock high command. There\'s nowhere left for those dogs to run.', 0);


-- The Dark Tower (26693) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26693;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26693, 0, 0, 0, 0, 0, 0, 0, 0, '<Keeshan comes in over the Bravo Company field kit.>\n\nNow that the easy part is over, it\'s time for phase two.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26693;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26693, 0, 0, 0, 0, 'I hear you loud and clear, $n.', 0);

-- The Grand Magus Doane (26694) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26694;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26694, 0, 0, 0, 0, 0, 0, 0, 0, 'He teleported away? Mentioned Darkblaze? What the hell is going on here.\n\nLooks like it\'s time to strap in and finish this nonsense.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26694;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26694, 0, 0, 0, 0, 'I hear you loud and clear, $n.', 0);


-- AHHHHHHHHHHHH! AHHHHHHHHH!!! (26708) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26708;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26708, 0, 0, 0, 0, 0, 0, 0, 0, 'The only thing left to deal with now are the orcs holding out at Stonewatch Keep.', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26708;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26708, 0, 0, 0, 0, 'We\'re ready for the final assault.', 0);


-- Showdown at Stonewatch (26713) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26713;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26713, 0, 0, 0, 0, 0, 0, 0, 0, '<The Bravo Company field kit beeps loudly.>\n\nCome in, $n. Are you there? If you can hear me: Blackrock has the upper hand. I repeat. Blackrock has the upper hand. GET BRAVO COMPANY OUT OF TH...', 0);
  
DELETE FROM `quest_request_items` WHERE `ID` = 26713;
INSERT INTO `quest_request_items` (`ID`, `EmoteOnComplete`, `EmoteOnIncomplete`, `EmoteOnCompleteDelay`, `EmoteOnIncompleteDelay`, `CompletionText`, `VerifiedBuild`)
VALUES
  (26713, 0, 0, 0, 0, 'This doesn\'t look good.', 0);


-- Darkblaze, Brood of the Worldbreaker (26714) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26714;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26714, 0, 0, 0, 0, 0, 0, 0, 0, '<A single tear rolls down Troteman\'s left cheek.>\n\nThey died in defense of the Alliance. In defense of our kingdom! Heroes, all of them, and they will be laid to rest as such.', 0);
  

-- Triumphant Return (26724) - Sets quest completion description.
DELETE FROM `quest_offer_reward` WHERE `ID` = 26726;
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`)
VALUES
  (26726, 0, 0, 0, 0, 0, 0, 0, 0, 'They will never be forgotten, $n. What you and the rest of Bravo Company sacrificed in order to save Redridge will be a tale told for ages to come.\n\nWhile I can in no way make up for the loss of your friends, I feel that you must be rewarded for such a heroic deed. These are Keeshan\'s possessions - the only things we found when we dredged the lake for his body. Troteman has informed me that Keeshan would have wanted you to take something as the lone surviving member of Bravo Company. Remember them, $n!', 0);