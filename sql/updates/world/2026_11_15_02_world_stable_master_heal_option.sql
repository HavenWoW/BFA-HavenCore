-- The stable master option "I'd like to heal and revive my battle pets." is now
-- recognised on the server by its broadcast text (64115) instead of by the
-- option icon. Without this, selecting any other plain gossip option of a
-- stable master would also heal the battle pets.
--
-- The stock stable master menus already carry that broadcast text id. The two
-- menus below spell the text out in `OptionText` with `OptionBroadcastTextId`
-- left at 0, so they have to be tagged or their heal option stops working.
-- Both statements are no-ops on a database that does not ship those menus.

UPDATE `gossip_menu_option`
SET `OptionBroadcastTextId` = 64115
WHERE `MenuId` = 14991 AND `OptionIndex` = 1;

UPDATE `gossip_menu_option`
SET `OptionBroadcastTextId` = 64115
WHERE `MenuId` = 9000001 AND `OptionIndex` = 1;
