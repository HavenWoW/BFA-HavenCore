-- 837 blizzlike: Grasping Tendrils (蔓生触须).
-- 315175 taken proc casts 315176 with SPELLVALUE_BASE_POINT0.
-- Amount = min(effectiveCorruption+10, 99). Do NOT write spell_proc.
-- Do NOT compare corruption thresholds in the script.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_grasping_tendrils_proc','spell_grasping_tendrils_slow');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(315175, 'spell_grasping_tendrils_proc'),
(315176, 'spell_grasping_tendrils_slow');
