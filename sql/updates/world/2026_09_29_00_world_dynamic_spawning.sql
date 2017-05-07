-- Create databases for spawn group template, and spawn group membership
-- Current flags
-- 		0x01	Legacy Spawn Mode (spawn using legacy spawn system)
--		0x02	Manual Spawn (don't automatically spawn, instead spawned by core as part of script)
DROP TABLE IF EXISTS `spawn_group_template`;
CREATE TABLE `spawn_group_template` (
  `groupId` int(10) unsigned NOT NULL,
  `groupName` varchar(100) NOT NULL,
  `groupFlags` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`groupId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `spawn_group`;
CREATE TABLE `spawn_group` (
  `groupId` int(10) unsigned NOT NULL,
  `spawnType` tinyint(10) unsigned NOT NULL,
  `spawnId` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`groupId`,`spawnType`,`spawnId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Create the default groups
INSERT INTO `spawn_group_template` (`groupId`, `groupName`, `groupFlags`) VALUES
(0, 'Default Group', 0x01),
(1, 'Legacy Group', (0x01|0x02)),
(2, 'Dynamic Scaling (Quest objectives)', (0x01|0x08)),
(3, 'Dynamic Scaling (Escort NPCs)', (0x01|0x08|0x10)),
(4, 'Dynamic Scaling (Gathering nodes)', (0x01|0x08));

-- Create creature dynamic spawns group (creatures with quest items, or subjects of quests with less than 30min spawn time)
DROP TABLE IF EXISTS `creature_temp_group`;
CREATE TEMPORARY TABLE `creature_temp_group`
(
  `creatureId` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `creature_temp_group`
SELECT `guid`
FROM `creature` C
INNER JOIN `creature_questitem` ON `CreatureEntry` = C.`id`
WHERE `spawntimesecs` < 1800
AND `map` IN (0, 1, 530, 571);

INSERT INTO `creature_temp_group`
SELECT `guid`
FROM `creature` C
INNER JOIN `quest_objectives` QO ON QO.`ObjectID` = C.`id` AND QO.`Type` = 0
WHERE `spawntimesecs` < 1800
AND `map` IN (0, 1, 530, 571);

INSERT INTO `spawn_group` (`groupId`, `spawnType`, `spawnId`)
SELECT DISTINCT 2, 0, `creatureId`
FROM `creature_temp_group`;

DROP TABLE `creature_temp_group`;

-- Create gameobject dynamic spawns group (gameobjects with quest items, or subjects of quests with less than 30min spawn time)

DROP TABLE IF EXISTS `gameobject_temp_group`;
CREATE TEMPORARY TABLE `gameobject_temp_group`
(
  `gameobjectId` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `gameobject_temp_group_ids`;
CREATE TEMPORARY TABLE `gameobject_temp_group_ids`
(
  `entryid` int(10) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

ALTER TABLE `gameobject_temp_group_ids` ADD INDEX (`entryid`);

INSERT INTO `gameobject_temp_group`
SELECT `guid`
FROM `gameobject` G
INNER JOIN `gameobject_questitem` ON `GameObjectEntry` = G.`id`
WHERE `spawntimesecs` < 1800
AND `map` IN (0, 1, 530, 571);

INSERT INTO `gameobject_temp_group_ids` (`entryid`)
SELECT DISTINCT `ObjectID`
FROM `quest_objectives`
WHERE `Type`=2;

INSERT INTO `gameobject_temp_group`
SELECT `guid`
FROM `gameobject` G
INNER JOIN `gameobject_temp_group_ids` ON `entryid` = G.`id`
WHERE `spawntimesecs` < 1800
AND `map` IN (0, 1, 530, 571);

INSERT INTO `spawn_group` (`groupId`, `spawnType`, `spawnId`)
SELECT DISTINCT 2, 1, `gameobjectId`
FROM `gameobject_temp_group`;

DROP TABLE `gameobject_temp_group`;
ALTER TABLE `gameobject_temp_group_ids` DROP INDEX `entryid`;
DROP TABLE `gameobject_temp_group_ids`;

-- Add mining nodes/herb nodes to profession node group
INSERT INTO `spawn_group` (`groupId`, `spawnType`, `spawnId`)
SELECT 4, 1, `guid`
FROM `gameobject` g
INNER JOIN `gameobject_template` gt
	ON gt.`entry` = g.`id`
WHERE `type` IN (3, 50)
AND `Data0` IN (2, 8, 9, 10, 11, 18, 19, 20, 21, 22, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 38, 39, 40, 41, 42, 45, 47, 48, 49, 50, 51, 379, 380, 399, 400, 439, 440, 441, 442, 443, 444, 519, 521, 719, 939, 1119, 1120, 
              1121, 1122, 1123, 1124, 1632, 1639, 1641, 1642, 1643, 1644, 1645, 1646, 1649, 1650, 1651, 1652, 1782, 1783, 1784, 1785, 1786, 1787, 1788, 1789, 1790, 1791, 1792, 1793, 1800, 1860);

-- remove potential duplicates
DELETE FROM `spawn_group` WHERE `groupId` != 4 AND `spawnType`=1 AND `spawnId` IN (SELECT `spawnId` FROM (SELECT `spawnId` FROM `spawn_group` WHERE `groupId`=4  AND `spawnType`=1) as `temp`);


-- Update trinity strings for various cs_list strings, to support showing spawn ID and guid.
UPDATE `trinity_string`
SET `content_default` = '%s (Entry: %d) - |cffffffff|Hgameobject:%s|h[%s X:%f Y:%f Z:%f MapId:%d]|h|r %s %s'
WHERE `entry` = 517;

UPDATE `trinity_string`
SET `content_default` = '%s - |cffffffff|Hcreature:%s|h[%s X:%f Y:%f Z:%f MapId:%d]|h|r %s %s'
WHERE `entry` = 515;

UPDATE `trinity_string`
SET `content_default` = '%s - %s X:%f Y:%f Z:%f MapId:%d %s %s'
WHERE `entry` = 1111;

UPDATE `trinity_string`
SET `content_default` = '%s - %s X:%f Y:%f Z:%f MapId:%d %s %s'
WHERE `entry` = 1110;

-- The npc info strings are already present at 5084-5086 in this database, and 5073/5074 hold the
-- .npc reload strings: write both sets explicitly instead of renumbering rows in place.
DELETE FROM `trinity_string` WHERE `entry` BETWEEN 5084 AND 5088;
INSERT INTO `trinity_string` (`entry`, `content_default`) VALUES
(5084, 'Unit Flags 2: %u'),
(5085, 'Unit Flags 3: %u'),
(5086, 'NPC Flags: %u'),
(5087, 'Selected creature reloaded.'),
(5088, 'Selected creature and all same entries in vision range reloaded.');

-- Add new trinity strings for extra npc/gobject info lines
DELETE FROM `trinity_string` WHERE `entry` BETWEEN 5070 AND 5082;
INSERT INTO `trinity_string` (`entry`, `content_default`) VALUES
(5070, 'Spawn group: %s (ID: %u, Flags: %u, Active: %u)'),
(5071, 'Compatibility Mode: %u'),
(5072, 'GUID: %s'),
(5073, 'SpawnID: %s, location (%f, %f, %f)'),
(5074, 'Distance from player %f'),
(5075, 'Spawn group %u not found'),
(5076, 'Spawned a total of %zu objects:'),
(5077, 'Listing %s respawns within %uyd'),
(5078, 'Listing %s respawns for %s (zone %u)'),
(5079, 'SpawnID | Entry | GridXY| Zone | Respawn time (Full)'),
(5080, 'overdue'),
(5081, 'creatures'),
(5082, 'gameobjects');

-- Add new NPC/Gameobject commands
DELETE FROM `command` WHERE `name` IN ('npc spawngroup', 'npc despawngroup', 'gobject spawngroup', 'gobject despawngroup', 'list respawns');
INSERT INTO `command` (`name`, `permission`, `help`) VALUES
('npc spawngroup', 856, 'Syntax: .npc spawngroup $groupId [ignorerespawn] [force]'),
('npc despawngroup', 857, 'Syntax: .npc despawngroup $groupId [removerespawntime]'),
('gobject spawngroup', 858, 'Syntax: .gobject spawngroup $groupId [ignorerespawn] [force]'),
('gobject despawngroup', 859, 'Syntax: .gobject despawngroup $groupId [removerespawntime]'),
('list respawns', 860, 'Syntax: .list respawns [distance]

Lists all pending respawns within <distance> yards, or within current zone if not specified.');
