/* Bralla Cloudwing - Flying Trainer */
DELETE FROM `npc_trainer` WHERE `ID` = 43769;
INSERT INTO `npc_trainer` (`ID`, `SpellID`, `MoneyCost`, `ReqSkillLine`, `ReqSkillRank`, `ReqLevel`, `Index`) VALUES 
(43769, 33389, 40000, 762, 0, 20, 0),
(43769, 33392, 500000, 762, 75, 40, 1),
(43769, 34092, 2500000, 762, 150, 60, 2),
(43769, 34093, 50000000, 762, 225, 70, 3),
(43769, 90266, 50000000, 762, 300, 80, 4),
(43769, 90269, 2500000, 762, 225, 60, 5);

UPDATE `creature_template` SET `trainer_type` = 2 WHERE `entry` = 43769;