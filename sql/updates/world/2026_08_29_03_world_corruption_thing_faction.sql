-- Thing From Beyond (161895): TDB ships faction 35 (Friendly), so the clone
-- renders green and reads as an ally. Retail shows it hostile but it cannot be
-- fought - only outrun. faction 14 (Monster) turns the name red;
-- unit_flags 0x300 = UNIT_FLAG_IMMUNE_TO_PC (0x100) | UNIT_FLAG_IMMUNE_TO_NPC
-- (0x200) keeps the owner's AoE and nearby mobs from attacking the personal
-- delusion (contact detection in npc_thing_from_beyond is unaffected).
UPDATE `creature_template` SET `faction`=14, `unit_flags`=`unit_flags`|0x300 WHERE `entry`=161895;
