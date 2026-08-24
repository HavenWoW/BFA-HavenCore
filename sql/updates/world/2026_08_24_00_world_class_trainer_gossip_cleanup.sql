DELETE gmo
FROM `gossip_menu_option` gmo
WHERE gmo.`MenuId` <> 0
AND EXISTS
(
    SELECT 1
    FROM `creature_template` ct
    WHERE ct.`gossip_menu_id` = gmo.`MenuId`
      AND ct.`trainer_class` BETWEEN 1 AND 11
      AND ct.`subname` NOT LIKE '%Portal Trainer%'
)
AND
(
       gmo.`OptionType` IN (5, 16, 18, 20)
    OR gmo.`OptionText` LIKE '%Dual Talent Specialization%'
    OR gmo.`OptionText` LIKE '%unlearn my talents%'
    OR gmo.`OptionText` LIKE '%reset my Class Specialization%'
);
