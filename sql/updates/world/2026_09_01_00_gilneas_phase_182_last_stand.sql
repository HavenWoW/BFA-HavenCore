-- Last Stand 14222 teleports into Duskhaven stocks. Genn 36332 guid 372321
-- and Krennan 36331 are PhaseId 182, but 13 gated 182 on Invasion (14321).
-- Open 182 from 14222 so the stocks NPCs are visible after the movie.
-- 68482 AUTOREMOVE would strip the phase aura if quest_start stayed 14321.
-- Tanaan 68482 rows untouched.

UPDATE `conditions`
SET `ConditionValue1` = 14222,
    `Comment` = 'Gilneas phase 182 after quest 14222 is rewarded'
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 182
  AND `SourceEntry` = 4714
  AND `ConditionValue1` = 14321
  AND `NegativeCondition` = 0;

UPDATE `spell_area`
SET `quest_start` = 14222
WHERE `spell` = 68482
  AND `area` IN (4714, 4786, 4792, 4793, 4806, 4807, 4808, 4818, 5720)
  AND `quest_start` = 14321;
