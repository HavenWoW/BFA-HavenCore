-- Gilneas zone 4714 phase 180. Dump leftover: comment says PHASE181, conditions
-- are 14222/14375 NOT rewarded, so every chapter-1 player gets 180. Questie has
-- no Gilneas chapter 180 (Lost Isles 5 / Tanaan). Map 654 has no PhaseId 180
-- spawns. Drop the zone row; leave 4720 and Tanaan 180 alone.

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 180
  AND `SourceEntry` = 4714;

DELETE FROM `phase_area`
WHERE `AreaId` = 4714 AND `PhaseId` = 180;
