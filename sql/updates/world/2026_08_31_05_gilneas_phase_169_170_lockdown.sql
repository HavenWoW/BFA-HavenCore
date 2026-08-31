-- 169/170 switch at Lockdown (14078) reward, not Something's Amiss (14091).
-- 14078 is turned in to Walden (34863, phase 169) and RewardNextQuest auto-offers
-- 14091; 14091 is turned in to Liam 34913 who only exists in phase 170. Gating
-- 170 on 14091 reward left the player stuck in 169 with no Liam to turn in to.
-- spell_area 59073 on 4756 (quest 14091) is a separate invisibility/aura layer.

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 169
  AND `SourceEntry` = 4755;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 169, 4755, 0, 0, 8, 0, 14078, 0, 0, 1, 0, 0, '',
 'Gilneas City phase 169 until quest 14078 is rewarded');

-- 04 swapped 170 onto 14091; base dump already has 170/14078. Drop both
-- before re-insert so a re-run is not a duplicate-key abort.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 170
  AND `SourceEntry` = 4755
  AND `ConditionValue1` IN (14091, 14078);

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 170, 4755, 0, 0, 8, 0, 14078, 0, 0, 0, 0, 0, '',
 'Gilneas City phase 170 after quest 14078 is rewarded');
