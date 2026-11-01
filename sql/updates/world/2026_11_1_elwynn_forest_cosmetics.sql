-- https://github.com/HavenWoW/BFA-HavenCore/issues/468
UPDATE creature SET MovementType=0 WHERE guid=177215 AND id=50526;

-- https://github.com/HavenWoW/BFA-HavenCore/issues/469
UPDATE creature SET MovementType=0 WHERE guid=177217 AND id=50527;

-- https://github.com/HavenWoW/BFA-HavenCore/issues/470
UPDATE creature SET MovementType=0 WHERE guid=177216 AND id=50528;

--  https://github.com/HavenWoW/BFA-HavenCore/issues/471
DELETE FROM creature WHERE guid=177291 AND id=99;
