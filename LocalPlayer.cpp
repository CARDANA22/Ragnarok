#pragma once
#include <vector>
#include <iostream>
#include "Utils.cpp"
#include "Memory.h"
#include "Offsets.cpp"
#include "Player.cpp"

struct Last
{

    Vector3 angle;
    float time;

};

class LocalPlayer
{
private:
    float curtime;
    long m_basePointer;

    long getUnresolvedBasePointer()
    {
        long unresolvedBasePointer = offsets::REGION + offsets::LOCAL_PLAYER;
        return unresolvedBasePointer;
    }
    long getBasePointer()
    {
        if (m_basePointer == 0)
            m_basePointer = Memory::Read<long>(getUnresolvedBasePointer());
        return m_basePointer;
    }

public:
    std::vector<Last> m_LastViewAngles;

    void markForPointerResolution()
    {
        m_basePointer = 0;
    }
    float getLocationX()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN;
        float result = Memory::Read<float>(ptrLong);
        return result;
    }
    float getLocationY()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN + sizeof(float);
        float result = Memory::Read<float>(ptrLong);
        return result;
    }
    float getLocationZ()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN + (sizeof(float) * 2);
        float result = Memory::Read<float>(ptrLong);
        return result;
    }
    int getTeamNumber()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::TEAM_NUMBER;
        int result = Memory::Read<int>(ptrLong);
        return result;
    }
    bool isInAttack()
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_ATTACK;
        int result = Memory::Read<int>(ptrLong);
        return result > 0;
    }

    int getHealth()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::OFFSET_HEALTH;
        int result = Memory::Read<int>(ptrLong);
        return result;
    }

    Vector3 getViewAngle()
    {
        Vector3 view {getPitch(), getYaw(), 0};

        curtime  = Memory::Read<float>(offsets::REGION + offsets::GlobalVars + 0x10);
        Last t {view, curtime};

        m_LastViewAngles.push_back(t);

        return view;
    }

    void setViewAngle(Vector3 angle)
    {
        if(angle.x > 90 || angle.x < -90)
            return;

        if(angle.y > 180 || angle.y < -180)
            return;

        return Memory::Write<Vector3>(m_basePointer + offsets::VIEW_ANGLE, angle);
    }
    std::string getName()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::NAME;
        std::string result = Memory::ReadString(ptrLong);
        return result;
    }
    bool isKnocked()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::BLEEDOUT_STATE;
        short result = Memory::Read<short>(ptrLong);
        return result > 0;
    }
    bool isDead()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LIFE_STATE;
        short result = Memory::Read<short>(ptrLong);
        return result > 0;
    }
    float getPunchPitch()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VEC_PUNCH_WEAPON_ANGLE;
        float result = Memory::Read<float>(ptrLong);
        return result;
    }
    float getPunchYaw()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VEC_PUNCH_WEAPON_ANGLE + sizeof(float);
        float result = Memory::Read<float>(ptrLong);
        return result;
    }
    float getPitch()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VIEW_ANGLE;
        float result = Memory::Read<float>(ptrLong);
        return result;
    }
    void setPitch(float angle)
    {
        if (angle > 90 || angle < -90)
            return;
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VIEW_ANGLE;
        Memory::Write<float>(ptrLong, angle);
    }
    float getYaw()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VIEW_ANGLE + sizeof(float);
        float result = Memory::Read<float>(ptrLong);
        return result;
    }
    void setYaw(float angle)
    {
        if (angle > 180 || angle < -180)
            return;
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VIEW_ANGLE + sizeof(float);
        Memory::Write<float>(ptrLong, angle);
    }
    void setTeam(int i)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::TEAM_NUMBER;
        Memory::Write<int>(ptrLong, i);
    }
    bool isZooming()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::ZOOMING;
        short result = Memory::Read<short>(ptrLong);
        return result > 0;
    }

    void purgeLastView()
    {
        if(m_LastViewAngles.empty())
        return;

        for(int i = 0; i < m_LastViewAngles.size(); i++)
        {
            Last t = m_LastViewAngles.at(i);

            if(fabsf(t.time-curtime) > 0.1f)
            {
                m_LastViewAngles.erase(m_LastViewAngles.begin()+i);
            }
        }
    }

    Vector3 getPosition()
    {
        return Memory::Read<Vector3>(getBasePointer() + offsets::LOCAL_ORIGIN);
    }

    int ViewModelHandle()
    {
        return Memory::Read<int>(getBasePointer()+ offsets::m_hViewModels) & 0xFFFF;
    }
};
void setPitch(uintptr_t ent, float pitch)
{
    Memory::Write<float>(ent + offsets::VIEW_ANGLE, pitch);
}
void setYaw(uintptr_t ent, float yaw)
{
    Memory::Write<float>(ent + offsets::VIEW_ANGLE + sizeof(float), yaw);
}

float getYaw(uintptr_t ent)
{
    return Memory::Read<float>(ent + offsets::VIEW_ANGLE + sizeof(float));
}

float flipYawIfNeeded(float angle)
{
    float myAngle = angle;
    if (myAngle > 179.9)
        myAngle = (359.9 - myAngle) * -1;
    else if (myAngle < -179.9)
        myAngle = (359.9 + myAngle);
    return myAngle;
}

float calculateAngleDelta(float oldAngle, float newAngle)
{
    float wayA = newAngle - oldAngle;
    float wayB = 360 - abs(wayA);
    if (wayA > 0 && wayB > 0)
        wayB *= -1;
    if (abs(wayA) < abs(wayB))
        return wayA;
    return wayB;
}

Vector3 GetViewAngles(uintptr_t ent)
{
    return Memory::Read<Vector3>(ent + offsets::VIEW_ANGLE);
}

float getDistance(Vector3 myPos, Vector3 targetPos)
{
    Vector3 delta { targetPos.x - myPos.x , targetPos.y -myPos.y, targetPos.z - myPos.z};
    return sqrt(delta.x*delta.x + delta.y*delta.y + delta.z*delta.z);
}

double getDistance2D(Vector2 myPos, Vector2 targetPos)
{
    Vector2 delta { targetPos.x - myPos.x , targetPos.y -myPos.y};
    return sqrt(delta.x*delta.x + delta.y*delta.y);
}

Vector3 GetPosition(uintptr_t ent)
{
    return Memory::Read<Vector3>(ent + offsets::LOCAL_ORIGIN);
}
float distanceToMeters2(float distance)
{
    const float INCHES_TO_METER_RATE = 39.3701;
    return distance / INCHES_TO_METER_RATE;
}

void AimAt(uintptr_t local, Vector3 target)
{
    bool lock = false;

    Vector3 localView = GetViewAngles(local);
    Vector3 localPos = GetPosition(local);

    const float activationFOv = 12;
    int smoothing = 10;

    Vector3 deltaVec = {target.x - localPos.x , target.y -localPos.y, target.z - localPos.z};
    float deltaVecLength = getDistance(localPos, target);
    float meter = distanceToMeters2(deltaVecLength);

    //if(meter > 20)
    //smoothing = (deltaVecLength /100);

    float pith = -asin(deltaVec.z / deltaVecLength) * (180 / M_PI);

    float desiredYaw = atan2(deltaVec.y, deltaVec.x) * (180 / M_PI);

    float oldYaw = getYaw(local);

    float angleDelta = calculateAngleDelta(oldYaw, desiredYaw);
    float angleDeltaAbs = abs(angleDelta);

    if(lock)
    {
        //setPitch(local, pith);
        setYaw(local, desiredYaw);
        return;
    }
    //setPitch(local, pith);
    //setYaw(local, desiredYaw);
    if(angleDeltaAbs > activationFOv)
        return;

    float newYaw = flipYawIfNeeded(oldYaw + (angleDelta / (smoothing)));

    if(newYaw)
        setYaw(local, newYaw);
}


