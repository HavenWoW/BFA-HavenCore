-- Quest 8330 "Solanian's Belongings" could not be turned in.
--
-- The quest is a pure item collection quest: its three objectives are
-- QUEST_OBJECTIVE_ITEM (items 20470, 20471, 20472).  quest_template_addon
-- however had SpecialFlags = 2 (QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT).
--
-- Player::CanCompleteQuest() refuses to complete such a quest until
-- QuestStatusData::Explored is true:
--
--     if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT) && !q_status.Explored)
--         return false;
--
-- Nothing ever sets Explored for this quest (the objectives are only advanced
-- by Player::ItemAddedQuestCheck), so the quest stayed QUEST_STATUS_INCOMPLETE
-- with all three objectives at 1/1.  The client therefore showed the quest as
-- complete, while the quest giver sent SMSG_QUESTGIVER_REQUEST_ITEMS with
-- StatusFlags = 0xFD (incomplete), which greys out the "Continue" button.
--
-- The BfA replacement of the same quest (37443, identical objectives and
-- StartItem) has SpecialFlags = 0, so drop the flag on the old entry as well.

UPDATE `quest_template_addon`
SET `SpecialFlags` = `SpecialFlags` & ~2
WHERE `ID` = 8330;
