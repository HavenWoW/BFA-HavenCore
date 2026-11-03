-- 837 blizzlike: Lash of the Void (Unguent Caress).
-- 35662 ProcTypeMask=4 (melee auto only). Damage = 317290 Dummy CalcValue
-- into 317291 (9yd / 60deg cone). Slow is 319241 via DBC trigger.
-- Do NOT write spell_proc. Do NOT cast 317292.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_lash_of_the_void','spell_lash_of_the_void_damage');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(317290, 'spell_lash_of_the_void'),
(317291, 'spell_lash_of_the_void_damage');
