-- Gilneas City 169/170: mutually exclusive layers for Merchant Square chapter 1.
-- Source of truth is spell_area 59073/49416 on area 4756 (quest_start 14091,
-- quest_start_status 64 = rewarded). Unconditional phase_area 170 on 4756 plus
-- gilneas_phases AddPhase(170) stacked both layers. Westfall/Tanaan 170 untouched.

-- 169 on Gilneas City: until Something's Amiss (14091) is rewarded, not Lockdown.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 169
  AND `SourceEntry` = 4755;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 169, 4755, 0, 0, 8, 0, 14091, 0, 0, 1, 0, 0, '',
 'Gilneas City phase 169 until quest 14091 is rewarded');

-- 170 on Gilneas City: 14091 rewarded (was complete) AND 14159 not rewarded.
UPDATE `conditions`
SET `ConditionTypeOrReference` = 8,
    `Comment` = 'Gilneas City phase 170 after quest 14091 is rewarded'
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 170
  AND `SourceEntry` = 4755
  AND `ConditionTypeOrReference` = 28
  AND `ConditionValue1` = 14091;

-- Merchant Square inherited 170 from 4755; the extra row had no conditions.
DELETE FROM `phase_area` WHERE `AreaId` = 4756 AND `PhaseId` = 170;

-- Orphan 169 condition: 4756 has no phase_area 169, so it never attached.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 169
  AND `SourceEntry` = 4756
  AND `ConditionValue1` = 14091;
