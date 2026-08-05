-- The Eye spawned on any positive utility cast - learning a mount was the report.
--
-- 315169 had no `spell_proc` row, so the generated entry took PROC_SPELL_TYPE_MASK_ALL and the
-- type filter in CanSpellTriggerProcOnEvent was skipped. Mask 3 keeps damage and heals and drops
-- PROC_SPELL_TYPE_NO_DMG_HEAL, the bucket Spell.cpp assigns to a cast that neither heals nor
-- damages.
--
-- Unlike 315175 and 315184 this row sets SpellPhaseMask and no AttributesMask, because 315169's
-- DB2 proc flags are all DONE: the load path never defaults the phase mask, and the generator's
-- addTriggerFlag is gated on TAKEN_HIT_PROC_FLAG_MASK, so there is no PROC_ATTR_TRIGGERED_CAN_PROC
-- to lose. The phase has to stay HIT-only - Spell::finish raises a second proc at
-- PROC_SPELL_PHASE_CAST passing MASK_ALL as the event's own type mask, which no SpellTypeMask can
-- reject.
DELETE FROM `spell_proc` WHERE `SpellId` = 315169;
INSERT INTO `spell_proc` (`SpellId`, `SpellTypeMask`, `SpellPhaseMask`) VALUES
(315169, 3, 2); -- PROC_SPELL_TYPE_DAMAGE | PROC_SPELL_TYPE_HEAL, PROC_SPELL_PHASE_HIT
