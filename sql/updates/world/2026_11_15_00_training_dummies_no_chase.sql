-- Training dummies must never walk towards their attacker.
--
-- CreatureAI::AttackStart only starts a chase when Unit::Attack succeeds, and
-- Unit::Attack refuses to run for units flagged UNIT_FLAG_PACIFIED (131072).
-- Most training dummies already carry that flag; the ones listed below were the
-- remaining dummy templates without it, so attacking them from range made them
-- chase the player into melee range instead of staying in place.
--
-- Confirmed case: entry 44937 (starting zone "Training Dummy") had
-- unit_flags = 4 (UNIT_FLAG_REMOVE_CLIENT_CONTROL) and no ScriptName, so
-- SmartAI::AttackStart happily called MoveChase() on every ranged attack.
--
-- UNIT_FLAG_PACIFIED only stops the unit from attacking/chasing on its own. It
-- can still be attacked, still takes damage and still enters combat, so these
-- creatures keep working as DPS dummies.
--
-- The entries are frozen here instead of being selected by `name` at runtime,
-- because creature names are language dependent and were never meant to drive
-- an UPDATE. The list is the complete set of `creature_template` rows whose
-- name contains Training/Practice/Target/Combat Dummy, minus:
--   * entries handled by npc_training_dummy / npc_training_dummy_start_zones
--   * "Move To Target" dummies and other helpers that have to move
--     (Scan / Event / Invisible / Escape / Decoy / Distraction / Bunny)
-- Rows that already carry the flag are skipped by the bit check below, so the
-- statement stays safe to re-run.
--
-- To regenerate the list:
--   SELECT entry FROM creature_template
--   WHERE (name LIKE '%Training Dummy%' OR name LIKE '%Practice Dummy%'
--       OR name LIKE '%Target Dummy%'   OR name LIKE '%Combat Dummy%')
--     AND ScriptName NOT IN ('npc_training_dummy', 'npc_training_dummy_start_zones')
--     AND name NOT LIKE '%Move To Target%' AND name NOT LIKE '%Scan%'
--     AND name NOT LIKE '%Event%' AND name NOT LIKE '%Invisible%'
--     AND name NOT LIKE '%Escape%' AND name NOT LIKE '%Decoy%'
--     AND name NOT LIKE '%Distraction%' AND name NOT LIKE '%Bunny%';

UPDATE `creature_template`
SET `unit_flags` = `unit_flags` | 131072
WHERE `entry` IN (
    1921, 4952, 4957, 5652, 5723, 11875, 12385, 12426, 16211, 16897,
    16898, 16899, 17059, 17060, 18156, 18157, 18158, 18215, 18504, 19139,
    19376, 19387, 19388, 25225, 32647, 36529, 42328, 44937, 48304, 56126,
    70245, 71024, 78984, 79414, 79987, 80017, 87317, 87320, 87321, 87322,
    87329, 87760, 87761, 87762, 88288, 88289, 88835, 88836, 88837, 88906,
    88967, 89078, 89321, 92164, 92165, 92166, 92167, 92168, 92169, 93828,
    96442, 97668, 98581, 100648, 101824, 103703, 104530, 107104, 107557, 108420,
    109595, 111824, 113858, 113859, 113860, 113862, 113863, 113864, 113871, 113963,
    113964, 113966, 113967, 114832, 114840, 115898, 115899, 117881, 119632, 122327,
    126340, 126712, 126781, 127019, 129485, 131975, 131983, 131985, 131989, 131990,
    131992, 131994, 131997, 131998, 132036, 132976, 134324, 138048, 143119, 143509,
    143947, 144074, 144075, 144076, 144077, 144078, 144079, 144080, 144081, 144082,
    144083, 144085, 144086, 144986, 153285, 153292, 159598, 159599, 159600, 163534
)
  AND (`unit_flags` & 131072) = 0;
