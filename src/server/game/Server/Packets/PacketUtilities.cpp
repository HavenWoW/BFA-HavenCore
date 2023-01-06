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

#include "PacketUtilities.h"
#include "Errors.h"

WorldPackets::PacketArrayMaxCapacityException::PacketArrayMaxCapacityException(std::size_t requestedSize, std::size_t sizeLimit)
{
    message().assign("Attempted to read more array elements from packet " + std::to_string(requestedSize) + " than allowed " + std::to_string(sizeLimit));
}

void WorldPackets::CheckCompactArrayMaskOverflow(std::size_t index, std::size_t limit)
{
    ASSERT(index < limit, "Attempted to insert " SZFMTD " values into CompactArray but it can only hold " SZFMTD, index, limit);
}
