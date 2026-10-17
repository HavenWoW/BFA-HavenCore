-- 837 blizzlike: Grand Delusions (宏伟妄想).
-- 315184 EFFECT_0 TriggerSpell is read from SpellInfo at runtime.
-- 313301 is the cloak extra — do not bind it.
-- Do NOT write spell_proc. Do NOT compare corruption thresholds.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_grand_delusions');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(315184, 'spell_grand_delusions');
