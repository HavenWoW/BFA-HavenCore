-- 837 blizzlike: Void Ritual (虚空仪式) rank-1 scripts
-- 35662: 318286/479/480 LINKED -> 316814 (RPPM 1, yellow+heal+hostile+periodic+trap).
-- Do NOT write spell_proc for 316814 — DBC already has the official mask.
-- 316823 stacks all secondary ratings; amount is filled by script from rank Dummy.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_void_ritual_end_is_coming','spell_void_ritual_proc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(316814, 'spell_void_ritual_proc'),
(316823, 'spell_void_ritual_end_is_coming');
