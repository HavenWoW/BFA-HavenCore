-- Eversong Woods (3430) and Ghostlands (3433) had no entry in
-- `fishing_loot_template`, so the server fell back to entry 1 whose rolls can
-- all fail and leave the loot window empty. Add low-level fishing loot with a
-- guaranteed catch.
DELETE FROM fishing_loot_template WHERE Entry IN (3430, 3433);

INSERT INTO fishing_loot_template
    (Entry, Item, Reference, Chance, QuestRequired, LootMode, GroupId, MinCount, MaxCount, Comment)
VALUES
    (3430, 6291, 0, 100, 0, 1, 1, 1, 1, 'Raw Brilliant Smallfish'),
    (3430, 6289, 0, 25,  0, 1, 0, 1, 1, 'Raw Longjaw Mud Snapper'),
    (3430, 6303, 0, 3,   0, 1, 0, 1, 1, 'Raw Slitherskin Mackerel'),
    (3433, 6291, 0, 100, 0, 1, 1, 1, 1, 'Raw Brilliant Smallfish'),
    (3433, 6289, 0, 40,  0, 1, 0, 1, 1, 'Raw Longjaw Mud Snapper'),
    (3433, 6303, 0, 5,   0, 1, 0, 1, 1, 'Raw Slitherskin Mackerel');
