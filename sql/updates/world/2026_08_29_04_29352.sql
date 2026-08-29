-- Quest 29352 A Fowl Shortage: click Dun Morogh Chicken 53568 for item 69982.
-- Spellclick only had dummy 99486 (player-on-player) and SAI listened for
-- SPELLHIT_TARGET of 99487, which is never cast. Mirror Sen'jin Frog 40176:
-- dummy on the chicken, CREATE_ITEM on the player, despawn on dummy hit.

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 53568;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(53568, 99486, 1, 0),
(53568, 99487, 3, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 53568 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `comment`) VALUES
(53568, 0, 0, 0, 8, 0, 100, 0, 99486, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'Dun Morogh Chicken - On Spellhit Pickup Chicken dummy - Despawn');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 53568;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 53568, 99486, 0, 0, 8, 0, 29352, 0, 0, 1, 0, 0, '', 'Forbidden rewarded quest for spellclick'),
(18, 53568, 99486, 0, 0, 9, 0, 29352, 0, 0, 0, 0, 0, '', 'Required quest active for spellclick'),
(18, 53568, 99487, 0, 0, 8, 0, 29352, 0, 0, 1, 0, 0, '', 'Forbidden rewarded quest for spellclick'),
(18, 53568, 99487, 0, 0, 9, 0, 29352, 0, 0, 0, 0, 0, '', 'Required quest active for spellclick');

DELETE FROM `creature_addon` WHERE `guid` IN (302907, 302937, 302938, 302939, 302940, 302941, 302942, 302943, 302944, 302945, 302946, 302947, 302948, 302949, 302950, 302951, 302952, 302953, 302954, 302955, 302956, 302957, 302958, 302960, 302961, 302962, 302963, 302964, 302965, 302966, 302967, 302968, 302969, 302970, 302971, 302972, 302973, 302974, 302975, 302976, 302977, 302978, 302979, 302980);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `aiAnimKit`, `movementAnimKit`, `meleeAnimKit`, `visibilityDistanceType`, `auras`) VALUES 
(302907, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302937, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302938, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302939, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302940, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302941, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302942, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302943, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302944, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302945, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302946, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302947, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302948, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302949, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302950, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302951, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302952, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302953, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302954, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302955, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302956, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302957, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302958, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302960, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302961, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302962, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302963, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302964, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302965, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302966, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302967, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302968, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302969, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302970, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302971, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302972, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302973, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302974, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302975, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302976, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302977, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302978, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302979, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357'),
(302980, 0, 0, 0, 1, 0, 0, 0, 0, 0, '94357');