-- https://github.com/HavenWoW/BFA-HavenCore/issues/528
DELETE FROM conditions WHERE SourceTypeOrReferenceId = 13 AND SourceEntry = 72070;
INSERT INTO conditions
(SourceTypeOrReferenceId,SourceGroup,SourceEntry,SourceId,ElseGroup,
 ConditionTypeOrReference,ConditionTarget,ConditionValue1,ConditionValue2,ConditionValue3,
 NegativeCondition,ErrorType,ErrorTextId,ScriptName,Comment)
VALUES
(13,1,72070,0,0,31,0,3,38560,0,0,0,0,'','Territorial Fetish - target is Spitescale Flag Bunny'),
(13,1,72070,0,0,1,0,72072,0,0,1,0,0,'','Territorial Fetish - target lacks aura 72072');

-- Remove quest aura
UPDATE creature_template_addon SET auras='' WHERE entry=38560;

DELETE FROM smart_scripts WHERE entryorguid IN (38560,3856000) AND source_type IN (0,9);
INSERT INTO smart_scripts
(entryorguid,source_type,id,link,event_type,event_phase_mask,event_chance,event_flags,
 event_param1,event_param2,event_param3,event_param4,event_param5,event_param_string,
 action_type,action_param1,action_param2,action_param3,action_param4,action_param5,action_param6,
 target_type,target_param1,target_param2,target_param3,target_x,target_y,target_z,target_o,comment)
VALUES
(38560,0,0,0,8,0,100,0,72070,0,120000,120000,0,'',80,3856000,2,0,0,0,0, 1,0,0,0,0,0,0,0,'On spellhit 72070 - Run timed actionlist'),
(3856000,9,0,0,0,0,100,0,0,0,0,0,0,'',11,72072,2,0,0,0,0, 1,0,0,0,0,0,0,0,'Actionlist - Add aura 72072'),
(3856000,9,1,0,0,0,100,0,120000,120000,0,0,0,'',28,72072,0,0,0,0,0, 1,0,0,0,0,0,0,0,'Actionlist - Remove aura 72072 after 120s'),
(3856000,9,2,0,0,0,100,0,0,0,0,0,0,'',28,72249,0,0,0,0,0, 1,0,0,0,0,0,0,0,'Actionlist - Remove aura 72249 after 120s');
