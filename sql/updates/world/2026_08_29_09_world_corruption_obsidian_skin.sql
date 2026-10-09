-- 837 blizzlike: Obsidian Skin (Sk'shuul Vaz). 317420 ticks in combat only;
-- stacks persist out of combat. 316661 damage = current armor * Dummy3.
-- 2020-01-28 split (SimC 15%/extra, cap 6) is script-side; DBC has no field.
-- Do NOT reset stacks on combat start. Do NOT write spell_proc. Do NOT double on dual 2H.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_obsidian_destruction','spell_obsidian_destruction_damage');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(317420, 'spell_obsidian_destruction'),
(316661, 'spell_obsidian_destruction_damage');
