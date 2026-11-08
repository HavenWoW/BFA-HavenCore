-- 120 seconds duration
DELETE FROM spell_duration WHERE ID = 62624;
INSERT INTO spell_duration (ID, VerifiedBuild, Duration, DurationPerLevel, MaxDuration)
VALUES (62624, 35662, 120000, 0, 120000);

-- ID comes from wow.tools
DELETE FROM spell_misc WHERE ID = 43104 AND SpellID = 62624;
INSERT INTO spell_misc
(ID, VerifiedBuild,
 Attributes1, Attributes2, Attributes3, Attributes4, Attributes5, Attributes6, Attributes7,
 Attributes8, Attributes9, Attributes10, Attributes11, Attributes12, Attributes13, Attributes14,
 DifficultyID, CastingTimeIndex, DurationIndex, RangeIndex, SchoolMask, Speed, LaunchDelay,
 MinDuration, SpellIconFileDataID, ActiveIconFileDataID, ContentTuningID, SpellID)
VALUES
(43104, 35662,
 256, 1024, 4, 268435456, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0,
 0, 4, 62624, 11, 1, 15, 0,
 0, 135433, 0, 0, 62624);
