-- Quest 14212 Sacrifices.
--
-- 1) Crowley's Horse 44427: spellclick 56685 had no quest gate, so anyone
--    could mount. 67001 (summon the vehicle) already required 14212 taken.
-- 2) Bloodfang Stalker 35229: 67063 SAI left them burning 15-30s then
--    stripped the DoT instead of dying. Duplicate spellhit rows also
--    double-credited with spell_gen_throw_torch. Credit stays in C++; SAI
--    only lights them and kills after a short burn.
-- 3) Cathedral 4761 listed phase 179 with no conditions, so Last Stand
--    NPCs were visible to every player who entered the cathedral (including
--    the 14212 ride). Gate it the same as city 4755 (14221 rewarded).

-- Spellclick 56685 only while Sacrifices is in the log (INCOMPLETE).
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 18
  AND `SourceGroup` = 44427
  AND `SourceEntry` = 56685;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(18, 44427, 56685, 0, 0, 9, 0, 14212, 0, 0, 0, 0, 0, '',
 'Crowley Horse 44427 - Spellclick 56685 only while quest 14212 is taken');

-- Replace the torch chain and the duplicate 30% enrage. Keep spawn evade
-- disable, the linked 30% enrage, and the enrage yell.
DELETE FROM `smart_scripts`
WHERE `entryorguid` = 35229
  AND `source_type` = 0
  AND ((`id` = 1 AND `link` = 2) OR `id` IN (2, 3, 4, 5, 6, 7));

INSERT INTO `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`,
 `event_chance`, `event_flags`,
 `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
 `action_type`, `action_param1`, `action_param2`, `action_param3`,
 `action_param4`, `action_param5`, `action_param6`,
 `target_type`, `target_param1`, `target_param2`, `target_param3`,
 `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
(35229, 0, 2, 3, 8, 0, 100, 512, 67063, 0, 0, 0, 0,
 11, 77676, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
 'Bloodfang Stalker - On Spellhit Throw Torch - Cast Flames'),
(35229, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0,
 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
 'Bloodfang Stalker - Link - Set React Passive'),
(35229, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0,
 18, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
 'Bloodfang Stalker - Link - Set Unselectable'),
(35229, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0,
 67, 1, 2000, 2500, 0, 0, 100, 1, 0, 0, 0, 0, 0, 0, 0,
 'Bloodfang Stalker - Link - Timed Event 2s'),
(35229, 0, 6, 0, 59, 0, 100, 0, 1, 0, 0, 0, 0,
 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
 'Bloodfang Stalker - On Timed Event 1 - Die');

-- Cathedral 4761 phase 179 had no conditions (always on).
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 179
  AND `SourceEntry` = 4761;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 179, 4761, 0, 0, 8, 0, 14221, 0, 0, 0, 0, 0, '',
 'Light''s Dawn Cathedral phase 179 after quest 14221 is rewarded');
