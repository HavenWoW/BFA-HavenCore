-- Quest 14159 The Rebel Lord's Arsenal.
--
-- Video / sniff 18019: Cellar Door GOs on the same hatch.
--   195621 phase 170, state 1 (closed) — click to open, stairs to Josiah.
--   195430 phase 171, state 0 (open)  — after the bite, 14204 / 14293.
--   195430 phase 172, state 0 (open)  — 14293 turns in to Godfrey at this
--   hatch; 171 drops and 172 starts. Legion 35906 is phaseMask 12 (171+172)
--   at the hatch; BFA already split him into 171 and 172 spawns. Without a
--   172 hatch the open door vanishes on the 14293 reward.
-- world.pkt showed 170 still on after 14159 reward, so both GOs spawned at
-- once. 170/171 were only on city 4755; the cellar is 4757, and OnAreaChange
-- walks parents without excluding the child. Mirror the windows onto 4757 so
-- the closed hatch drops when 171 (and the open hatch) come in.
-- 59073 (phase 170) and 72870 (phase 171) were AUTOCAST-only (flags=1).
-- SendQuestUpdate only strips area auras with AUTOREMOVE; leftover 59073
-- is SPELL_AURA_PHASE and OnConditionChange puts 170 back. flags=3 matches
-- 49416 on 4757. 4756 had quest_end=0 and would recast 59073 in Merchant
-- Square after 14159 — same window as 4755/4757.
-- 35370 SmartAI never ran under npc_josiah_avery — drop it.

UPDATE `creature_template`
SET `AIName` = ''
WHERE `entry` = 35370;

UPDATE `creature_template_addon` SET `emote` = 473 WHERE `entry` = 35753;
UPDATE `creature_addon` SET `emote` = 473 WHERE `guid` = 210115275;

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 35370 AND `source_type` = 0;

DELETE FROM `phase_area`
WHERE `AreaId` = 4757 AND `PhaseId` IN (170, 171);

INSERT INTO `phase_area` (`AreaId`, `PhaseId`, `Comment`)
VALUES
(4757, 170, 'Military District phase 170 until quest 14159 is rewarded'),
(4757, 171, 'Military District phase 171 after quest 14159 is rewarded');

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` IN (170, 171)
  AND `SourceEntry` = 4757;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`,
 `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`,
 `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(26, 170, 4757, 0, 0, 8, 0, 14078, 0, 0, 0, 0, 0, '',
 'Military District phase 170 after quest 14078 is rewarded'),
(26, 170, 4757, 0, 0, 8, 0, 14159, 0, 0, 1, 0, 0, '',
 'Military District phase 170 until quest 14159 is rewarded'),
(26, 171, 4757, 0, 0, 8, 0, 14159, 0, 0, 0, 0, 0, '',
 'Military District phase 171 after quest 14159 is rewarded'),
(26, 171, 4757, 0, 0, 8, 0, 14293, 0, 0, 1, 0, 0, '',
 'Military District phase 171 until quest 14293 is rewarded');

UPDATE `conditions`
SET `Comment` = 'Gilneas City phase 170 until quest 14159 is rewarded'
WHERE `SourceTypeOrReferenceId` = 26
  AND `SourceGroup` = 170
  AND `SourceEntry` = 4755
  AND `ConditionValue1` = 14159
  AND `NegativeCondition` = 1;

UPDATE `spell_area`
SET `flags` = 3
WHERE `spell` = 59073
  AND `area` IN (4714, 4755, 4757);

UPDATE `spell_area`
SET `quest_start` = 14099,
    `quest_start_status` = 66,
    `quest_end` = 14159,
    `quest_end_status` = 43,
    `flags` = 3
WHERE `spell` = 59073
  AND `area` = 4756;

UPDATE `spell_area`
SET `flags` = 3
WHERE `spell` = 72870
  AND `area` IN (4757, 4758);

DELETE FROM `gameobject`
WHERE (`guid` IN (20406566, 20406567, 20406568))
   OR (`id` IN (195430, 195621) AND `map` = 654);

DELETE FROM `gameobject_addon`
WHERE `guid` IN (20406566, 20406567, 20406568);

INSERT INTO `gameobject`
(`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnDifficulties`, `phaseUseFlags`,
 `PhaseId`, `PhaseGroup`, `terrainSwapMap`,
 `position_x`, `position_y`, `position_z`, `orientation`,
 `rotation0`, `rotation1`, `rotation2`, `rotation3`,
 `spawntimesecs`, `animprogress`, `state`, `isActive`, `ScriptName`, `VerifiedBuild`)
VALUES
(20406566, 195621, 654, 4755, 4757, '0', 0,
 170, 0, -1,
 -1790.03, 1435.53, 21.7358, 1.668168,
 -0.01063776, 0.01383495, 0.7405329, 0.6717935,
 7200, 255, 1, 0, '', 18019),
(20406567, 195430, 654, 4755, 4757, '0', 0,
 171, 0, -1,
 -1790.04, 1435.53, 21.7363, 1.668168,
 -0.01063776, 0.01383495, 0.7405329, 0.6717935,
 7200, 255, 0, 0, '', 18019),
(20406568, 195430, 654, 4755, 4757, '0', 0,
 172, 0, -1,
 -1790.04, 1435.53, 21.7363, 1.668168,
 -0.01063776, 0.01383495, 0.7405329, 0.6717935,
 7200, 255, 0, 0, '', 18019);

DELETE FROM `creature_addon` WHERE `guid` IN (210115275);
DELETE FROM `creature` WHERE `guid` IN (210115275);
