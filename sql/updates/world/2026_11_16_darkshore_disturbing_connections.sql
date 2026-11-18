-- https://github.com/HavenWoW/BFA-HavenCore/issues/459
UPDATE creature_loot_template SET QuestRequired=0 WHERE Entry=32862 AND Item=46318;

-- Delete hack quest giver
DELETE FROM creature_queststarter WHERE id=32971 AND quest=13591;
