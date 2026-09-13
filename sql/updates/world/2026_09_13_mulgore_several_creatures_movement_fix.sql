-- Bristleback Invaders (2952)
-- Adult Plainstrider (2956)
-- Prairie Wolf (2958)
-- Windfury Wind Witch (2962)
-- Windfury Harpy (2963)
-- Windfury Sorceress (2964)
-- Windfury Matriarch (2965)
-- Young Battleboar (2966)
-- Swoop (2970)
-- Gazelle Fawn (62176)
UPDATE creature_template SET MovementType=1 WHERE entry IN(2952,2956,2958,2962,2963,2964,2965,2966,2970,62176);
UPDATE creature SET spawndist=10.0,MovementType=1 WHERE id IN(2952,2956,2958,2962,2963,2964,2965,2966,2970,62176);
