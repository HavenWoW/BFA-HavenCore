-- Infinite Stars is driven exclusively by equipped item effects.
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_corruption_infinite_stars','spell_infinite_stars_damage','spell_infinite_stars_proc','spell_infinite_stars_selector');
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_corruption_infinite_stars','spell_infinite_stars_damage','spell_infinite_stars_proc','spell_infinite_stars_selector');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(317257, 'spell_infinite_stars_proc'),
(317260, 'spell_infinite_stars_selector'),
(317265, 'spell_infinite_stars_damage');
