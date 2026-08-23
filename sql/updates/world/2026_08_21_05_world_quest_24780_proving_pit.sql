-- Proving Pit class variants - restore the static cage challenge flow.

DELETE FROM `gossip_menu_option`
WHERE `MenuId` = 10974;

INSERT INTO `gossip_menu_option`
    (`MenuId`, `OptionIndex`, `OptionIcon`, `OptionText`, `OptionBroadcastTextId`, `OptionType`, `OptionNpcFlag`, `VerifiedBuild`)
VALUES
    (10974, 0, 0, 'I''m ready to face my challenge.', 37885, 1, 1, 27101);

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 15
  AND `SourceGroup` = 10974;

INSERT INTO `conditions`
    (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
     `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`,
     `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
    (15, 10974, 0, 0, 0, 47, 0, 24642, 8, 0, 0, 0, 0, '', 'Show Proving Pit challenge while quest 24642 is in progress'),
    (15, 10974, 0, 0, 1, 47, 0, 24754, 8, 0, 0, 0, 0, '', 'Show Proving Pit challenge while quest 24754 is in progress'),
    (15, 10974, 0, 0, 2, 47, 0, 24762, 8, 0, 0, 0, 0, '', 'Show Proving Pit challenge while quest 24762 is in progress'),
    (15, 10974, 0, 0, 3, 47, 0, 24768, 8, 0, 0, 0, 0, '', 'Show Proving Pit challenge while quest 24768 is in progress'),
    (15, 10974, 0, 0, 4, 47, 0, 24774, 8, 0, 0, 0, 0, '', 'Show Proving Pit challenge while quest 24774 is in progress'),
    (15, 10974, 0, 0, 5, 47, 0, 24780, 8, 0, 0, 0, 0, '', 'Show Proving Pit challenge while quest 24780 is in progress'),
    (15, 10974, 0, 0, 6, 47, 0, 24786, 8, 0, 0, 0, 0, '', 'Show Proving Pit challenge while quest 24786 is in progress'),
    (15, 10974, 0, 0, 7, 47, 0, 26276, 8, 0, 0, 0, 0, '', 'Show Proving Pit challenge while quest 26276 is in progress'),
    (15, 10974, 0, 0, 8, 47, 0, 31161, 8, 0, 0, 0, 0, '', 'Show Proving Pit challenge while quest 31161 is in progress');

-- Only the class trainers start and end these class-specific quests.
DELETE FROM `creature_queststarter`
WHERE `id` IN (38142, 39062)
  AND `quest` IN (24642, 24754, 24762, 24768, 24774, 24780, 24786, 26276, 31161);

UPDATE `creature_template`
SET `npcflag` = 1,
    `unit_flags` = `unit_flags` | 512
WHERE `entry` = 39062;

-- The static scouts wait neutral and protected in their cages and respawn promptly after each challenge.
UPDATE `creature_template`
SET `faction` = 7,
    `npcflag` = 0,
    `unit_flags` = `unit_flags` | 258
WHERE `entry` = 38142;

UPDATE `creature`
SET `spawntimesecs` = 10
WHERE `guid` IN (252630, 252672)
  AND `id` = 38142;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 39062
  AND `source_type` = 0;

INSERT INTO `smart_scripts`
    (`entryorguid`, `source_type`, `id`, `link`,
     `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
     `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`,
     `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
     `target_type`, `target_param1`, `target_param2`, `target_param3`,
     `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
    (39062, 0, 0, 8, 37, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On AI Initialize - Set Ready Phase'),
    (39062, 0, 1, 2, 62, 1, 100, 0, 10974, 0, 0, 0, 0, '', 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Gossip Select - Close Gossip'),
    (39062, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Gossip Select - Set Challenge Phase'),
    (39062, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Gossip Select - Remove Gossip Flag'),
    (39062, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 39062, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Gossip Select - Grant Challenge Credit'),
    (39062, 0, 5, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Gossip Select - Set Walk'),
    (39062, 0, 6, 7, 38, 0, 100, 0, 1, 1, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Challenge Reset - Set Ready Phase'),
    (39062, 0, 7, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Challenge Reset - Add Gossip Flag'),
    (39062, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On AI Initialize - Set Passive'),
    (39062, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 69, 1, 0, 0, 2, 0, 0, 20, 201968, 30, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Gossip Select - Walk To Cage'),
    (39062, 0, 10, 11, 34, 0, 100, 0, 8, 1, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 20, 201968, 30, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Reaching Cage - Face Cage'),
    (39062, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 5, 432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Reaching Cage - Use Cage'),
    (39062, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 67, 2, 750, 750, 0, 0, 100, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Reaching Cage - Delay Cage Opening'),
    (39062, 0, 13, 0, 59, 0, 100, 0, 2, 0, 0, 0, 0, '', 99, 2, 0, 0, 0, 0, 0, 20, 201968, 30, 0, 0, 0, 0, 0, 'Darkspear Jailor - After Use Animation - Open Cage'),
    (39062, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Challenge Reset - Set Passive'),
    (39062, 0, 15, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor - On Challenge Reset - Return Home');

-- Quest 24780 uses the southern Jailor and cage. Override only these spawns
-- so the presentation path does not affect the second proving pit.
DELETE FROM `smart_scripts`
WHERE `entryorguid` = -251547
  AND `source_type` = 0;

INSERT INTO `smart_scripts`
    (`entryorguid`, `source_type`, `id`, `link`,
     `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
     `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`,
     `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
     `target_type`, `target_param1`, `target_param2`, `target_param3`,
     `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
    (-251547, 0, 0, 8, 37, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On AI Initialize - Set Ready Phase'),
    (-251547, 0, 1, 2, 62, 1, 100, 0, 10974, 0, 0, 0, 0, '', 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Gossip Select - Close Gossip'),
    (-251547, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Gossip Select - Set Challenge Phase'),
    (-251547, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Gossip Select - Remove Gossip Flag'),
    (-251547, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 33, 39062, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Gossip Select - Grant Challenge Credit'),
    (-251547, 0, 5, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 59, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Gossip Select - Set Walk'),
    (-251547, 0, 6, 7, 38, 0, 100, 0, 1, 1, 0, 0, 0, '', 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Challenge Reset - Set Ready Phase'),
    (-251547, 0, 7, 20, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Challenge Reset - Add Gossip Flag'),
    (-251547, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On AI Initialize - Set Passive'),
    (-251547, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1158.792358, -5523.962891, 11.990767, 0, 'Darkspear Jailor 251547 - Walk Around Spikes - Point 1'),
    (-251547, 0, 10, 0, 34, 0, 100, 0, 8, 1, 0, 0, 0, '', 67, 3, 1, 1, 0, 0, 100, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Point 1 - Defer Point 2 Movement'),
    (-251547, 0, 11, 0, 34, 0, 100, 0, 8, 2, 0, 0, 0, '', 67, 4, 1, 1, 0, 0, 100, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Point 2 - Defer Final Cage Movement'),
    (-251547, 0, 12, 13, 34, 0, 100, 0, 8, 3, 0, 0, 0, '', 66, 0, 0, 0, 0, 0, 0, 14, 172893, 201968, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Reaching Cage - Face Cage 172893'),
    (-251547, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 5, 432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Reaching Cage - Use Cage'),
    (-251547, 0, 14, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 67, 2, 750, 750, 0, 0, 100, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - After Use Animation - Delay Cage Opening'),
    (-251547, 0, 15, 16, 59, 0, 100, 0, 2, 0, 0, 0, 0, '', 9, 0, 0, 0, 0, 0, 0, 14, 172893, 201968, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - After Use Animation - Activate Cage 172893'),
    (-251547, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - After Cage Opening - Arm Delayed Return Phase'),
    (-251547, 0, 17, 22, 60, 4, 100, 1, 750, 750, 0, 0, 0, '', 69, 4, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1157.820068, -5519.375000, 12.101708, 0, 'Darkspear Jailor 251547 - After Cage Opening Delay - Return Through Waypoint 2'),
    (-251547, 0, 18, 0, 34, 0, 100, 0, 8, 4, 0, 0, 0, '', 67, 5, 1, 1, 0, 0, 100, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Return Waypoint 2 - Defer Waypoint 1 Movement'),
    (-251547, 0, 19, 0, 34, 0, 100, 0, 8, 5, 0, 0, 0, '', 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Return Waypoint 1 - Walk To Exact Home Position'),
    (-251547, 0, 20, 21, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Challenge Reset - Set Passive'),
    (-251547, 0, 21, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Challenge Reset - Return Home'),
    (-251547, 0, 22, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 22, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Return Start - Set Spectator Phase'),
    (-251547, 0, 23, 0, 59, 2, 100, 0, 3, 0, 0, 0, 0, '', 69, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1157.820068, -5519.375000, 12.101708, 0, 'Darkspear Jailor 251547 - After Point 1 Completion - Walk Around Spikes - Point 2'),
    (-251547, 0, 24, 0, 59, 2, 100, 0, 4, 0, 0, 0, 0, '', 69, 3, 0, 1, 0, 0, 0, 8, 0, 0, 0, -1152.20, -5519.25, 12.04, 0, 'Darkspear Jailor 251547 - After Point 2 Completion - Walk Exact Final Segment To Cage Door'),
    (-251547, 0, 25, 0, 21, 0, 100, 0, 0, 0, 0, 0, 0, '', 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Jailor 251547 - On Reaching Home - Clear Walking Movement Flag'),
    (-251547, 0, 26, 0, 59, 8, 100, 0, 5, 0, 0, 0, 0, '', 69, 5, 0, 0, 0, 0, 0, 8, 0, 0, 0, -1158.792358, -5523.962891, 11.990767, 0, 'Darkspear Jailor 251547 - After Return Waypoint 2 - Walk Around Spikes - Waypoint 1');

-- Cage 172893 retains the existing scout-release handshake while also
-- restoring its visible state whenever the scout resets its loot state.
DELETE FROM `smart_scripts`
WHERE `entryorguid` = -172893
  AND `source_type` = 1;

INSERT INTO `smart_scripts`
    (`entryorguid`, `source_type`, `id`, `link`,
     `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
     `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`,
     `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
     `target_type`, `target_param1`, `target_param2`, `target_param3`,
     `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
    (-172893, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, '', 45, 1, 1, 0, 0, 0, 0, 11, 38142, 20, 0, 0, 0, 0, 0, 'Darkspear Cage 172893 - On Activated Loot State - Release Scout'),
    (-172893, 1, 1, 0, 70, 0, 100, 0, 1, 0, 0, 0, 0, '', 118, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Cage 172893 - On Ready Loot State - Restore Closed Visual State');

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 38142
  AND `source_type` = 0;

INSERT INTO `smart_scripts`
    (`entryorguid`, `source_type`, `id`, `link`,
     `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
     `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `event_param_string`,
     `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
     `target_type`, `target_param1`, `target_param2`, `target_param3`,
     `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
    (38142, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, '', 99, 1, 0, 0, 0, 0, 0, 20, 201968, 30, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Respawn - Reset Closest Cage'),
    (38142, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 2, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Respawn - Set Neutral Faction'),
    (38142, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Respawn - Set Passive'),
    (38142, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 18, 258, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Respawn - Set Waiting Flags'),
    (38142, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 45, 1, 1, 0, 0, 0, 0, 19, 39062, 30, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Respawn - Reset Jailor'),
    (38142, 0, 5, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, '', 67, 1, 750, 750, 0, 0, 100, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Cage Opened - Delay Exit'),
    (38142, 0, 6, 7, 34, 0, 100, 0, 8, 16777214, 0, 0, 0, '', 2, 14, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Movement Complete - Set Hostile Faction'),
    (38142, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Movement Complete - Set Aggressive'),
    (38142, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 19, 258, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Movement Complete - Remove Waiting Flags'),
    (38142, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 49, 0, 0, 0, 0, 0, 0, 21, 30, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Movement Complete - Attack Nearest Player'),
    (38142, 0, 10, 0, 0, 0, 100, 1, 1000, 1000, 2000, 2000, 0, '', 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - In Combat - Say Line 0'),
    (38142, 0, 11, 12, 7, 0, 100, 0, 0, 0, 0, 0, 0, '', 2, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Evade - Set Neutral Faction'),
    (38142, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Evade - Set Passive'),
    (38142, 0, 13, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 18, 258, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Evade - Set Waiting Flags'),
    (38142, 0, 14, 15, 21, 0, 100, 0, 0, 0, 0, 0, 0, '', 99, 1, 0, 0, 0, 0, 0, 20, 201968, 30, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Reached Home - Reset Closest Cage'),
    (38142, 0, 15, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 45, 1, 1, 0, 0, 0, 0, 19, 39062, 30, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Reached Home - Reset Jailor'),
    (38142, 0, 16, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, '', 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - On Death - Despawn After Credit'),
    (38142, 0, 17, 18, 59, 0, 100, 0, 1, 0, 0, 0, 0, '', 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captive Spitescale Scout - After Cage Delay - Set Run'),
    (38142, 0, 18, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 114, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 10, 0, 0, 'Captive Spitescale Scout - After Cage Delay - Run Into Proving Area');
