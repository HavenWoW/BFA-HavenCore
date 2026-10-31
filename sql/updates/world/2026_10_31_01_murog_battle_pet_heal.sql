-- Quest 31589 "On The Mend": Murog the Stable Master (entry 47764) had no gossip menu
-- assigned, so the battle pet heal option was never sent to the client and the quest
-- objective (kill credit 65212) could not be completed.
DELETE FROM gossip_menu_option_locale WHERE MenuId = 9000001;
DELETE FROM gossip_menu_option WHERE MenuId = 9000001;

INSERT INTO gossip_menu_option
    (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild)
VALUES
    (9000001, 0, 12, 'I''d like to stable my pet here.', 0, 14, 4194304, 35662),
    (9000001, 1, 0, 'I''d like to heal and revive my battle pets.', 0, 1, 1, 35662),
    (9000001, 2, 1, 'I want to browse your goods.', 0, 3, 128, 35662);

INSERT INTO gossip_menu_option_locale (MenuId, OptionIndex, Locale, OptionText, BoxText)
VALUES
    (9000001, 0, 'zhCN', '我想在这里寄存宠物。', NULL),
    (9000001, 1, 'zhCN', '我想治疗并复活我的战斗宠物。', NULL),
    (9000001, 2, 'zhCN', '我想看看你的货物。', NULL);

UPDATE creature_template SET gossip_menu_id = 9000001 WHERE entry = 47764;

-- Keep the existing SmartAI chain (credit 65212 + cast 133994) pointing at the heal option.
UPDATE smart_scripts
SET event_param1 = 9000001
WHERE entryOrGuid = 47764 AND source_type = 0 AND id = 0 AND event_type = 62 AND event_param2 = 1;
