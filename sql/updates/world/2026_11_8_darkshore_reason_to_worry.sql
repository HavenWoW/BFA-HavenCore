-- https://github.com/HavenWoW/BFA-HavenCore/issues/444
-- Remove quest starter hack
DELETE FROM creature_queststarter WHERE id=32963 AND quest=13506;

-- Remove quest requirement
UPDATE creature_loot_template SET QuestRequired=0 WHERE Entry=32863 AND Item=44979;

-- Remove weird flags
UPDATE creature_template SET unit_flags=0,unit_flags2=0,unit_flags3=0 WHERE entry=32863;
UPDATE creature_template SET unit_flags=0,unit_flags2=0,unit_flags3=0 WHERE entry=32861;
UPDATE creature_template_addon SET bytes2=1 WHERE entry=32863;
UPDATE creature_template_addon SET bytes2=1,emote=233 WHERE entry=32861;

-- Shatterspear Laborer - miners should be idle
UPDATE creature SET MovementType=0 WHERE id=32861;

-- Feign death with 0 HP and immune to NPC/PC
UPDATE creature SET unit_flags=768,unit_flags2=1,unit_flags3=16785408 WHERE guid IN(245585,245593,245712,245714,245716,245718);
