-- Seize the Ambassador (26118): C++ owns the council scene and escort.
-- Broken SAI on 42153 completed the quest on a 1s timer and talked through
-- Outland GUIDs 82277/82278 instead of High Seat guards.

UPDATE `quest_template_addon` SET `PrevQuestID` = 26112 WHERE `ID` = 26118;

UPDATE `creature_template` SET `AIName` = '' WHERE `entry` = 42146;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_ambassador_slaghammer' WHERE `entry` = 42153;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 42146 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 42153 AND `source_type` = 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4215300 AND `source_type` = 9;

DELETE FROM `spell_script_names` WHERE `spell_id` = 78628 AND `ScriptName` = 'spell_ironforge_arrest_slaghammer';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(78628, 'spell_ironforge_arrest_slaghammer');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (13, 17) AND `SourceEntry` = 78628;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 78628, 0, 0, 31, 0, 3, 42146, 0, 0, 0, 0, '', 'Arrest dest must be Ambassador Slaghammer'),
(13, 2, 78628, 0, 0, 31, 0, 3, 42146, 0, 0, 0, 0, '', 'Arrest dummy must hit Ambassador Slaghammer'),
(17, 0, 78628, 0, 0, 9, 0, 26118, 0, 0, 0, 0, 0, '', 'Arrest requires Seize the Ambassador taken');

UPDATE `creature` SET `MovementType` = 0 WHERE `guid` IN (180727, 180726, 180861);
UPDATE `creature` SET `unit_flags2` = 2048 WHERE `guid` = 180861;

DELETE FROM `creature_addon` WHERE `guid` IN (180727, 180726, 180861);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`) VALUES
(180727, 0, 0, 0, 1, 0, 0, 0, 0, 0),
(180726, 0, 0, 0, 1, 0, 0, 0, 0, 0),
(180861, 0, 0, 0, 1, 0, 0, 0, 0, 0);
