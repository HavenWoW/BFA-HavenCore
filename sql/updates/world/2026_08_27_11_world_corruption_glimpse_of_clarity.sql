-- 837 blizzlike: Glimpse of Clarity (须臾洞察).
-- 315573 is Dummy-only; cooldown trim is player_glimpse_of_clarity
-- (OnSuccessfulSpellCast). Do NOT write spell_proc.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_glimpse_of_clarity');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(315573, 'spell_glimpse_of_clarity');
