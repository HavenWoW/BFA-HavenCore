-- Existing build 35662 cylinder for Twilight Devastation; no temporary sphere.
UPDATE `areatrigger_template` SET `ScriptName`='at_twilight_devastation' WHERE `Id`=23070;
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_twilight_devastation_beam';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (317155,'spell_twilight_devastation_beam');
