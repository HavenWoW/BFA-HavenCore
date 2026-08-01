-- Grasping Tendrils, the 1+ corruption drawback.
--
-- 315175 procs on damage taken and triggers the 5s snare 315176. SpellEffect.db2 gives
-- 315176 a magnitude of 0 - retail computed it server-side - so the aura landed at
-- "movement speed reduced by 0%". spell_corruption_grasping_tendrils supplies it.
DELETE FROM `spell_script_names` WHERE `spell_id` = 315176 AND `ScriptName` = 'spell_corruption_grasping_tendrils';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(315176, 'spell_corruption_grasping_tendrils');

-- 315175 ships every TAKEN proc flag (0x800AAAA8), three of which are not damage -
-- including PROC_FLAG_TAKEN_PERIODIC, which SpellMgr.h documents as "damage / healing".
-- With no spell_proc row the auto-generated one takes PROC_SPELL_TYPE_MASK_ALL, so
-- standing in a druid's Efflorescence rooted the player once per healing tick.
-- AttributesMask restores PROC_ATTR_TRIGGERED_CAN_PROC, which the generator sets for a
-- TAKEN proc-trigger aura and a hand-written row would otherwise drop. Every other column
-- stays 0 so ProcFlags, Chance, Cooldown and Charges keep their DB2 values.
DELETE FROM `spell_proc` WHERE `SpellId` = 315175;
INSERT INTO `spell_proc` (`SpellId`, `SpellTypeMask`, `AttributesMask`) VALUES
(315175, 1, 2); -- PROC_SPELL_TYPE_DAMAGE, PROC_ATTR_TRIGGERED_CAN_PROC
