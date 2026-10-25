-- 837 blizzlike: Honed Mind (磨砺心灵). 318216 DBC BP is 0; script fills
-- mastery rating from the rank driver Dummy. Do NOT write spell_proc.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_honed_mind_buff','spell_honed_mind_proc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(318214, 'spell_honed_mind_proc'),
(318216, 'spell_honed_mind_buff');
