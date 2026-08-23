-- Tenaron Stormgrip (ID 3514) says his lines now and cast Winds Embrace on player.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 3514 AND `source_type` = 0;
INSERT INTO `smart_scripts` (
  `entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
  `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`,
  `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
  `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`
) VALUES
  (3514, 0, 0, 0, 10, 0, 100, 0, 1, 10, 20000, 20000, 0, '', 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tenaron Stormgrip - Ooc los  - Say line 1'),
  (3514, 0, 0, 1, 19, 0, 100, 0, 28731, 0, 0, 0, 0, '', 33, 3515, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, ''),
  (3514, 0, 1, 2, 19, 0, 100, 0, 28731, 0, 0, 0, 0, '', 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tenaron Stormgrip - On quest accepted  - Talk'),
  (3514, 0, 2, 0, 19, 0, 100, 0, 28731, 0, 0, 0, 0, '', 11, 65453, 0, 0, 1, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tenaron Stormgrip - On quest accepted  - Cast Spell');