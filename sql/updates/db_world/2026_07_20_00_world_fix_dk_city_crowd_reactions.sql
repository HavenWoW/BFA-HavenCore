-- Death Knight introduction: fix city crowd reactions
--
-- The affected SmartAI OOC LOS events were missing quest conditions.
-- Without these conditions, normal players and nearby creatures could invoke
-- Death Knight-specific crowd actions such as rotten fruit throws, spitting,
-- waving, and hostile dialogue.
--
-- The reactions are intended to run while the appropriate Death Knight
-- introduction quest is complete but not yet rewarded:
--
-- Alliance: 13188 - Where Kings Walk
-- Horde:    13189 - Saurfang's Blessing
--
-- SourceTypeOrReferenceId 22 = SmartAI event condition
-- ConditionTypeOrReference 28 = Quest complete
-- ConditionTarget 0 = SmartAI event invoker
--
-- SmartAI condition SourceGroup is the SmartAI event ID plus one.

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 22
  AND `SourceId` = 0
  AND
  (
      (`SourceGroup` = 24 AND `SourceEntry` = 68)    OR
      (`SourceGroup` = 2  AND `SourceEntry` = 1432)  OR
      (`SourceGroup` = 24 AND `SourceEntry` = 1756)  OR
      (`SourceGroup` = 24 AND `SourceEntry` = 1976)  OR
      (`SourceGroup` = 24 AND `SourceEntry` = 3296)  OR
      (`SourceGroup` = 1  AND `SourceEntry` = 4047)  OR
      (`SourceGroup` = 2  AND `SourceEntry` = 6174)  OR
      (`SourceGroup` = 24 AND `SourceEntry` = 12480) OR
      (`SourceGroup` = 1  AND `SourceEntry` = 45337) OR
      (`SourceGroup` = 24 AND `SourceEntry` = 74228)
  );

INSERT INTO `conditions`
(
    `SourceTypeOrReferenceId`,
    `SourceGroup`,
    `SourceEntry`,
    `SourceId`,
    `ElseGroup`,
    `ConditionTypeOrReference`,
    `ConditionTarget`,
    `ConditionValue1`,
    `ConditionValue2`,
    `ConditionValue3`,
    `NegativeCondition`,
    `ErrorType`,
    `ErrorTextId`,
    `ScriptName`,
    `Comment`
)
VALUES
-- Stormwind: quest 13188 - Where Kings Walk
(22, 24, 68, 0, 0, 28, 0, 13188, 0, 0, 0, 0, 0, '',
 'Stormwind City Guard - Run Death Knight crowd reaction when Where Kings Walk is complete'),

(22, 2, 1432, 0, 0, 28, 0, 13188, 0, 0, 0, 0, 0, '',
 'Renato Gallina - Run Death Knight crowd reaction when Where Kings Walk is complete'),

(22, 24, 1756, 0, 0, 28, 0, 13188, 0, 0, 0, 0, 0, '',
 'Stormwind Royal Guard - Run Death Knight crowd reaction when Where Kings Walk is complete'),

(22, 24, 1976, 0, 0, 28, 0, 13188, 0, 0, 0, 0, 0, '',
 'Stormwind City Patroller - Run Death Knight crowd reaction when Where Kings Walk is complete'),

(22, 2, 6174, 0, 0, 28, 0, 13188, 0, 0, 0, 0, 0, '',
 'Stephanie Turner - Run Death Knight crowd reaction when Where Kings Walk is complete'),

(22, 24, 12480, 0, 0, 28, 0, 13188, 0, 0, 0, 0, 0, '',
 'Melris Malagan - Run Death Knight crowd reaction when Where Kings Walk is complete'),

-- Orgrimmar: quest 13189 - Saurfang's Blessing
(22, 24, 3296, 0, 0, 28, 0, 13189, 0, 0, 0, 0, 0, '',
 'Orgrimmar Grunt - Run Death Knight crowd reaction when Saurfang''s Blessing is complete'),

(22, 1, 4047, 0, 0, 28, 0, 13189, 0, 0, 0, 0, 0, '',
 'Zor Lonetree - Run Death Knight crowd reaction when Saurfang''s Blessing is complete'),

(22, 1, 45337, 0, 0, 28, 0, 13189, 0, 0, 0, 0, 0, '',
 'Tyelis - Run Death Knight crowd reaction when Saurfang''s Blessing is complete'),

(22, 24, 74228, 0, 0, 28, 0, 13189, 0, 0, 0, 0, 0, '',
 'Darkspear Headhunter - Run Death Knight crowd reaction when Saurfang''s Blessing is complete');