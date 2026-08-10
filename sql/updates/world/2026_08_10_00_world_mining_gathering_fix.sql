-- BFA-HavenCore mining/gathering final fix
-- Issue #34 and related Copper/Tin mining problems.
--
-- Core changes handle profession skill resolution and player-specific loot
-- completion. Do NOT remove legitimate QuestRequired mining loot rows such as
-- item 2798 or 22634; the core fix allows those rows to remain safely.

-- Copper Vein 1731 and Tin Vein 1732 render incorrectly when IconName is
-- populated with 'Mining' on these BFA gathering-node templates.
UPDATE `gameobject_template`
SET `IconName` = ''
WHERE `entry` IN (1731, 1732);

-- Copper Vein 103713 was non-interactable with its incorrect faction template.
UPDATE `gameobject_template_addon`
SET `faction` = 94
WHERE `entry` = 103713;

-- Tin Vein 1732 incorrectly contains Wintersbite (item 3819) as guaranteed loot.
DELETE FROM `gameobject_loot_template`
WHERE `Entry` = 1732
  AND `Item` = 3819;
