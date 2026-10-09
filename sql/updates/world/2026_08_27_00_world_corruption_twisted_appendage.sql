-- 837 blizzlike: Twisted Appendage (扭曲的附肢) rank-1 scripts
-- 35662: 318481/82/83 LINKED -> 316815 (RPPM 1, ProcFlags 69908, no haste).
-- Do NOT write spell_proc for 316815 — DBC already has the official mask.
-- 162764 may exist as a world spawn; npc AI no-ops unless the owner wears 316815.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_twisted_appendage_flay','spell_twisted_appendage_proc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(316815, 'spell_twisted_appendage_proc'),
(316835, 'spell_twisted_appendage_flay');

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_twisted_appendage'
WHERE `entry` = 162764;
