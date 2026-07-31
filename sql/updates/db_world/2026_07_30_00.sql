UPDATE creature_template_scaling 
SET LevelScalingMin = 1, LevelScalingMax = 1 
WHERE Entry = 49842;

UPDATE creature_template_scaling 
SET LevelScalingMin = 1, LevelScalingMax = 3 
WHERE Entry IN (4075, 62313);