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

#ifndef ItemInteractionPackets_h__
#define ItemInteractionPackets_h__

#include "Packet.h"
#include "ObjectGuid.h"

namespace WorldPackets
{
    namespace ItemInteraction
    {
        class UiItemInteractionNpc final : public ServerPacket
        {
        public:
            UiItemInteractionNpc() : ServerPacket(SMSG_UI_ITEM_INTERACTION_NPC, 16 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid Npc;
            int32 InteractionID = 0;
        };

        class PerformItemInteraction final : public ClientPacket
        {
        public:
            PerformItemInteraction(WorldPacket&& packet) : ClientPacket(CMSG_PERFORM_ITEM_INTERACTION, std::move(packet)) { }

            void Read() override;

            ObjectGuid Item;
            ObjectGuid Npc;
        };

        class ItemInteractionComplete final : public ServerPacket
        {
        public:
            ItemInteractionComplete() : ServerPacket(SMSG_ITEM_INTERACTION_COMPLETE, 0) { }

            WorldPacket const* Write() override { return &_worldPacket; }
        };
    }
}

#endif // ItemInteractionPackets_h__
