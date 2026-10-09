# Corruption loot and MOTHER item interactions

The loot branch loads ItemBonusListGroupEntry and preserves the twelve original
Ny'alotha weapon bonus mappings. Random rolls default to zero. The BFA expansion,
equipment class and slot checks only narrow candidates; they do not establish
Season 4 source eligibility or retail probabilities. Keep the loot PR a draft
until those questions and drops in game are validated.

The stacked MOTHER branch implements contaminant effect 223, item ownership and
target checks, purification, UiItemInteraction storage and packets, and gossip
on existing MOTHER creatures. Group 158 membership and corruption stat bonuses
identify removable corruption; unrelated bonus-granted effects are preserved.
Normal vendor/event purchase paths remain in use. The automatic world update
binds existing templates and quest enders without adding creatures.

The item interaction packet layout has no recorded 8.3.7 sniff in the original
contribution. Keep this PR a draft pending client packet validation, successful
purification on equipped and bagged items, currency charging and persistence
checks, and contaminant use/consumption checks. Also test rejected targets,
traded items, quest gating, stale NPC interaction tokens and unrelated effects.

The full original eight-window, 52-item vendor rotation is retained in
`docs/sql/examples/mother-contaminant-rotation.sql.example`, outside the updater.
A maintainer must first verify an existing MOTHER spawn, allocate eight distinct
unused event IDs, check both event tables and validate reset timezone and costs.
Its NULL parameters prevent accidental installation with local identifiers.

Excluded local behavior remains in the contributor's integration branch:
HavenLab notifications and bonus dumps; QA all-rotation vendor mode and its
purchase override; hardcoded spawn GUID and guessed floor coordinates; global
event deletion; Chinese names forced into every locale; local broadcast text,
hotfix record IDs and hardcoded shelf-name tables. Locale text should use normal
world/hotfix locale records in a separate verified contribution.

Sources: original commits f27735ab1ef9985b314ff53c71ddcbd0381c25b7,
6558a0665baffcacd12bd62aea7c20bedffb580e and
5e0a9c54f649e6f95c83d4394589323b8e4f90d1. The third commit's global locale
mutation and local hotfix record updates are intentionally excluded.
