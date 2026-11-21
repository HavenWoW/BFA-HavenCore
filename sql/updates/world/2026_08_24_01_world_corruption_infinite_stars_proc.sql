-- 837 blizzlike: Infinite Stars uses DBC proc flags (spells/abilities only).
-- White auto-attacks must not proc. Remove any spell_proc override.

DELETE FROM `spell_proc` WHERE `SpellId` = 317257;
