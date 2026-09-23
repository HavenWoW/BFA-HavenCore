/*
 * 2026 BFA-HavenCore
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// -----------------------------------------------------------------------------
// Guild recipe opcodes (CMSG_GUILD_QUERY_RECIPES, CMSG_GUILD_QUERY_MEMBERS_FOR_
// RECIPE, CMSG_GUILD_QUERY_MEMBER_RECIPES). Moved unchanged from
// ClubFinderHandler; mask rules live in Guilds/GuildRecipes.
// -----------------------------------------------------------------------------

#include "WorldSession.h"
#include "CharacterCache.h"
#include "DatabaseEnv.h"
#include "DB2Stores.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "World.h"
#include <algorithm>
#include <array>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "GuildRecipes.h"
#include "SpellMgr.h"

using namespace GuildRecipes;
void WorldSession::HandleGuildQueryRecipes(WorldPacket& recvData)
{
    ObjectGuid guildGuid;
    recvData >> guildGuid;
    if (recvData.rpos() < recvData.size())
        recvData.rfinish();

    Player* player = GetPlayer();
    Guild* guild = player ? player->GetGuild() : nullptr;
    if (!player || !guild || guildGuid != guild->GetGUID())
    {
        return;
    }

    // Union of every recipe known by any member: saved spells, skill-granted
    // (dependent, never saved) spells re-derived from skills, and live spells of
    // online members (covers recipes learned this session).
    std::unordered_set<uint32> knownSpells;
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER_SPELLS);
    stmt->setUInt64(0, guild->GetId());
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
    {
        do
        {
            knownSpells.insert(result->Fetch()[1].GetUInt32());
        }
        while (result->NextRow());
    }

    CharacterDatabasePreparedStatement* skillStmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER_SKILLS);
    skillStmt->setUInt64(0, guild->GetId());
    if (PreparedQueryResult result = CharacterDatabase.Query(skillStmt))
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 skillId = fields[1].GetUInt16();
            if (!Guild::GetRootProfessionSkillLine(skillId))
                continue;

            ObjectGuid memberGuid = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt64());
            if (CharacterCacheEntry const* cache = sCharacterCache->GetCharacterCacheByGuid(memberGuid))
                Guild::AppendSkillGrantedSpells(skillId, fields[2].GetUInt16(), cache->Race, cache->Class, cache->Level, knownSpells);
        }
        while (result->NextRow());
    }

    auto appendOnline = [&knownSpells](Player* member) { Guild::AppendLiveKnownSpells(member, knownSpells); };
    guild->BroadcastWorker(appendOnline);

    std::unordered_map<uint32, GuildRecipeMask> recipes;
    for (uint32 spellId : knownSpells)
        AddSpellToGuildRecipeMasks(spellId, recipes);

    WorldPacket response(SMSG_GUILD_KNOWN_RECIPES, 4 + recipes.size() * (4 + GUILD_RECIPE_MASK_SIZE));
    size_t countPos = response.wpos();
    response << uint32(0);
    uint32 count = 0;
    for (auto const& recipe : recipes)
    {
        bool empty = std::all_of(recipe.second.begin(), recipe.second.end(), [](uint8 value) { return value == 0; });
        if (empty)
            continue;

        response << uint32(recipe.first);
        response.append(recipe.second.data(), recipe.second.size());
        ++count;
    }
    response.put<uint32>(countPos, count);
    SendPacket(&response);
}

void WorldSession::HandleGuildQueryMembersForRecipe(WorldPacket& recvData)
{
    ObjectGuid guildGuid;
    uint32 skillLineId = 0;
    uint32 spellId = 0;
    uint32 uniqueBit = 0;
    recvData >> guildGuid >> skillLineId >> spellId >> uniqueBit;

    Player* player = GetPlayer();
    Guild* guild = player ? player->GetGuild() : nullptr;
    if (!player || !guild || guildGuid != guild->GetGUID() || !GetRootProfessionSkillLine(skillLineId))
        return;

    // UniqueBit is supplied by the client for the same recipe-mask slot; the spell
    // is authoritative. Crafters come from the same sources as the recipe masks:
    //  1) saved spells (character_spell),
    //  2) skill-granted recipes re-derived from character_skills (never saved),
    //  3) online members' live state, which overrides 1) and 2).
    (void)uniqueBit;
    std::set<ObjectGuid> crafters;
    CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBERS_WITH_SPELL);
    stmt->setUInt64(0, guild->GetId());
    stmt->setUInt32(1, spellId);
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
    {
        do
            crafters.insert(ObjectGuid::Create<HighGuid::Player>(result->Fetch()[0].GetUInt64()));
        while (result->NextRow());
    }

    SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
    if (bounds.first != bounds.second)
    {
        CharacterDatabasePreparedStatement* skillStmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER_SKILLS);
        skillStmt->setUInt64(0, guild->GetId());
        if (PreparedQueryResult result = CharacterDatabase.Query(skillStmt))
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 skillId = fields[1].GetUInt16();
                ObjectGuid memberGuid = ObjectGuid::Create<HighGuid::Player>(fields[0].GetUInt64());
                CharacterCacheEntry const* cache = nullptr;
                for (SkillLineAbilityMap::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
                {
                    if (uint32(itr->second->SkillLine) != skillId)
                        continue;

                    if (!cache && !(cache = sCharacterCache->GetCharacterCacheByGuid(memberGuid)))
                        break;

                    if (Guild::IsSkillGrantedAbility(itr->second, fields[2].GetUInt16(), cache->Race, cache->Class, cache->Level))
                    {
                        crafters.insert(memberGuid);
                        break;
                    }
                }
            }
            while (result->NextRow());
        }
    }

    auto applyOnline = [&crafters, spellId](Player* member)
    {
        if (member->HasSpell(spellId))
            crafters.insert(member->GetGUID());
        else
            crafters.erase(member->GetGUID());
    };
    guild->BroadcastWorker(applyOnline);

    std::vector<ObjectGuid> members(crafters.begin(), crafters.end());

    WorldPacket response(SMSG_GUILD_MEMBERS_WITH_RECIPE, 12 + members.size() * 16);
    response << uint32(skillLineId);
    response << uint32(spellId);
    response << uint32(members.size());
    for (ObjectGuid const& memberGuid : members)
        response << memberGuid;
    SendPacket(&response);
}

void WorldSession::HandleGuildQueryMemberRecipes(WorldPacket& recvData)
{
    ObjectGuid memberGuid;
    ObjectGuid guildGuid;
    uint32 skillLineId = 0;
    recvData >> memberGuid >> guildGuid >> skillLineId;

    Player* player = GetPlayer();
    Guild* guild = player ? player->GetGuild() : nullptr;
    if (!player || !guild || guildGuid != guild->GetGUID() || !guild->IsMember(memberGuid) || !GetRootProfessionSkillLine(skillLineId))
    {
        return;
    }

    uint32 requestedRoot = GetRootProfessionSkillLine(skillLineId);

    // Online: live data (skill-granted + this session's recipes). Offline: DB plus
    // re-derived skill-granted recipes (see Guild::AppendOfflineKnownSpells).
    Player* onlineMember = ObjectAccessor::FindConnectedPlayer(memberGuid);
    std::unordered_set<uint32> knownSpells;
    if (onlineMember)
        Guild::AppendLiveKnownSpells(onlineMember, knownSpells);
    else
        Guild::AppendOfflineKnownSpells(memberGuid, guild->GetId(), knownSpells);

    GuildRecipeMask mask{};
    {
        for (uint32 spellId : knownSpells)
        {
            // One mask under the root ID covers every tier of the profession; each
            // known ability sets its own UniqueBit (see SetAbilityRecipeBit).
            SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellId);
            for (SkillLineAbilityMap::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
            {
                SkillLineAbilityEntry const* ability = itr->second;
                if (!ability || GetRootProfessionSkillLine(uint32(ability->SkillLine)) != requestedRoot)
                    continue;

                SetAbilityRecipeBit(ability, mask);
                break;
            }
        }
    }

    int32 skillRank = 0;
    int32 skillStep = 0;
    bool haveRootRow = false;
    if (onlineMember && onlineMember->HasSkill(requestedRoot))
    {
        skillRank = int32(onlineMember->GetPureSkillValue(requestedRoot));
        skillStep = Guild::GetLegacyProfessionStep(onlineMember->GetPureMaxSkillValue(requestedRoot));
        haveRootRow = true;
    }

    CharacterDatabasePreparedStatement* skillStmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER_SKILLS_BY_GUID);
    skillStmt->setUInt64(0, guild->GetId());
    skillStmt->setUInt64(1, memberGuid.GetCounter());
    PreparedQueryResult skillResult = haveRootRow ? PreparedQueryResult() : CharacterDatabase.Query(skillStmt);
    if (PreparedQueryResult result = skillResult)
    {
        do
        {
            Field* fields = result->Fetch();
            // character_skills.skill is SMALLINT: read as uint16, matching Player::_LoadSkills.
            uint32 savedSkill = fields[0].GetUInt16();
            if (GetRootProfessionSkillLine(savedSkill) != requestedRoot)
                continue;

            // Same rule as the roster: root line value + legacy step from root max.
            bool isRootRow = savedSkill == requestedRoot;
            if (isRootRow || (!haveRootRow && !skillStep))
            {
                skillRank = fields[1].GetUInt16();
                skillStep = Guild::GetLegacyProfessionStep(fields[2].GetUInt16());
                haveRootRow = haveRootRow || isRootRow;
            }
        }
        while (result->NextRow());
    }

    WorldPacket response(SMSG_GUILD_MEMBER_RECIPES, 16 + GUILD_RECIPE_MASK_SIZE);
    response << memberGuid;
    response << uint32(skillLineId);
    response << uint32(skillRank);
    response << uint32(skillStep);
    response.append(mask.data(), mask.size());
    SendPacket(&response);
}
