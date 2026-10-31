-- Goram (entry 46572) is the Orgrimmar guild vendor. He only had the vendor flag,
-- and every item was gated by guild reputation/achievement conditions that a newly
-- created guild cannot meet, so the vendor showed no usable items.
UPDATE creature_template SET npcflag = npcflag | 1 WHERE entry = 46572;

DELETE FROM conditions WHERE SourceTypeOrReferenceId = 23 AND SourceGroup = 46572;
