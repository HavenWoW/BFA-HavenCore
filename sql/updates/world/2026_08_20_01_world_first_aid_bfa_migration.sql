-- BFA-HavenCore - First Aid removal / Bandage Trainer migration
--
-- Battle for Azeroth removed First Aid as a standalone profession.
-- Legacy First Aid trainers must no longer teach skill line 129 or First Aid ranks.
-- Bandage recipes are provided through the BFA Bandage Trainer list and use
-- expansion-specific Tailoring skill lines.
--
-- This update:
--   * removes obsolete First Aid rows from npc_trainer;
--   * remaps legacy trainer 160 users to BFA Bandage Trainer 880;
--   * synchronizes affected creature gossip menus with creature_trainer;
--   * removes the now-unused legacy trainer 160 and its spell list.
--
-- Tested in-game with Dannelor (4211).

START TRANSACTION;

DELETE FROM `npc_trainer`
WHERE `SpellID` = -202007
   OR `ReqSkillLine` = 129
   OR `SpellID` IN (
       3273,3274,3279,7924,10846,10847,27028,45542,74559,110406,195113
   );

UPDATE `creature_trainer`
SET `TrainerId` = 880
WHERE `TrainerId` = 160;

UPDATE `creature_template` AS `c`
JOIN `creature_trainer` AS `ct`
    ON `ct`.`CreatureId` = `c`.`entry`
   AND `ct`.`TrainerId` = 880
SET `c`.`gossip_menu_id` = `ct`.`MenuId`
WHERE `c`.`entry` IN (
    2326,2327,2329,3181,4211,6094,10296,10299,12939,
    16662,16731,17214,17424,19184,22477,23734,45563,50574
)
  AND `ct`.`MenuId` <> 0
  AND `c`.`gossip_menu_id` <> `ct`.`MenuId`;

DELETE FROM `trainer_spell`
WHERE `TrainerId` = 160;

DELETE FROM `trainer`
WHERE `Id` = 160;

COMMIT;
