-- It's Raid Night Every Night (25932): C++ owns raider movement; empty SAI waypoints froze summons in place.
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_frostmane_raider' WHERE `entry` = 41603;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 41603 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4160300 AND `source_type` = 9;
