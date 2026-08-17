-- Fix Vile Familiar (3101) right-click attack behavior.
-- npcflag 2 causes hostile creatures to be treated as interactable NPCs.

UPDATE `creature_template`
SET `npcflag` = 0
WHERE `entry` = 3101;
