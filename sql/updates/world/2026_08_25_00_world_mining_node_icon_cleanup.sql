-- BFA-HavenCore mining node icon metadata cleanup
-- Follow-up to PR #180 (General Mining / Gathering Repair)
--
-- Affected mining gathering nodes use IconName = 'Mining', which causes
-- the BFA client to display a black-box cursor/icon when hovering the node.
--
-- Clearing IconName restores the correct mining cursor.
-- No loot, lock, skill-up, respawn, or core behavior is changed.
--
-- IMPORTANT:
-- After applying this update, restart the worldserver and clear the client
-- Cache directory before testing, as GameObject query data may be cached.

UPDATE `gameobject_template`
SET `IconName` = ''
WHERE `entry` IN (
    181555, -- Fel Iron Deposit
    181556, -- Adamantite Deposit
    181557, -- Khorium Vein
    181569, -- Rich Adamantite Deposit
    181570, -- Rich Adamantite Deposit
    185877, -- Nethercite Deposit
    202738, -- Elementium Vein
    202741, -- Rich Elementium Vein
    228563, -- Blackrock Deposit
    241726, -- Leystone Deposit
    241743, -- Felslate Deposit
    245325, -- Rich Felslate Deposit
    272768, -- Empereits Reserves
    272778, -- Rich Empyrium Deposit
    276616, -- Monelite Deposit
    276617, -- Storm Silver Deposit
    276618, -- Platinum Deposit
    276621, -- Rich Monelite Deposit
    276622, -- Rich Storm Silver Deposit
    325875  -- Osmenite Deposit
);
