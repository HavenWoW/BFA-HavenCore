-- Training dummies must never walk towards their attacker.
--
-- CreatureAI::AttackStart only starts a chase when Unit::Attack succeeds, and
-- Unit::Attack refuses to run for units flagged UNIT_FLAG_PACIFIED (131072).
-- Most training dummies in the database already carry that flag; the entries
-- matched below were missing it, so attacking them from range made them chase
-- the player into melee range instead of staying in place.
--
-- Confirmed case: entry 44937 (starting zone "Training Dummy", map 530) had
-- unit_flags = 4 (UNIT_FLAG_REMOVE_CLIENT_CONTROL), no ScriptName and SmartAI,
-- so SmartAI::AttackStart happily called MoveChase() on every ranged attack.
--
-- UNIT_FLAG_PACIFIED only stops the unit from attacking/chasing on its own. It
-- can still be attacked, still takes damage and still enters combat, so these
-- creatures keep working as DPS dummies.
--
-- Excluded on purpose:
--   * entries already handled by npc_training_dummy / npc_training_dummy_start_zones
--   * "Brewfest Target Dummy Move To Target" and other dummies whose name says
--     they have to move
--   * event / scan / invisible / decoy / distraction / bunny helpers

UPDATE `creature_template`
SET `unit_flags` = `unit_flags` | 131072
WHERE (`name` LIKE '%Training Dummy%'
    OR `name` LIKE '%Practice Dummy%'
    OR `name` LIKE '%Target Dummy%'
    OR `name` LIKE '%Combat Dummy%')
  AND (`unit_flags` & 131072) = 0
  AND `ScriptName` NOT IN ('npc_training_dummy', 'npc_training_dummy_start_zones')
  AND `name` NOT LIKE '%Move To Target%'
  AND `name` NOT LIKE '%Scan%'
  AND `name` NOT LIKE '%Event%'
  AND `name` NOT LIKE '%Invisible%'
  AND `name` NOT LIKE '%Escape%'
  AND `name` NOT LIKE '%Decoy%'
  AND `name` NOT LIKE '%Distraction%'
  AND `name` NOT LIKE '%Bunny%';
