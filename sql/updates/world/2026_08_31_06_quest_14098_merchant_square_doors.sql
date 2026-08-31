-- Quest 14098 Evacuate the Merchant Square.
-- Doors are phaseGroup 379 (169-172), so they are already visible in 170.
-- C++ go_merchant_square_door now credits GO 195327 and casts Knocking 67869;
-- leftover SAI summons and 34981/35836 SmartAI would double-spawn / fight the
-- citizen script. 35830 is an invisible facing marker, not a quest giver.

UPDATE `gameobject_template`
SET `AIName` = ''
WHERE `entry` = 195327;

DELETE FROM `smart_scripts`
WHERE (`entryorguid` IN (195327, 19532700, 19532701) AND `source_type` IN (1, 9))
   OR (`entryorguid` IN (34981, 35836) AND `source_type` = 0);

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_frightened_citizen'
WHERE `entry` IN (34981, 35836);

DELETE FROM `spell_script_names`
WHERE `spell_id` = 67869 AND `ScriptName` = 'spell_gilneas_knocking';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`)
VALUES (67869, 'spell_gilneas_knocking');

DELETE FROM `creature_queststarter`
WHERE `id` = 35830 AND `quest` = 14098;
