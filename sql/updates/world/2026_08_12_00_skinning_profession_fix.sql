-- Remove the permanent gatherable flag from creatures that have
-- corpse gathering loot. The core applies UNIT_FLAG_SKINNABLE dynamically
-- after normal corpse loot has been removed.
UPDATE `creature_template`
SET `unit_flags` = `unit_flags` & ~67108864
WHERE `skinloot` > 0
  AND (`unit_flags` & 67108864) <> 0;
