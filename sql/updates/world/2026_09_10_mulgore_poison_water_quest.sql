-- Quest Poison Water (20440) - missing creature quest hints
DELETE FROM creature_questitem WHERE CreatureEntry IN(2956,2957,3068);
INSERT INTO creature_questitem
(CreatureEntry, Idx, ItemId, VerifiedBuild)
VALUES
(2956, 0, 4759, 35662),
(2956, 1, 4806, 35662),
(2956, 2, 33009, 35662),
(2957, 0, 4759, 35662),
(2957, 1, 4806, 35662),
(2957, 2, 33009, 35662),
(3068, 0, 4759, 35662),
(3068, 1, 4806, 35662);
