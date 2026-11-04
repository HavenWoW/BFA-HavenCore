-- BFA-HavenCore 8.3.7.35662 | Shadowglen | A Favor for Melithar (28734)
-- A Favor for Melithar is an optional breadcrumb after Fel Moss Corruption (28714).
-- It should be offered only while Demonic Thieves (28715) has not been taken.
-- Quest 28734 already has PrevQuestID = 28714; preserve that chain.
-- Existing core condition support: SourceType 19 (quest available), Type 14 (quest none).
-- Follow-up to https://github.com/HavenWoW/BFA-HavenCore/issues/238 and PR #704.
-- Deliberately do not delete other condition rows: avoid overwriting unexpected DB data.
INSERT INTO `conditions`
  (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
   `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
   `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
SELECT 19, 0, 28734, 0, 0,
       14, 0, 28715, 0,
       0, 0, 0, 0, '',
       'Offer A Favor for Melithar only while Demonic Thieves has not been taken'
WHERE NOT EXISTS (
  SELECT 1 FROM `conditions`
  WHERE `SourceTypeOrReferenceId` = 19 AND `SourceEntry` = 28734
);
