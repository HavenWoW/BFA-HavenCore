-- https://github.com/HavenWoW/BFA-HavenCore/issues/523

-- Change spell target
DELETE FROM spell_effect WHERE ID = 33882;
INSERT INTO spell_effect
(ID, EffectAura, DifficultyID, EffectIndex, Effect, EffectAmplitude, EffectAttributes, EffectAuraPeriod,
 EffectBonusCoefficient, EffectChainAmplitude, EffectChainTargets, EffectItemType, EffectMechanic,
 EffectPointsPerResource, EffectPosFacing, EffectRealPointsPerLevel, EffectTriggerSpell,
 BonusCoefficientFromAP, PvpMultiplier, Coefficient, Variance, ResourceCoefficient,
 GroupSizeBasePointsCoefficient, EffectBasePoints, EffectMiscValue1, EffectMiscValue2,
 EffectRadiusIndex1, EffectRadiusIndex2, EffectSpellClassMask1, EffectSpellClassMask2,
 EffectSpellClassMask3, EffectSpellClassMask4, ImplicitTarget1, ImplicitTarget2, SpellID, VerifiedBuild)
VALUES
(33882, 0, 0, 1, 76, 0.0, 0, 0,
 0.0, 1.0, 0, 0, 0,
 0.0, 0.0, 0.0, 0,
 0.0, 1.0, 0.0, 0.0, 0.0,
 1.0, 0.0, 186265, 0,
 15, 0, 0, 0,
 0, 0, 0, 0, 42222, 35662);
