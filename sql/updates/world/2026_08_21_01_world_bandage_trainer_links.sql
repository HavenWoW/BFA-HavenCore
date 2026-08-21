-- BFA-HavenCore - Restore missing Bandage / First Aid trainer links
--
-- Complements the BFA First Aid migration by restoring creature_trainer links
-- for active legacy Bandage / First Aid NPCs that had no trainer mapping.
--
-- Elder Muur (66222) is intentionally excluded because gossip menu 14972
-- contains no trainer option in the current data set.

START TRANSACTION;

UPDATE `creature_template`
SET `gossip_menu_id` = 5855
WHERE `entry` IN (
    2325,4591,18990,18991,19478,26956,26992,28706,29233,33589,
    56796,59077,64482,65862,65983,66357,85930,86034,93529,
    100013,126022,133109,133396
)
AND `gossip_menu_id` = 0;

DELETE FROM `creature_trainer`
WHERE `CreatureId` IN (
    2325,2798,3373,4591,5024,5150,5759,5939,5943,16272,
    18990,18991,19478,26956,26992,28706,29233,33589,45540,
    49879,56796,59077,64482,65862,65983,66357,85930,86034,
    93529,100013,126022,133109,133396
);

INSERT INTO `creature_trainer`
(`CreatureId`,`TrainerId`,`MenuId`,`OptionIndex`)
VALUES
(3373,880,4761,0),
(5024,880,657,0),
(2798,880,5856,0),
(5150,880,4762,0),
(5759,880,5856,0),
(5939,880,5856,0),
(5943,880,5856,0),
(16272,880,5856,0),
(45540,880,4761,0),
(2325,880,5855,0),
(4591,880,5855,0),
(18990,880,5855,0),
(18991,880,5855,0),
(19478,880,5855,0),
(26956,880,5855,0),
(26992,880,5855,0),
(28706,880,5855,0),
(29233,880,5855,0),
(33589,880,5855,0),
(56796,880,5855,0),
(59077,880,5855,0),
(64482,880,5855,0),
(65862,880,5855,0),
(65983,880,5855,0),
(66357,880,5855,0),
(85930,880,5855,0),
(86034,880,5855,0),
(93529,880,5855,0),
(100013,880,5855,0),
(126022,880,5855,0),
(133109,880,5855,0),
(133396,880,5855,0),
(49879,877,10826,0);

COMMIT;
