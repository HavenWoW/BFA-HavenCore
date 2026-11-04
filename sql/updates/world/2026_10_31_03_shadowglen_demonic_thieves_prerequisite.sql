-- BFA-HavenCore 8.3.7.35662 | Shadowglen | Demonic Thieves (28715)
-- Correct the direct prerequisite to The Balance of Nature (28713).
-- A Favor for Melithar (28734) is an optional breadcrumb, not a required quest.
-- References:
-- https://github.com/HavenWoW/BFA-HavenCore/issues/238
-- https://warcraft.wiki.gg/wiki/Demonic_Thieves
-- https://warcraft.wiki.gg/wiki/A_Favor_for_Melithar
-- Preserve quest availability, NPC links, and all other quest records.
-- Guard against an unexpected world DB revision; no update if the old prerequisite differs.
UPDATE `quest_template_addon`
SET `PrevQuestID` = 28713
WHERE `ID` = 28715 AND `PrevQuestID` = 28734;
