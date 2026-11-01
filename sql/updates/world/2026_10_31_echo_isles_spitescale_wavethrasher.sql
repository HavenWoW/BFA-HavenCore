-- https://github.com/HavenWoW/BFA-HavenCore/issues/531

-- show as dead with 0 HP
UPDATE creature SET unit_flags3=16785408 WHERE guid IN(251623,251624,251626,251632,251634,251737) AND id=38300;
