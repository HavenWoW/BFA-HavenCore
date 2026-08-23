-- Tiki Targets must retain incoming damage until normal death instead of
-- entering evade when they cannot select an attacker as their victim.
UPDATE `creature_template`
SET `npcflag` = 0,
    `AIName` = 'NullCreatureAI'
WHERE `entry` = 38038;

DELETE FROM `creature_queststarter`
WHERE `id` = 38038;

-- Quest 26273 already credits normal kills of creature 38038.
DELETE FROM `smart_scripts`
WHERE `entryorguid` = 38038
  AND `source_type` = 0;

-- Arcane Missiles Trainer is obsolete in BFA; retain the Tiki visual aura.
UPDATE `creature_template_addon`
SET `auras` = '71064'
WHERE `entry` = 38038;

UPDATE `creature_addon` AS `ca`
INNER JOIN `creature` AS `c`
    ON `c`.`guid` = `ca`.`guid`
SET `ca`.`auras` = CASE `ca`.`auras`
    WHEN '71064 83470' THEN '71064'
    WHEN '71066 83470' THEN '71066'
END
WHERE `c`.`id` = 38038
  AND `ca`.`auras` IN ('71064 83470', '71066 83470');
