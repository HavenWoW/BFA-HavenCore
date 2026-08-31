-- Fix https://github.com/HavenWoW/BFA-HavenCore/issues/174
-- Obj1 TALKTO 46185 on board; obj2 MONSTER 46185 when the sanitron finishes.
-- SpecialFlags 2 (EXPLORATION_OR_EVENT) would still block turn-in without Explored.

UPDATE `quest_template_addon` SET `SpecialFlags` = 0 WHERE `ID` = 27635;

DELETE FROM `quest_objectives` WHERE `ID` = 276351;
INSERT INTO `quest_objectives`
(`ID`, `QuestID`, `Type`, `Order`, `StorageIndex`, `ObjectID`, `Amount`, `Flags`, `Flags2`, `ProgressBarWeight`, `Description`, `VerifiedBuild`)
VALUES
(276351, 27635, 0, 1, 1, 46185, 1, 0, 0, 0, 'Complete Decontamination Process', 35662);

DELETE FROM `quest_objectives_locale` WHERE `ID` = 276351;
INSERT INTO `quest_objectives_locale`
(`ID`, `locale`, `QuestId`, `StorageIndex`, `Description`, `VerifiedBuild`)
VALUES
(276351, 'deDE', 27635, 1, 'Dekontaminationsprozess abgeschlossen', 35662),
(276351, 'esES', 27635, 1, 'Proceso de descontaminación completado', 35662),
(276351, 'esMX', 27635, 1, 'Proceso de descontaminación completado', 35662),
(276351, 'frFR', 27635, 1, 'Processus de décontamination terminé', 35662),
(276351, 'itIT', 27635, 1, 'Procedura di decontaminazione completata', 35662),
(276351, 'koKR', 27635, 1, '정화 단계 완료', 35662),
(276351, 'ptBR', 27635, 1, 'Processo de Descontaminação concluído', 35662),
(276351, 'ruRU', 27635, 1, 'Процедура очищения завершена', 35662),
(276351, 'zhCN', 27635, 1, '完成净化处理', 35662),
(276351, 'zhTW', 27635, 1, '完成消毒程序', 35662);

-- C++ ScriptName owns the ride. SAI killcredit on click would fill obj2 too early.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 46185 AND `source_type` = 0;

-- Wreckage technician (guid 169037). Actionlist 4623000 was bound to guid 84873 (Netherstorm 20044).
DELETE FROM `smart_scripts` WHERE `entryorguid` = -84873 AND `source_type` = 0 AND `id` = 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = -169037 AND `source_type` = 0;
INSERT INTO `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
 `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
 `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
 `target_type`, `target_param1`, `target_param2`, `target_param3`,
 `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
(-169037, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 17, 173, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'S.A.F.E. Technician - On Respawn - Emote Work'),
(-169037, 0, 1, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 80, 4623000, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'S.A.F.E. Technician - On Data Set 1 1 - Actionlist');

-- Wreckage line is group 1, not the cannon complaint (group 0).
UPDATE `smart_scripts` SET `action_param1` = 1, `comment` = 'Speech wreckage'
WHERE `entryorguid` = 4623000 AND `source_type` = 9 AND `id` = 2;

-- Quest 27635: drop the leftover AreaDescription tracker line, sequence obj2,
-- and allow Sanitron spellclick only while the quest is in the log.

-- AreaDescription is the third tracker row (no 0/1). Obj2 already covers it.
UPDATE `quest_template` SET `AreaDescription` = '' WHERE `ID` = 27635;
UPDATE `quest_template_locale` SET `AreaDescription` = '' WHERE `ID` = 27635;

-- Client hides this row until the previous objective is complete.
UPDATE `quest_objectives` SET `Flags` = 2 WHERE `ID` = 276351 AND `QuestID` = 27635;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 46185 AND `SourceEntry` = 125095;
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(18, 46185, 125095, 0, 0, 9, 0, 27635, 0, 0, 0, 0, 0, '', 'Sanitron 500 - Spellclick only while quest 27635 is taken');
