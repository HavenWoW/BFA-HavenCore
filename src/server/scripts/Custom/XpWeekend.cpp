#include "Player.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "Config.h"
#include "GameTime.h"
#include <atomic>

namespace
{
    constexpr uint32 WEEKEND_XP_MULTIPLIER = 2;

    std::atomic<bool> _doubleXpActive{ false };

    class XpWeekendWorld : public WorldScript
    {
    public:
        XpWeekendWorld() : WorldScript("XpWeekendWorld") { }

        void OnUpdate(uint32 /*diff*/) override
        {
            if (!sConfigMgr->GetBoolDefault("DynamicXP.Enable", true))
            {
                _doubleXpActive.store(false, std::memory_order_relaxed);
                return;
            }

            tm const* localTime = GameTime::GetDateAndTime();
            bool weekend = localTime->tm_wday == 0 || localTime->tm_wday == 6;
            _doubleXpActive.store(weekend, std::memory_order_relaxed);
        }
    };

    class XpWeekend : public PlayerScript
    {
    public:
        XpWeekend() : PlayerScript("XpWeekend") { }

        void OnGiveXP(Player* /*player*/, uint32& amount, Unit* /*victim*/) override
        {
            if (_doubleXpActive.load(std::memory_order_relaxed))
                amount *= WEEKEND_XP_MULTIPLIER;
        }

        void OnLogin(Player* player, bool /*firstLogin*/) override
        {
            if (_doubleXpActive.load(std::memory_order_relaxed))
                ChatHandler(player->GetSession()).PSendSysMessage("Double XP has started now! Call your friends and keep leveling faster!");
        }
    };
}

void AddSC_XpWeekend()
{
    new XpWeekendWorld();
    new XpWeekend();
}
