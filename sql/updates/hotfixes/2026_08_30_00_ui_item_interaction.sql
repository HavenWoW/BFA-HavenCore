-- Empty 8.3.7 hotfix overlay for UiItemInteraction.db2.
-- ClientData remains the source of truth; this table exists so HOTFIX_SEL
-- can prepare. Do not INSERT retail rows.
CREATE TABLE IF NOT EXISTS `ui_item_interaction` (
  `TutorialText` text,
  `TitleText` text,
  `Description` text,
  `ButtonText` text,
  `ID` int unsigned NOT NULL DEFAULT '0',
  `UiTextureKitID` int NOT NULL DEFAULT '0',
  `OpenSoundKitID` int NOT NULL DEFAULT '0',
  `CloseSoundKitID` int NOT NULL DEFAULT '0',
  `Cost` int NOT NULL DEFAULT '0',
  `ItemInteractionFrameType` tinyint NOT NULL DEFAULT '0',
  `InteractionSpellID` int NOT NULL DEFAULT '0',
  `CurrencyTypeID` int NOT NULL DEFAULT '0',
  `Flags` tinyint NOT NULL DEFAULT '0',
  `DropInSlotSoundKitID` int NOT NULL DEFAULT '0',
  `TakeOutSlotSoundKitID` int NOT NULL DEFAULT '0',
  `VerifiedBuild` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS `ui_item_interaction_locale` (
  `ID` int unsigned NOT NULL DEFAULT '0',
  `locale` varchar(4) NOT NULL,
  `TutorialText_lang` text,
  `TitleText_lang` text,
  `Description_lang` text,
  `ButtonText_lang` text,
  `VerifiedBuild` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`locale`,`VerifiedBuild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
