
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_gilnean_crow' WHERE `entry` = 50260;
UPDATE `creature` SET `ScriptName` = 'npc_gilnean_crow' WHERE `id` = 50260;
UPDATE `creature` SET `PhaseId` = 169 WHERE `guid` = 801692;