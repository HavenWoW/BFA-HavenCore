DELETE FROM `game_event_npc_vendor` WHERE `eventEntry` = 10 AND `guid` = 10675053 AND `item` IN (69895, 69896);
DELETE FROM `game_event_npc_vendor` WHERE `eventEntry` = 10 AND `guid` = 299101 AND `item` IN (69895, 69896);
INSERT INTO `game_event_npc_vendor` (`eventEntry`, `guid`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `BonusListIDs`, `PlayerConditionID`, `IgnoreFiltering`) VALUES (10, 10675053, 0, 69895, 0, 0, 0, 1, NULL, 0, 0);
INSERT INTO `game_event_npc_vendor` (`eventEntry`, `guid`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `BonusListIDs`, `PlayerConditionID`, `IgnoreFiltering`) VALUES (10, 10675053, 0, 69896, 0, 0, 0, 1, NULL, 0, 0);
INSERT INTO `game_event_npc_vendor` (`eventEntry`, `guid`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `BonusListIDs`, `PlayerConditionID`, `IgnoreFiltering`) VALUES (10, 299101, 0, 69895, 0, 0, 0, 1, NULL, 0, 0);
INSERT INTO `game_event_npc_vendor` (`eventEntry`, `guid`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `BonusListIDs`, `PlayerConditionID`, `IgnoreFiltering`) VALUES (10, 299101, 0, 69896, 0, 0, 0, 1, NULL, 0, 0);

DELETE FROM `npc_vendor` WHERE `entry` = 52358 AND `item` IN (69895, 69896);
DELETE FROM `npc_vendor` WHERE `entry` = 52809 AND `item` IN (69895, 69896);

/* Alliance */
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 52358 AND `SourceEntry` = 69057;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`) VALUES (23, 52358, 69057, 47, 171, 42);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 52358 AND `SourceEntry` = 68890;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`) VALUES (23, 52358, 68890, 47, 29117, 42);

/* Horde */
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 52809 AND `SourceEntry` = 69057;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`) VALUES (23, 52809, 69057, 47, 5502, 42);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 23 AND `SourceGroup` = 52809 AND `SourceEntry` = 69231;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`) VALUES (23, 52809, 69231, 47, 29190, 42);