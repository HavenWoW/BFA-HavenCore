-- Fix for NPC Sorata Firespiner to not be phased to allow turn in of quest Wash Out 
UPDATE creature_template_addon SET auras=0 WHERE entry=36210;
