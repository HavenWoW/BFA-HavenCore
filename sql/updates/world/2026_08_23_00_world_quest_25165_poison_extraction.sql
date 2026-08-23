-- Quest 25165 grants venom credit only when Envenom hits a player using
-- the Poison Extraction Totem.

UPDATE `creature_template`
SET `KillCredit2` = 0
WHERE `entry` = 3125
  AND `KillCredit2` = 39236;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 3125
  AND `source_type` = 0
  AND `id` = 3
  AND `link` = 0;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 3125
  AND `source_type` = 0
  AND `id` = 1
  AND `link` = 2;

UPDATE `smart_scripts`
SET `event_flags` = 1
WHERE `entryorguid` = 3125
  AND `source_type` = 0
  AND `id` = 1
  AND `link` = 0
  AND `event_type` = 0
  AND `action_type` = 11
  AND `action_param1` = 73672;
