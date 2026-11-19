-- Silvermoon guard profession submenus were empty, so after clicking a
-- profession the gossip window closed with no trainer dialogue. Fill the
-- profession submenus with the actual Silvermoon trainer names and link them
-- back to the profession POIs.

DELETE FROM gossip_menu WHERE MenuId = 15800;
INSERT INTO gossip_menu (MenuId, TextId, VerifiedBuild) VALUES (15800, 9352, 0);

DELETE FROM gossip_menu_option WHERE MenuId IN (7655,12248,7860,7657,7658,7659,7661,10022,7662,7663,7664,7665,7666,15800);
DELETE FROM gossip_menu_option_action WHERE MenuId IN (7655,12248,7860,7657,7658,7659,7661,10022,7662,7663,7664,7665,7666,15800);
DELETE FROM gossip_menu_option_locale WHERE MenuId IN (7655,12248,7860,7657,7658,7659,7661,10022,7662,7663,7664,7665,7666,15800);

INSERT INTO gossip_menu_option
    (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild)
VALUES
    (7655, 0, 0, 'Camberon', 0, 1, 1, 0),
    (7655, 1, 0, 'Razia', 0, 1, 1, 0),
    (12248, 0, 0, 'Elynara', 0, 1, 1, 0),
    (12248, 1, 0, 'Toban', 0, 1, 1, 0),
    (7860, 0, 0, 'Bemarrin', 0, 1, 1, 0),
    (7657, 0, 0, 'Sylann', 0, 1, 1, 0),
    (7658, 0, 0, 'Sedana', 0, 1, 1, 0),
    (7659, 0, 0, 'Danwe', 0, 1, 1, 0),
    (7659, 1, 0, 'Gloresse', 0, 1, 1, 0),
    (7661, 0, 0, 'Drathen', 0, 1, 1, 0),
    (10022, 0, 0, 'Botanist Nathera', 0, 1, 1, 0),
    (7662, 0, 0, 'Zantasia', 0, 1, 1, 0),
    (7663, 0, 0, 'Amin', 0, 1, 1, 0),
    (7664, 0, 0, 'Lynalis', 0, 1, 1, 0),
    (7664, 1, 0, 'Talmar', 0, 1, 1, 0),
    (7665, 0, 0, 'Belil', 0, 1, 1, 0),
    (7666, 0, 0, 'Tyn', 0, 1, 1, 0),
    (15800, 0, 0, 'Keelen Sheets', 0, 1, 1, 0);

INSERT INTO gossip_menu_option_action (MenuId, OptionIndex, ActionMenuId, ActionPoiId)
VALUES
    (7655, 0, 0, 10368),
    (7655, 1, 0, 10368),
    (12248, 0, 0, 10495),
    (12248, 1, 0, 10495),
    (7860, 0, 0, 10369),
    (7657, 0, 0, 10370),
    (7658, 0, 0, 10371),
    (7659, 0, 0, 10372),
    (7659, 1, 0, 10372),
    (7661, 0, 0, 10374),
    (10022, 0, 0, 10375),
    (7662, 0, 0, 10376),
    (7663, 0, 0, 10377),
    (7664, 0, 0, 10378),
    (7664, 1, 0, 10378),
    (7665, 0, 0, 10379),
    (7666, 0, 0, 10380),
    (15800, 0, 0, 10381);

INSERT INTO gossip_menu_option_locale (MenuId, OptionIndex, Locale, OptionText, BoxText)
VALUES
    (7655, 0, 'zhCN', '卡博隆', NULL),
    (7655, 1, 'zhCN', '拉斯亚', NULL),
    (12248, 0, 'zhCN', '埃莱娜拉', NULL),
    (12248, 1, 'zhCN', '图班', NULL),
    (7860, 0, 'zhCN', '波玛尔', NULL),
    (7657, 0, 'zhCN', '塞莱恩', NULL),
    (7658, 0, 'zhCN', '瑟丹娜', NULL),
    (7659, 0, 'zhCN', '丹文', NULL),
    (7659, 1, 'zhCN', '葛洛莉丝', NULL),
    (7661, 0, 'zhCN', '德拉森', NULL),
    (10022, 0, 'zhCN', '植物学家娜萨兰', NULL),
    (7662, 0, 'zhCN', '赞塔希娅', NULL),
    (7663, 0, 'zhCN', '埃米恩', NULL),
    (7664, 0, 'zhCN', '莱纳里斯', NULL),
    (7664, 1, 'zhCN', '塔尔玛', NULL),
    (7665, 0, 'zhCN', '比利尔', NULL),
    (7666, 0, 'zhCN', '提恩', NULL),
    (15800, 0, 'zhCN', '基伦·希斯', NULL);

UPDATE gossip_menu_option_action SET ActionMenuId = 7655  WHERE MenuId = 7667 AND OptionIndex = 0;
UPDATE gossip_menu_option_action SET ActionMenuId = 12248 WHERE MenuId = 7667 AND OptionIndex = 1;
UPDATE gossip_menu_option_action SET ActionMenuId = 7860  WHERE MenuId = 7667 AND OptionIndex = 2;
UPDATE gossip_menu_option_action SET ActionMenuId = 7657  WHERE MenuId = 7667 AND OptionIndex = 3;
UPDATE gossip_menu_option_action SET ActionMenuId = 7658  WHERE MenuId = 7667 AND OptionIndex = 4;
UPDATE gossip_menu_option_action SET ActionMenuId = 7659  WHERE MenuId = 7667 AND OptionIndex = 5;
UPDATE gossip_menu_option_action SET ActionMenuId = 7661  WHERE MenuId = 7667 AND OptionIndex = 7;
UPDATE gossip_menu_option_action SET ActionMenuId = 10022 WHERE MenuId = 7667 AND OptionIndex = 8;
UPDATE gossip_menu_option_action SET ActionMenuId = 7662  WHERE MenuId = 7667 AND OptionIndex = 9;
UPDATE gossip_menu_option_action SET ActionMenuId = 7663  WHERE MenuId = 7667 AND OptionIndex = 10;
UPDATE gossip_menu_option_action SET ActionMenuId = 7664  WHERE MenuId = 7667 AND OptionIndex = 11;
UPDATE gossip_menu_option_action SET ActionMenuId = 7665  WHERE MenuId = 7667 AND OptionIndex = 12;
UPDATE gossip_menu_option_action SET ActionMenuId = 7666  WHERE MenuId = 7667 AND OptionIndex = 13;
UPDATE gossip_menu_option_action SET ActionMenuId = 15800 WHERE MenuId = 7667 AND OptionIndex = 14;
