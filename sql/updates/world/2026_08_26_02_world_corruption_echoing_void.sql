-- 837 blizzlike: Echoing Void (虚空回响) rank-1 scripts + hotfix proc mask
-- 35662: 318280/485/486 LINKED -> 317014 (ProcFlags hotfixed 69904 -> 0, ICD 700ms).
-- spell_proc restores the pre-hotfix ability mask only (no white hits, no periodic).
-- 317022/317029 are shared with Hivemind; scripts no-op unless the caster has 317014.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_echoing_void_collapse','spell_echoing_void_damage','spell_echoing_void_proc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(317014, 'spell_echoing_void_proc'),
(317022, 'spell_echoing_void_collapse'),
(317029, 'spell_echoing_void_damage');

DELETE FROM `spell_proc` WHERE `SpellId` = 317014;
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
    317014,
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
    700,     -- milliseconds (2020-01-27 hotfix 100ms -> 700ms)
    0
);
