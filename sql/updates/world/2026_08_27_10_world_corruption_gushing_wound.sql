-- 837 blizzlike: Gushing Wound (龟裂创伤). 318187 DBC BP is 0; script fills
-- per-tick damage from Dummy 13% of max(AP,SP). Do NOT divide by tick count.
-- Do NOT write spell_proc.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_gushing_wound_dot','spell_gushing_wound_proc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(318179, 'spell_gushing_wound_proc'),
(318187, 'spell_gushing_wound_dot');
