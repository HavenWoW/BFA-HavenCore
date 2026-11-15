-- Silvermoon Guardian (entry 16221, name "银月城卫兵") uses gossip menu 8185.
-- Its "Profession Trainer" option opened empty menu 15734 and pointed at
-- POI 10496 (Falconwing Square), which is far away from the guards patrolling
-- outside Silvermoon. Open the Silvermoon profession list (menu 7667) instead;
-- each profession then sends its own, correct Silvermoon POI.
UPDATE gossip_menu_option_action
SET ActionMenuId = 7667, ActionPoiId = 0
WHERE MenuId = 8185 AND OptionIndex = 5;
