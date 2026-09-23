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

#include "GuildFinderPackets.h"
#include "PacketUtilities.h"

void WorldPackets::GuildFinder::LFGuildAddRecruit::Read()
{
    _worldPacket >> GuildGUID;
    _worldPacket >> PlayStyle;
    _worldPacket >> Availability;
    _worldPacket >> ClassRoles;
    Comment = _worldPacket.ReadString(_worldPacket.ReadBits(10));
}

void WorldPackets::GuildFinder::LFGuildBrowse::Read()
{
    _worldPacket >> PlayStyle;
    _worldPacket >> Availability;
    _worldPacket >> ClassRoles;
    _worldPacket >> CharacterLevel;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::GuildFinder::LFGuildBrowseData const& guildData)
{
    data.WriteBits(guildData.GuildName.length(), 7);
    data.WriteBits(guildData.Comment.length(), 10);
    data << guildData.GuildGUID;
    data << uint32(guildData.GuildVirtualRealm);
    data << int32(guildData.GuildMembers);
    data << int32(guildData.GuildAchievementPoints);
    data << int32(guildData.PlayStyle);
    data << int32(guildData.Availability);
    data << int32(guildData.ClassRoles);
    data << int32(guildData.LevelRange);
    data << int32(guildData.EmblemStyle);
    data << int32(guildData.EmblemColor);
    data << int32(guildData.BorderStyle);
    data << int32(guildData.BorderColor);
    data << int32(guildData.Background);
    data << int8(guildData.Cached);
    data << int8(guildData.MembershipRequested);
    data.WriteString(guildData.GuildName);
    data.WriteString(guildData.Comment);
    return data;
}

WorldPacket const* WorldPackets::GuildFinder::LFGuildBrowseResult::Write()
{
    _worldPacket << uint32(Post.size());
    for (LFGuildBrowseData const& guildData : Post)
        _worldPacket << guildData;

    return &_worldPacket;
}

void WorldPackets::GuildFinder::LFGuildDeclineRecruit::Read()
{
    _worldPacket >> RecruitGUID;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::GuildFinder::LFGuildApplicationData const& application)
{
    data << application.GuildGUID;
    data << uint32(application.GuildVirtualRealm);
    data << int32(application.ClassRoles);
    data << int32(application.PlayStyle);
    data << int32(application.Availability);
    data << uint32(application.SecondsSinceCreated);
    data << uint32(application.SecondsUntilExpiration);
    data.WriteBits(application.GuildName.length(), 7);
    data.WriteBits(application.Comment.length(), 10);
    data.FlushBits();
    data.WriteString(application.GuildName);
    data.WriteString(application.Comment);
    return data;
}

WorldPacket const* WorldPackets::GuildFinder::LFGuildApplications::Write()
{
    _worldPacket << int32(NumRemaining);
    _worldPacket << uint32(Application.size());
    for (LFGuildApplicationData const& application : Application)
        _worldPacket << application;

    return &_worldPacket;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::GuildFinder::GuildPostData const& post)
{
    data.WriteBit(post.Active);
    data.WriteBits(post.Comment.length(), 10);
    data << int32(post.PlayStyle);
    data << int32(post.Availability);
    data << int32(post.ClassRoles);
    data << int32(post.LevelRange);
    data << int32(post.SecondsRemaining);
    data.WriteString(post.Comment);
    return data;
}

WorldPacket const* WorldPackets::GuildFinder::LFGuildPost::Write()
{
    _worldPacket.WriteBit(Post.has_value());
    _worldPacket.FlushBits();
    if (Post)
        _worldPacket << *Post;

    return &_worldPacket;
}

void WorldPackets::GuildFinder::LFGuildGetRecruits::Read()
{
    _worldPacket >> LastUpdate;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::GuildFinder::LFGuildRecruitData const& recruit)
{
    data << recruit.RecruitGUID;
    data << uint32(recruit.RecruitVirtualRealm);
    data << int32(recruit.CharacterClass);
    data << int32(recruit.CharacterGender);
    data << int32(recruit.CharacterLevel);
    data << int32(recruit.ClassRoles);
    data << int32(recruit.PlayStyle);
    data << int32(recruit.Availability);
    data << uint32(recruit.SecondsSinceCreated);
    data << uint32(recruit.SecondsUntilExpiration);
    data.WriteBits(recruit.Name.length(), 6);
    data.WriteBits(recruit.Comment.length(), 10);
    data.FlushBits();
    data.WriteString(recruit.Name);
    data.WriteString(recruit.Comment);
    return data;
}

WorldPacket const* WorldPackets::GuildFinder::LFGuildRecruits::Write()
{
    _worldPacket << uint32(Recruits.size());
    _worldPacket << uint32(UpdateTime);
    for (LFGuildRecruitData const& recruit : Recruits)
        _worldPacket << recruit;

    return &_worldPacket;
}

void WorldPackets::GuildFinder::LFGuildRemoveRecruit::Read()
{
    _worldPacket >> GuildGUID;
}

void WorldPackets::GuildFinder::LFGuildSetGuildPost::Read()
{
    _worldPacket >> PlayStyle;
    _worldPacket >> Availability;
    _worldPacket >> ClassRoles;
    _worldPacket >> LevelRange;
    Active = _worldPacket.ReadBit();
    Comment = _worldPacket.ReadString(_worldPacket.ReadBits(10));
}

void WorldPackets::GuildFinder::ClubFinderPost::Read()
{
    // BFA carries the length header followed by the fixed recruitment fields
    // and then the two variable strings. Newer retail captures add one opaque
    // uint32 between MinItemLevel and those strings. Detect that word from the
    // declared string lengths instead of hard-coding a post-BFA layout.
    uint32 nameLength = _worldPacket.ReadBits(7);
    uint32 descriptionLength = _worldPacket.ReadBits(12);
    Unknown5 = uint8(_worldPacket.ReadBits(5));

    _worldPacket >> ClubID;
    _worldPacket >> SpecMask;
    _worldPacket >> RecruitmentFlags;
    _worldPacket >> MinItemLevel;

    std::size_t stringBytes = std::size_t(nameLength) + std::size_t(descriptionLength);
    std::size_t remainingBytes = _worldPacket.size() - _worldPacket.rpos();
    if (remainingBytes == stringBytes + sizeof(uint32))
        _worldPacket >> Unknown32;

    // Never let a malformed length header walk beyond the packet. The normal
    // BFA and captured newer-retail variants both reach this point with exactly
    // stringBytes remaining.
    if (_worldPacket.size() - _worldPacket.rpos() < stringBytes)
    {
        _worldPacket.rfinish();
        Name.clear();
        Description.clear();
        return;
    }

    Name = _worldPacket.ReadString(nameLength);
    Description = _worldPacket.ReadString(descriptionLength);
}

// [8.3.7 LAYOUT] VERIFIED in-game: posting GUID + one byte of two 3-bit fields
// (result: 0 created / 1 updated, then a field retail always sets to 1) ->
// 0x04 / 0x24. Matches retail captures; saved settings are confirmed stored.
WorldPacket const* WorldPackets::GuildFinder::ClubFinderResponsePostRecruitmentMessage::Write()
{
    // Retail success response is the packed ClubFinderGUID followed by one
    // response/state byte (0x24 in the supplied successful guild POST capture).
    _worldPacket << ClubFinderGUID;
    _worldPacket << Flags;
    return &_worldPacket;
}
