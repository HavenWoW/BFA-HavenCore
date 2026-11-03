-- 837 blizzlike: Twilight Devastation (腐蚀 - 暮光毁灭) spell scripts
-- 35662: driver 318276/477/478 LINKED_2 -> hidden proc 317147 (RPPM 1 haste, 4s ICD),
-- beam 317155, shadow damage 317159 (BP=0, script fills maxHP * dummy/10 %).

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_twilight_devastation_damage','spell_twilight_devastation_proc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(317147, 'spell_twilight_devastation_proc'),
(317159, 'spell_twilight_devastation_damage');
