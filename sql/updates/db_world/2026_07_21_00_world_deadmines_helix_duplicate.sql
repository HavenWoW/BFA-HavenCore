-- BFA-HavenCore
-- Deadmines: prevent duplicate Helix Gearbreaker vehicle accessory spawns
--
-- The encounter already has a permanent Helix Gearbreaker (47296) DB spawn.
-- boss_helix_gearbreaker::OafSupport() mounts that existing Helix onto the
-- Lumbering Oaf. These vehicle_template_accessory rows caused the Oaf to
-- summon an additional temporary Helix, resulting in two Helixes after the
-- Oaf died.
--
-- Normal Lumbering Oaf: 47297
-- Heroic Lumbering Oaf: 48939
-- Helix Gearbreaker: 47296

DELETE FROM `vehicle_template_accessory`
WHERE `entry` IN (47297, 48939)
  AND `accessory_entry` = 47296;
