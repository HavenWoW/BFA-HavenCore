-- Gilneas zone 4714 phase 191 had no conditions. Questie uses 191 for Hyjal
-- dailies, not Gilneas. Map 654 has no PhaseId 191 spawns. Drop the zone row.

DELETE FROM `phase_area`
WHERE `AreaId` = 4714 AND `PhaseId` = 191;
