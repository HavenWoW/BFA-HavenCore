-- Apply the same fix as Goram (46572) to the remaining city guild vendors:
-- Thunder Bluff, Undercity, Silvermoon, Ironforge, Darnassus and Exodar.
-- They only had the vendor flag, and all items were gated by guild reputation and
-- guild achievements that a newly created guild cannot meet.
UPDATE creature_template SET npcflag = npcflag | 1 WHERE entry IN (51495, 51496, 51501, 51502, 51503, 51504);

DELETE FROM conditions
WHERE SourceTypeOrReferenceId = 23
  AND SourceGroup IN (51495, 51496, 51501, 51502, 51503, 51504);
