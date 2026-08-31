-- Gilneas phase 171 (Save Krennan / 14293): 35753 is PhaseId 171 with
-- Generic Quest Invisibility 1 (49414). Two conditions hid him.
--
-- 1) Westfall 171 used SourceEntry=0. addToPhases then attaches 26232 to
--    every area that has phase 171, including Gilneas City 4755.
-- 2) 171 on 4755 required 14159 NOT rewarded — inverted vs Questie and
--    spell_area 72870 (14159 rewarded until 14293 rewarded).
-- 3) Detect 49416 on Military District 4757 required 14294 rewarded, so
--    it never applied during 14293. Align it with 72870.

-- Westfall 171 is area 40 only, not a global phase-171 gate.
UPDATE `conditions`
SET `SourceEntry` = 40,
    `Comment` = 'Westfall phase 171 after quest 26232 is complete or rewarded'
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 171
  AND `SourceEntry` = 0
  AND `ConditionValue1` = 26232;

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 171
  AND `SourceEntry` = 4755;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 171, 4755, 0, 0, 8, 0, 14159, 0, 0, 0, 0, 0, '',
 'Gilneas City phase 171 after quest 14159 is rewarded'),
(26, 171, 4755, 0, 0, 8, 0, 14293, 0, 0, 1, 0, 0, '',
 'Gilneas City phase 171 until quest 14293 is rewarded');

UPDATE `spell_area`
SET `quest_start` = 14159,
    `quest_end` = 14293,
    `quest_start_status` = 64,
    `quest_end_status` = 43
WHERE `spell` = 49416
  AND `area` = 4757;
