-- Gilneas City 187/188/190 were in phase_area with no conditions, so every
-- chapter-1 player also saw the Battle for Gilneas City layer. 38218 (mounted
-- Liam, "The Forsaken think we're weak") is PhaseId 187; 38474 is 187 at the
-- court; 38614 is 190. Windows follow spell_area 69485/69486/70695 and Questie.
-- Gate both 4755 (city) and 4714 (Gilneas zone) — OnAreaChange walks parents.

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` IN (187, 188, 190)
  AND `SourceEntry` IN (4755, 4714);

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
-- 187: Push Them Out (24676) until The Hunt For Sylvanas (24902)
(26, 187, 4755, 0, 0, 8, 0, 24676, 0, 0, 0, 0, 0, '',
 'Gilneas City phase 187 after quest 24676 is rewarded'),
(26, 187, 4755, 0, 0, 8, 0, 24902, 0, 0, 1, 0, 0, '',
 'Gilneas City phase 187 until quest 24902 is rewarded'),
(26, 187, 4714, 0, 0, 8, 0, 24676, 0, 0, 0, 0, 0, '',
 'Gilneas phase 187 after quest 24676 is rewarded'),
(26, 187, 4714, 0, 0, 8, 0, 24902, 0, 0, 1, 0, 0, '',
 'Gilneas phase 187 until quest 24902 is rewarded'),
-- 188: 24902 until 24680 They Have Allies, But So Do We
(26, 188, 4755, 0, 0, 8, 0, 24902, 0, 0, 0, 0, 0, '',
 'Gilneas City phase 188 after quest 24902 is rewarded'),
(26, 188, 4755, 0, 0, 8, 0, 24680, 0, 0, 1, 0, 0, '',
 'Gilneas City phase 188 until quest 24680 is rewarded'),
(26, 188, 4714, 0, 0, 8, 0, 24902, 0, 0, 0, 0, 0, '',
 'Gilneas phase 188 after quest 24902 is rewarded'),
(26, 188, 4714, 0, 0, 8, 0, 24680, 0, 0, 1, 0, 0, '',
 'Gilneas phase 188 until quest 24680 is rewarded'),
-- 190: 24680 until 14434 (Keel Harbor / leave the start zone)
(26, 190, 4755, 0, 0, 8, 0, 24680, 0, 0, 0, 0, 0, '',
 'Gilneas City phase 190 after quest 24680 is rewarded'),
(26, 190, 4755, 0, 0, 8, 0, 14434, 0, 0, 1, 0, 0, '',
 'Gilneas City phase 190 until quest 14434 is rewarded');
