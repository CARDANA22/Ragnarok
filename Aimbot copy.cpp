#pragma once
#include <vector>
#include <iostream>
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Sense.cpp"
#include "Level.cpp"
#include "math.h"


float distanceToMeters(float distance)
{
    const float INCHES_TO_METER_RATE = 39.3701;
    return distance / INCHES_TO_METER_RATE;
}

class AimAssist{
private:
    Level *m_level;
    LocalPlayer *m_localPlayer;
    std::vector<Player *> *m_players ;
	uintptr_t local;
	Vector3 aimPos;
	Vector3 bonePos;

public:
    AimAssist( Level *level, LocalPlayer *localPlayer, std::vector<Player *> *players );
    void Update();
	Player * getClosestEnemy();
};


static float lastLowestFov = 2.5f;

AimAssist::AimAssist( Level *level, LocalPlayer *localPlayer, std::vector<Player *> *players )
{
    m_level = level;
    m_localPlayer = localPlayer;
    m_players = players;

	local = getLocal();
}

void AimAssist::Update() {
	Player *enemy = getClosestEnemy();

	if(enemy == nullptr)
	return;

	bonePos = enemy->getBonePos(7);
	//std::cout << enemy->isVisible() << std::endl;

	if(m_localPlayer->isInAttack())
	{
		//aimPos = enemy->PredictPos(bonePos);
		//std::cout << bonePos.x << "\t" << bonePos.y << "\t" << bonePos.z << std::endl;
		AimAt(local, bonePos);
	}

}

Player * AimAssist::getClosestEnemy()
{
	float colsestDist = 1000000.0;
	int closestIndex = -1;
	Player *closest;

	if (m_level->isTrainingArea())
	{
		for ( int i = 0; i < 10000; i++)
		{
			uintptr_t ent = GetEntityById(i);

			if(ent == 0)
			continue;

			if(getSignifier(ent) == "npc_dummie")
			{
				float dist = getDistance(GetPosition(local), GetPosition(ent));

				if(dist < colsestDist)
				{
					colsestDist = dist;
					closestIndex = i;
				}

			}

		}
	}else{

		for ( int i = 0; i < 60; i++)
		{
			Player *ent = new Player(i);

			if(ent == nullptr)
			continue;

			if(ent->getHealth() == 0)
			continue;

			//if(ent->getSignifier() == "player")
			//continue;

			if(ent->isKnocked())
			continue;

			if(ent->isTeam())
			continue;

			if(!ent->isVisible())
               continue;

			float dist = getDistance(GetPosition(local), ent->GetPosition());
			if(dist < colsestDist)
			{
				colsestDist = dist;
				closest = ent;
			}

		}
	}

	if(distanceToMeters(colsestDist) > 50)
		return nullptr;

	//if(closestIndex != -1)
	//	closest = new Player(closestIndex);

	return closest;
}