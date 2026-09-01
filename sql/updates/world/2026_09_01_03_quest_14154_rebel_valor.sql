-- Quest 14154: Rebel Valor (67503) must not stick after the roof defense.
-- Crowley keeps the area aura via creature_template_addon; script gates targets.

DELETE FROM `spell_script_names` WHERE `spell_id` = 67503 AND `ScriptName` = 'spell_gilneas_rebel_valor';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(67503, 'spell_gilneas_rebel_valor');
