-- 837 blizzlike: Twilight beam spline unique points. Duplicate 0/28 endpoints stall Catmullrom.
-- Length 28 yd from Wowhead 25-30 (DBC has no length). TimeToTarget stays 4000.

DELETE FROM `spell_areatrigger_splines` WHERE `SpellMiscId` = 19034;
INSERT INTO `spell_areatrigger_splines` (`SpellMiscId`, `Idx`, `X`, `Y`, `Z`, `VerifiedBuild`) VALUES
(19034, 0,  0,    0, 0, 35662),
(19034, 1,  9.24, 0, 0, 35662),
(19034, 2, 18.48, 0, 0, 35662),
(19034, 3, 28,    0, 0, 35662);
