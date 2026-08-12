-- William Pestle (253) - Now says the 2 lines ONLY after turn-in of Collecting Kelp (112) quest
UPDATE smart_scripts
SET event_param1 = 112
WHERE entryorguid = 253
  AND source_type = 0
  AND id = 0
  AND event_type = 20;