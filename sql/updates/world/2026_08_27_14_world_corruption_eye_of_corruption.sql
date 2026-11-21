-- 837 blizzlike: Eye of Corruption (腐蚀之眼).
-- 315169 EFFECT_0 TriggerSpell is read from SpellInfo at runtime.
-- 315270 is the companion pet — do not bind it.
-- Do NOT write spell_proc. Do NOT compare corruption thresholds.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_eye_of_corruption');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(315169, 'spell_eye_of_corruption');
