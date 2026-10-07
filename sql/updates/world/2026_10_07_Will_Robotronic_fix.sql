-- Fix for NPC Will Robotronic to not be phased to allow collection turn in of quest Shear Will 
UPDATE creature_template_addon SET auras=0 WHERE entry=35648;
