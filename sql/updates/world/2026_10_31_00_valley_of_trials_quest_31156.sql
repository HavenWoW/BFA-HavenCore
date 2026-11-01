-- BFA-HavenCore 8.3.7.35662: Valley of Trials / Durotar
-- Quest 31156 (Calligraphed Parchment) is an obsolete Monk class quest.
-- It was removed in patch 7.0.3, but erroneously offered to an Orc Rogue.
-- Ref: https://github.com/HavenWoW/BFA-HavenCore/issues/350
-- Reference: https://warcraft.wiki.gg/wiki/Calligraphed_Parchment_(quest)
-- Minimal repair: make the quest unobtainable, retain its historical template.
-- Using the established disables table pattern in HavenCore SQL migrations.

DELETE FROM `creature_queststarter` WHERE `quest` = 31156;
DELETE FROM `gameobject_queststarter` WHERE `quest` = 31156;
INSERT IGNORE INTO `disables`
    (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`)
VALUES
    (1, 31156, 0, '', '', 'Quest 31156 Calligraphed Parchment: removed in 7.0.3; unavailable in BFA 8.3.7');
