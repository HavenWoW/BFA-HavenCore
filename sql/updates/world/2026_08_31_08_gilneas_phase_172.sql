-- Gilneas phase 172 starts when Save Krennan (14293) is rewarded, not Time to
-- Regroup (14294). 14293 turns in to Godfrey 35906 in phase 171; 14294 is
-- started by the phase-172 Godfrey spawn. Gating 172 on 14294 left a gap with
-- no Godfrey after 171 dropped. Window matches spell_area 72872 and Questie
-- (14293 rewarded until 14221 Never Surrender).

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 172
  AND `SourceEntry` = 4755;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 172, 4755, 0, 0, 8, 0, 14293, 0, 0, 0, 0, 0, '',
 'Gilneas City phase 172 after quest 14293 is rewarded'),
(26, 172, 4755, 0, 0, 8, 0, 14221, 0, 0, 1, 0, 0, '',
 'Gilneas City phase 172 until quest 14221 is rewarded');
