-- BFA guard navigation gossip repair
-- Corrects mismatched/missing BroadcastText references and removes obsolete guard directions.

UPDATE `gossip_menu_option`
SET `OptionBroadcastTextId` = CASE
    WHEN `MenuId` = 421 AND `OptionIndex` = 1 THEN 44649
    WHEN `MenuId` = 421 AND `OptionIndex` = 8 THEN 44664
    WHEN `MenuId` = 421 AND `OptionIndex` = 14 THEN 44663
    WHEN `MenuId` = 435 AND `OptionIndex` = 0 THEN 67342
    WHEN `MenuId` = 435 AND `OptionIndex` = 2 THEN 56155
    WHEN `MenuId` = 435 AND `OptionIndex` = 3 THEN 53080
    WHEN `MenuId` = 435 AND `OptionIndex` = 12 THEN 32180
    WHEN `MenuId` = 435 AND `OptionIndex` = 16 THEN 44612
    WHEN `MenuId` = 721 AND `OptionIndex` = 0 THEN 5316
    WHEN `MenuId` = 721 AND `OptionIndex` = 1 THEN 3426
    WHEN `MenuId` = 721 AND `OptionIndex` = 3 THEN 2870
    WHEN `MenuId` = 721 AND `OptionIndex` = 4 THEN 5513
    WHEN `MenuId` = 721 AND `OptionIndex` = 5 THEN 4895
    WHEN `MenuId` = 721 AND `OptionIndex` = 6 THEN 2869
    WHEN `MenuId` = 721 AND `OptionIndex` = 7 THEN 8508
    WHEN `MenuId` = 721 AND `OptionIndex` = 8 THEN 3427
    WHEN `MenuId` = 721 AND `OptionIndex` = 9 THEN 5518
    WHEN `MenuId` = 751 AND `OptionIndex` = 0 THEN 2952
    WHEN `MenuId` = 751 AND `OptionIndex` = 1 THEN 44649
    WHEN `MenuId` = 751 AND `OptionIndex` = 2 THEN 2942
    WHEN `MenuId` = 751 AND `OptionIndex` = 3 THEN 2945
    WHEN `MenuId` = 751 AND `OptionIndex` = 4 THEN 3006
    WHEN `MenuId` = 751 AND `OptionIndex` = 5 THEN 2943
    WHEN `MenuId` = 751 AND `OptionIndex` = 7 THEN 3005
    WHEN `MenuId` = 751 AND `OptionIndex` = 8 THEN 2950
    WHEN `MenuId` = 751 AND `OptionIndex` = 9 THEN 31542
    WHEN `MenuId` = 751 AND `OptionIndex` = 10 THEN 15267
    WHEN `MenuId` = 751 AND `OptionIndex` = 11 THEN 2947
    WHEN `MenuId` = 751 AND `OptionIndex` = 12 THEN 2944
    WHEN `MenuId` = 751 AND `OptionIndex` = 13 THEN 2948
    WHEN `MenuId` = 751 AND `OptionIndex` = 14 THEN 2951
    WHEN `MenuId` = 1942 AND `OptionIndex` = 0 THEN 2952
    WHEN `MenuId` = 1942 AND `OptionIndex` = 1 THEN 44649
    WHEN `MenuId` = 1942 AND `OptionIndex` = 2 THEN 2942
    WHEN `MenuId` = 1942 AND `OptionIndex` = 3 THEN 2945
    WHEN `MenuId` = 1942 AND `OptionIndex` = 4 THEN 3006
    WHEN `MenuId` = 1942 AND `OptionIndex` = 5 THEN 2943
    WHEN `MenuId` = 1942 AND `OptionIndex` = 7 THEN 3005
    WHEN `MenuId` = 1942 AND `OptionIndex` = 8 THEN 44664
    WHEN `MenuId` = 1942 AND `OptionIndex` = 9 THEN 2950
    WHEN `MenuId` = 1942 AND `OptionIndex` = 10 THEN 31542
    WHEN `MenuId` = 1942 AND `OptionIndex` = 11 THEN 15267
    WHEN `MenuId` = 1942 AND `OptionIndex` = 12 THEN 2947
    WHEN `MenuId` = 1942 AND `OptionIndex` = 13 THEN 2944
    WHEN `MenuId` = 1942 AND `OptionIndex` = 14 THEN 44663
    WHEN `MenuId` = 1942 AND `OptionIndex` = 15 THEN 2948
    WHEN `MenuId` = 1942 AND `OptionIndex` = 16 THEN 2951
    WHEN `MenuId` = 1951 AND `OptionIndex` = 0 THEN 67342
    WHEN `MenuId` = 1951 AND `OptionIndex` = 2 THEN 56155
    WHEN `MenuId` = 1951 AND `OptionIndex` = 3 THEN 53080
    WHEN `MenuId` = 1951 AND `OptionIndex` = 4 THEN 5316
    WHEN `MenuId` = 1951 AND `OptionIndex` = 5 THEN 3426
    WHEN `MenuId` = 1951 AND `OptionIndex` = 6 THEN 31340
    WHEN `MenuId` = 1951 AND `OptionIndex` = 7 THEN 15232
    WHEN `MenuId` = 1951 AND `OptionIndex` = 9 THEN 2863
    WHEN `MenuId` = 1951 AND `OptionIndex` = 10 THEN 2870
    WHEN `MenuId` = 1951 AND `OptionIndex` = 11 THEN 5513
    WHEN `MenuId` = 1951 AND `OptionIndex` = 12 THEN 9749
    WHEN `MenuId` = 1951 AND `OptionIndex` = 13 THEN 5914
    WHEN `MenuId` = 1951 AND `OptionIndex` = 14 THEN 2869
    WHEN `MenuId` = 1951 AND `OptionIndex` = 15 THEN 8508
    WHEN `MenuId` = 1951 AND `OptionIndex` = 16 THEN 5518
    WHEN `MenuId` = 2121 AND `OptionIndex` = 0 THEN 5316
    WHEN `MenuId` = 2121 AND `OptionIndex` = 1 THEN 5078
    WHEN `MenuId` = 2121 AND `OptionIndex` = 2 THEN 5914
    WHEN `MenuId` = 2121 AND `OptionIndex` = 3 THEN 5081
    WHEN `MenuId` = 2121 AND `OptionIndex` = 4 THEN 2870
    WHEN `MenuId` = 2121 AND `OptionIndex` = 5 THEN 4893
    WHEN `MenuId` = 2121 AND `OptionIndex` = 6 THEN 4895
    WHEN `MenuId` = 2121 AND `OptionIndex` = 7 THEN 8508
    WHEN `MenuId` = 2121 AND `OptionIndex` = 8 THEN 10359
    WHEN `MenuId` = 2121 AND `OptionIndex` = 9 THEN 31340
    WHEN `MenuId` = 2121 AND `OptionIndex` = 11 THEN 2869
    WHEN `MenuId` = 2168 AND `OptionIndex` = 0 THEN 2952
    WHEN `MenuId` = 2168 AND `OptionIndex` = 1 THEN 44649
    WHEN `MenuId` = 2168 AND `OptionIndex` = 2 THEN 2942
    WHEN `MenuId` = 2168 AND `OptionIndex` = 3 THEN 2945
    WHEN `MenuId` = 2168 AND `OptionIndex` = 4 THEN 3006
    WHEN `MenuId` = 2168 AND `OptionIndex` = 5 THEN 2943
    WHEN `MenuId` = 2168 AND `OptionIndex` = 7 THEN 3005
    WHEN `MenuId` = 2168 AND `OptionIndex` = 8 THEN 2950
    WHEN `MenuId` = 2168 AND `OptionIndex` = 9 THEN 31542
    WHEN `MenuId` = 2168 AND `OptionIndex` = 10 THEN 15267
    WHEN `MenuId` = 2168 AND `OptionIndex` = 11 THEN 2947
    WHEN `MenuId` = 2168 AND `OptionIndex` = 12 THEN 2944
    WHEN `MenuId` = 2168 AND `OptionIndex` = 13 THEN 2948
    WHEN `MenuId` = 2168 AND `OptionIndex` = 14 THEN 2951
    WHEN `MenuId` = 2351 AND `OptionIndex` = 1 THEN 44649
    WHEN `MenuId` = 2351 AND `OptionIndex` = 2 THEN 2942
    WHEN `MenuId` = 2351 AND `OptionIndex` = 3 THEN 2945
    WHEN `MenuId` = 2351 AND `OptionIndex` = 4 THEN 3006
    WHEN `MenuId` = 2351 AND `OptionIndex` = 5 THEN 2943
    WHEN `MenuId` = 2351 AND `OptionIndex` = 7 THEN 3005
    WHEN `MenuId` = 2351 AND `OptionIndex` = 8 THEN 2950
    WHEN `MenuId` = 2351 AND `OptionIndex` = 9 THEN 31542
    WHEN `MenuId` = 2351 AND `OptionIndex` = 10 THEN 15267
    WHEN `MenuId` = 2351 AND `OptionIndex` = 11 THEN 2947
    WHEN `MenuId` = 2351 AND `OptionIndex` = 12 THEN 2944
    WHEN `MenuId` = 2351 AND `OptionIndex` = 13 THEN 2948
    WHEN `MenuId` = 2351 AND `OptionIndex` = 14 THEN 2951
    WHEN `MenuId` = 2352 AND `OptionIndex` = 0 THEN 5316
    WHEN `MenuId` = 2352 AND `OptionIndex` = 1 THEN 4888
    WHEN `MenuId` = 2352 AND `OptionIndex` = 2 THEN 5330
    WHEN `MenuId` = 2352 AND `OptionIndex` = 4 THEN 4893
    WHEN `MenuId` = 2352 AND `OptionIndex` = 5 THEN 4895
    WHEN `MenuId` = 2352 AND `OptionIndex` = 6 THEN 8508
    WHEN `MenuId` = 2352 AND `OptionIndex` = 7 THEN 7241
    WHEN `MenuId` = 2352 AND `OptionIndex` = 8 THEN 10359
    WHEN `MenuId` = 2352 AND `OptionIndex` = 10 THEN 2869
    WHEN `MenuId` = 2352 AND `OptionIndex` = 11 THEN 32998
    WHEN `MenuId` = 2849 AND `OptionIndex` = 1 THEN 4888
    WHEN `MenuId` = 2849 AND `OptionIndex` = 2 THEN 31340
    WHEN `MenuId` = 2849 AND `OptionIndex` = 3 THEN 6790
    WHEN `MenuId` = 2849 AND `OptionIndex` = 6 THEN 4893
    WHEN `MenuId` = 2849 AND `OptionIndex` = 7 THEN 33141
    WHEN `MenuId` = 2849 AND `OptionIndex` = 9 THEN 8515
    WHEN `MenuId` = 2849 AND `OptionIndex` = 13 THEN 6723
    WHEN `MenuId` = 3580 AND `OptionIndex` = 7 THEN 5330
    WHEN `MenuId` = 7667 AND `OptionIndex` = 2 THEN 2942
    WHEN `MenuId` = 7667 AND `OptionIndex` = 3 THEN 2945
    WHEN `MenuId` = 7667 AND `OptionIndex` = 4 THEN 3006
    WHEN `MenuId` = 7667 AND `OptionIndex` = 5 THEN 2943
    WHEN `MenuId` = 7667 AND `OptionIndex` = 7 THEN 3005
    WHEN `MenuId` = 7667 AND `OptionIndex` = 8 THEN 2950
    WHEN `MenuId` = 7667 AND `OptionIndex` = 9 THEN 31542
    WHEN `MenuId` = 7667 AND `OptionIndex` = 10 THEN 15267
    WHEN `MenuId` = 7667 AND `OptionIndex` = 11 THEN 2947
    WHEN `MenuId` = 7667 AND `OptionIndex` = 12 THEN 2944
    WHEN `MenuId` = 7667 AND `OptionIndex` = 13 THEN 2948
    WHEN `MenuId` = 7777 AND `OptionIndex` = 0 THEN 5316
    WHEN `MenuId` = 7777 AND `OptionIndex` = 1 THEN 3426
    WHEN `MenuId` = 7777 AND `OptionIndex` = 2 THEN 5330
    WHEN `MenuId` = 7777 AND `OptionIndex` = 3 THEN 2870
    WHEN `MenuId` = 7777 AND `OptionIndex` = 4 THEN 5513
    WHEN `MenuId` = 7777 AND `OptionIndex` = 5 THEN 4895
    WHEN `MenuId` = 7777 AND `OptionIndex` = 6 THEN 8508
    WHEN `MenuId` = 7777 AND `OptionIndex` = 7 THEN 15230
    WHEN `MenuId` = 7777 AND `OptionIndex` = 8 THEN 15232
    WHEN `MenuId` = 7777 AND `OptionIndex` = 9 THEN 2869
    WHEN `MenuId` = 7788 AND `OptionIndex` = 0 THEN 2952
    WHEN `MenuId` = 7788 AND `OptionIndex` = 1 THEN 44649
    WHEN `MenuId` = 7788 AND `OptionIndex` = 2 THEN 2942
    WHEN `MenuId` = 7788 AND `OptionIndex` = 3 THEN 3006
    WHEN `MenuId` = 7788 AND `OptionIndex` = 4 THEN 2943
    WHEN `MenuId` = 7788 AND `OptionIndex` = 6 THEN 3005
    WHEN `MenuId` = 7788 AND `OptionIndex` = 7 THEN 2950
    WHEN `MenuId` = 7788 AND `OptionIndex` = 8 THEN 31542
    WHEN `MenuId` = 7788 AND `OptionIndex` = 9 THEN 15267
    WHEN `MenuId` = 7788 AND `OptionIndex` = 10 THEN 2947
    WHEN `MenuId` = 7788 AND `OptionIndex` = 11 THEN 2944
    WHEN `MenuId` = 7788 AND `OptionIndex` = 12 THEN 2948
    WHEN `MenuId` = 7788 AND `OptionIndex` = 13 THEN 2951
    WHEN `MenuId` = 7788 AND `OptionIndex` = 14 THEN 2945
    WHEN `MenuId` = 8357 AND `OptionIndex` = 8 THEN 2869
    WHEN `MenuId` = 9727 AND `OptionIndex` = 0 THEN 28607
    WHEN `MenuId` = 9727 AND `OptionIndex` = 1 THEN 28608
    WHEN `MenuId` = 10043 AND `OptionIndex` = 5 THEN 2863
    WHEN `MenuId` = 10043 AND `OptionIndex` = 6 THEN 2870
    WHEN `MenuId` = 10043 AND `OptionIndex` = 7 THEN 5513
    WHEN `MenuId` = 10043 AND `OptionIndex` = 9 THEN 4895
    WHEN `MenuId` = 10043 AND `OptionIndex` = 11 THEN 8508
    WHEN `MenuId` = 10043 AND `OptionIndex` = 13 THEN 32216
    WHEN `MenuId` = 10082 AND `OptionIndex` = 0 THEN 2868
    WHEN `MenuId` = 10082 AND `OptionIndex` = 5 THEN 2869
    WHEN `MenuId` = 10173 AND `OptionIndex` = 4 THEN 32712
    WHEN `MenuId` = 10767 AND `OptionIndex` = 1 THEN 44649
    WHEN `MenuId` = 10767 AND `OptionIndex` = 2 THEN 2942
    WHEN `MenuId` = 10767 AND `OptionIndex` = 3 THEN 2945
    WHEN `MenuId` = 10767 AND `OptionIndex` = 4 THEN 3006
    WHEN `MenuId` = 10767 AND `OptionIndex` = 5 THEN 2943
    WHEN `MenuId` = 10767 AND `OptionIndex` = 7 THEN 3005
    WHEN `MenuId` = 10767 AND `OptionIndex` = 8 THEN 2950
    WHEN `MenuId` = 10767 AND `OptionIndex` = 9 THEN 31542
    WHEN `MenuId` = 10767 AND `OptionIndex` = 10 THEN 15267
    WHEN `MenuId` = 10767 AND `OptionIndex` = 11 THEN 2947
    WHEN `MenuId` = 10767 AND `OptionIndex` = 12 THEN 2944
    WHEN `MenuId` = 10767 AND `OptionIndex` = 13 THEN 2948
    WHEN `MenuId` = 10767 AND `OptionIndex` = 14 THEN 2951
    WHEN `MenuId` = 11839 AND `OptionIndex` = 0 THEN 6370
    WHEN `MenuId` = 11839 AND `OptionIndex` = 1 THEN 44592
    WHEN `MenuId` = 11841 AND `OptionIndex` = 0 THEN 44596
    WHEN `MenuId` = 11841 AND `OptionIndex` = 1 THEN 44597
    WHEN `MenuId` = 11843 AND `OptionIndex` = 0 THEN 44603
    WHEN `MenuId` = 11843 AND `OptionIndex` = 1 THEN 44604
    WHEN `MenuId` = 11845 AND `OptionIndex` = 0 THEN 44652
    WHEN `MenuId` = 11845 AND `OptionIndex` = 2 THEN 44635
    WHEN `MenuId` = 11845 AND `OptionIndex` = 3 THEN 44639
    WHEN `MenuId` = 11845 AND `OptionIndex` = 4 THEN 29416
    WHEN `MenuId` = 11845 AND `OptionIndex` = 5 THEN 44637
    WHEN `MenuId` = 11846 AND `OptionIndex` = 0 THEN 44610
    WHEN `MenuId` = 11846 AND `OptionIndex` = 1 THEN 44609
    WHEN `MenuId` = 11848 AND `OptionIndex` = 0 THEN 5316
    WHEN `MenuId` = 11848 AND `OptionIndex` = 1 THEN 44659
    WHEN `MenuId` = 11848 AND `OptionIndex` = 2 THEN 44654
    WHEN `MenuId` = 11848 AND `OptionIndex` = 3 THEN 44656
    WHEN `MenuId` = 11848 AND `OptionIndex` = 4 THEN 44657
    WHEN `MenuId` = 11848 AND `OptionIndex` = 5 THEN 2869
    WHEN `MenuId` = 11969 AND `OptionIndex` = 0 THEN 5316
    WHEN `MenuId` = 11969 AND `OptionIndex` = 1 THEN 3426
    WHEN `MenuId` = 11969 AND `OptionIndex` = 3 THEN 5513
    WHEN `MenuId` = 11969 AND `OptionIndex` = 4 THEN 4895
    WHEN `MenuId` = 11969 AND `OptionIndex` = 5 THEN 2869
    WHEN `MenuId` = 11969 AND `OptionIndex` = 6 THEN 8508
    WHEN `MenuId` = 11969 AND `OptionIndex` = 7 THEN 44612
    WHEN `MenuId` = 11977 AND `OptionIndex` = 0 THEN 2945
    WHEN `MenuId` = 11977 AND `OptionIndex` = 1 THEN 2950
    WHEN `MenuId` = 11977 AND `OptionIndex` = 2 THEN 31542
    WHEN `MenuId` = 11977 AND `OptionIndex` = 3 THEN 3005
    WHEN `MenuId` = 11978 AND `OptionIndex` = 0 THEN 32712
    WHEN `MenuId` = 11978 AND `OptionIndex` = 1 THEN 45446
    WHEN `MenuId` = 11978 AND `OptionIndex` = 2 THEN 45447
    WHEN `MenuId` = 11978 AND `OptionIndex` = 3 THEN 32719
    WHEN `MenuId` = 12243 AND `OptionIndex` = 1 THEN 129109
    WHEN `MenuId` = 12245 AND `OptionIndex` = 1 THEN 129152
    WHEN `MenuId` = 12245 AND `OptionIndex` = 2 THEN 129175
    WHEN `MenuId` = 12245 AND `OptionIndex` = 3 THEN 129176
    WHEN `MenuId` = 12245 AND `OptionIndex` = 5 THEN 129111
    WHEN `MenuId` = 12245 AND `OptionIndex` = 7 THEN 129108
    WHEN `MenuId` = 12245 AND `OptionIndex` = 8 THEN 129157
    WHEN `MenuId` = 12637 AND `OptionIndex` = 0 THEN 50627
    WHEN `MenuId` = 12637 AND `OptionIndex` = 1 THEN 50628
    WHEN `MenuId` = 12638 AND `OptionIndex` = 0 THEN 50627
    WHEN `MenuId` = 12638 AND `OptionIndex` = 1 THEN 50628
    WHEN `MenuId` = 12639 AND `OptionIndex` = 0 THEN 50627
    WHEN `MenuId` = 12639 AND `OptionIndex` = 1 THEN 50628
    WHEN `MenuId` = 12865 AND `OptionIndex` = 0 THEN 2952
    WHEN `MenuId` = 12865 AND `OptionIndex` = 2 THEN 2942
    WHEN `MenuId` = 12865 AND `OptionIndex` = 3 THEN 2945
    WHEN `MenuId` = 12865 AND `OptionIndex` = 4 THEN 3006
    WHEN `MenuId` = 12865 AND `OptionIndex` = 5 THEN 2943
    WHEN `MenuId` = 12865 AND `OptionIndex` = 7 THEN 3005
    WHEN `MenuId` = 12865 AND `OptionIndex` = 8 THEN 2950
    WHEN `MenuId` = 12865 AND `OptionIndex` = 9 THEN 31542
    WHEN `MenuId` = 12865 AND `OptionIndex` = 10 THEN 15267
    WHEN `MenuId` = 12865 AND `OptionIndex` = 11 THEN 2947
    WHEN `MenuId` = 12865 AND `OptionIndex` = 12 THEN 2944
    WHEN `MenuId` = 12865 AND `OptionIndex` = 14 THEN 2948
    WHEN `MenuId` = 12865 AND `OptionIndex` = 15 THEN 2951
    ELSE `OptionBroadcastTextId`
END
WHERE `MenuId` IN (421, 435, 721, 751, 1942, 1951, 2121, 2168, 2351, 2352, 2849, 3580, 7667, 7777, 7788, 8357, 9727, 10043, 10082, 10173, 10767, 11839, 11841, 11843, 11845, 11846, 11848, 11969, 11977, 11978, 12243, 12245, 12637, 12638, 12639, 12865);

UPDATE `gossip_menu_option` SET `OptionText` = 'Weapon Master', `OptionBroadcastTextId` = 15230 WHERE `MenuId` = 7777 AND `OptionIndex` = 7;
UPDATE `gossip_menu_option` SET `OptionText` = 'Battlemasters', `OptionBroadcastTextId` = 15232 WHERE `MenuId` = 7777 AND `OptionIndex` = 8;
UPDATE `gossip_menu_option` SET `OptionText` = 'Profession Trainer', `OptionBroadcastTextId` = 2869 WHERE `MenuId` = 7777 AND `OptionIndex` = 9;
UPDATE `gossip_menu_option_action` AS `dst` JOIN `gossip_menu_option_action` AS `src` ON `src`.`MenuId` = 7777 AND `src`.`OptionIndex` = 10 SET `dst`.`ActionMenuId` = `src`.`ActionMenuId`, `dst`.`ActionPoiId` = `src`.`ActionPoiId` WHERE `dst`.`MenuId` = 7777 AND `dst`.`OptionIndex` = 9;
DELETE FROM `gossip_menu_option_action` WHERE `MenuId` = 7777 AND `OptionIndex` = 10;
DELETE FROM `gossip_menu_option` WHERE `MenuId` = 7777 AND `OptionIndex` = 10;

DELETE FROM `gossip_menu_option_action` WHERE `MenuId` = 721 AND `OptionIndex` IN (10, 11, 12);
DELETE FROM `gossip_menu_option` WHERE `MenuId` = 721 AND `OptionIndex` IN (10, 11, 12);

DELETE FROM `gossip_menu_option_action`
WHERE (`MenuId`, `OptionIndex`) IN (
    (435, 7),
    (721, 2),
    (1951, 8),
    (2121, 10),
    (2352, 9),
    (2849, 12),
    (3285, 4),
    (3331, 4),
    (3356, 4),
    (3533, 5),
    (3580, 5),
    (7633, 3),
    (8185, 4),
    (8357, 7),
    (8851, 2),
    (11969, 2),
    (421, 6),
    (751, 6),
    (1942, 6),
    (2168, 6),
    (2351, 6),
    (3355, 5),
    (7667, 6),
    (8424, 4),
    (10767, 6),
    (12865, 6),
    (14769, 6),
    (17349, 6),
    (7788, 5)
);

DELETE FROM `gossip_menu_option`
WHERE (`MenuId`, `OptionIndex`) IN (
    (435, 7),
    (721, 2),
    (1951, 8),
    (2121, 10),
    (2352, 9),
    (2849, 12),
    (3285, 4),
    (3331, 4),
    (3356, 4),
    (3533, 5),
    (3580, 5),
    (7633, 3),
    (8185, 4),
    (8357, 7),
    (8851, 2),
    (11969, 2),
    (421, 6),
    (751, 6),
    (1942, 6),
    (2168, 6),
    (2351, 6),
    (3355, 5),
    (7667, 6),
    (8424, 4),
    (10767, 6),
    (12865, 6),
    (14769, 6),
    (17349, 6),
    (7788, 5)
);

-- Verification: rows returned here still have mismatched BroadcastText on patched navigation menus.
-- SELECT g.MenuId, g.OptionIndex, g.OptionText, g.OptionBroadcastTextId, b.Text
-- FROM gossip_menu_option g
-- LEFT JOIN bfa_hotfixes.broadcast_text b ON b.ID = g.OptionBroadcastTextId AND b.VerifiedBuild = 35662
-- WHERE g.MenuId IN (421, 435, 721, 751, 1942, 1951, 2121, 2168, 2351, 2352, 2849, 3580, 7667, 7777, 7788, 8357, 9727, 10043, 10082, 10173, 10767, 11839, 11841, 11843, 11845, 11846, 11848, 11969, 11977, 11978, 12243, 12245, 12637, 12638, 12639, 12865)
--   AND g.OptionBroadcastTextId <> 0
--   AND LOWER(TRIM(g.OptionText)) <> LOWER(TRIM(b.Text))
-- ORDER BY g.MenuId, g.OptionIndex;
