-- Gilneas zone 4714 phase 183 had no conditions. Window is Questie
-- GILNEAS_CHAPTER_7: Leader of the Pack (14386) until any Duskhaven escape
-- (14402 / 14405 / 14463). Lost Isles 4720 / Tanaan 183 untouched.

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 183
  AND `SourceEntry` = 4714;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 183, 4714, 0, 0, 8, 0, 14386, 0, 0, 0, 0, 0, '',
 'Gilneas phase 183 after quest 14386 is rewarded'),
(26, 183, 4714, 0, 0, 8, 0, 14402, 0, 0, 1, 0, 0, '',
 'Gilneas phase 183 until quest 14402 is rewarded'),
(26, 183, 4714, 0, 0, 8, 0, 14405, 0, 0, 1, 0, 0, '',
 'Gilneas phase 183 until quest 14405 is rewarded'),
(26, 183, 4714, 0, 0, 8, 0, 14463, 0, 0, 1, 0, 0, '',
 'Gilneas phase 183 until quest 14463 is rewarded');
