--
DROP TABLE IF EXISTS `secret_digest`;
CREATE TABLE `secret_digest` (
  `id` int(10) unsigned not null,
  `digest` varchar(100) not null,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB;

ALTER TABLE `account` ADD COLUMN `totp_secret` VARBINARY(128) DEFAULT NULL AFTER `s`;

-- 
DELETE FROM `rbac_permissions` WHERE `id` BETWEEN 2011 AND 2014;
INSERT INTO `rbac_permissions` (`id`,`name`) VALUES
(2011, 'Command: account 2fa'),
(2012, 'Command: account 2fa setup'),
(2013, 'Command: account 2fa remove'),
(2014, 'Command: account set 2fa');

DELETE FROM `rbac_linked_permissions` WHERE `linkedId` BETWEEN 2011 AND 2014;
INSERT INTO `rbac_linked_permissions` (`id`,`linkedId`) VALUES
(199, 2011),
(199, 2012),
(199, 2013);
