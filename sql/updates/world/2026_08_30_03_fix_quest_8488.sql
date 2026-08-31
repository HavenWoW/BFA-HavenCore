-- Fix https://github.com/HavenWoW/BFA-HavenCore/issues/147
UPDATE `quest_poi` SET `QuestObjectiveID` = 289859, `QuestObjectID` = 15402 WHERE `QuestID` = 8488 AND `BlobIndex` = 0 AND `Idx1` = 0;
UPDATE `quest_poi` SET `ObjectiveIndex` = 30, `QuestObjectiveID` = 289860, `QuestObjectID` = 15958 WHERE `QuestID` = 8488 AND `BlobIndex` = 0 AND `Idx1` = 1;