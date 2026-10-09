-- 837 blizzlike: Eye of Corruption pulse.
-- 315154 CREATE_AREATRIGGER SpellMisc 18755 already maps to template 22815.
-- Bind the periodic pulse script. Do not recreate the template or change the cylinder.

UPDATE `areatrigger_template`
SET `ScriptName` = 'at_eye_of_corruption'
WHERE `Id` = 22815;
