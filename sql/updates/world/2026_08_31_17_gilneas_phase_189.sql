-- Gilneas zone 4714 phase 189 had no conditions (late Battle for Gilneas
-- City layer). Window is Questie GILNEAS_CHAPTER_12: Patriarch's Blessing
-- (24679) until Rut'theran (14434). 188/190 already gated in 09.

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 189
  AND `SourceEntry` = 4714;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 189, 4714, 0, 0, 8, 0, 24679, 0, 0, 0, 0, 0, '',
 'Gilneas phase 189 after quest 24679 is rewarded'),
(26, 189, 4714, 0, 0, 8, 0, 14434, 0, 0, 1, 0, 0, '',
 'Gilneas phase 189 until quest 14434 is rewarded');
