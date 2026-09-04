-- Cow (ID 2442) - Fix HealthModifier.
UPDATE `creature_template` SET `HealthModifier` = 0.1 WHERE (`entry` = 2442);