-- Gilneas zone 4714 phase 184 had no conditions. Window is Questie
-- GILNEAS_CHAPTER_8: any Duskhaven escape (14402 / 14405 / 14463) until
-- Alas, Gilneas (14467). ElseGroup OR for the three starters. Lost Isles
-- 4720 / Tanaan 184 untouched.

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 184
  AND `SourceEntry` = 4714;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 184, 4714, 0, 0, 8, 0, 14402, 0, 0, 0, 0, 0, '',
 'Gilneas phase 184 after quest 14402 is rewarded'),
(26, 184, 4714, 0, 0, 8, 0, 14467, 0, 0, 1, 0, 0, '',
 'Gilneas phase 184 until quest 14467 is rewarded'),
(26, 184, 4714, 0, 1, 8, 0, 14405, 0, 0, 0, 0, 0, '',
 'Gilneas phase 184 after quest 14405 is rewarded'),
(26, 184, 4714, 0, 1, 8, 0, 14467, 0, 0, 1, 0, 0, '',
 'Gilneas phase 184 until quest 14467 is rewarded'),
(26, 184, 4714, 0, 2, 8, 0, 14463, 0, 0, 0, 0, 0, '',
 'Gilneas phase 184 after quest 14463 is rewarded'),
(26, 184, 4714, 0, 2, 8, 0, 14467, 0, 0, 1, 0, 0, '',
 'Gilneas phase 184 until quest 14467 is rewarded');
