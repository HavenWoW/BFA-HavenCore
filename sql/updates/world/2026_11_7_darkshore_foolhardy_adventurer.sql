-- https://github.com/HavenWoW/BFA-HavenCore/issues/373
UPDATE quest_objectives SET Description='Scavenging Greymist Murlocs' WHERE ID=256718 AND QuestID=13560;
UPDATE creature SET MovementType=0 WHERE guid=245667 AND id=34207;

-- Fix spellbar and add the vehicle script
UPDATE creature_template SET spell1=62684, AIName='', ScriptName='vehicle_foolhardy_adventurer' WHERE entry=34207;

DELETE FROM creature_text WHERE CreatureID=34207;
INSERT INTO creature_text (CreatureID, GroupID, ID, `Text`, `Type`, `Language`, Probability, Emote, Duration, Sound, BroadcastTextId, TextRange, comment) VALUES
(34207, 0, 0, 'Murloc pests, I have come to steal your treasure!
Attempt to stop me! Quickly!', 12, 0, 100.0, 0, 0, 0, 0, 0, ''),
(34207, 1, 0, 'Detonation imminent.', 12, 0, 100.0, 0, 0, 0, 0, 0, '');
