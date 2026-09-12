-- Fix Goblin Assassin (50039) right-click attack behavior.
-- npcflag 2 causes this hostile creature to be treated as an interactable NPC.

UPDATE `creature_template`
SET `npcflag` = 0
WHERE `entry` = 50039;
