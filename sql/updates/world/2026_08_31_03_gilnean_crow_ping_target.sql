-- Spell 93275 (Ping Gilnean Crow) is cast by Crow Scatter Trap 207417 onto the
-- approaching player. Without an implicit-target condition the effect has no
-- entry filter, so only some nearby units are hit and the rest of the flock stays.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 93275;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 93275, 0, 0, 31, 0, 3, 50260, 0, 0, 0, 0, '', 'Spell Ping Gilnean Crow (effect 0) hits unit Gilnean Crow');
