-- ---------------------------------------------------------------------------
-- Sandstone Drake (50269) passenger render fix  [supersedes _01]
--
-- Live .npc data shows the ONLY difference between the broken Sandstone Drake
-- and the working Obsidian Nightwing (62454) is unit_flags2:
--     50269 (broken):  unit_flags2 = 2048  (UNIT_FLAG2_REGENERATE_POWER, 0x800)
--     62454 (working): unit_flags2 = 0
-- All other live fields match (Unit Flags 0, InhabitType 3, faction 35,
-- displayid resolves correctly, spellclick present). The earlier _01 file
-- targeted unit_flags (already 0 on both) and was a no-op for this symptom.
--
-- Clear the stray 0x800 bit so 50269 matches the known-good 62454.
-- Reversible: set unit_flags2 = 2048 on 50269 to roll back.
-- ---------------------------------------------------------------------------
UPDATE `creature_template`
   SET `unit_flags2` = `unit_flags2` & ~2048
 WHERE `entry` = 50269;
