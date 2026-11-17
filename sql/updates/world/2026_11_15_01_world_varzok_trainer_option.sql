-- Varzok (entry 63626, Orgrimmar battle pet trainer) was given gossip menu 14991
-- to restore the battle pet heal option, but that custom menu has no trainer
-- option. The default menu (0) used to provide "Train me!", so after the change
-- new Horde characters could no longer learn Battle Pet Training (spell 119467).
-- Add the trainer option back to menu 14991.
DELETE FROM gossip_menu_option_action WHERE MenuId = 14991 AND OptionIndex = 3;
DELETE FROM gossip_menu_option_box WHERE MenuId = 14991 AND OptionIndex = 3;
DELETE FROM gossip_menu_option_locale WHERE MenuId = 14991 AND OptionIndex = 3;
DELETE FROM gossip_menu_option WHERE MenuId = 14991 AND OptionIndex = 3;

INSERT INTO gossip_menu_option
    (MenuId, OptionIndex, OptionIcon, OptionText, OptionBroadcastTextId, OptionType, OptionNpcFlag, VerifiedBuild)
VALUES
    (14991, 3, 3, 'Train me!', 0, 5, 16, 35662);

INSERT INTO gossip_menu_option_locale (MenuId, OptionIndex, Locale, OptionText, BoxText)
VALUES
    (14991, 3, 'zhCN', '训练我！', NULL);
