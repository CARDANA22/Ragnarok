#include "AimAssist.h"
#include <iostream>

#include "../../sdk/Level.h"
#include "../../sdk/LocalPlayer.h"
#include "../../sdk/Entity.h"
#include "../../sdk/EntityList.h"
#include "../../Utils/Utils.h"

AimAssist::AimAssist() : Module(true, "AimAssist") {}

void AimAssist::OnEnable() {

}

void AimAssist::OnDisable() {
    
}

void AimAssist::Update() {

    if (!Level::isPlayable())
        return;

    if (!LocalPlayer::IsAlive())
        return;

    if (LocalPlayer::IsKnocked())
        return;

    if(true)
    {
        std::cout << LocalPlayer::IsAttacking() << std::endl;
        Entity *ent = getClosestEnemy();

        if(ent == 0)
        return;

        aimPos = ent->getBonePos(3);
            AimAt(aimPos);
    }
}

Entity* AimAssist::getClosestEnemy()
{
	float closestDist = 1000000.0;
	int indexClosest = -1;
     Vector3 tmp = {0, 0, 0};

	for ( int i = 0; i < EntityList::GetSize(); i++)
	{
		Entity *ent = EntityList::GetEntity(i);

		if(ent == 0)
		continue;
        
        if(ent->getBase() == LocalPlayer::m_basePointer)
        continue;

        if(!ent->isValid())
        continue;

		if(ent->GetTeamNumber() != LocalPlayer::GetTeamNumber())
		{
			float dist = tmp.distance(LocalPlayer::GetPosition(), ent->GetPosition());
            float meters = distanceToMeters(dist);
            if(meters < closestDist)
			{
				closestDist = meters;
				indexClosest = i;
			}	
		}
	}
    
    //std::cout << closestDist << "\t" << indexClosest << std::endl;
    if(closestDist > range)
    return 0;

	if(indexClosest == -1)
		return 0;

	return EntityList::GetEntity(indexClosest);;
}

void AimAssist::AimAt(Vector3 target)
{
    Vector3 localPos = LocalPlayer::GetPosition();
    Vector3 deltaVec = {target.x - localPos.x , target.y -localPos.y, target.z - localPos.z}; 

    float desiredYaw = atan2(deltaVec.y, deltaVec.x) * (180 / M_PI);
    float oldYaw = LocalPlayer::getViewAngle().y;

    float angleDelta = calculateAngleDelta(oldYaw, desiredYaw);
    float angleDeltaAbs = abs(angleDelta);
    if(angleDeltaAbs > activationFOv)
    return;

    float newYaw = flipYawIfNeeded(oldYaw + (angleDelta/ smoothing));
    LocalPlayer::setYaw(newYaw);
}

float AimAssist::calculateAngleDelta(float oldAngle, float newAngle)
{
    float wayA = newAngle - oldAngle;
    float wayB = 360 - abs(wayA);
    if (wayA > 0 && wayB > 0)
        wayB *= -1;
    if (abs(wayA) < abs(wayB))
        return wayA;
    return wayB;
}

float AimAssist::flipYawIfNeeded(float angle)
{
    float myAngle = angle;
    if (myAngle > 180)
        myAngle = (360 - myAngle) * -1;
    else if (myAngle < -180)
        myAngle = (360 + myAngle);
    return myAngle;
}