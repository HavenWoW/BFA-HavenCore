-- ============================================================================
-- HavenCore guild achievement availability - WORLD database (merged).
--
-- Replaces (delete them from sql/updates/world):
--   2026_09_22_00_world_guild_achievement_availability.sql
--   2026_09_22_01_world_guild_realm_first_disables.sql
--
-- Policy (BFA progression realm): guild achievements that are not obtainable in
-- BFA are disabled; everything from 8.0 on stays enabled, including the BFA
-- Hall of Fame races.
--   sourceType 4 = criteria disable: criteria used ONLY by unobtainable guild
--                  Feats of Strength (Challenge Modes, Ordos, guild level ...).
--                  Criteria shared by several disabled achievements appear once
--                  (the earlier file inserted them twice -> duplicate key).
--   sourceType 9 = achievement disable (DISABLE_TYPE_ACHIEVEMENT, Step 52): the
--                  pre-BFA Realm Firsts, whose criteria are shared with
--                  obtainable guild runs. Without Step 52 the core skips these
--                  rows with an "invalid type" error and nothing else changes.
--
-- Idempotent: previous rows are removed by comment, INSERT IGNORE on the rest.
-- Generated from Achievement/Criteria/CriteriaTree.db2 (8.3.7).
-- Apply to bfa_world, then '.reload disables' or restart.
-- ============================================================================

DELETE FROM `disables` WHERE `sourceType` IN (4, 9) AND `comment` LIKE 'Guild FoS %';

INSERT IGNORE INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(4, 13878, 0, '', '', 'Guild FoS 5407: Realm First! Guild Level 25 (not obtainable in BFA)'),
(4, 13864, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 13865, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 13866, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 13867, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 13868, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 13869, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 13870, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 13871, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 13872, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 13873, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 13874, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 14590, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 14591, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 14592, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team + 6628 (not obtainable in BFA)'),
(4, 21156, 0, '', '', 'Guild FoS 6632: Challenge Conquerors: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 21157, 0, '', '', 'Guild FoS 6632: Challenge Conquerors: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 21158, 0, '', '', 'Guild FoS 6632: Challenge Conquerors: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 21159, 0, '', '', 'Guild FoS 6632: Challenge Conquerors: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 21160, 0, '', '', 'Guild FoS 6632: Challenge Conquerors: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 21161, 0, '', '', 'Guild FoS 6632: Challenge Conquerors: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 21162, 0, '', '', 'Guild FoS 6632: Challenge Conquerors: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 21163, 0, '', '', 'Guild FoS 6632: Challenge Conquerors: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 21164, 0, '', '', 'Guild FoS 6632: Challenge Conquerors: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 21168, 0, '', '', 'Guild FoS 6633: Challenge Conquerors: Silver - Guild Edition (not obtainable in BFA)'),
(4, 21169, 0, '', '', 'Guild FoS 6633: Challenge Conquerors: Silver - Guild Edition (not obtainable in BFA)'),
(4, 21170, 0, '', '', 'Guild FoS 6633: Challenge Conquerors: Silver - Guild Edition (not obtainable in BFA)'),
(4, 21171, 0, '', '', 'Guild FoS 6633: Challenge Conquerors: Silver - Guild Edition (not obtainable in BFA)'),
(4, 21172, 0, '', '', 'Guild FoS 6633: Challenge Conquerors: Silver - Guild Edition (not obtainable in BFA)'),
(4, 21173, 0, '', '', 'Guild FoS 6633: Challenge Conquerors: Silver - Guild Edition (not obtainable in BFA)'),
(4, 21174, 0, '', '', 'Guild FoS 6633: Challenge Conquerors: Silver - Guild Edition (not obtainable in BFA)'),
(4, 21175, 0, '', '', 'Guild FoS 6633: Challenge Conquerors: Silver - Guild Edition (not obtainable in BFA)'),
(4, 21176, 0, '', '', 'Guild FoS 6633: Challenge Conquerors: Silver - Guild Edition (not obtainable in BFA)'),
(4, 21177, 0, '', '', 'Guild FoS 6634: Challenge Conquerors: Gold - Guild Edition (not obtainable in BFA)'),
(4, 21178, 0, '', '', 'Guild FoS 6634: Challenge Conquerors: Gold - Guild Edition (not obtainable in BFA)'),
(4, 21179, 0, '', '', 'Guild FoS 6634: Challenge Conquerors: Gold - Guild Edition (not obtainable in BFA)'),
(4, 21180, 0, '', '', 'Guild FoS 6634: Challenge Conquerors: Gold - Guild Edition (not obtainable in BFA)'),
(4, 21181, 0, '', '', 'Guild FoS 6634: Challenge Conquerors: Gold - Guild Edition (not obtainable in BFA)'),
(4, 21182, 0, '', '', 'Guild FoS 6634: Challenge Conquerors: Gold - Guild Edition (not obtainable in BFA)'),
(4, 21183, 0, '', '', 'Guild FoS 6634: Challenge Conquerors: Gold - Guild Edition (not obtainable in BFA)'),
(4, 21184, 0, '', '', 'Guild FoS 6634: Challenge Conquerors: Gold - Guild Edition (not obtainable in BFA)'),
(4, 21185, 0, '', '', 'Guild FoS 6634: Challenge Conquerors: Gold - Guild Edition (not obtainable in BFA)'),
(4, 21165, 0, '', '', 'Guild FoS 6641: Guild Hardware - Bronze + 6642 + 6643 (not obtainable in BFA)'),
(4, 21147, 0, '', '', 'Guild FoS 6921: Challenge Conquerors - Guild Edition (not obtainable in BFA)'),
(4, 21148, 0, '', '', 'Guild FoS 6921: Challenge Conquerors - Guild Edition (not obtainable in BFA)'),
(4, 21149, 0, '', '', 'Guild FoS 6921: Challenge Conquerors - Guild Edition (not obtainable in BFA)'),
(4, 21150, 0, '', '', 'Guild FoS 6921: Challenge Conquerors - Guild Edition (not obtainable in BFA)'),
(4, 21151, 0, '', '', 'Guild FoS 6921: Challenge Conquerors - Guild Edition (not obtainable in BFA)'),
(4, 21152, 0, '', '', 'Guild FoS 6921: Challenge Conquerors - Guild Edition (not obtainable in BFA)'),
(4, 21153, 0, '', '', 'Guild FoS 6921: Challenge Conquerors - Guild Edition (not obtainable in BFA)'),
(4, 21154, 0, '', '', 'Guild FoS 6921: Challenge Conquerors - Guild Edition (not obtainable in BFA)'),
(4, 21155, 0, '', '', 'Guild FoS 6921: Challenge Conquerors - Guild Edition (not obtainable in BFA)'),
(4, 21186, 0, '', '', 'Guild FoS 7444: Scenario Challenges (not obtainable in BFA)'),
(4, 23707, 0, '', '', 'Guild FoS 8512: Realm First! Garrosh Hellscream (10 player) (not obtainable in BFA)'),
(4, 23708, 0, '', '', 'Guild FoS 8513: Realm First! Garrosh Hellscream (25 player) (not obtainable in BFA)'),
(4, 24171, 0, '', '', 'Guild FoS 8790: Ordos Guild Run (not obtainable in BFA)'),
(4, 26501, 0, '', '', 'Guild FoS 9648: Challenge Warlords - Guild Edition (not obtainable in BFA)'),
(4, 26502, 0, '', '', 'Guild FoS 9648: Challenge Warlords - Guild Edition (not obtainable in BFA)'),
(4, 26503, 0, '', '', 'Guild FoS 9648: Challenge Warlords - Guild Edition (not obtainable in BFA)'),
(4, 26504, 0, '', '', 'Guild FoS 9648: Challenge Warlords - Guild Edition (not obtainable in BFA)'),
(4, 26505, 0, '', '', 'Guild FoS 9648: Challenge Warlords - Guild Edition (not obtainable in BFA)'),
(4, 26506, 0, '', '', 'Guild FoS 9648: Challenge Warlords - Guild Edition (not obtainable in BFA)'),
(4, 26507, 0, '', '', 'Guild FoS 9648: Challenge Warlords - Guild Edition (not obtainable in BFA)'),
(4, 26508, 0, '', '', 'Guild FoS 9648: Challenge Warlords - Guild Edition (not obtainable in BFA)'),
(4, 26509, 0, '', '', 'Guild FoS 9649: Challenge Warlords: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 26510, 0, '', '', 'Guild FoS 9649: Challenge Warlords: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 26511, 0, '', '', 'Guild FoS 9649: Challenge Warlords: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 26512, 0, '', '', 'Guild FoS 9649: Challenge Warlords: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 26513, 0, '', '', 'Guild FoS 9649: Challenge Warlords: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 26514, 0, '', '', 'Guild FoS 9649: Challenge Warlords: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 26515, 0, '', '', 'Guild FoS 9649: Challenge Warlords: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 26516, 0, '', '', 'Guild FoS 9649: Challenge Warlords: Bronze - Guild Edition (not obtainable in BFA)'),
(4, 26517, 0, '', '', 'Guild FoS 9650: Challenge Warlords: Silver - Guild Edition (not obtainable in BFA)'),
(4, 26518, 0, '', '', 'Guild FoS 9650: Challenge Warlords: Silver - Guild Edition (not obtainable in BFA)'),
(4, 26519, 0, '', '', 'Guild FoS 9650: Challenge Warlords: Silver - Guild Edition (not obtainable in BFA)'),
(4, 26520, 0, '', '', 'Guild FoS 9650: Challenge Warlords: Silver - Guild Edition (not obtainable in BFA)'),
(4, 26521, 0, '', '', 'Guild FoS 9650: Challenge Warlords: Silver - Guild Edition (not obtainable in BFA)'),
(4, 26522, 0, '', '', 'Guild FoS 9650: Challenge Warlords: Silver - Guild Edition (not obtainable in BFA)'),
(4, 26523, 0, '', '', 'Guild FoS 9650: Challenge Warlords: Silver - Guild Edition (not obtainable in BFA)'),
(4, 26524, 0, '', '', 'Guild FoS 9650: Challenge Warlords: Silver - Guild Edition (not obtainable in BFA)'),
(4, 26525, 0, '', '', 'Guild FoS 9651: Challenge Warlords: Gold - Guild Edition (not obtainable in BFA)'),
(4, 26526, 0, '', '', 'Guild FoS 9651: Challenge Warlords: Gold - Guild Edition (not obtainable in BFA)'),
(4, 26527, 0, '', '', 'Guild FoS 9651: Challenge Warlords: Gold - Guild Edition (not obtainable in BFA)'),
(4, 26528, 0, '', '', 'Guild FoS 9651: Challenge Warlords: Gold - Guild Edition (not obtainable in BFA)'),
(4, 26529, 0, '', '', 'Guild FoS 9651: Challenge Warlords: Gold - Guild Edition (not obtainable in BFA)'),
(4, 26530, 0, '', '', 'Guild FoS 9651: Challenge Warlords: Gold - Guild Edition (not obtainable in BFA)'),
(4, 26531, 0, '', '', 'Guild FoS 9651: Challenge Warlords: Gold - Guild Edition (not obtainable in BFA)'),
(4, 26532, 0, '', '', 'Guild FoS 9651: Challenge Warlords: Gold - Guild Edition (not obtainable in BFA)');

INSERT IGNORE INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(9, 5408, 0, '', '', 'Guild FoS 5408: Realm First! Working as a Team (pre-BFA realm first)'),
(9, 5409, 0, '', '', 'Guild FoS 5409: Realm First! Nefarian (pre-BFA realm first)'),
(9, 5410, 0, '', '', 'Guild FoS 5410: Realm First! Al''Akir (pre-BFA realm first)'),
(9, 5411, 0, '', '', 'Guild FoS 5411: Realm First! Sinestra (pre-BFA realm first)'),
(9, 5985, 0, '', '', 'Guild FoS 5985: Realm First! Ragnaros (pre-BFA realm first)'),
(9, 6126, 0, '', '', 'Guild FoS 6126: Realm First! Deathwing (pre-BFA realm first)'),
(9, 6628, 0, '', '', 'Guild FoS 6628: Realm First! Working as a Better Team (pre-BFA realm first)'),
(9, 6678, 0, '', '', 'Guild FoS 6678: Realm First! Sha of Fear (pre-BFA realm first)'),
(9, 6679, 0, '', '', 'Guild FoS 6679: Realm First! Grand Empress Shek''zeer (pre-BFA realm first)'),
(9, 6680, 0, '', '', 'Guild FoS 6680: Realm First! Will of the Emperor (pre-BFA realm first)'),
(9, 8257, 0, '', '', 'Guild FoS 8257: Realm First! Ra-den (pre-BFA realm first)'),
(9, 9397, 0, '', '', 'Guild FoS 9397: Realm First! Imperator''s Fall (pre-BFA realm first)'),
(9, 9399, 0, '', '', 'Guild FoS 9399: Realm First! Warlord Blackhand (pre-BFA realm first)'),
(9, 10380, 0, '', '', 'Guild FoS 10380: Realm First! Archimonde (pre-BFA realm first)'),
(9, 10854, 0, '', '', 'Guild FoS 10854: Realm First! Xavius (pre-BFA realm first)'),
(9, 10855, 0, '', '', 'Guild FoS 10855: Realm First! Gul''dan (pre-BFA realm first)'),
(9, 11405, 0, '', '', 'Guild FoS 11405: Realm First! Helya (pre-BFA realm first)'),
(9, 11783, 0, '', '', 'Guild FoS 11783: Realm First! Kil''jaeden (pre-BFA realm first)'),
(9, 12258, 0, '', '', 'Guild FoS 12258: Realm First! Argus the Unmaker (pre-BFA realm first)');
