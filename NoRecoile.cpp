#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Sense.cpp"
#include "Level.cpp"
#include "math.h"

class NoRecoil
{
private:
    Level *m_level;
    LocalPlayer *m_localPlayer;
    std::vector<Player *> *m_players ;

    double m_previousPunchPitch = 0;
    double m_previousPunchYaw = 0;

public:
    NoRecoil(Level *level, LocalPlayer *localPlayer, std::vector<Player *> *players )
    {
        m_level = level;
        m_localPlayer = localPlayer;
        m_players = players;
    }
    void update()
    {
        // validations
        if (!m_level->isPlayable())
            return;
        if (m_localPlayer->isDead())
            return;
        if (m_localPlayer->isKnocked())
            return;

        float punchPitchDelta = 0;
        float punchYawDelta = 0;


        double norecoilPitchStrength = 0.70f + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(0.98f-0.70f)));
        double norecoilYawStrength = 0.50f + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(0.98f-0.50f)));
        Vector3 punchAngle = {m_localPlayer->getPunchPitch(), m_localPlayer->getPunchYaw()};

        // adjust pitch
        if (punchAngle.x != 0)
        {
            punchPitchDelta = (punchAngle.x - m_previousPunchPitch) * norecoilPitchStrength;
            m_previousPunchPitch = punchAngle.x;
            m_localPlayer->setViewAngle({m_localPlayer->getViewAngle().x - punchPitchDelta, m_localPlayer->getViewAngle().y});
        }

        // adjust yaw
        if (punchAngle.y != 0)
        {
            punchYawDelta = (punchAngle.y - m_previousPunchYaw) * norecoilYawStrength;
            m_previousPunchYaw = punchAngle.y;
            m_localPlayer->setViewAngle({m_localPlayer->getViewAngle().x, m_localPlayer->getViewAngle().y - punchYawDelta});
        }

       // uintptr_t wpn = m_localPlayer->getLocalWeapon();
        //std::cout << wpn << std::hex << std::endl;

        /*if(wpn == 0)
        {
            mem::WriteFloat(wpn + offsets::WEAPON_MOVE_SPREAD, 0.0000f);
            mem::WriteFloat(wpn + offsets::WEAPON_KICK_SPREAD_ADS, 0.0000f);
            mem::WriteFloat(wpn + offsets::WEAPON_KICK_SPREAD_HIP, 0.0000f);
        }*/
    }
};