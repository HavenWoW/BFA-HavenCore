-- BFA-HavenCore 8.3.7.35662 | Darkshore | Quest 13953 (Naga In Our Midst)
-- Remove the incorrect quest-start relationship from Balren of the Claw (34402).
-- Darkscale Assassin (34498) remains the quest starter; Balren stays the quest ender.
-- Ref: https://github.com/HavenWoW/BFA-HavenCore/issues/536
-- Source: https://warcraft.wiki.gg/wiki/Naga_In_Our_Midst

DELETE FROM `creature_queststarter`
WHERE `id` = 34402 AND `quest` = 13953;
