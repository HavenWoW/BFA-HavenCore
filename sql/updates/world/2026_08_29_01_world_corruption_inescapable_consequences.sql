-- 837 blizzlike: Inescapable Consequences (337612) has PERIODIC_TRIGGER_SPELL
-- with TriggerSpell 0. Combat ticks cast 337816 (DAMAGE_FROM_MAX_HEALTH_PCT
-- from SpellEffect). Do not hardcode the percent.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_inescapable_consequences');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(337612, 'spell_inescapable_consequences');
