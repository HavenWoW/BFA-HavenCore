-- https://github.com/HavenWoW/BFA-HavenCore/issues/431

-- Add Spirit of the Moonstalker quest
DELETE FROM creature_queststarter WHERE id=33131 and quest=13568;
INSERT INTO creature_queststarter (id, quest) VALUES(33131, 13568);

-- Add creatures to proper phase
UPDATE creature SET PhaseId=170 WHERE guid=308263 AND id=33131;
UPDATE creature SET PhaseId=170 WHERE guid=308262 AND id=33132;
UPDATE creature SET PhaseId=170 WHERE guid=308261 AND id=33133;

-- Keeper Karithus - Add npc for phase
DELETE FROM creature WHERE guid=3530203330003182 AND id=33048;
INSERT INTO creature (guid, id, `map`, zoneId, areaId, spawnDifficulties, phaseUseFlags, PhaseId, PhaseGroup, terrainSwapMap, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, MovementType, npcflag, unit_flags, unit_flags2, unit_flags3, dynamicflags, ScriptName, VerifiedBuild) VALUES
(3530203330003182, 33048, 1, 148, 4675, '0', 0, 170, 0, -1, 0, 0, 6541.61, 238.912, 7.65152, 1.69297, 300, 0.0, 0, 260, 0, 0, 0, 0, 0, 0, 0, '', 0);

-- Missing quest_reward
DELETE FROM quest_offer_reward WHERE ID = 13568;
INSERT INTO quest_offer_reward (ID, Emote1, Emote2, Emote3, Emote4, EmoteDelay1, EmoteDelay2, EmoteDelay3, EmoteDelay4, RewardText, VerifiedBuild)
VALUES(13568, 0, 0, 0, 0, 0, 0, 0, 0, 'I share these forests willingly, accepting night elf kind into my home, and on occasion calling them friend. Though they have been tainted greatly in recent memory, I can see in you the same strength, honor, and spirit that first endeared them to me.

If you place value in the ferocity necessary to overcome all odds, then I am willing to call you my champion. Know now that if you side with my kind, the other spirits will not accept you, so consider your choice carefully.', 35662);

-- This is a requirement because spell_area only activate on enter/exit an area
DELETE FROM smart_scripts WHERE source_type = 0 AND entryorguid IN (33131, 33132, 33133);
INSERT INTO smart_scripts
(entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags,
 event_param1, event_param2, event_param3, event_param4, event_param5, event_param_string,
 action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6,
 target_type, target_param1, target_param2, target_param3,
 target_x, target_y, target_z, target_o, comment)
VALUES
(33131, 0, 0, 0, 20, 0, 100, 0, 13568, 0, 0, 0, 0, '', 85, 64340, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On quest rewarded - Cast Blessing of the Moonstalker on invoker'),
(33132, 0, 0, 0, 20, 0, 100, 0, 13597, 0, 0, 0, 0, '', 85, 64329, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On quest rewarded - Cast Blessing of the Thistle Bear on invoker'),
(33133, 0, 0, 0, 20, 0, 100, 0, 13567, 0, 0, 0, 0, '', 85, 64341, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 'On quest rewarded - Cast Blessing of the Stag on invoker');


DELETE FROM spell_area WHERE spell IN (64340, 64329, 64341) AND area = 148;
INSERT INTO spell_area
(spell, area, quest_start, quest_end, aura_spell, teamId, racemask, gender, flags, quest_start_status, quest_end_status)
VALUES
(64340, 148, 13568, 0, 0, -1, 0, 2, 3, 64, 64),
(64329, 148, 13597, 0, 0, -1, 0, 2, 3, 64, 64),
(64341, 148, 13567, 0, 0, -1, 0, 2, 3, 64, 64);

DELETE FROM conditions WHERE SourceTypeOrReferenceId = 15 AND SourceGroup = 10410 AND SourceEntry = 0;
INSERT INTO conditions
(SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup,
 ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3,
 NegativeCondition, ErrorType, ErrorTextId, ScriptName, Comment)
VALUES
(15, 10410, 0, 0, 0,
 9, 0, 13569, 0, 0,
 0, 0, 0, '', 'Grovekeeper''s Incense - option 0 only visible with quest 13569 taken');
