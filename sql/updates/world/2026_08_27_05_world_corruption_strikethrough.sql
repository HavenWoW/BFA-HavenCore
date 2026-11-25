-- 837 blizzlike: Strikethrough (击穿). 320249 EFFECT_0 DBC BP is 0; script
-- fills crit-damage Dummy only. Driver EFFECT_1 is the live crit-heal aura;
-- leave 320249 heal at 0. Do NOT write spell_proc.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_strikethrough_driver','spell_strikethrough_hidden');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(315277, 'spell_strikethrough_driver'),
(315281, 'spell_strikethrough_driver'),
(315282, 'spell_strikethrough_driver'),
(320249, 'spell_strikethrough_hidden');
