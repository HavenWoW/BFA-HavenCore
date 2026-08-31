-- Anson Hastings - Hallow's End - Spawn
-- Fix https://github.com/HavenWoW/BFA-HavenCore/issues/138
DELETE FROM `pool_template` WHERE `entry` = 113811;
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES 
(113811, 1, "Anson Hastings - Hallow\'s End - Spawn");

DELETE FROM `pool_creature` WHERE `guid` = 20500872;
INSERT INTO `pool_creature` (`guid`, `pool_entry`, `chance`, `description`) VALUES 
(20500872, 113811, 100, "Anson Hastings - Hallow\'s End - Spawn");

DELETE FROM `game_event_pool` WHERE `eventEntry` = 12 AND `pool_entry` = 113811;
INSERT INTO `game_event_pool` (`eventEntry`, `pool_entry`) VALUES (12, 113811);