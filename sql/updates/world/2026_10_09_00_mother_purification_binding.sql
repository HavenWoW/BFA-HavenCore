-- Bind existing Chamber of Heart MOTHER templates without adding or moving spawns.
UPDATE `creature_template`
SET `npcflag` = `npcflag` | 131,
    `ScriptName` = 'npc_mother_chamber_of_heart'
WHERE `entry` = 152194;
INSERT IGNORE INTO `creature_questender` (`id`, `quest`) VALUES
(152194, 58991), (152194, 59000);
