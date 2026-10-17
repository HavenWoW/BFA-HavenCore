-- Empty 8.3.7 hotfix overlay for ItemBonusListGroupEntry.db2.
-- ClientData remains the source of truth; this table exists so HOTFIX_SEL
-- can prepare. Do not fill weights here.
CREATE TABLE IF NOT EXISTS `item_bonus_list_group_entry` (
  `ID` int unsigned NOT NULL DEFAULT '0',
  `ItemBonusListID` int NOT NULL DEFAULT '0',
  `ItemLevelSelectorID` int NOT NULL DEFAULT '0',
  `SequenceValue` int NOT NULL DEFAULT '0',
  `ItemExtendedCostID` int NOT NULL DEFAULT '0',
  `ItemBonusListGroupID` int NOT NULL DEFAULT '0',
  `VerifiedBuild` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
