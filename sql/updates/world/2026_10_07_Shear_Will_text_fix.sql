-- Quest text for completeing the quest added per wowhead.com 
-- https://www.wowhead.com/quest=14249/shear-will
UPDATE quest_offer_reward SET RewardText = 'Ha ha! Perfect! Let me toss this together really fast. The Archmage is sure to be taken in by my style!' WHERE ID = 14249;
INSERT IGNORE INTO quest_request_items (ID, EmoteOnComplete, EmoteOnIncomplete, EmoteOnCompleteDelay, EmoteOnIncompleteDelay, CompletionText, VerifiedBuild) VALUES (14249, 1, 0, 0, 0, 'My robes looking pretty good. Did you bring those feathers?', 20574);
