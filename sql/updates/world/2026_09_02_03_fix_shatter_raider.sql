-- Shatterspear Raider (ID 33071) - Show as dead and not attackable
UPDATE `creature_template` SET `unit_flags` = 768, `unit_flags3` = 8192 WHERE (`entry` = 33071);