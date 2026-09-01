-- ============================================================
-- Ragefire Chasm - Complete Normal-Mode Fix
-- BFA-HavenCore 8.3.7 / Build 35662
--
-- Consolidates the validated RFC SQL fixes for:
--   * Adarogg
--   * Dark Shaman Koranthal
--   * Slagmaw
--   * Lava Guard Gordoth
--   * No Man Left Behind / No Orc Left Behind
--   * Deprecated Animal Control quests
--   * Gordoth laboratory intro
--
-- Faction-specific NPC filtering is handled in
-- instance_ragefire_chasm.cpp.
-- ============================================================

-- ============================================================
-- Ragefire Chasm - Adarogg (61408)
-- Full SQL fix for BFA-HavenCore
--
-- Fixes:
--   * Switches Adarogg from incomplete SmartAI to C++ boss script
--   * Removes conflicting SmartAI behavior
--   * Corrects boss gold
--   * Replaces generic/trash loot with proper boss loot
--
-- C++ boss mechanics are handled in boss_adarogg.cpp:
--   * Melee combat
--   * Inferno Charge
--   * Visible physical charge movement
--   * Flame Breath
--   * Retail-like recurring ability rotation
-- ============================================================


-- ------------------------------------------------------------
-- 1. Attach Adarogg C++ boss script
--
-- Previously:
--   AIName = 'SmartAI'
--   ScriptName = ''
--
-- SmartAI only cast Inferno Charge once on aggro and
-- then repeated Flame Breath.
-- ------------------------------------------------------------

UPDATE `creature_template`
SET
    `AIName` = '',
    `ScriptName` = 'boss_adarogg'
WHERE `entry` = 61408;


-- ------------------------------------------------------------
-- 2. Remove obsolete/incomplete SmartAI
-- ------------------------------------------------------------

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 61408
  AND `source_type` = 0;


-- ------------------------------------------------------------
-- 3. Correct Adarogg boss gold
--
-- Previous behavior was approximately 6-7 silver.
--
-- Retail/reference boss range:
--   65000-75000 copper = 6g 50s - 7g 50s
-- ------------------------------------------------------------

UPDATE `creature_template`
SET
    `lootid` = 61408,
    `mingold` = 65000,
    `maxgold` = 75000
WHERE `entry` = 61408;


-- ------------------------------------------------------------
-- 4. Replace generic/trash loot with Adarogg boss loot
--
-- GroupId 1 + Chance 0 selects one item from the boss group.
-- ------------------------------------------------------------

DELETE FROM `creature_loot_template`
WHERE `Entry` = 61408;

INSERT INTO `creature_loot_template`
(
    `Entry`,
    `Item`,
    `Reference`,
    `Chance`,
    `QuestRequired`,
    `LootMode`,
    `GroupId`,
    `MinCount`,
    `MaxCount`,
    `Comment`
)
VALUES
(
    61408,
    82879,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    'Adarogg - Collarspike Bracers'
),
(
    61408,
    82772,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    'Adarogg - Snarlmouth Leggings'
),
(
    61408,
    82880,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    'Adarogg - Fang of Adarogg'
),
(
    61408,
    151421,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    'Adarogg - Scorched Blazehound Boots'
),
(
    61408,
    151422,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    'Adarogg - Bonecoal Waistguard'
);

-- ============================================================
-- Ragefire Chasm - Dark Shaman Koranthal (61412)
-- Full SQL fix for BFA-HavenCore
--
-- Fixes:
--   * Switches boss from broken SmartAI to C++ boss script
--   * Removes conflicting SmartAI spell loop
--   * Binds Shadow Storm helper spell script
--   * Restores retail-like boss dialogue
--   * Corrects boss gold reward
--   * Replaces generic trash loot with proper boss loot
--
-- C++ boss mechanics are handled in boss_koranthal.cpp:
--   * Melee combat
--   * Twisted Elements rotation
--   * Shadow Storm phase
--   * Shadow Storm targeting
--   * Return to normal rotation after storm
-- ============================================================


-- ------------------------------------------------------------
-- 1. Attach Koranthal C++ boss script
--
-- Previously:
--   AIName = 'SmartAI'
--   ScriptName = ''
--
-- SmartAI was causing the permanent Shadow Storm loop.
-- ------------------------------------------------------------

UPDATE `creature_template`
SET
    `AIName` = '',
    `ScriptName` = 'boss_koranthal'
WHERE `entry` = 61412;


-- ------------------------------------------------------------
-- 2. Remove obsolete/broken SmartAI for Koranthal
-- ------------------------------------------------------------

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 61412
  AND `source_type` = 0;


-- ------------------------------------------------------------
-- 3. Bind Shadow Storm helper SpellScript
--
-- Required for proper Shadow Storm target handling.
-- ------------------------------------------------------------

DELETE FROM `spell_script_names`
WHERE `spell_id` = 119973;

INSERT INTO `spell_script_names`
(
    `spell_id`,
    `ScriptName`
)
VALUES
(
    119973,
    'spell_koranthal_shadow_storm'
);


-- ------------------------------------------------------------
-- 4. Retail-like Koranthal dialogue
--
-- Group 0 = Aggro
-- Group 1 = Shadow Storm warning
-- Group 2 = Death
-- ------------------------------------------------------------

DELETE FROM `creature_text`
WHERE `CreatureID` = 61412;

INSERT INTO `creature_text`
(
    `CreatureID`,
    `GroupID`,
    `ID`,
    `Text`,
    `Type`,
    `Language`,
    `Probability`,
    `Emote`,
    `Duration`,
    `Sound`,
    `BroadcastTextId`,
    `TextRange`
)
VALUES
(
    61412,
    0,
    0,
    'The power of the Dark Shaman will overwhelm you!',
    14,
    0,
    100,
    0,
    0,
    0,
    61305,
    0
),
(
    61412,
    1,
    0,
    '|TInterface\\Icons\\spell_shadow_shadowfury:20|tDark Shaman Koranthal summons a |r|cFF9E09DE|Hspell:119971|h[Shadow Storm]|h|r!',
    41,
    0,
    100,
    0,
    0,
    0,
    61139,
    0
),
(
    61412,
    2,
    0,
    'My death... means... nothing...',
    14,
    0,
    100,
    0,
    0,
    0,
    61306,
    0
);


-- ------------------------------------------------------------
-- 5. Correct Koranthal boss gold
--
-- Previous HavenCore behavior was approximately 6 silver.
--
-- Retail/reference boss range:
--   65000-75000 copper = 6g 50s - 7g 50s
-- ------------------------------------------------------------

UPDATE `creature_template`
SET
    `lootid` = 61412,
    `mingold` = 65000,
    `maxgold` = 75000
WHERE `entry` = 61412;


-- ------------------------------------------------------------
-- 6. Replace generic trash loot with Koranthal boss loot
--
-- GroupId 1 + Chance 0 selects one item from the boss group.
-- ------------------------------------------------------------

DELETE FROM `creature_loot_template`
WHERE `Entry` = 61412;

INSERT INTO `creature_loot_template`
(
    `Entry`,
    `Item`,
    `Reference`,
    `Chance`,
    `QuestRequired`,
    `LootMode`,
    `GroupId`,
    `MinCount`,
    `MaxCount`,
    `Comment`
)
VALUES
(
    61412,
    82882,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    'Dark Shaman Koranthal - Dark Ritual Cape'
),
(
    61412,
    82877,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    'Dark Shaman Koranthal - Grasp of the Broken Totem'
),
(
    61412,
    82881,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    'Dark Shaman Koranthal - Cuffs of Black Elements'
),
(
    61412,
    132551,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    'Dark Shaman Koranthal - Dark Shaman''s Jerkin'
);

-- ============================================================
-- Ragefire Chasm - Slagmaw (61463)
-- Full SQL fix for BFA-HavenCore
--
-- Fixes:
--   * Attaches the Slagmaw C++ boss script
--   * Removes obsolete Magnaw's Head helper spawn
--   * Expands Slagmaw melee reach / hitbox
--   * Corrects boss gold reward
--   * Replaces generic trash loot with proper boss loot
--
-- C++ boss mechanics are handled in boss_slagmaw.cpp:
--   * 5x Lava Spit
--   * Submerge
--   * Random emerge location
--   * No movement/chasing onto land
--   * Boundary reset
--   * Encounter integration
-- ============================================================


-- ------------------------------------------------------------
-- 1. Attach Slagmaw boss script
-- ------------------------------------------------------------

UPDATE `creature_template`
SET `ScriptName` = 'boss_slagmaw'
WHERE `entry` = 61463;


-- ------------------------------------------------------------
-- 2. Remove obsolete Magnaw's Head helper
--
-- This was an old/incomplete implementation artifact.
-- Modern Slagmaw logic is handled entirely by creature 61463.
-- ------------------------------------------------------------

DELETE FROM `creature`
WHERE `guid` = 10615949
  AND `id` = 61800
  AND `map` = 389;


-- Ensure no unused script remains attached to Magnaw's Head
UPDATE `creature_template`
SET `ScriptName` = ''
WHERE `entry` = 61800;


-- ------------------------------------------------------------
-- 3. Correct Slagmaw melee accessibility
--
-- Original values were:
--   BoundingRadius = 0
--   CombatReach    = 0
--
-- 10.0 allows melee players to attack safely from the
-- surrounding platform without entering the lava.
-- ------------------------------------------------------------

UPDATE `creature_model_info`
SET
    `BoundingRadius` = 3.5,
    `CombatReach` = 10.0
WHERE `DisplayID` = 42247;


-- ------------------------------------------------------------
-- 4. Correct Slagmaw boss gold
--
-- Previous HavenCore values:
--   600-700 copper = 6-7 silver
--
-- Correct boss range:
--   65000-75000 copper = 6g 50s - 7g 50s
-- ------------------------------------------------------------

UPDATE `creature_template`
SET
    `lootid` = 61463,
    `mingold` = 65000,
    `maxgold` = 75000
WHERE `entry` = 61463;


-- ------------------------------------------------------------
-- 5. Replace incorrect generic loot table with Slagmaw boss loot
--
-- GroupId 1 + Chance 0 causes the group to select one item.
-- ------------------------------------------------------------

DELETE FROM `creature_loot_template`
WHERE `Entry` = 61463;

INSERT INTO `creature_loot_template`
(
    `Entry`,
    `Item`,
    `Reference`,
    `Chance`,
    `QuestRequired`,
    `LootMode`,
    `GroupId`,
    `MinCount`,
    `MaxCount`,
    `Comment`
)
VALUES
    (61463, 82885,  0, 0, 0, 1, 1, 1, 1, 'Slagmaw - Flameseared Carapace'),
    (61463, 82884,  0, 0, 0, 1, 1, 1, 1, 'Slagmaw - Chitonous Bracers'),
    (61463, 82878,  0, 0, 0, 1, 1, 1, 1, 'Slagmaw - Fireworm Robes'),
    (61463, 132552, 0, 0, 0, 1, 1, 1, 1, 'Slagmaw - Chitonous Bindings');

-- ============================================================
-- Ragefire Chasm - quests, faction filtering support, rescue
-- behavior bindings, Gordoth intro/combat, and Gordoth loot
-- BFA-HavenCore 8.3.7
-- ============================================================

-- ------------------------------------------------------------
-- Deprecated quests: Animal Control
-- Remove both faction versions from quest givers.
-- ------------------------------------------------------------

DELETE FROM `creature_queststarter`
WHERE `quest` IN (30982, 30997);

DELETE FROM `creature_questender`
WHERE `quest` IN (30982, 30997);


-- ------------------------------------------------------------
-- No Man Left Behind / No Orc Left Behind
-- Bind hidden rescue props to the C++ rescue script.
-- ------------------------------------------------------------

UPDATE `creature_template`
SET
    `AIName` = '',
    `ScriptName` = 'npc_rfc_hidden_prisoner'
WHERE `entry` IN (61780, 61790);


-- Bind the three final prisoner cages.
UPDATE `gameobject_template`
SET `ScriptName` = 'go_rfc_prisoner_cage'
WHERE `entry` IN (211883, 211884, 211885);


-- Ensure the rescue props remain gossip-enabled.
UPDATE `creature_template`
SET
    `gossip_menu_id` = CASE
        WHEN `entry` = 61780 THEN 13814
        WHEN `entry` = 61790 THEN 13815
        ELSE `gossip_menu_id`
    END,
    `npcflag` = 1
WHERE `entry` IN (61780, 61790);


-- Retail/reference rescue lines.
-- BroadcastText IDs preserve client localization.
DELETE FROM `creature_text`
WHERE `CreatureID` IN (61788, 61680);

INSERT INTO `creature_text`
(
    `CreatureID`,
    `GroupID`,
    `ID`,
    `Text`,
    `Type`,
    `Language`,
    `Probability`,
    `Emote`,
    `Duration`,
    `Sound`,
    `BroadcastTextId`,
    `TextRange`
)
VALUES
    (61788, 0, 0, 'RUN!!',                                      12, 0, 20, 0, 0, 0, 61290, 0),
    (61788, 0, 1, 'Thanks!',                                    12, 0, 20, 0, 0, 0, 61286, 0),
    (61788, 0, 2, 'I didn''t think anyone was coming for us.',  12, 0, 20, 0, 0, 0, 61287, 0),
    (61788, 0, 3, 'I owe you an ale!',                          12, 0, 20, 0, 0, 0, 61288, 0),
    (61788, 0, 4, 'I thought I was going to die down here!',    12, 0, 20, 0, 0, 0, 61285, 0),

    (61680, 0, 0, 'RUN!!',                                      12, 0, 20, 0, 0, 0, 61290, 0),
    (61680, 0, 1, 'Thanks!',                                    12, 0, 20, 0, 0, 0, 61286, 0),
    (61680, 0, 2, 'I didn''t think anyone was coming for us.',  12, 0, 20, 0, 0, 0, 61287, 0),
    (61680, 0, 3, 'I owe you an ale!',                          12, 0, 20, 0, 0, 0, 61288, 0),
    (61680, 0, 4, 'I thought I was going to die down here!',    12, 0, 20, 0, 0, 0, 61285, 0);


-- ------------------------------------------------------------
-- Lava Guard Gordoth (61528)
-- Use the C++ script for both the retail-like breakout intro
-- and the validated combat rotation.
-- ------------------------------------------------------------

UPDATE `creature_template`
SET
    `AIName` = '',
    `ScriptName` = 'boss_gordoth',
    `lootid` = 61528,
    `mingold` = 160000,
    `maxgold` = 170000
WHERE `entry` = 61528;


-- Remove the obsolete SmartAI combat implementation.
DELETE FROM `smart_scripts`
WHERE `entryorguid` = 61528
  AND `source_type` = 0;


-- Bind Gordoth's laboratory intro trigger.
DELETE FROM `areatrigger_scripts`
WHERE `entry` = 7899;

INSERT INTO `areatrigger_scripts`
(
    `entry`,
    `ScriptName`
)
VALUES
(
    7899,
    'at_rfc_gordoth_intro'
);


-- ------------------------------------------------------------
-- Gordoth boss loot
-- ------------------------------------------------------------

DELETE FROM `creature_loot_template`
WHERE `Entry` = 61528;

INSERT INTO `creature_loot_template`
(
    `Entry`,
    `Item`,
    `Reference`,
    `Chance`,
    `QuestRequired`,
    `LootMode`,
    `GroupId`,
    `MinCount`,
    `MaxCount`,
    `Comment`
)
VALUES
    (61528, 82888,  0, 0, 0, 1, 1, 1, 1, 'Lava Guard Gordoth - Heartboiler Staff'),
    (61528, 82886,  0, 0, 0, 1, 1, 1, 1, 'Lava Guard Gordoth - Gorewalker Treads'),
    (61528, 82883,  0, 0, 0, 1, 1, 1, 1, 'Lava Guard Gordoth - Bloodcursed Felblade'),
    (61528, 151424, 0, 0, 0, 1, 1, 1, 1, 'Lava Guard Gordoth - Belt of Boundless Fury'),
    (61528, 151425, 0, 0, 0, 1, 1, 1, 1, 'Lava Guard Gordoth - Gordoth''s Crushers');

-- NOTE:
-- Faction-specific NPC visibility is handled in
-- instance_ragefire_chasm.cpp so the same DB supports both factions.
