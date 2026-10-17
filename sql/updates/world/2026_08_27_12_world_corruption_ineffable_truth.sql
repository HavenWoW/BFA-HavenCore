-- 837 blizzlike: Ineffable Truth (不可言喻的真相).
-- 316801 auras 143/173 are NYI. Rate lives on spell_ineffable_truth
-- (Apply/Remove + PlayerScript cooldown start). Do NOT write spell_proc.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_ineffable_truth');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(316801, 'spell_ineffable_truth');
