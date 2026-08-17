-- Fix quest 37446 "Lazy Peons"
-- Let SmartAI control the sleep/wake state for Lazy Peon (10556).

UPDATE `creature_template_addon`
SET `bytes1` = 0,
    `auras` = ''
WHERE `entry` = 10556;
