-- Shatterspear Laborer (ID 32861) - Show as dead and not attackable
UPDATE `creature_template` SET `unit_flags` = 768 WHERE (`entry` = 32861);
UPDATE `creature_template` SET `unit_flags3` = 8192 WHERE (`entry` = 32861);

-- Shatterspear Overseer (ID 32863) - Show as dead and not attackable
UPDATE `creature_template` SET `unit_flags` = 768 WHERE (`entry` = 32863);
UPDATE `creature_template` SET `unit_flags3` = 8192 WHERE (`entry` = 32863);