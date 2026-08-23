-- Cleanup on Isle E. was removed before Cataclysm and is not part of BFA progression.
DELETE FROM `creature_queststarter`
WHERE `id` = 3188
  AND `quest` = 26951;

DELETE FROM `creature_questender`
WHERE `id` = 3188
  AND `quest` = 26951;
