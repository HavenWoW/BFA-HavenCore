-- Mr. Smite (Deadmines): replace boss_mr_smite C++ with SmartAI and add a live spawn for the normal dungeon
-- Behaviour sources: cMaNGOS boss_mr_smite (sniff-annotated), AzerothCore boss_mr_smite, wiki (Smite's Reaver / Smite's Mighty Hammer)
SET @ENTRY := 646;
SET @CGUID := 470124;

UPDATE `creature_template` SET `ScriptName`='' WHERE `entry`=@ENTRY;

-- 29266 (Permanent Feign Death) only belongs to the "A Vision of the Past" corpse, which keeps it through its own creature_addon (guid 470082)
UPDATE `creature_template_addon` SET `auras`='6433' WHERE `entry`=@ENTRY;

DELETE FROM `creature` WHERE `guid`=@CGUID;
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnDifficulties`,`PhaseId`,`PhaseGroup`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`,`VerifiedBuild`) VALUES
(@CGUID,@ENTRY,36,1581,1581,'1,2',0,0,0,1,-22.8472,-797.283,20.3745,1.0472,86400,0,0,0,0,0,0,0,0,0);

DELETE FROM `creature_equip_template` WHERE `CreatureID`=@ENTRY AND `ID` IN (2,3);
INSERT INTO `creature_equip_template` (`CreatureID`,`ID`,`ItemID1`,`AppearanceModID1`,`ItemVisual1`,`ItemID2`,`AppearanceModID2`,`ItemVisual2`,`ItemID3`,`AppearanceModID3`,`ItemVisual3`,`VerifiedBuild`) VALUES
(@ENTRY,2,2183,0,0,2183,0,0,0,0,0,0),
(@ENTRY,3,10756,0,0,0,0,0,0,0,0,0);

DELETE FROM `creature_text` WHERE `CreatureID`=@ENTRY;
INSERT INTO `creature_text` (`CreatureID`,`GroupID`,`ID`,`Text`,`Type`,`Language`,`Probability`,`Emote`,`Duration`,`Sound`,`BroadcastTextId`,`TextRange`,`comment`) VALUES
(@ENTRY,0,0,'You there! Check out that noise!',14,0,100,0,0,5775,1148,2,'Mr. Smite - Ironclad alarm 1'),
(@ENTRY,1,0,'We''re under attack! Avast, ye swabs! Repel the invaders!',14,0,100,0,0,5777,1149,2,'Mr. Smite - Ironclad alarm 2'),
(@ENTRY,2,0,'You landlubbers are tougher than I thought! I''ll have to improvise!',12,0,100,21,0,5778,1344,0,'Mr. Smite - Swap to axes'),
(@ENTRY,3,0,'D''ah! Now you''re making me angry!',12,0,100,15,0,5779,1345,0,'Mr. Smite - Swap to hammer');

-- Event phases: 1 sword, 2 axes, 3 hammer, 4 weapon swap in progress (no event listens to it)
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid` BETWEEN @ENTRY*100+0 AND @ENTRY*100+5 AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`event_param_string`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,4,0,100,0,0,0,0,0,0,'',22,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Aggro - Set Event Phase 1'),
(@ENTRY,0,1,2,2,1,100,1,33,65,0,0,0,'',22,4,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - Between 33-65% Health - Set Event Phase 4 (Phase 1) (No Repeat)'),
(@ENTRY,0,2,0,61,0,100,0,0,0,0,0,0,'',80,@ENTRY*100+0,1,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Link - Run Script (Stomp, Swap to Axes)'),
(@ENTRY,0,3,4,2,1,100,1,0,32,0,0,0,'',22,4,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - Between 0-32% Health - Set Event Phase 4 (Phase 1) (No Repeat)'),
(@ENTRY,0,4,0,61,0,100,0,0,0,0,0,0,'',80,@ENTRY*100+1,1,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Link - Run Script (Stomp, Swap to Hammer)'),
(@ENTRY,0,5,6,2,2,100,1,0,32,0,0,0,'',22,4,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - Between 0-32% Health - Set Event Phase 4 (Phase 2) (No Repeat)'),
(@ENTRY,0,6,0,61,0,100,0,0,0,0,0,0,'',80,@ENTRY*100+1,1,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Link - Run Script (Stomp, Swap to Hammer)'),
(@ENTRY,0,7,0,0,4,100,0,9000,9000,11000,11000,0,'',11,6435,0,0,0,0,0,2,0,0,0,0,0,0,0,'Mr. Smite - In Combat - Cast Smite Slam (Phase 3)'),
(@ENTRY,0,8,0,34,0,100,0,8,1,0,0,0,'',80,@ENTRY*100+2,1,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Reached Point 1 - Run Script (Equip Axes)'),
(@ENTRY,0,9,0,34,0,100,0,8,2,0,0,0,'',80,@ENTRY*100+3,1,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Reached Point 2 - Run Script (Equip Hammer)'),
(@ENTRY,0,10,0,25,0,100,0,0,0,0,0,0,'',80,@ENTRY*100+4,2,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Reset - Run Script (Reset)'),
(@ENTRY,0,11,0,38,0,100,257,1,3,0,0,0,'',80,@ENTRY*100+5,2,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Data Set 1 3 - Run Script (Ironclad Alarm) (No Repeat)'),
-- Stomp, then run to his chest (cMaNGOS timing; point from AzerothCore)
(@ENTRY*100+0,9,0,0,0,0,100,0,0,0,0,0,0,'',11,6432,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Cast Smite Stomp'),
(@ENTRY*100+0,9,1,0,0,0,100,0,0,0,0,0,0,'',1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Say Line 2'),
(@ENTRY*100+0,9,2,0,0,0,100,0,0,0,0,0,0,'',28,6433,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Remove Aura Nimble Reflexes III'),
(@ENTRY*100+0,9,3,0,0,0,100,0,0,0,0,0,0,'',8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set React State Passive'),
(@ENTRY*100+0,9,4,0,0,0,100,0,0,0,0,0,0,'',21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Disable Combat Movement'),
(@ENTRY*100+0,9,5,0,0,0,100,0,2500,2500,0,0,0,'',59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Run On'),
(@ENTRY*100+0,9,6,0,0,0,100,0,0,0,0,0,0,'',69,1,0,0,0,0,0,8,0,0,0,1.859,-780.72,9.831,0,'Mr. Smite - On Script - Move To Chest (Point 1)'),
(@ENTRY*100+1,9,0,0,0,0,100,0,0,0,0,0,0,'',11,6432,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Cast Smite Stomp'),
(@ENTRY*100+1,9,1,0,0,0,100,0,0,0,0,0,0,'',1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Say Line 3'),
(@ENTRY*100+1,9,2,0,0,0,100,0,0,0,0,0,0,'',28,6433,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Remove Aura Nimble Reflexes III'),
(@ENTRY*100+1,9,3,0,0,0,100,0,0,0,0,0,0,'',28,12787,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Remove Aura Thrash'),
(@ENTRY*100+1,9,4,0,0,0,100,0,0,0,0,0,0,'',8,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set React State Passive'),
(@ENTRY*100+1,9,5,0,0,0,100,0,0,0,0,0,0,'',21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Disable Combat Movement'),
(@ENTRY*100+1,9,6,0,0,0,100,0,2500,2500,0,0,0,'',59,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Run On'),
(@ENTRY*100+1,9,7,0,0,0,100,0,0,0,0,0,0,'',69,2,0,0,0,0,0,8,0,0,0,1.859,-780.72,9.831,0,'Mr. Smite - On Script - Move To Chest (Point 2)'),
-- At the chest: kneel unarmed, swap weapons, resume combat
(@ENTRY*100+2,9,0,0,0,0,100,0,0,0,0,0,0,'',66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.558,'Mr. Smite - On Script - Set Orientation (Chest)'),
(@ENTRY*100+2,9,1,0,0,0,100,0,0,0,0,0,0,'',40,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Sheath Unarmed'),
(@ENTRY*100+2,9,2,0,0,0,100,0,0,0,0,0,0,'',90,8,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Stand State Kneel'),
(@ENTRY*100+2,9,3,0,0,0,100,0,0,0,0,0,0,'',124,0,1,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Unequip All'),
(@ENTRY*100+2,9,4,0,0,0,100,0,3000,3000,0,0,0,'',124,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Load Equipment 2 (Axes)'),
(@ENTRY*100+2,9,5,0,0,0,100,0,0,0,0,0,0,'',91,8,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Remove Stand State Kneel'),
(@ENTRY*100+2,9,6,0,0,0,100,0,1000,1000,0,0,0,'',40,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Sheath Melee'),
(@ENTRY*100+2,9,7,0,0,0,100,0,0,0,0,0,0,'',11,12787,2,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Cast Thrash (Triggered)'),
(@ENTRY*100+2,9,8,0,0,0,100,0,0,0,0,0,0,'',21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Enable Combat Movement'),
(@ENTRY*100+2,9,9,0,0,0,100,0,0,0,0,0,0,'',8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set React State Aggressive'),
(@ENTRY*100+2,9,10,0,0,0,100,0,0,0,0,0,0,'',22,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Event Phase 2'),
(@ENTRY*100+3,9,0,0,0,0,100,0,0,0,0,0,0,'',66,0,0,0,0,0,0,8,0,0,0,0,0,0,5.558,'Mr. Smite - On Script - Set Orientation (Chest)'),
(@ENTRY*100+3,9,1,0,0,0,100,0,0,0,0,0,0,'',40,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Sheath Unarmed'),
(@ENTRY*100+3,9,2,0,0,0,100,0,0,0,0,0,0,'',90,8,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Stand State Kneel'),
(@ENTRY*100+3,9,3,0,0,0,100,0,0,0,0,0,0,'',124,0,1,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Unequip All'),
(@ENTRY*100+3,9,4,0,0,0,100,0,3000,3000,0,0,0,'',124,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Load Equipment 3 (Hammer)'),
(@ENTRY*100+3,9,5,0,0,0,100,0,0,0,0,0,0,'',91,8,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Remove Stand State Kneel'),
(@ENTRY*100+3,9,6,0,0,0,100,0,1000,1000,0,0,0,'',40,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Sheath Melee'),
(@ENTRY*100+3,9,7,0,0,0,100,0,0,0,0,0,0,'',21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Enable Combat Movement'),
(@ENTRY*100+3,9,8,0,0,0,100,0,0,0,0,0,0,'',8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set React State Aggressive'),
(@ENTRY*100+3,9,9,0,0,0,100,0,0,0,0,0,0,'',22,3,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Event Phase 3'),
-- OnReset keeps the event phase, a paused action list and combat movement; restore them explicitly
(@ENTRY*100+4,9,0,0,0,0,100,0,0,0,0,0,0,'',124,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Load Equipment 1 (Sword)'),
(@ENTRY*100+4,9,1,0,0,0,100,0,0,0,0,0,0,'',91,8,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Remove Stand State Kneel'),
(@ENTRY*100+4,9,2,0,0,0,100,0,0,0,0,0,0,'',40,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Sheath Melee'),
(@ENTRY*100+4,9,3,0,0,0,100,0,0,0,0,0,0,'',21,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Enable Combat Movement'),
(@ENTRY*100+4,9,4,0,0,0,100,0,0,0,0,0,0,'',8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set React State Aggressive'),
(@ENTRY*100+4,9,5,0,0,0,100,0,0,0,0,0,0,'',22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Set Event Phase 0'),
-- Raised by go_defias_cannon when the Ironclad door is blown (cMaNGOS instance timing)
(@ENTRY*100+5,9,0,0,0,0,100,0,500,500,0,0,0,'',1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Say Line 0'),
(@ENTRY*100+5,9,1,0,0,0,100,0,15000,15000,0,0,0,'',1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mr. Smite - On Script - Say Line 1');
