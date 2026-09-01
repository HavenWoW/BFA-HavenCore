-- Quest 14098 Evacuate the Merchant Square.
-- Door-summoned Rampaging Worgen shares npc_frightened_citizen with the citizens.

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_frightened_citizen'
WHERE `entry` = 35660;
