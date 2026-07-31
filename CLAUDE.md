# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

BFA-HavenCore — a TrinityCore-derived World of Warcraft server emulator targeting client **8.3.7 (build 35662)**. C++17, CMake ≥ 3.27. Two server binaries: `worldserver` (game) and `bnetserver` (Battle.net login). Open source under GPL-3.0 and community-maintained: changes arrive as pull requests from many contributors, so a change has to be legible to someone who was not in the room when it was written.

**This file is the repository-wide rule file.** It is tracked, it applies to every branch and every contributor, and it is **not** one of the AI planning artefacts covered by the hygiene rule below — never delete or gitignore it when cleaning a branch for merge.

### Current branch context

`feature/corruption-system` is developed in a separate clone of the community repo. The planning artefacts for that work — `docs/superpowers/` and `.superpowers/` — are clone-local and are deleted before the branch merges back; the community repo carries no AI planning docs. Design of record: `docs/superpowers/specs/2026-07-30-bfa-corruption-system-design.md`, with plans in `docs/superpowers/plans/`. Task ledgers live in `.superpowers/sdd/<plan-slug>/progress.md` — read the ledger before continuing an in-flight plan; it records deferred items and controller rulings that the commits do not.

## Build

Windows / Visual Studio 2022 is the primary toolchain. The configured tree is `./build` (VS 17 2022, x64, `TOOLS=0`).

```powershell
# configure (only needed after CMakeLists/option changes)
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DTOOLS=0

# build one target — worldserver is the usual one
cmake --build build --config RelWithDebInfo --target worldserver

# everything
cmake --build build --config RelWithDebInfo
```

Targets: `worldserver`, `bnetserver`, `game`, `scripts`, `shared`, `common`, `database`, `proto`. Binaries land in `build/bin/RelWithDebInfo/`.

Useful options (`cmake/options.cmake`): `SCRIPTS` (`static` default / `dynamic` / `minimal-*` / `none`), `TOOLS` (map/vmap/mmap extractors, off here), `WITH_WARNINGS`, `WITH_COREDEBUG`, `NOPCH` (kills both PCHs). In-source builds are blocked by `CMAKE_DISABLE_IN_SOURCE_BUILD`.

Third-party deps are vendored under `dep/` (boost stub, CascLib, protobuf, recastnavigation, fmt, jemalloc, …). Host prerequisites: Boost 1.81, MySQL ≥ 8.0, OpenSSL 3.x.

Linux builds go through Docker; `docker/README.md` is the authoritative guide (`docker compose build`, `docker compose run --rm db-import`, `docker compose up -d bnetserver worldserver`). For an iterative C++ loop use the `dev-builder` override documented in `docker-compose.dev.yml`.

## Tests

**There is no test framework in this repository.** Every verification is either a build check or a stated manual in-game check. Do not claim a change is verified on the strength of a clean compile alone — say which of the two you actually ran, and if an in-game check is required and was not performed, say so explicitly.

## Architecture

### Layering
`src/common` (platform, logging, config, crypto, collision, threading) → `src/server/database` (MySQL pools) + `src/server/shared` (networking, realm, packet plumbing) + `src/server/proto` (Battle.net protobuf services) → `src/server/game` (all gameplay) → `src/server/scripts` (content scripts) → `src/server/{worldserver,bnetserver}` (entry points). `src/tools` holds the client-data extractors.

### Four databases
`auth` (login), `characters`, `world`, `hotfixes` — connection strings in `worldserver.conf` (`LoginDatabaseInfo`, `WorldDatabaseInfo`, …). All queries go through per-database prepared-statement enums in `src/server/database/Database/Implementation/{Login,Character,World,Hotfix}Database.h` (`CHAR_SEL_*`, `CHAR_UPD_*`, … terminated by `MAX_*DATABASE_STATEMENTS`), used as `CharacterDatabase.GetPreparedStatement(CHAR_UPD_ITEM_INSTANCE)`. Adding a query means adding the enum member *and* its `PrepareStatement` line in the matching `.cpp`. Async results come back via `QueryCallback` / query holders — never block a map thread on a query.

Migrations live in `sql/updates/db_world/YYYY_MM_DD_NN[_description].sql` and are applied at startup by `DBUpdater`/`UpdateFetcher` according to the `Updates.EnableDatabases` bitmask. Base dumps (`sql/base/*.sql`) are gitignored and not part of the repo.

### Client data (DB2)
`src/server/game/DataStores/` mirrors the client's `.db2` files: `DB2Stores.cpp` (store declarations + load order), `DB2Structure.h` (row structs), `DB2LoadInfo.h` / `DB2Metadata.h` (field layouts that **must** match the client build), `DBCEnums.h` (constants and ID enums). Touching one of these usually means touching all of them. Row data is read from `DataDir` (`.\ClientData`), extracted from the 8.3.7 client — treat the DB2s as ground truth and never hardcode a value you could read from them.

### Opcodes and packets
`src/server/game/Server/Protocol/Opcodes.cpp` maps every opcode via `DEFINE_HANDLER(opcode, status, processing, &WorldSession::Handler)`. Two fields carry real weight: `STATUS_*` gates the session state a packet is legal in, and `PROCESS_*` decides *where* the handler runs — `PROCESS_THREADSAFE`, `PROCESS_THREADUNSAFE` (runs inside the map update, may touch map objects), `PROCESS_INPLACE`. Choosing the wrong `PROCESS_*` is a data race, not a style issue. Wire structs live in `src/server/game/Server/Packets/*Packets.h`.

### Map threading
`MapManager::Update` hands each `Map` to the `MapUpdater` thread pool (`MapUpdate.Threads`), waits for all of them, then runs `DelayedUpdate` per map. Consequence: **two maps update concurrently**, so code running on a map thread must never reach into objects belonging to another map. For deferred work on a unit use `Unit::AddDelayedEvent(timeout, fn)` (`src/server/game/Entities/Unit/Unit.h:1768`) or `UnitAI::AddDelayedEvent` (`src/server/game/AI/CoreAI/UnitAI.h:314`), or the unit's `EventProcessor m_Events`. There is no `Map::AddDelayedEvent` in this codebase.

### Scripts
Scripts are statically linked by default. Each module directory `src/server/scripts/<Module>/` owns a `<module>_script_loader.cpp` that declares and calls the `AddSC_<file>()` function of every script file in it — **both** the forward declaration and the call are required, and a new file that misses either compiles fine and silently never registers. CMake generates the top-level `ScriptLoader.cpp` from `ScriptLoader.cpp.in.cmake`; the per-module hook is `Add<Module>Scripts()`. Registration and every gameplay hook flow through `src/server/game/Scripting/ScriptMgr.{h,cpp}`. Project-specific additions go in `src/server/scripts/Custom/`.

### Item bonuses (context for the current branch)
`BonusData` (`src/server/game/Entities/Item/Item.h:74`) is the per-instance resolved bonus state for an item. `Item::GetEffects()` (`Item.h:200`) returns template effects **plus** bonus-granted ones (`ITEM_BONUS_ITEM_EFFECT_ID`); `ItemTemplate::Effects` returns only the static list. The migration rule from the Layer 1 plan still holds: a call site uses `item->GetEffects()` **if and only if it holds an `Item*`**; sites holding only an `ItemTemplate const*` keep `proto->Effects` and carry a comment saying why. Effect order is load-bearing — spell-charge slots are keyed to effect index, capped at `MAX_ITEM_SPELLS`, and `Item::Get/SetSpellCharges` index a 5-wide update field **without bounds checking**.

---

# Development rules

These are mandatory for every contributor on every branch, and they override any default behaviour an AI agent would otherwise fall back on.

## 1. Human verification checkpoints (anti-slop gate)

Treat development as an interactive pipeline. Stop and wait for explicit human approval between phases:

1. **Analysis** — present the logical plan and pseudo-architecture. **STOP and wait for approval.**
2. **Drafting** — present the targeted diff / code blocks. **STOP and wait for approval.**
3. **Compilation** — run the build verification. **STOP and report results.**

**No assumptions.** If a packet structure, database layout, or function signature is ambiguous, do not guess and do not generate speculative code. Stop and ask.

## 2. Code-quality guardrails

- **Zero placeholders.** No `// TODO`, no `...`, no stub comments. Implement it fully or don't write it.
- **Delete dead code.** Remove replaced or deprecated blocks outright; never comment them out.
- **Surgical scope.** Touch only the files and lines the task calls for. No drive-by formatting, refactors, or style cleanups.
- **"Why" comments only.** Never comment what the C++ does. Comment only why a path is required — an 8.3.7 packet sniff, a client quirk, a retail behaviour being matched.
- **No absolute paths.** Every path — in docs, comments, commit messages, scripts, CMake, and config examples — is relative to the repository root (`src/server/game/...`, `sql/updates/db_world/...`, `build/bin/...`). A machine-local path like `F:\WorkDir\...` or `/home/you/...` is meaningless to the next contributor. Where a real local path is unavoidable, name it as a placeholder (`<repo-root>`, `<client-install>`) or read it from config.

## 3. TrinityCore & C++ conventions

- **No magic numbers.** Spell, item, faction, and opcode IDs come from existing enums, DB/DB2 lookups, or a named constant you declare. Never inline a bare ID.
- **Thread safety.** Respect the multi-threaded map architecture described above. Never mutate world entities or grid data across maps from a map thread; use a thread-safe hook or a delayed event on the owning unit.
- **Memory.** Use `std::unique_ptr` / `std::shared_ptr` and `ObjectGuid` rather than raw pointer ownership; a raw pointer is a borrow, never a hand-off.
- **Database-backed scripts.** World/creature behaviour belongs in SmartAI data or an explicit C++ hook. Never embed ad-hoc inline SQL in a script.

## 4. Commit conventions

Real commit messages: an imperative subject and, when the change needs justification, a wrapped body explaining **why**. Commit at the end of a task, never mid-task.

Subject form used on this branch is `area: imperative summary`, area lowercase and slash-scoped:

```
core/items: fix spell charge serialization for bonus-granted effects
core/spells: read charges from the item's effect set, not its template
build(cmake): discover MySQL installs by glob instead of a hardcoded version list
docs: correct Task 5 prose to match code reality
```

- Areas in use: `core`, `core/<System>`, `build(cmake)`, `docs`, `spec`, `plan`, `db/<area>`. Older history also carries `Core/Grids:` and `DB/Quest:` capitalised forms — match the lowercase form for new work.
- Subject in the imperative mood, no trailing period, ~72 chars.
- Body explains the reasoning, references the spec/plan section or the issue, and states what was verified. Wrap at 72.
- Cherry-picks from other projects **must** use `--author` to credit the original author.
- The PR template points at the AzerothCore commit-message guidelines; `pull_request_template.md` must be filled in honestly — including the source-of-truth boxes (sniffs / live research / video evidence) and the tests-performed boxes.

## 5. AI-assisted contributions

- **Superpowers is required.** Any contributor using an AI coding agent on this repository must have the Superpowers skill set installed and must actually use its workflows (`brainstorming` before design, `writing-plans` before implementation, `test-driven-development` / `systematic-debugging` during, `verification-before-completion` before any completion claim, `requesting-code-review` before merge). If it is not installed:

  ```
  /plugin marketplace add anthropics/claude-plugins-official
  /plugin install superpowers@claude-plugins-official
  ```

  Confirm it loaded before starting work — the session should surface the `superpowers:*` skills.
- **Disclose it.** The `### AI-assisted Pull Requests` section of the PR template is not optional: check the box and name the tools and models used.
- **Own it.** You must understand every line you submit and be able to justify it to maintainers on request. Do not open a PR you cannot defend without the agent.
