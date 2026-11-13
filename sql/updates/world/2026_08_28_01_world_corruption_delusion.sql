-- 837 blizzlike: Grand Delusions (宏伟妄想).
-- 315186 summons Thing From Beyond. 35662 creature_template entry 161895 already exists.
-- Do not bind cloak extra 313301 / 160966.
-- Do NOT write spell_proc unless worldserver logs 315184 ProcFlags=0 (compare 315175).

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_thing_from_beyond'
WHERE `entry` = 161895;
