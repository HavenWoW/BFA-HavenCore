-- Brubaker NPC (ID 43508)
-- Show as dead.
-- Not assist units.
UPDATE `creature_template` 
SET `unit_flags3` = 8192, `type_flags` = 262144 
WHERE (`entry` = 43508);

-- Brubaker NPC (ID 43508)
-- Position fixed.
DELETE FROM `creature` WHERE `id` = 43508;
INSERT INTO `creature`
  (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `ScriptName`, `VerifiedBuild`)
VALUES
  (372545, 43508, 0, 44, 44, '0', 0, 0, 0, -1, 0, 0, -9676.08984375, -2842, 54.468456, 0.07098700106143951, 300, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '', 35662);