-- Gilneas zone 4714 phase 182 had no conditions, so OnAreaChange (4757→4755→4714)
-- gave every chapter-1 player the Duskhaven invasion layer. Window is Questie
-- GILNEAS_CHAPTER_6 / spell_area 68482: Invasion (14321) until Leader of the
-- Pack (14386). Lost Isles 4720 / Tanaan 182 untouched.

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 182
  AND `SourceEntry` = 4714;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 182, 4714, 0, 0, 8, 0, 14321, 0, 0, 0, 0, 0, '',
 'Gilneas phase 182 after quest 14321 is rewarded'),
(26, 182, 4714, 0, 0, 8, 0, 14386, 0, 0, 1, 0, 0, '',
 'Gilneas phase 182 until quest 14386 is rewarded');
