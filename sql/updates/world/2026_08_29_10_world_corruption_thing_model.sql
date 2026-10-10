-- Thing From Beyond (161895): 8.3.7.35662 sniff (creature_template_model row
-- verified build 35662) ships native display 11686 (invisible) at scale 0.5.
-- The retail client keeps that display and builds the void-shadow reflection
-- itself from UNIT_FLAG2_MIRROR_IMAGE plus the mirror-image data reply;
-- without the row the clone renders as a plain full-color player copy.
DELETE FROM `creature_template_model` WHERE `CreatureID`=161895;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(161895, 0, 11686, 0.5, 1, 35662);
