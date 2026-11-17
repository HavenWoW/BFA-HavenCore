-- Darnassian Scout (15968) west of West Sanctum
-- Incriminating Documents (20765) is the quest starter for quest 8482,
-- so it must not be flagged as QuestRequired (that deadlocks the drop:
-- the item can only drop while already on the quest it starts).
-- Matches the original 3.3.5 data: 100% drop chance.

UPDATE `creature_loot_template`
SET `Chance` = 100, `QuestRequired` = 0
WHERE `Entry` = 15968 AND `Item` = 20765;
