-- EN: Fix quest 12619 "The Emblazoned Runeblade" (Death Knight starter quest) not
-- teaching the player spell 53428 "Runeforging" upon completion. The quest's
-- RewardSpell field was 0, so the core never cast the reward spell on turn-in,
-- leaving the player without the Runeforging skill. RewardDisplaySpell1 (53431)
-- only affects the client-side quest log preview and does not actually grant the
-- spell, so it is cleared to avoid confusion.
--
-- ES: Corrige que la misión 12619 "The Emblazoned Runeblade" (misión inicial del
-- Caballero de la Muerte) no enseñara al jugador el hechizo 53428 "Forja de runas"
-- al completarla. El campo RewardSpell estaba en 0, por lo que el core nunca
-- lanzaba el hechizo de recompensa al entregar la misión, dejando al jugador sin
-- la habilidad de Forja de runas. RewardDisplaySpell1 (53431) solo afecta a la
-- vista previa del registro de misiones en el cliente y no otorga realmente el
-- hechizo, por lo que se limpia para evitar confusión.

UPDATE quest_template 
SET RewardSpell = 53431, 
    RewardDisplaySpell1 = 53431
WHERE ID = 12619;