-- Gilneas zone 4714 phase 186 had no conditions (500+ Duskhaven/city NPCs).
-- Window is Questie GILNEAS_CHAPTER_9 / spell_area 69484: Alas, Gilneas
-- (14467) until Push Them Out (24676). Tanaan 186 untouched.

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 186
  AND `SourceEntry` = 4714;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 186, 4714, 0, 0, 8, 0, 14467, 0, 0, 0, 0, 0, '',
 'Gilneas phase 186 after quest 14467 is rewarded'),
(26, 186, 4714, 0, 0, 8, 0, 24676, 0, 0, 1, 0, 0, '',
 'Gilneas phase 186 until quest 24676 is rewarded');
