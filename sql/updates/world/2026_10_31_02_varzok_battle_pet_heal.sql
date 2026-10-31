-- Quest 31589 "On The Mend" giver Varzok (entry 63626) uses gossip menu 14991 in
-- SmartAI, but no gossip_menu_id was assigned to him, so the battle pet heal option
-- was never offered. Menu 14991 also lacked the vendor option.
DELETE FROM gossip_menu_option_locale WHERE MenuId = 14991 AND OptionIndex IN (0, 1, 2);
DELETE FROM gossip_menu_option WHERE MenuId = 14991 AND OptionIndex = 2;

UPDATE gossip_menu_option
SET OptionText = 'I''d like to heal and revive my battle pets.', OptionType = 1, OptionNpcFlag = 1
WHERE MenuId = 14991 AND OptionIndex = 1;

INSERT INTO gossip_menu_option
    (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild)
VALUES
    (14991, 2, 1, 'I want to browse your goods.', 0, 3, 128, 35662);

INSERT INTO gossip_menu_option_locale (MenuId, OptionIndex, Locale, OptionText, BoxText)
VALUES
    (14991, 0, 'zhCN', '你对捕捉稀有宠物感兴趣吗？', NULL),
    (14991, 1, 'zhCN', '我想治疗并复活我的战斗宠物。', NULL),
    (14991, 2, 'zhCN', '我想看看你的货物。', NULL);

UPDATE creature_template SET gossip_menu_id = 14991 WHERE entry = 63626;
