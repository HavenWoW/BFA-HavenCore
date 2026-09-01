-- Quest 14154 By the Skin of His Teeth.
-- Single SQL update for this quest (PR-ready).
-- Crowley (35077) owns the shared roof-wave event; runts (35456) path/jump via C++.
-- Tobias (35124) assists on the prison roof without mmap chase evade.
-- 35188 must not carry npc_worgen_runt — C++ summons 35456 only.

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_lord_darius_crowley'
WHERE `entry` = 35077;

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_tobias_mistmantle_prison'
WHERE `entry` = 35124;

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_worgen_runt'
WHERE `entry` = 35456;

-- Clear erroneous ScriptName (summons use entry 35456, not 35188).
UPDATE `creature_template`
SET `ScriptName` = ''
WHERE `entry` = 35188 AND `ScriptName` = 'npc_worgen_runt';

-- Accept must not cast personal 66914 waves (Crowley C++ starts the shared event).
DELETE FROM `smart_scripts`
WHERE `entryorguid` = 35077 AND `source_type` = 0 AND `event_type` = 19 AND `event_param1` = 14154;

-- Crowley's combat spell 67825 and combat-movement disable are handled in C++.
DELETE FROM `smart_scripts`
WHERE `entryorguid` = 35077 AND `source_type` = 0 AND `id` IN (1, 2);

-- Enrage is handled in npc_worgen_runt; drop SAI so it does not double-cast.
DELETE FROM `smart_scripts`
WHERE `entryorguid` = 35456 AND `source_type` = 0;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 35124 AND `source_type` = 0;

-- Fix creature emote 35753
UPDATE `creature_addon` SET `emote` = 473 WHERE `guid` = 20556808;

-- Fix creature aura 69196
UPDATE `creature_addon` SET `auras` = '69196' WHERE `guid` IN (804909, 804915);