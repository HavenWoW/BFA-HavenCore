-- ============================================================================
-- HavenCore guild / community restoration - CHARACTERS database (merged).
--
-- Replaces these individual updates (delete them from sql/updates/characters):
--   2026_09_19_00_guild_club_message.sql
--   2026_09_20_00_club_finder_bfa.sql
--   2026_09_21_00_characters_guild_club_message_destroy.sql
--   2026_09_21_02_characters_club_finder_tables.sql      (if present)
--   2026_09_21_03_characters_club_finder_storage.sql
--   2026_09_21_04_characters_club_stream_history.sql
--
-- Works on a fresh base (legacy guild_finder_* tables, no club tables) and on an
-- existing database in any partial state of the updates above. Every step is
-- idempotent: CREATE TABLE IF NOT EXISTS, INSERT IGNORE, and migrations that run
-- only when their source tables / columns exist. Legacy tables are never dropped.
--
-- Result:
--   club_finder_posting / club_finder_application   (ClubFinderMgr)
--   club_message / club_stream_view_marker /
--   club_mention_view_marker / club_member_mention  (ClubStreamHistoryMgr)
-- ============================================================================

-- ---------------------------------------------------------------------------
-- 1. Club Finder storage (independent postingId). Repairs the earlier direct
--    rename of guild_finder_* to club_finder_* if that ran, then migrates.
-- ---------------------------------------------------------------------------
SET @schema_name := DATABASE();

-- If the bad rename already ran, move the legacy-shaped tables back first.
SET @bad_posting := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'club_finder_posting' AND COLUMN_NAME = 'guildId'
);
SET @proper_posting := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'club_finder_posting' AND COLUMN_NAME = 'postingId'
);
SET @legacy_posting := (
    SELECT COUNT(*) FROM information_schema.TABLES
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_guild_settings'
);
SET @sql := IF(@bad_posting > 0 AND @proper_posting = 0 AND @legacy_posting = 0,
    'RENAME TABLE `club_finder_posting` TO `guild_finder_guild_settings`',
    'SELECT 1');
PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @bad_application := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'club_finder_application' AND COLUMN_NAME = 'guildId'
);
SET @proper_application := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'club_finder_application' AND COLUMN_NAME = 'postingId'
);
SET @legacy_application := (
    SELECT COUNT(*) FROM information_schema.TABLES
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_applicant'
);
SET @sql := IF(@bad_application > 0 AND @proper_application = 0 AND @legacy_application = 0,
    'RENAME TABLE `club_finder_application` TO `guild_finder_applicant`',
    'SELECT 1');
PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

CREATE TABLE IF NOT EXISTS `club_finder_posting` (
    `postingId`             INT UNSIGNED     NOT NULL,
    `clubId`                BIGINT UNSIGNED  NOT NULL DEFAULT 0,
    `name`                  VARCHAR(96)      NOT NULL DEFAULT '',
    `description`           TEXT             NULL,
    `recruitingSpecs`       BIGINT UNSIGNED  NOT NULL DEFAULT 0,
    `recruitmentFlags`      INT UNSIGNED     NOT NULL DEFAULT 0,
    `itemLevelRequirement`  INT UNSIGNED     NOT NULL DEFAULT 0,
    `avatarId`              INT UNSIGNED     NOT NULL DEFAULT 0,
    `displayFlags`          INT UNSIGNED     NOT NULL DEFAULT 0,
    `type`                  TINYINT UNSIGNED NOT NULL DEFAULT 1,
    `crossFaction`          TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `lastPosterGuid`        BIGINT UNSIGNED  NOT NULL DEFAULT 0,
    `lastUpdatedTime`       BIGINT           NOT NULL DEFAULT 0,
    -- BFA's legacy LF_GUILD opcodes still exist beside Club Finder. Keep their
    -- four filters as compatibility state, but they are not posting identity.
    `legacyAvailability`    TINYINT UNSIGNED NOT NULL DEFAULT 3,
    `legacyClassRoles`      TINYINT UNSIGNED NOT NULL DEFAULT 7,
    `legacyInterests`       TINYINT UNSIGNED NOT NULL DEFAULT 31,
    `legacyLevel`           TINYINT UNSIGNED NOT NULL DEFAULT 1,
    PRIMARY KEY (`postingId`),
    UNIQUE KEY `idx_clubId` (`clubId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS `club_finder_application` (
    `postingId`       INT UNSIGNED     NOT NULL,
    `playerGuid`      BIGINT UNSIGNED  NOT NULL,
    `comment`         TEXT             NULL,
    `specs`           BIGINT UNSIGNED  NOT NULL DEFAULT 0,
    `status`          TINYINT UNSIGNED NOT NULL DEFAULT 1 COMMENT 'PlayerClubRequestStatus',
    `lastUpdatedTime` BIGINT           NOT NULL DEFAULT 0,
    -- Compatibility fields required by the BFA LF_GUILD view.
    `availability`    TINYINT UNSIGNED NOT NULL DEFAULT 3,
    `classRole`       TINYINT UNSIGNED NOT NULL DEFAULT 7,
    `interests`       TINYINT UNSIGNED NOT NULL DEFAULT 31,
    `submitTime`      BIGINT UNSIGNED  NOT NULL DEFAULT 0,
    `itemLevel`       INT UNSIGNED     NOT NULL DEFAULT 0,
    PRIMARY KEY (`postingId`, `playerGuid`),
    KEY `idx_playerGuid` (`playerGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Build migration expressions according to which columns exist in the legacy
-- table. This supports both the original Haven schema and the expanded schema
-- produced by the earlier Club Finder patches.
SET @has_specMask := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_guild_settings' AND COLUMN_NAME = 'specMask'
);
SET @has_recruitmentFlags := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_guild_settings' AND COLUMN_NAME = 'recruitmentFlags'
);
SET @has_minItemLevel := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_guild_settings' AND COLUMN_NAME = 'minItemLevel'
);
SET @has_lastUpdatedTime := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_guild_settings' AND COLUMN_NAME = 'lastUpdatedTime'
);

SET @spec_expr := IF(@has_specMask > 0, 'COALESCE(gfs.specMask, 0)', '0');
SET @min_ilvl_expr := IF(@has_minItemLevel > 0, 'COALESCE(gfs.minItemLevel, 0)', '0');
SET @existing_flags_expr := IF(@has_recruitmentFlags > 0, 'COALESCE(gfs.recruitmentFlags, 0)', '0');
SET @updated_expr := IF(@has_lastUpdatedTime > 0,
    'CASE WHEN gfs.listed <> 0 THEN IF(COALESCE(gfs.lastUpdatedTime, 0) = 0, UNIX_TIMESTAMP(), gfs.lastUpdatedTime) ELSE COALESCE(gfs.lastUpdatedTime, 0) END',
    'CASE WHEN gfs.listed <> 0 THEN UNIX_TIMESTAMP() ELSE 0 END');

-- Clear the legacy enable/max-level bits before rebuilding them from the old
-- explicit columns. Focus flags are ORed from the old interests mask.
SET @flags_expr := CONCAT('(((', @existing_flags_expr, ' & 4294955007)',
    ' | IF(gfs.listed <> 0, 4096, 0)',
    ' | IF(gfs.level = 2, 8192, 0)',
    ' | IF((gfs.interests & 2) <> 0, 2, 0)',
    ' | IF((gfs.interests & 4) <> 0, 4, 0)',
    ' | IF((gfs.interests & 8) <> 0, 8, 0)',
    ' | IF((gfs.interests & 16) <> 0, 16, 0)',
    ' | IF((gfs.interests & 1) <> 0, 32, 0)))');

SET @legacy_posting_now := (
    SELECT COUNT(*) FROM information_schema.TABLES
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_guild_settings'
);
SET @sql := IF(@legacy_posting_now = 0, 'SELECT 1', CONCAT(
    'INSERT IGNORE INTO club_finder_posting ',
    '(postingId, clubId, name, description, recruitingSpecs, recruitmentFlags, itemLevelRequirement, avatarId, displayFlags, type, crossFaction, lastPosterGuid, lastUpdatedTime, legacyAvailability, legacyClassRoles, legacyInterests, legacyLevel) ',
    'SELECT CAST(gfs.guildId AS UNSIGNED), gfs.guildId, COALESCE(g.name, ''''), gfs.comment, ',
    @spec_expr, ', ', @flags_expr, ', ', @min_ilvl_expr, ', 0, 0, 1, 0, COALESCE(g.leaderguid, 0), ', @updated_expr, ', ',
    'IF(gfs.availability = 0, 3, gfs.availability), ',
    'IF(gfs.classRoles = 0, 7, gfs.classRoles), ',
    'IF(gfs.interests = 0, 31, gfs.interests), ',
    'IF(gfs.level = 0, 1, gfs.level) ',
    'FROM guild_finder_guild_settings gfs LEFT JOIN guild g ON g.guildid = gfs.guildId'
));
PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @has_app_specMask := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_applicant' AND COLUMN_NAME = 'specMask'
);
SET @has_app_itemLevel := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_applicant' AND COLUMN_NAME = 'itemLevel'
);
SET @has_app_status := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_applicant' AND COLUMN_NAME = 'status'
);
SET @has_app_updateTime := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_applicant' AND COLUMN_NAME = 'updateTime'
);

SET @app_spec_expr := IF(@has_app_specMask > 0, 'COALESCE(gfa.specMask, 0)', '0');
SET @app_item_expr := IF(@has_app_itemLevel > 0, 'COALESCE(gfa.itemLevel, 0)', '0');
SET @app_status_expr := IF(@has_app_status > 0, 'IF(COALESCE(gfa.status, 0) = 0, 1, gfa.status)', '1');
SET @app_updated_expr := IF(@has_app_updateTime > 0, 'COALESCE(NULLIF(gfa.updateTime, 0), gfa.submitTime, 0)', 'COALESCE(gfa.submitTime, 0)');

SET @legacy_application_now := (
    SELECT COUNT(*) FROM information_schema.TABLES
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_finder_applicant'
);
SET @sql := IF(@legacy_application_now = 0, 'SELECT 1', CONCAT(
    'INSERT IGNORE INTO club_finder_application ',
    '(postingId, playerGuid, comment, specs, status, lastUpdatedTime, availability, classRole, interests, submitTime, itemLevel) ',
    'SELECT cfp.postingId, gfa.playerGuid, gfa.comment, ', @app_spec_expr, ', ', @app_status_expr, ', ', @app_updated_expr, ', ',
    'IF(gfa.availability = 0, 3, gfa.availability), ',
    'IF(gfa.classRole = 0, 7, gfa.classRole), ',
    'IF(gfa.interests = 0, 31, gfa.interests), ',
    'COALESCE(gfa.submitTime, 0), ', @app_item_expr, ' ',
    'FROM guild_finder_applicant gfa INNER JOIN club_finder_posting cfp ON cfp.clubId = gfa.guildId'
));
PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- ---------------------------------------------------------------------------
-- 2. Club stream history (Draconic layout + Haven tombstone columns). Migrates
--    guild_club_message / guild_club_stream_view if they exist.
-- ---------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS `club_message` (
    `clubId`           BIGINT UNSIGNED NOT NULL,
    `streamId`         BIGINT UNSIGNED NOT NULL,
    `epoch`            BIGINT UNSIGNED NOT NULL COMMENT 'MessageId.epoch - microseconds since unix epoch',
    `position`         BIGINT UNSIGNED NOT NULL COMMENT 'MessageId.position - monotonic per (club, stream)',
    `authorAccountId`  INT UNSIGNED    NOT NULL DEFAULT 0,
    `authorGuid`       BIGINT UNSIGNED NOT NULL,
    `content`          TEXT            NOT NULL,
    `createdTime`      BIGINT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'unix seconds, for age based retention',
    `destroyerGuid`    BIGINT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Haven: tombstone - who deleted the message',
    `destroyTime`      BIGINT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Haven: tombstone - microseconds, 0 = not deleted',
    PRIMARY KEY (`clubId`, `streamId`, `epoch`, `position`),
    KEY `idx_createdTime` (`createdTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS `club_stream_view_marker` (
    `clubId`       BIGINT UNSIGNED NOT NULL,
    `streamId`     BIGINT UNSIGNED NOT NULL,
    `memberGuid`   BIGINT UNSIGNED NOT NULL,
    `lastViewTime` BIGINT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'microseconds since unix epoch',
    PRIMARY KEY (`clubId`, `streamId`, `memberGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS `club_mention_view_marker` (
    `memberGuid`   BIGINT UNSIGNED NOT NULL,
    `lastViewTime` BIGINT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'microseconds since unix epoch',
    PRIMARY KEY (`memberGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE IF NOT EXISTS `club_member_mention` (
    `clubId`          BIGINT UNSIGNED NOT NULL,
    `streamId`        BIGINT UNSIGNED NOT NULL,
    `memberGuid`      BIGINT UNSIGNED NOT NULL COMMENT 'the mentioned member',
    `epoch`           BIGINT UNSIGNED NOT NULL,
    `position`        BIGINT UNSIGNED NOT NULL,
    `authorGuid`      BIGINT UNSIGNED NOT NULL,
    `authorAccountId` INT UNSIGNED    NOT NULL DEFAULT 0,
    `createdTime`     BIGINT UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (`memberGuid`, `epoch`, `position`),
    KEY `idx_createdTime` (`createdTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- ---------------------------------------------------------------------------
-- Migration (only if the Haven tables exist).
-- ---------------------------------------------------------------------------
SET @schema_name := DATABASE();

SET @has_old_messages := (
    SELECT COUNT(*) FROM information_schema.TABLES
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_club_message'
);
SET @has_old_tombstones := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_club_message' AND COLUMN_NAME = 'destroyTime'
);

-- position = running number per (guildId, streamId) in (epoch, id) order.
-- User variables instead of ROW_NUMBER() so this also runs on MySQL 5.7.
-- authorAccountId = the author's game account (characters.account); 0 if the
-- character no longer exists.
SET @destroyer_expr := IF(@has_old_tombstones > 0, 'm.destroyerGuid', '0');
SET @destroy_expr   := IF(@has_old_tombstones > 0, 'm.destroyTime', '0');
SET @sql := IF(@has_old_messages > 0, CONCAT(
    'INSERT IGNORE INTO club_message ',
    '(clubId, streamId, epoch, position, authorAccountId, authorGuid, content, createdTime, destroyerGuid, destroyTime) ',
    'SELECT o.guildId, o.streamId, o.epoch, o.position, COALESCE(c.account, 0), o.authorGuid, o.content, ',
    'o.epoch DIV 1000000, o.destroyerGuid, o.destroyTime ',
    'FROM (',
        'SELECT m.guildId, m.streamId, m.epoch, m.authorGuid, COALESCE(m.content, '''') AS content, ',
        @destroyer_expr, ' AS destroyerGuid, ', @destroy_expr, ' AS destroyTime, ',
        '@pos := IF(@grp = CONCAT(m.guildId, '':'', m.streamId), @pos + 1, 1) AS position, ',
        '@grp := CONCAT(m.guildId, '':'', m.streamId) AS grp ',
        'FROM guild_club_message m CROSS JOIN (SELECT @pos := 0, @grp := '''') vars ',
        'ORDER BY m.guildId, m.streamId, m.epoch, m.id',
    ') o LEFT JOIN characters c ON c.guid = o.authorGuid'),
    'SELECT 1');
PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

SET @has_old_views := (
    SELECT COUNT(*) FROM information_schema.TABLES
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_club_stream_view'
);
SET @sql := IF(@has_old_views > 0,
    'INSERT IGNORE INTO club_stream_view_marker (clubId, streamId, memberGuid, lastViewTime) SELECT guildId, streamId, memberGuid, lastReadTime FROM guild_club_stream_view',
    'SELECT 1');
PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- ---------------------------------------------------------------------------
-- 3. Guild news: serialized ItemInstance column (item news events).
--    MySQL has no ADD COLUMN IF NOT EXISTS (MariaDB only): check the schema.
-- ---------------------------------------------------------------------------
SET @schema_name := DATABASE();
SET @has_news_data := (
    SELECT COUNT(*) FROM information_schema.COLUMNS
    WHERE TABLE_SCHEMA = @schema_name AND TABLE_NAME = 'guild_newslog' AND COLUMN_NAME = 'Data'
);
SET @sql := IF(@has_news_data = 0,
    'ALTER TABLE `guild_newslog` ADD COLUMN `Data` TEXT NOT NULL AFTER `TimeStamp`',
    'SELECT 1');
PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- ---------------------------------------------------------------------------
-- 4. Guild news IDs match retail: never 0, unique, never reused.
--    The core now assigns 1, 2, 3, ... per guild. Guilds that still have an ID 0
--    row get all their IDs shifted up by one, in two steps so the
--    (guildid, LogGuid) primary key never collides. A temporary table holds the
--    affected guilds (MySQL can reject updating a table joined to itself).
--    Idempotent: once no guild has an ID 0 row, nothing changes.
-- ---------------------------------------------------------------------------
DROP TEMPORARY TABLE IF EXISTS `tmp_guild_news_zero_id`;
CREATE TEMPORARY TABLE `tmp_guild_news_zero_id` AS
    SELECT DISTINCT `guildid` FROM `guild_newslog` WHERE `LogGuid` = 0;

UPDATE `guild_newslog` n
JOIN `tmp_guild_news_zero_id` z ON z.`guildid` = n.`guildid`
SET n.`LogGuid` = n.`LogGuid` + 1000001;

UPDATE `guild_newslog`
SET `LogGuid` = `LogGuid` - 1000000
WHERE `LogGuid` >= 1000001;

DROP TEMPORARY TABLE IF EXISTS `tmp_guild_news_zero_id`;

-- HavenCore: Guild News Flags carry only the sticky bit (retail: 0 or 1).
-- (Follow-up to 2026_09_22_03_guild_overhaul.sql, which does not include this step.)
--
-- Achievement news used to store the achievement's SHOW_IN_GUILD_HEADER flag
-- (0x2000) in guild_newslog.Flags. The 8.3.7 client's news sort only handles the
-- sticky bit, so those rows made the order change on every reopen and pushed
-- unpinned entries into the pinned section. Keep the sticky bit, drop the rest.
-- Idempotent: rows that already hold only 0/1 are not touched.
 
UPDATE `guild_newslog` SET `Flags` = `Flags` & 1 WHERE `Flags` > 1;
