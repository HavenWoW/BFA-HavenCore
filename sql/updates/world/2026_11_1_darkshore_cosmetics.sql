-- Cosmetic changes in darkshore

-- Vigilant Protector shouldn't move
UPDATE creature SET MovementType=0 WHERE id=34056;

-- Serendia OakWhisper - add missing NPC
DELETE FROM creature WHERE guid=3530203330003176 and id=32972;
INSERT INTO creature (guid, id, `map`, zoneId, areaId, spawnDifficulties, phaseUseFlags, PhaseId, PhaseGroup, terrainSwapMap, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, MovementType, npcflag, unit_flags, unit_flags2, unit_flags3, dynamicflags, ScriptName, VerifiedBuild) VALUES
(3530203330003176, 32972, 1, 148, 4659, '0', 0, 2001, 0, -1, 0, 0, 7416.68, -266.034, 22.8846, 2.75323, 300, 0.0, 0, 2383, 0, 2, 0, 0, 0, 0, 0, '', 35662);

DELETE FROM creature_addon WHERE guid = 3530203330003176;
INSERT INTO creature_addon (guid, path_id, mount, bytes1, bytes2, emote, aiAnimKit, movementAnimKit, meleeAnimKit, visibilityDistanceType, auras) VALUES
(3530203330003176, 32972, 0, 0, 0, 0, 0, 0, 0, 0, NULL);

UPDATE creature SET MovementType = 2 WHERE guid = 3530203330003176;

DELETE FROM waypoint_data WHERE id = 32972;
INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, orientation, delay, move_type, action, action_chance, wpguid) VALUES
(32972, 1, 7416.81, -265.65, 22.88, 2.92, 0, 0, 0, 100, 0),
(32972, 2, 7414.98, -276.67, 22.37, 4.55, 0, 0, 0, 100, 0),
(32972, 3, 7433.43, -275.61, 12.78, 0.05, 0, 0, 0, 100, 0),
(32972, 4, 7447.73, -269.01, 12.74, 1.37, 0, 0, 0, 100, 0),
(32972, 5, 7447.78, -267.75, 12.74, 2.42, 10000, 0, 0, 100, 0),
(32972, 6, 7447.73, -269.01, 12.74, 1.37, 0, 0, 0, 100, 0),
(32972, 7, 7443.23, -269.29, 12.77, 1.44, 0, 0, 0, 100, 0),
(32972, 8, 7443.54, -267.64, 12.77, 0.44, 30000, 0, 0, 100, 0),
(32972, 9, 7443.60, -274.99, 12.77, 5.82, 30000, 0, 0, 100, 0),
(32972, 10, 7433.57, -275.94, 12.78, 3.24, 0, 0, 0, 100, 0),
(32972, 11, 7414.98, -276.67, 22.37, 4.55, 0, 0, 0, 100, 0),
(32972, 12, 7416.81, -265.65, 22.88, 2.92, 10000, 0, 0, 100, 0);

-- The Boon of the Seas - missing text
DELETE FROM quest_offer_reward WHERE ID = 13520;
INSERT INTO quest_offer_reward (ID, Emote1, Emote2, Emote3, Emote4, EmoteDelay1, EmoteDelay2, EmoteDelay3, EmoteDelay4, RewardText, VerifiedBuild)
VALUES(13520, 0, 0, 0, 0, 0, 0, 0, 0, 'This will do perfectly. Thank you so much!

I suspect they will want to thank you personally should they recover, so perhaps you can stop by later on.', 35662);

-- Bearer of Good Fortune - missing text
DELETE FROM quest_offer_reward WHERE ID = 13557;
INSERT INTO quest_offer_reward (ID, Emote1, Emote2, Emote3, Emote4, EmoteDelay1, EmoteDelay2, EmoteDelay3, EmoteDelay4, RewardText, VerifiedBuild)
VALUES(13557, 0, 0, 0, 0, 0, 0, 0, 0, 'I am pleased to hear that you were able to free some of the remaining wildlife. With a bit of good fortune, we will find the cure for the corruption that the others have faced before any more of them die.', 35662);

-- Volcor - remove "cannot be tamed" message
UPDATE creature_template SET dynamicflags=0 WHERE entry = 32960;

-- Volcor and Gershala Nightwhisper - No regen
UPDATE creature_template SET RegenHealth = 0 WHERE entry IN (32960, 32962);

-- 10% HP on respawn
UPDATE creature SET curhealth = 26 WHERE id IN (32960, 32962);

-- Tharnariun Treetender - add missing NPC
DELETE FROM creature WHERE guid=3530203330003179 and id=32978;
INSERT INTO creature (guid, id, `map`, zoneId, areaId, spawnDifficulties, phaseUseFlags, PhaseId, PhaseGroup, terrainSwapMap, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, MovementType, npcflag, unit_flags, unit_flags2, unit_flags3, dynamicflags, ScriptName, VerifiedBuild) VALUES
(3530203330003179, 32978, 1, 148, 445, '0', 0, 2002, 0, -1, 0, 0, 6927.97, -655.794, 68.7104, 4.4339, 300, 0.0, 0, 2383, 0, 0, 0, 0, 0, 0, 0, '', 35662);

DELETE FROM creature_addon WHERE guid = 3530203330003179;
INSERT INTO creature_addon (guid, path_id, mount, bytes1, bytes2, emote, aiAnimKit, movementAnimKit, meleeAnimKit, visibilityDistanceType, auras) VALUES
(3530203330003179, 0, 0, 8, 0, 0, 0, 0, 0, 0, NULL);

DELETE FROM phase_area WHERE AreaId=445 AND PhaseId=2002;
INSERT INTO phase_area (AreaId, PhaseId, Comment)
VALUES(445, 2002, 'Darkshore - quest 13529 NOT ready to turn in or completed');

DELETE FROM conditions WHERE SourceTypeOrReferenceId = 26 AND SourceGroup = 2002 AND SourceEntry = 445;
INSERT INTO conditions
(SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup,
 ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3,
 NegativeCondition, ErrorType, ErrorTextId, ScriptName, Comment)
VALUES
(26, 2002, 445, 0, 0, 47, 0, 13529, 41, 0, 0, 0, 0, '', 'Darkshore - Phase 2002 Visible when quest 13529 is not taken or in progress');

-- Decomposing Thistle Bear - make it lootable
UPDATE creature_template SET faction=35 WHERE entry=32975;
