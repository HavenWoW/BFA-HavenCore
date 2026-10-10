-- 837 blizzlike: Inevitable Doom (315179) has BP 0 on damage/healing/absorb
-- taken. Script fills Dummy(EFFECT_3) * (effective corruption - 50), floor 0
-- (Wowhead 2019-10 measured table, hotfix whitelist). Do not hardcode percents.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_inevitable_doom');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(315179, 'spell_inevitable_doom');
