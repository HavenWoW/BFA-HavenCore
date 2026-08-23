-- BFA-HavenCore - Fishing trainer BFA migration / gossip repair
--
-- Restores missing Fishing trainer mappings, removes obsolete pre-BFA Fishing
-- rank rows from TrainerId 10, and synchronizes legacy Fishing NPCs that now
-- use the common BFA Fishing gossip menu 17157.
--
-- Verified in-game with Arnold Leland (5493), Grimnur Stonebrand (5161),
-- and Juno Dufrain (18911).
--
-- The BFA base Fishing wrapper 271617 is intentionally retained.
-- Special Fishing trainers using menus such as 7361/8826/9833 are preserved.

START TRANSACTION;

DELETE FROM `trainer_spell`
WHERE `TrainerId` = 10
  AND `SpellId` IN (
      7734,18249,51293,54083,54084,88869,110412,158744,210829
  );

DELETE FROM `creature_trainer`
WHERE `CreatureId` IN (
    1651,3332,4573,4997,5938,7946,12032,12961,14740,18018,
    26909,26957,26993,32474,44975,70398,77733,79892,84580,
    85825,86628,94311,95844,96097,121904,122705,136102,
    138026,138460,151462
);

INSERT INTO `creature_trainer`
(`CreatureId`,`TrainerId`,`MenuId`,`OptionIndex`)
VALUES
(1651,10,17157,0),
(3332,10,17157,0),
(4573,10,17157,0),
(5938,10,17157,0),
(7946,10,17157,0),
(12032,10,17157,0),
(12961,10,17157,0),
(18018,10,17157,0),
(26909,10,17157,0),
(26957,10,17157,0),
(26993,10,17157,0),
(32474,10,17157,0),
(70398,10,17157,0),
(77733,10,17157,0),
(79892,10,17157,0),
(85825,10,17157,0),
(86628,10,17157,0),
(94311,10,17157,0),
(96097,10,17157,0),
(122705,10,17157,0),
(136102,10,17157,0),
(138026,10,17157,0),
(138460,10,17157,0),
(4997,10,5665,0),
(84580,10,5665,0),
(14740,10,7361,0),
(44975,10,7361,0),
(95844,10,7361,0),
(121904,10,7361,0),
(151462,10,9541284,1);

UPDATE `creature_trainer` AS `ctr`
JOIN `creature_template` AS `ct`
    ON `ct`.`entry` = `ctr`.`CreatureId`
SET `ctr`.`MenuId` = 17157
WHERE `ct`.`gossip_menu_id` = 17157
  AND `ctr`.`TrainerId` = 10
  AND `ctr`.`OptionIndex` = 0
  AND `ctr`.`MenuId` <> 17157;

COMMIT;
