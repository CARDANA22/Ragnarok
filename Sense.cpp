#pragma once
#include <vector>
#include <filesystem>
#include <string>
#include <thread>
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Level.cpp"

const int gunRun = 1701147238; //freedm
const int control = 1953394531; //control
const int tdm = 1701147238;    //freedm
const int game = 1987212659;    //survival


long GetEntityById(int Ent)
    {
        long EntityList =  offsets::REGION + offsets::ENTITY_LIST;
        long BaseEntity = Memory::Read<long>(EntityList);
        if (!BaseEntity)
            return 0;
        return Memory::Read<long>(EntityList + (Ent << 5));
    }

bool isKnocked(uintptr_t Entit)
{
    return Memory::Read<bool>(Entit + offsets::BLEEDOUT_STATE);
}

uintptr_t getLocal()
{
    return Memory::Read<uintptr_t>(offsets::REGION + offsets::LOCAL_PLAYER);
}
int getHealth(uintptr_t Entity)
{
    return Memory::Read<int>(Entity + offsets::OFFSET_HEALTH);
}
    int getItemId(uintptr_t ent)
    {
        long ptrLong = ent + offsets::OFFSET_ITEM_ID;
        int result = Memory::Read<int>(ptrLong);
        return result;
    }

    auto getSignifier(uintptr_t Entity) -> std::string
    {
        auto SigAddress = Memory::Read<int>(Entity + offsets::OFFSET_SIGNIFIER);
        char Buffer[64] = { 0 };
        Memory::ReadRaw(SigAddress, &Buffer, 64);
        return Buffer;
    }

    int getTeamNumber(uintptr_t Entity)
    {
        long ptrLong = Entity + offsets::TEAM_NUMBER;
        int result = Memory::Read<int>(ptrLong);
        return result;
    }

    int getShield(uintptr_t Entity)
    {
        return Memory::Read<int>(Entity + offsets::OFFSET_SHIELD);
    }

class Sense
{
private:
    Level *m_level;
    LocalPlayer *m_localPlayer;
    std::vector<Player *> *m_players;
    int m_spectator;

public:
    Sense(Level *level,
          LocalPlayer *localPlayer,
          std::vector<Player *> *players)
    {
        m_level = level;
        m_localPlayer = localPlayer;
        m_players = players;
        m_spectator = 0;
    }
    void update()
    {

        //std::cout << m_localPlayer->getYaw() << satd::endl;

        GlowMode g {109,0,0,100};

        Vector3 notVisible {0.f, 0.1f, 10.f};
        Vector3 visible {3.f, 0.f, 0.f};

        GlowMode itemGlow {101, 102, 46, 100};

        uintptr_t mp_gamemode = Memory::Read<uintptr_t>(offsets::REGION + offsets::mp_gamemode);

        int buffer = Memory::Read<int>(mp_gamemode);

        char arr[64] = {0};

        Memory::ReadRaw(mp_gamemode, &arr, 64);
        
        //std::cout << buffer << std::endl;
        //std::cout << arr << std::endl;

        if (!m_level->isPlayable())
        {
            return;
        }

        if(m_level->isTrainingArea())
        {

            for (int i = 0; i < 10000; i++)
            {
                uintptr_t player = GetEntityById(i);

                if(player == 0)
                continue;


                if (getSignifier(player) == "npc_dummie")
                {
                    Player *ent = new Player(i);

                    if(ent->isVisible())
                    {
                        Memory::Write<int>(player + offsets::GLOW_ENABLE, 1);
                        Memory::Write<int>(player + offsets::GLOW_THROUGH_WALL, 2);
                        Memory::Write<GlowMode>(player + offsets::OFFSET_GLOW_MODE, g);
                        Memory::Write<Vector3>(player + offsets::OFFSET_GLOW_COLOR, visible);
                        Memory::Write<float>(player + offsets::OFFSET_GLOW_DISTANCE, 40000.f);
                    }else{
                        Memory::Write<int>(player + offsets::GLOW_ENABLE, 1);
                        Memory::Write<int>(player + offsets::GLOW_THROUGH_WALL, 2);
                        Memory::Write<GlowMode>(player + offsets::OFFSET_GLOW_MODE, g);
                        Memory::Write<Vector3>(player + offsets::OFFSET_GLOW_COLOR, notVisible);
                        Memory::Write<float>(player + offsets::OFFSET_GLOW_DISTANCE, 40000.f);

                        //Memory::Write<int>(player + 0x02c0, 1363184265);
                    }


                }

            }
        return;
        }

        for (int i = 0; i < 100; i++)
        {

            Player *player = new Player(i);

            if(player == nullptr)
			continue;

			if(player->getHealth() == 0)
			continue;

            //std::cout << player->getTeamNumber() << std::endl;

			if(buffer == control || buffer == tdm) {
                if((player->getTeamNumber() & 1) == (m_localPlayer->getTeamNumber() & 1))
                    {
                        continue;
                    }
            }else{
                if(player->getTeamNumber() == m_localPlayer->getTeamNumber())
                    {
                        continue;
                    }
           }

            if(player->isVisible())
            {
                //std::cout << getHealth(player)<< std::endl;
                player->setGlowEnable(1);
                player->setGlowThroughWall(2);
                player->setGlowMode(g);
                player->setGlowColor(visible);
                player->setGlowDistance(40000.f);
                //Memory::Write<int>(player + 0x02c0, 1363184265);
            }else{
                player->setGlowEnable(1);
                player->setGlowThroughWall(2);
                player->setGlowMode(itemGlow);
                player->setGlowColor(notVisible);
                player->setGlowDistance(40000.f);
            }
        }

        Vector3 temp = m_localPlayer->getViewAngle();
        countSpectator();

        //std::cout << m_localPlayer->getTeamNumber() << "\t"  << nextRing << std::endl;

    }

    void countSpectator()
    {
        if (!m_level->isPlayable())
            return;

        int spectatorcount = 0;
        LocalPlayer *local = m_localPlayer;
        local->purgeLastView();

        for (int i = 0; i < 60; i++)
        {

            Player *player = new Player(i);
            if (player == nullptr)
                    continue;

            float targetangle = player->GetYaw();
            float targetyaw = -targetangle; // yaw is inverted
            if (targetyaw < 0)
                targetyaw += 360;
            targetyaw += 90; // yaw is off by 90
            if (targetyaw > 360)
                targetyaw -= 360;

            for(int i = 0; i < local->m_LastViewAngles.size(); i++)
            {
                Last t = local->m_LastViewAngles.at(i);

                float localangle = t.angle.y;
                float localyaw = -localangle; // yaw is inverted
                if (localyaw < 0)
                    localyaw += 360;
                localyaw += 90; // yaw is off by 90
                if (localyaw > 360)
                    localyaw -= 360;

                if (targetyaw == localyaw && player->getHealth() == 0)
                    spectatorcount++;
            }
        }

        if (spectatorcount > 0 && local->getHealth() > 0)
            m_spectator = spectatorcount;
    }

    void printSpec()
    {
        std::cout << "Spectatorcount:       " << m_spectator << "\n" << std::endl;
    }

    void hand_model_glow()
    {
        if (!m_level->isPlayable())
            return;

        Vector3 color {5.f , 0.f, 0.f};

        int ViewModelHandle = m_localPlayer->ViewModelHandle();

        uintptr_t ViewModelPtr = Memory::Read<uintptr_t>(offsets::REGION + offsets::ENTITY_LIST + ViewModelHandle * 0x20);

        Memory::Write<int>(ViewModelPtr + 0x3C8, 1);
        Memory::Write<int>(ViewModelPtr + 0x3D0, 2);
        Memory::Write<GlowMode>(ViewModelPtr + 0x2C4, { 126,101,46,0 }); // 109, 112, 114

        color = Rainbow(0.007f);
        Memory::Write<Vector3>(ViewModelPtr + 0x1D0, color);
        

    }

    float oldDelay = 0;

    Vector3 Rainbow(float delay)
    {
        static uint32_t cnt = 0;
        float freq = delay;

        if (++cnt >= (uint32_t)-1)
        {
            cnt = 0;
        }
        if (delay != oldDelay)
        {
            cnt = 0;
            oldDelay = delay;
        }
        Vector3 cur = {std::sin(freq * cnt + 0) * 1.3f, std::sin(freq * cnt + 2) * 1.6f, std::sin(freq * cnt + 4) * 1.9f};

        return cur;
    }

    void NightMode()
    {
        if (!m_level->isPlayable())
            return;

        if (m_localPlayer->isDead())
        {
            return;
        }

        std::cout << Memory::Read<int>(offsets::REGION + 0x0138ca10 + 0x6C) << std::endl;
    }

};