-- Strigid Screecher (ID 1996) - Set unit_flags to IMMUNE_TO_NPC
UPDATE `creature_template`
SET `unit_flags` = 512
WHERE `entry` = 1996;


-- Peacebloom (ID 1618) - was floating.
UPDATE `gameobject`
SET `position_z` = 1322.1
WHERE `guid` = 14979 AND `id` = 1618