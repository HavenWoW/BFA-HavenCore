-- Quest 14222 Last Stand. Crowley 35566 had two SMART_EVENT_LINK rows
-- with id=1; FindLinkedEvent takes the first (link=0) so 72788 never
-- fired after the reward movie. Unique ids, no teleport in the chain —
-- 72788 waits for movie 21 complete or skip (CMSG_COMPLETE_MOVIE).

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 35566 AND `source_type` = 0 AND `id` IN (0, 1, 2);

INSERT INTO `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`,
 `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`,
 `event_param4`, `event_param5`, `event_param_string`, `action_type`,
 `action_param1`, `action_param2`, `action_param3`, `action_param4`,
 `action_param5`, `action_param6`, `target_type`, `target_param1`,
 `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`,
 `target_o`, `comment`)
VALUES
(35566, 0, 0, 1, 20, 0, 100, 0, 14222, 0, 0, 0, 0, '', 85,
 98274, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Lord Darius Crowley - On Quest 14222 Rewarded - Invoker Cast 98274'),
(35566, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 75,
 76642, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Lord Darius Crowley - On Link - Add Aura 76642 to Invoker'),
(35566, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 85,
 68630, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Lord Darius Crowley - On Link - Invoker Cast 68630');
