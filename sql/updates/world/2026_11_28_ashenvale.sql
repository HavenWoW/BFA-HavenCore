####################
#                  #
#     ASHENVALE    #
#                  #
####################

-- https://github.com/HavenWoW/BFA-HavenCore/issues/747
-- Remove scaling for critter
DELETE FROM creature_template_scaling WHERE Entry=15475 AND DifficultyID=0;

-- https://github.com/HavenWoW/BFA-HavenCore/issues/746
-- Remove PVP flag
UPDATE creature_template SET unit_flags=0 WHERE entry=3812;
UPDATE creature_template SET unit_flags=0 WHERE entry=3814;

-- https://github.com/HavenWoW/BFA-HavenCore/issues/745
UPDATE quest_objectives SET Description='Wrathtail naga slain' WHERE ID=263537 AND QuestID=13602;

-- https://github.com/HavenWoW/BFA-HavenCore/issues/744
-- Earthen Ring Guide - Add guid to Midsummer Fire Festival event
DELETE FROM game_event_creature WHERE eventEntry=1 AND guid IN(353174,369904,369917,370727,370739,370750,370752);
INSERT INTO game_event_creature (eventEntry, guid) VALUES
(1, 353174),
(1, 369904),
(1, 369917),
(1, 370727),
(1, 370739),
(1, 370750),
(1, 370752);

-- https://github.com/HavenWoW/BFA-HavenCore/issues/743
DELETE FROM quest_poi WHERE QuestID=26465;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(26465, 0, 0, -1, 0, 0, 1, 63, 0, 1, 0, 0, 0, 0, 35662),
(26465, 0, 1, 0, 265721, 5424, 1, 63, 0, 1, 0, 0, 0, 0, 35662);

DELETE FROM quest_poi_points WHERE QuestID=26465;
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild) VALUES
(26465, 0, 0, 3884, 649, 35662),
(26465, 1, 0, 3953, 735, 35662),
(26465, 1, 1, 4069, 773, 35662),
(26465, 1, 2, 4169, 1058, 35662),
(26465, 1, 3, 3929, 1196, 35662),
(26465, 1, 4, 3714, 1248, 35662),
(26465, 1, 5, 3571, 1274, 35662),
(26465, 1, 6, 3481, 1233, 35662),
(26465, 1, 7, 3452, 1034, 35662),
(26465, 1, 8, 3475, 906, 35662),
(26465, 1, 9, 3587, 815, 35662);

-- https://github.com/HavenWoW/BFA-HavenCore/issues/742
DELETE FROM quest_poi WHERE QuestID=13617;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(13617, 0, 0, -1, 0, 0, 1, 63, 0, 1, 0, 0, 0, 0, 35662);

DELETE FROM quest_poi_points WHERE QuestID=13617;
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild) VALUES
(13617, 0, 0, 3884, 649, 35662);

-- https://github.com/HavenWoW/BFA-HavenCore/issues/741
DELETE FROM creature_addon WHERE guid=239804;
INSERT INTO creature_addon (guid, path_id, mount, bytes1, bytes2, emote, aiAnimKit, movementAnimKit, meleeAnimKit, visibilityDistanceType, auras) VALUES
(239804,239804, 9991, 1, 0, 0, 0, 0, 0, 0, NULL);

DELETE FROM waypoint_data WHERE id = 239804;
INSERT INTO waypoint_data (id, `point`, position_x, position_y, position_z, orientation, delay, move_type, `action`, action_chance, wpguid) VALUES
(239804, 1, 3816.11, 150.18, 7.57, 0.0, 10000, 0, 0, 100, 0),
(239804, 2, 3823.02, 147.91, 7.97, 0.0, 0, 0, 0, 100, 0),
(239804, 3, 3830.63, 147.39, 7.74, 0.0, 0, 0, 0, 100, 0),
(239804, 4, 3836.24, 148.89, 7.32, 0.0, 10000, 0, 0, 100, 0),
(239804, 5, 3830.63, 147.39, 7.74, 0.0, 0, 0, 0, 100, 0),
(239804, 6, 3823.02, 147.91, 7.97, 0.0, 0, 0, 0, 100, 0);

-- Set waypoint movement
UPDATE creature SET MovementType = 2 WHERE guid = 239804;


--  https://github.com/HavenWoW/BFA-HavenCore/issues/740
UPDATE quest_objectives SET Description='Bathran''s corpse burned' WHERE ID=257793 AND QuestID=13595;

DELETE FROM quest_poi WHERE QuestID=13595;
INSERT INTO quest_poi (QuestID, BlobIndex, Idx1, ObjectiveIndex, QuestObjectiveID, QuestObjectID, MapID, UiMapID, Priority, Flags, WorldEffectID, PlayerConditionID, SpawnTrackingID, AlwaysAllowMergingBlobs, VerifiedBuild) VALUES
(13595, 0, 0, -1, 0, 0, 1, 63, 0, 1, 0, 0, 0, 0, 35662),
(13595, 0, 1, 0, 257793, 33183, 1, 63, 0, 1, 0, 0, 0, 0, 35662);

DELETE FROM quest_poi_points WHERE QuestID=13595;
INSERT INTO quest_poi_points (QuestID, Idx1, Idx2, X, Y, VerifiedBuild) VALUES
(13595, 0, 0, 3738, 27, 35662),
(13595, 1, 0, 3845, -215, 35662);

DELETE FROM conditions WHERE SourceTypeOrReferenceId = 13 AND SourceEntry = 62506;
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, ErrorType, ErrorTextId, ScriptName, Comment) VALUES
(13, 1, 62506, 0, 0, 31, 0, 3, 33183, 0, 0, 0, 0, '', 'Target must be creature 33183'),
(13, 1, 62506, 0, 0, 1, 0, 64111, 0, 0, 1, 0, 0, '', 'Target must not have aura 64111');

DELETE FROM smart_scripts WHERE entryorguid=33183;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(33183, 0, 0, 1, 8, 0, 100, 0, 62506, 0, 0, 0, 0, '', 33, 33183, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'SpellHit - KillCredit'),
(33183, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, '', 11, 64111, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'SpellHit - Cast');

-- set creature HP to 0 and friendly
UPDATE creature_template SET unit_flags = 262912,unit_flags3=16785408,faction=35 WHERE entry=33183;

-- https://github.com/HavenWoW/BFA-HavenCore/issues/739
UPDATE quest_objectives SET Description='Horde at Bathran''s Haunt slain' WHERE ID=266439 AND QuestID=13594;
