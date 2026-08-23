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

#include "Random.h"
#include "Errors.h"
#include "SFMTRand.h"
#include <memory>
#include <random>

static thread_local std::unique_ptr<SFMTRand> sfmtRand;
static SFMTEngine engine;

static SFMTRand* GetRng()
{
    if (!sfmtRand)
        sfmtRand = std::make_unique<SFMTRand>();

    return sfmtRand.get();
}

int32 irand(int32 min, int32 max)
{
    ASSERT(max >= min);
    return int32(GetRng()->IRandom(min, max));
}

uint32 urand(uint32 min, uint32 max)
{
    ASSERT(max >= min);
    return GetRng()->URandom(min, max);
}

uint32 urandms(uint32 min, uint32 max)
{
    ASSERT(max >= min);
    ASSERT(std::numeric_limits<uint32>::max() / Milliseconds::period::den >= max);
    return GetRng()->URandom(min * Milliseconds::period::den, max * Milliseconds::period::den);
}

float frand(float min, float max)
{
    ASSERT(max >= min);
    return float(GetRng()->Random() * (max - min) + min);
}

Milliseconds randtime(Milliseconds const& min, Milliseconds const& max)
{
    long long diff = max.count() - min.count();
    ASSERT(diff >= 0);
    ASSERT(diff <= 0xFFFFFFFF);
    return min + Milliseconds(urand(0, uint32(diff)));
}

uint32 rand32()
{
    return GetRng()->BRandom();
}

double rand_norm()
{
    return GetRng()->Random();
}

double rand_chance()
{
    return GetRng()->Random() * 100.0;
}

uint32 urandweighted(size_t count, double const* chances)
{
    std::discrete_distribution<uint32> dd(chances, chances + count);
    return dd(SFMTEngine::Instance());
}

SFMTEngine& SFMTEngine::Instance()
{
    return engine;
}
