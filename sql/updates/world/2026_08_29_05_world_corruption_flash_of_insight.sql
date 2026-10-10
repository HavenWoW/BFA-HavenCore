-- 837 blizzlike: Flash of Insight (Mar'kowa). Reroll 316744 to 1..N stacks.
-- Not Glimpse of Clarity (315573). Do NOT write spell_proc.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_flash_of_insight_buff','spell_flash_of_insight_proc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(316717, 'spell_flash_of_insight_proc'),
(316744, 'spell_flash_of_insight_buff');
