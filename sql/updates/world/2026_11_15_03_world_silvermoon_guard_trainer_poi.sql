-- Silvermoon City guard gossip menu 7667 ("Profession Trainer") had every POI
-- after Alchemy shifted by one: Blacksmithing pointed at Cooking, Cooking at
-- Enchanting, ... and Skinning/Tailoring both pointed at Tailoring. Restore the
-- correct Silvermoon profession POIs.
UPDATE gossip_menu_option_action SET ActionPoiId = 10369 WHERE MenuId = 7667 AND OptionIndex = 2;  -- Blacksmithing
UPDATE gossip_menu_option_action SET ActionPoiId = 10370 WHERE MenuId = 7667 AND OptionIndex = 3;  -- Cooking
UPDATE gossip_menu_option_action SET ActionPoiId = 10371 WHERE MenuId = 7667 AND OptionIndex = 4;  -- Enchanting
UPDATE gossip_menu_option_action SET ActionPoiId = 10372 WHERE MenuId = 7667 AND OptionIndex = 5;  -- Engineering
UPDATE gossip_menu_option_action SET ActionPoiId = 10374 WHERE MenuId = 7667 AND OptionIndex = 7;  -- Fishing
UPDATE gossip_menu_option_action SET ActionPoiId = 10375 WHERE MenuId = 7667 AND OptionIndex = 8;  -- Herbalism
UPDATE gossip_menu_option_action SET ActionPoiId = 10376 WHERE MenuId = 7667 AND OptionIndex = 9;  -- Inscription
UPDATE gossip_menu_option_action SET ActionPoiId = 10377 WHERE MenuId = 7667 AND OptionIndex = 10; -- Jewelcrafting
UPDATE gossip_menu_option_action SET ActionPoiId = 10378 WHERE MenuId = 7667 AND OptionIndex = 11; -- Leatherworking
UPDATE gossip_menu_option_action SET ActionPoiId = 10379 WHERE MenuId = 7667 AND OptionIndex = 12; -- Mining
UPDATE gossip_menu_option_action SET ActionPoiId = 10380 WHERE MenuId = 7667 AND OptionIndex = 13; -- Skinning
