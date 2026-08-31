-- Gilneas zone 4714 phase 194 had no conditions. Questie uses 194 for Hyjal
-- chapter 1, not Gilneas. Map 654 has no PhaseId 194 spawns. Drop the zone row.

DELETE FROM `phase_area`
WHERE `AreaId` = 4714 AND `PhaseId` = 194;
