-- Grand Delusions, the 40+ corruption tier.
--
-- Container 315184 procs and triggers 315186, whose only effect summons creature 161895,
-- the Thing From Beyond. Everything up to the summon is client data and already worked;
-- the creature is the part with no behaviour attached to it.
--
-- AIName has to be cleared, not merely joined by a ScriptName: ObjectMgr keeps AIName when
-- a template carries both and silently drops the ScriptName. 161895 is marked SmartAI with
-- no smart_scripts rows of its own, which is exactly how it was inert to begin with.
UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_corruption_thing_from_beyond'
WHERE `entry` = 161895;

-- Same reasoning as 315175 in 2026_08_02_00: 315184's DB2 proc flags include the TAKEN
-- flags that are not damage, so without this row the player's own heals summon Things as
-- readily as being hit does. Both containers share SpellProcsPerMinuteID 86, so only the
-- spell type needs restricting.
DELETE FROM `spell_proc` WHERE `SpellId` = 315184;
INSERT INTO `spell_proc` (`SpellId`, `SpellTypeMask`, `AttributesMask`) VALUES
(315184, 1, 2); -- PROC_SPELL_TYPE_DAMAGE, PROC_ATTR_TRIGGERED_CAN_PROC
