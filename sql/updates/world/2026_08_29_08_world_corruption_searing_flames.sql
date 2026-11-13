-- 837 blizzlike: Searing Flames (Faralos).
-- 35662 SpellAuraOptions 316698: ProcTypeMask_0=0, ProcTypeMask_1=0x4 (cast successful).
-- This core only loads mask0 into SpellInfo::ProcFlags, so the aura never procs
-- without a spell_proc row. Restore mask0=69904 (yellow melee/ranged + hostile
-- none/magic; no autos, no DoT ticks), SpellTypeMask=DAMAGE, ICD 100ms.
-- Do NOT copy SimC PF_ALL_DAMAGE / CAST_HEAL / aoe split.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_searing_breath','spell_searing_flames_proc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(316698, 'spell_searing_flames_proc'),
(316704, 'spell_searing_breath');

DELETE FROM `spell_proc` WHERE `SpellId` = 316698;
INSERT INTO `spell_proc` (
    `SpellId`,
    `SchoolMask`,
    `SpellFamilyName`,
    `SpellFamilyMask0`,
    `SpellFamilyMask1`,
    `SpellFamilyMask2`,
    `SpellFamilyMask3`,
    `ProcFlags`,
    `SpellTypeMask`,
    `SpellPhaseMask`,
    `HitMask`,
    `AttributesMask`,
    `DisableEffectsMask`,
    `ProcsPerMinute`,
    `Chance`,
    `Cooldown`,
    `Charges`
) VALUES (
    316698,
    0,
    0,
    0,
    0,
    0,
    0,
    69904,   -- 0x11110: yellow melee/ranged + hostile generic/magic. No autos, no DoT ticks.
    1,       -- PROC_SPELL_TYPE_DAMAGE
    2,       -- PROC_SPELL_PHASE_HIT (required: 69904 is in REQ_SPELL_PHASE_PROC_FLAG_MASK)
    0,
    0,
    0,
    0,
    101,
    100,     -- milliseconds (35662 ICD; not Echoing Void's 700ms hotfix)
    0
);
