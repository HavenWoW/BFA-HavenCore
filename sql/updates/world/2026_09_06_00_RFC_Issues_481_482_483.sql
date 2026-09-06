-- Ragefire Chasm retail polish
-- Fixes:
--   #481 Corrupted Houndmaster - Incite Frenzy targets players/caster
--   #482 Missing Adarogg intro speeches
--   #483 Oggleflint should appear dead
--
-- Reference basis:
--   - Wowhead / retail behavior: Incite Frenzy affects nearby Flame Hounds.
--   - TrinityCore retail sniff/reference data for RFC:
--       * BroadcastText 61203 / 61204
--       * AreaTrigger 7904
--       * Oggleflint aura 35356 (Spawn Feign Death)
--       * Oggleflint unit_flags 768, unit_flags2 2048, unit_flags3 1
--   - HavenCore corpse presentation requires dynamicflags = 32
--     (UNIT_DYNFLAG_DEAD) so the client presents the NPC as dead.

-- ---------------------------------------------------------------------------
-- #481 - Incite Frenzy (120093)
-- Restrict all three aura effects to the two RFC Flame Hound entries.
--
-- SourceTypeOrReferenceId 13 = spell implicit target
-- SourceGroup 1/2/4 = effect masks for effects 0/1/2
-- Condition 31 = legacy object entry/guid
-- ConditionValue1 3 = TYPEID_UNIT
-- ElseGroup provides OR behavior between Adolescent and Mature Flame Hounds.
-- ---------------------------------------------------------------------------

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 13
  AND `SourceEntry` = 120093;

INSERT INTO `conditions`
(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,
 `ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,
 `NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`)
VALUES
(13,1,120093,0,0,31,0,3,61657,0,0,0,0,'','Incite Frenzy effect 0 - Adolescent Flame Hound'),
(13,1,120093,0,1,31,0,3,61658,0,0,0,0,'','Incite Frenzy effect 0 - Mature Flame Hound'),

(13,2,120093,0,0,31,0,3,61657,0,0,0,0,'','Incite Frenzy effect 1 - Adolescent Flame Hound'),
(13,2,120093,0,1,31,0,3,61658,0,0,0,0,'','Incite Frenzy effect 1 - Mature Flame Hound'),

(13,4,120093,0,0,31,0,3,61657,0,0,0,0,'','Incite Frenzy effect 2 - Adolescent Flame Hound'),
(13,4,120093,0,1,31,0,3,61658,0,0,0,0,'','Incite Frenzy effect 2 - Mature Flame Hound');

-- Existing SmartAI remains unchanged:
-- Corrupted Houndmaster casts 120093 on self at 60% health.
-- The spell's implicit targets are now filtered to Flame Hounds only.

-- ---------------------------------------------------------------------------
-- #482 - Missing Adarogg intro speeches
-- BroadcastText IDs preserve client localization.
-- ---------------------------------------------------------------------------

DELETE FROM `creature_text`
WHERE `CreatureID` = 61666
  AND `GroupID` IN (0,1);

INSERT INTO `creature_text`
(`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,
 `Emote`,`Duration`,`Sound`,`BroadcastTextId`,`TextRange`)
VALUES
(61666,0,0,'He''s cornered!',14,0,100,0,0,0,61203,0),
(61666,1,0,'We''ve got him now!',14,0,100,0,0,0,61204,0);

DELETE FROM `areatrigger_scripts`
WHERE `entry` = 7904;

INSERT INTO `areatrigger_scripts`
(`entry`,`ScriptName`)
VALUES
(7904,'at_rfc_adarogg_intro');

-- ---------------------------------------------------------------------------
-- #483 - Oggleflint (61669)
-- Retail presents Oggleflint as a corpse using Spawn Feign Death (35356).
-- HavenCore also needs UNIT_DYNFLAG_DEAD for the client to show him as dead.
-- ---------------------------------------------------------------------------

UPDATE `creature_template`
SET
    `AIName`       = '',
    `ScriptName`   = '',
    `unit_flags`   = 768,
    `unit_flags2`  = 2048,
    `unit_flags3`  = 1,
    `dynamicflags` = 32
WHERE `entry` = 61669;

INSERT INTO `creature_template_addon`
(`entry`,`auras`)
VALUES
(61669,'35356')
ON DUPLICATE KEY UPDATE
`auras` = '35356';

-- Keep the existing Oggleflint spawn.
-- Current HavenCore location already matches retail/reference data closely:
--   -276.682, -34.6181, -60.6085
