-- BFA-HavenCore 8.3.7.35662 | Darkshore | Quest 13896 (Unearthed Knowledge)
-- Fixes the incorrect quest-start relationship on Archaeologist Groff (34340).
-- Larien (34404) remains the quest starter; Groff remains the quest ender.
-- Ref: https://github.com/HavenWoW/BFA-HavenCore/issues/572
-- Evidence: https://warcraft.wiki.gg/wiki/Unearthed_Knowledge

DELETE FROM `creature_queststarter`
WHERE `id` = 34340 AND `quest` = 13896;
