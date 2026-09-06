UPDATE `creature_template`
SET `unit_flags` = `unit_flags` - 67108864
WHERE (`unit_flags` & 67108864) = 67108864;