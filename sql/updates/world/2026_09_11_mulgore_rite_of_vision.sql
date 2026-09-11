--  Wolf Spirit (36834) - add unit flag UNIT_FLAG_NON_ATTACKABLE
UPDATE creature_template SET unit_flags=2, speed_run=3.0 WHERE entry=36834;

--  Rite of vision(20441) - add quest objective text
UPDATE quest_objectives SET Description='Water of Vision consumed' WHERE QuestID = 20441 AND ID=266086 ;
