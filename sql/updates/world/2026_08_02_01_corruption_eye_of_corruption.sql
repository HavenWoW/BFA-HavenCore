-- Eye of Corruption, the 20+ corruption drawback.
--
-- The Eye already spawned: container 315169 procs on damage dealt and triggers 315154,
-- which creates areatrigger 22815. But 22815 had no ScriptName and no rows in
-- `areatrigger_template_actions`, and nothing anywhere cast the damage spell 315161, so
-- the Eye was purely cosmetic - it appeared, sat for its eight seconds and did nothing.
--
-- A template action cannot serve here: AreaTrigger::DoActions fires once, from
-- HandleUnitEnterExit, while the Eye has to tick for as long as the player stays in
-- range. That needs AreaTriggerAI::OnPeriodicProc, reached only through a ScriptName.
UPDATE `areatrigger_template` SET `ScriptName` = 'at_corruption_eye_of_corruption' WHERE `Id` = 22815;

DELETE FROM `spell_script_names` WHERE `spell_id` = 315161 AND `ScriptName` = 'spell_corruption_eye_of_corruption';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(315161, 'spell_corruption_eye_of_corruption');
