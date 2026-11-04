-- https://github.com/HavenWoW/BFA-HavenCore/issues/6
-- https://github.com/HavenWoW/BFA-HavenCore/issues/76

-- Fix Honk Horn target
DELETE FROM spell_effect WHERE ID=70809;
INSERT INTO spell_effect
(ID, VerifiedBuild, EffectAura, DifficultyID, EffectIndex, Effect, EffectAmplitude, EffectAttributes, EffectAuraPeriod, EffectBonusCoefficient, EffectChainAmplitude, EffectChainTargets, EffectItemType, EffectMechanic, EffectPointsPerResource, EffectPosFacing, EffectRealPointsPerLevel, EffectTriggerSpell, BonusCoefficientFromAP, PvpMultiplier, Coefficient, Variance, ResourceCoefficient, GroupSizeBasePointsCoefficient, EffectBasePoints, EffectMiscValue1, EffectMiscValue2, EffectRadiusIndex1, EffectRadiusIndex2, EffectSpellClassMask1, EffectSpellClassMask2, EffectSpellClassMask3, EffectSpellClassMask4, ImplicitTarget1, ImplicitTarget2, SpellID)
VALUES
(70809, 35662, 0, 0, 0, 131, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 22491, 0, 0, 0, 0, 0, 0, 0, 96, 0, 66298);
