#pragma once
#include <vector>
#include <iostream>
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Sense.cpp"
#include "Level.cpp"
#include "math.h"

const float activationFOv = 12;
int smoothing = 25;

class Aimbot
{
private:
    Level *m_level;
    LocalPlayer *m_localPlayer;
    std::vector<Player *> *m_players;

    double m_previousPunchPitch = 0;
    Player *m_lockedOnPlayer = nullptr;

public:
    Aimbot(
           Level *level,
           LocalPlayer *localPlayer,
           std::vector<Player *> *players
           )
    {

        m_level = level;
        m_localPlayer = localPlayer;
        m_players = players;

    }
    void update()
    {
        // validations
        if (!m_level->isPlayable())
        {
            m_lockedOnPlayer = nullptr;
            return;
        }
        if (m_localPlayer->isDead())
        {
            m_lockedOnPlayer = nullptr;
            return;
        }
        if (m_localPlayer->isKnocked())
        {
            m_lockedOnPlayer = nullptr;
            return;
        }

		if (!m_localPlayer->isInAttack())
		{
			m_lockedOnPlayer = nullptr;
			return;
		}
        // get desired angle to an enemy
        double desiredViewAngleYaw = 0;
        double desiredViewAnglePitch = 0;

        //Vector3 enemyPos = {m_lockedOnPlayer->getBonePos(7)};

        if (m_lockedOnPlayer == nullptr)
            m_lockedOnPlayer = findClosestEnemy();
        if (m_lockedOnPlayer == nullptr)
            return;
        if(!m_lockedOnPlayer->isVisible())
            return;

        Vector3 predictEnemyPos = m_lockedOnPlayer->PredictPos({m_lockedOnPlayer->getLocationX(),
                                                                    m_lockedOnPlayer->getLocationY(),
                                                                    m_lockedOnPlayer->getLocationZ()});

        double distanceToTarget = getDistance2({m_localPlayer->getLocationX(),
                                                                    m_localPlayer->getLocationY(),
                                                                    m_localPlayer->getLocationZ()},
                                                                    {m_lockedOnPlayer->getLocationX(),
                                                                    m_lockedOnPlayer->getLocationY(),
                                                                    m_lockedOnPlayer->getLocationZ()});
        if (distanceToMeters2(distanceToTarget) > 50)
            return;
        desiredViewAngleYaw = calculateDesiredYaw(m_localPlayer->getLocationX(),
                                                    m_localPlayer->getLocationY(),
                                                    m_lockedOnPlayer->getLocationX(),
                                                    m_lockedOnPlayer->getLocationY());

        // Setup Yaw
        const double yaw = m_localPlayer->getYaw();
        const double angleDelta = calculateAngleDelta(yaw, desiredViewAngleYaw);
        const double angleDeltaAbs = abs(angleDelta);
        if (angleDeltaAbs > activationFOv)
            return;

        double newYaw = flipYawIfNeeded(yaw + (angleDelta / smoothing));
        m_localPlayer->setYaw(newYaw);

    }
    double flipYawIfNeeded(double angle)
    {
        double myAngle = angle;
        if (myAngle > 180)
            myAngle = (360 - myAngle) * -1;
        else if (myAngle < -180)
            myAngle = (360 + myAngle);
        return myAngle;
    }
    double calculatePitchAngleDelta(double oldAngle, double newAngle)
    {
        
        double wayA = newAngle - oldAngle;
        return wayA;
    }
    double calculateAngleDelta(double oldAngle, double newAngle)
    {

        double wayA = newAngle - oldAngle;
        double wayB = 360 - abs(wayA);
        if (wayA > 0 && wayB > 0)
            wayB *= -1;
        if (abs(wayA) < abs(wayB))
            return wayA;
        return wayB;
    }
    double calculateDesiredYaw(
        double localPlayerLocationX,
        double localPlayerLocationY,
        double enemyPlayerLocationX,
        double enemyPlayerLocationY)
    {
        const double locationDeltaX = enemyPlayerLocationX - localPlayerLocationX;
        const double locationDeltaY = enemyPlayerLocationY - localPlayerLocationY;
        const double yawInRadians = atan2(locationDeltaY, locationDeltaX);
        const double yawInDegrees = yawInRadians * (180 / M_PI);
        return yawInDegrees;
    }
    double calculateDesiredPitch(
        double localPlayerLocationX,
        double localPlayerLocationY,
        double localPlayerLocationZ,
        Vector3 pos)
    {
        float locationDeltaZ = pos.z - localPlayerLocationZ;
        float distanceBetweenPlayers = getDistance2D({pos.x, pos.y}, {localPlayerLocationX, localPlayerLocationY});
        const double pitchInRadians = -asin((locationDeltaZ / distanceBetweenPlayers));
        const double pitchInDegrees = pitchInRadians * (180 / M_PI);
        return pitchInDegrees;
    }

    Player *findClosestEnemy()
    {
        uintptr_t mp_gamemode = Memory::Read<uintptr_t>(offsets::REGION + offsets::mp_gamemode);
        int buffer = Memory::Read<int>(mp_gamemode);

        Player *closestPlayerSoFar = nullptr;
        int closestIndex = -1;
        float cloasestDist = 1000000.f;
        
        if(m_level->isTrainingArea())
        {
            for ( int i = 0; i < 10000; i++)
            {
                uintptr_t ent = GetEntityById(i);

                if(ent == 0)
                continue;

                if(getSignifier(ent) == "npc_dummie")
                {
                    float dist = getDistance(m_localPlayer->getPosition(), GetPosition(ent));

                    if(dist < cloasestDist)
                    {
                        cloasestDist = dist;
                        closestIndex = i;
                    }

                }
            }
        }
        else{

            for (int i = 0; i < m_players->size(); i++)
            {
                Player *player = m_players->at(i);
                if (!player->isValid())
                    continue;

                if (player->isKnocked())
                    continue;

                if(player->getSignifier() != "player")
                    continue;

                if(buffer == control || buffer == tdm) {
                if((player->getTeamNumber() & 1) == ( m_localPlayer->getTeamNumber() & 1))
                    {
                        continue;
                    }
            }else{
                if(player->getTeamNumber() ==  m_localPlayer->getTeamNumber())
                    {
                        continue;
                    }
           }

                if (!player->isVisible())
                    continue;


                //std::cout << player->isItem()<<"\t"<< player->getHealth() <<"\t"<<player->isValid() << "\t" << player->isKnocked() << "\t"<< player->getTeamNumber() << "\t" << player->isVisible() <<std::endl;

                Vector3 localPos = {m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), m_localPlayer->getLocationZ()};
                Vector3 enemyPos = {player->getLocationX(), player->getLocationY(), player->getLocationZ()};
                float dist = sqrt((enemyPos.x - localPos.x) * (enemyPos.x - localPos.x) + (enemyPos.y - localPos.y) * (enemyPos.y - localPos.y) + (enemyPos.z - localPos.z) * (enemyPos.z - localPos.z) );

                if(dist < cloasestDist)
                    {
                        cloasestDist = dist;
                        closestPlayerSoFar = player;
                    }
            }
        }
        if(closestIndex != -1)
        {
            closestPlayerSoFar = new Player(closestIndex);
        }
        return closestPlayerSoFar;
    }
};