-- Silvermoon guard profession-trainer gossip:
-- 1) add a central "Profession Trainers" POI so opening the profession list always
--    updates the map pin (ActionPoiId 0 left the previous, wrong pin visible);
-- 2) snap each profession POI to the actual trainer NPC position;
-- 3) stop opening the empty submenus (7655/7860/...), which made the client drop
--    the gossip window and look like the pointer was wrong.

DELETE FROM points_of_interest WHERE ID = 60000;
INSERT INTO points_of_interest (ID, PositionX, PositionY, Icon, Flags, Importance, Name, VerifiedBuild)
VALUES (60000, 9880.0, -7300.0, 7, 99, 0, 'Silvermoon City, Profession Trainers', 0);

UPDATE points_of_interest SET PositionX = 10018.3, PositionY = -7209.1 WHERE ID = 10368; -- Alchemy (Camberon)
UPDATE points_of_interest SET PositionX = 9842.0,  PositionY = -7362.4 WHERE ID = 10369; -- Blacksmithing (Bemarrin)
UPDATE points_of_interest SET PositionX = 9576.3,  PositionY = -7244.5 WHERE ID = 10370; -- Cooking (Sylann)
UPDATE points_of_interest SET PositionX = 9962.5,  PositionY = -7247.4 WHERE ID = 10371; -- Enchanting (Sedana)
UPDATE points_of_interest SET PositionX = 9822.6,  PositionY = -7333.3 WHERE ID = 10372; -- Engineering (Danwe)
UPDATE points_of_interest SET PositionX = 9607.1,  PositionY = -7324.3 WHERE ID = 10374; -- Fishing (Drathen)
UPDATE points_of_interest SET PositionX = 10005.3, PositionY = -7217.5 WHERE ID = 10375; -- Herbalism (Nathera)
UPDATE points_of_interest SET PositionX = 9959.0,  PositionY = -7242.5 WHERE ID = 10376; -- Inscription (Zantasia)
UPDATE points_of_interest SET PositionX = 9553.0,  PositionY = -7507.7 WHERE ID = 10377; -- Jewelcrafting (Amin)
UPDATE points_of_interest SET PositionX = 9503.6,  PositionY = -7431.0 WHERE ID = 10378; -- Leatherworking (Lynalis)
UPDATE points_of_interest SET PositionX = 9804.8,  PositionY = -7356.6 WHERE ID = 10379; -- Mining (Belil)
UPDATE points_of_interest SET PositionX = 9513.7,  PositionY = -7430.0 WHERE ID = 10380; -- Skinning (Tyn)
UPDATE points_of_interest SET PositionX = 9749.6,  PositionY = -7095.9 WHERE ID = 10381; -- Tailoring (Keelen Sheets)
UPDATE points_of_interest SET PositionX = 9638.4,  PositionY = -7387.6 WHERE ID = 10495; -- Archaeology (Elynara)

UPDATE gossip_menu_option_action
SET ActionMenuId = 0
WHERE MenuId = 7667;

UPDATE gossip_menu_option_action
SET ActionPoiId = 60000
WHERE (MenuId = 8185 AND OptionIndex = 5) OR (MenuId = 7633 AND OptionIndex = 7);
