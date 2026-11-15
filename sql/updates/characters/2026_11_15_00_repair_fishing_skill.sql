-- Characters who learned Fishing (spell 7620) before the skill-step effect was
-- fixed never received the fishing skill line. Repair them with apprentice
-- fishing (1/75) on both the classic parent line and the BfA child line.
INSERT INTO character_skills (guid, skill, value, max)
SELECT cs.guid, 356, 1, 75
FROM character_spell cs
WHERE cs.spell = 7620
  AND NOT EXISTS (SELECT 1 FROM character_skills s WHERE s.guid = cs.guid AND s.skill = 356);

INSERT INTO character_skills (guid, skill, value, max)
SELECT cs.guid, 2592, 1, 75
FROM character_spell cs
WHERE cs.spell = 7620
  AND NOT EXISTS (SELECT 1 FROM character_skills s WHERE s.guid = cs.guid AND s.skill = 2592);
