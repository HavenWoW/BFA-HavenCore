-- Elfin Rabbit (ID 49728)
UPDATE `creature_template_scaling`
SET `LevelScalingMax` = 1, `LevelScalingMin` = 1
WHERE `entry` = 49728;

-- Red-Tailed Chipmunk (ID 49778)
UPDATE `creature_template_scaling`
SET `LevelScalingMax` = 1, `LevelScalingMin` = 1
WHERE `entry` = 49778;

-- Spider (ID 14881) - Unit_Flags corrected (Was 33024 (Only_Swim, IMMUNE_TO_PC)), Family corrected (was NONE, set to Spider (3))
UPDATE `creature_template`
SET `unit_flags` = 512, `family` = 3
WHERE `entry` = 14881;