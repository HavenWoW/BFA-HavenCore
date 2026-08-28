/* Quest: Pilfered Supplies (25668) requires quest 25668 */
UPDATE `gameobject_template` SET `Data8` = 25668 WHERE `entry` IN (203129, 203130);

/* Gameobject: Shimmerweed Basket (276) needs quest 315 */
UPDATE `gameobject_template` SET `Data8` = 315 WHERE `entry` = 276;