-- 837 blizzlike: Racing Pulse (急速脉搏). 318227 DBC BP is 0; script fills
-- haste rating from the rank driver Dummy. Do NOT write spell_proc.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_racing_pulse_buff','spell_racing_pulse_proc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(318220, 'spell_racing_pulse_proc'),
(318227, 'spell_racing_pulse_buff');
