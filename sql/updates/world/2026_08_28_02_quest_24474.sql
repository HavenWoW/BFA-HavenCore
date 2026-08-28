/* Quest: First Things First: We're Gonna Need Some Beer (24474) */
DELETE FROM `quest_poi_points` WHERE `QuestID` = 24474;   
INSERT INTO `quest_poi_points` (`QuestID`, `Idx1`, `Idx2`, `X`, `Y`, `VerifiedBuild`) VALUES 
(24474, 3, 0, -6087, 378, 0),
(24474, 2, 0, -6142, 322, 0),
(24474, 0, 0, -6142, 432, 0),
(24474, 1, 0, -6149, 596, 0);  

DELETE FROM `quest_poi` WHERE `QuestID` = 24474;
INSERT INTO `quest_poi` (`QuestID`, `BlobIndex`, `Idx1`, `ObjectiveIndex`, `QuestObjectiveID`, `QuestObjectID`, `MapID`, `UiMapID`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `SpawnTrackingID`, `AlwaysAllowMergingBlobs`, `VerifiedBuild`) VALUES 
(24474, 0, 3, -1, 0, 0, 0, 427, 0, 0, 0, 0, 0, 0, 0),
(24474, 0, 0, 0, 255019, 49744, 0, 427, 0, 0, 0, 0, 0, 0, 0),
(24474, 0, 1, 0, 255020, 49745, 0, 427, 0, 0, 0, 0, 0, 0, 0),
(24474, 0, 2, 0, 255021, 49746, 0, 427, 0, 0, 0, 0, 0, 0, 0);

/* Gameobject: Keg of Theramore Pale Ale (201609) needs quest 24474 */
UPDATE `gameobject_template` SET `Data8` = 24474 WHERE `entry` = 201609;  

/* Gameobject: Keg of Stormhammer Stout (201610) needs quest 24474 */
UPDATE `gameobject_template` SET `Data8` = 24474 WHERE `entry` = 201610;

/* Gameobject: Keg of Gnomenbrau (201611) needs quest 24474 */
UPDATE `gameobject_template` SET `Data8` = 24474 WHERE `entry` = 201611;