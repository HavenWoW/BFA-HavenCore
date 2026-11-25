-- Correct Silvermoon guard profession submenu texts. The TextIds were shifted:
-- e.g. menu 7860 (Blacksmithing) displayed the Cooking text, Cooking displayed
-- Enchanting, and so on. Remove the trainer-name options added earlier (they
-- are not part of the retail dialogue) and point each submenu at the right text.
DELETE FROM gossip_menu_option WHERE MenuId IN (7655,12248,7860,7657,7658,7659,7661,10022,7662,7663,7664,7665,7666,15800);
DELETE FROM gossip_menu_option_action WHERE MenuId IN (7655,12248,7860,7657,7658,7659,7661,10022,7662,7663,7664,7665,7666,15800);
DELETE FROM gossip_menu_option_locale WHERE MenuId IN (7655,12248,7860,7657,7658,7659,7661,10022,7662,7663,7664,7665,7666,15800);

UPDATE gossip_menu SET TextId = 9339  WHERE MenuId = 7655;  -- Alchemy (Camberon)
UPDATE gossip_menu SET TextId = 9340  WHERE MenuId = 7860;  -- Blacksmithing (Bemarrin)
UPDATE gossip_menu SET TextId = 9624  WHERE MenuId = 7657;  -- Cooking (Sylann)
UPDATE gossip_menu SET TextId = 9341  WHERE MenuId = 7658;  -- Enchanting (Sedana)
UPDATE gossip_menu SET TextId = 9342  WHERE MenuId = 7659;  -- Engineering (Danwe)
UPDATE gossip_menu SET TextId = 9344  WHERE MenuId = 7661;  -- Fishing (Drathen)
UPDATE gossip_menu SET TextId = 9345  WHERE MenuId = 10022; -- Herbalism (Nathera)
UPDATE gossip_menu SET TextId = 13893 WHERE MenuId = 7662;  -- Inscription (Zantasia)
UPDATE gossip_menu SET TextId = 9346  WHERE MenuId = 7663;  -- Jewelcrafting (Kalinda)
UPDATE gossip_menu SET TextId = 9347  WHERE MenuId = 7664;  -- Leatherworking (Lynalis)
UPDATE gossip_menu SET TextId = 9348  WHERE MenuId = 7665;  -- Mining (Belil)
UPDATE gossip_menu SET TextId = 9349  WHERE MenuId = 7666;  -- Skinning (Tyn)
UPDATE gossip_menu SET TextId = 9350  WHERE MenuId = 15800; -- Tailoring (Keelen Sheets)
UPDATE gossip_menu SET TextId = 17209 WHERE MenuId = 12248; -- Archaeology (Reliquary)
