#include "LocalPlayer.h"

#include <iostream>

#include "../Memory/Memory.h"
#include "../Offsets.h"
#include "../Utils/Utils.h"

unsigned long LocalPlayer::m_basePointer = 0;

std::string LocalPlayer::GetName() {
    // TODO: Change to reading from NameList
    return Memory::ReadString(m_basePointer + Offsets::m_iName);
}

Vector3 LocalPlayer::GetPosition() {
    return Memory::Read<Vector3>(m_basePointer + Offsets::m_localOrigin);
}

int LocalPlayer::GetHealth() {
    return Memory::Read<int>(m_basePointer + Offsets::m_iHealth);
}

int LocalPlayer::GetTeamNumber() {
    return Memory::Read<int>(m_basePointer + Offsets::m_iTeamNum);
}

bool LocalPlayer::IsAlive() {
    return Memory::Read<short>(m_basePointer + Offsets::m_lifeState) == 0;
}

bool LocalPlayer::IsKnocked() {
    return Memory::Read<short>(m_basePointer + Offsets::m_bleedoutState) > 0;
}

bool LocalPlayer::IsAttacking() {
    return Memory::Read<short>(Memory::GetLastModuleBase() + Offsets::in_attack) > 0;
}

bool LocalPlayer::IsZooming() {
    return Memory::Read<short>(Memory::GetLastModuleBase() + Offsets::in_zoom) > 0;
}

Vector3 LocalPlayer::getViewAngle()
{
    return Memory::Read<Vector3>(m_basePointer + Offsets::m_viewAngles);
}

Vector3 LocalPlayer::getPunchAngle()
{
    return Memory::Read<Vector3>(m_basePointer + Offsets::m_vecPunchWeapon_Angle);
}

void LocalPlayer::setViewAngle(Vector3 angle)
{
    if(angle.x > 90 || angle.x < -90)
        return;

    if(angle.y > 180 || angle.y < -180)
        return;

    return Memory::Write<Vector3>(m_basePointer + Offsets::m_viewAngles, angle);
}

void LocalPlayer::setYaw(float angle)
{
    return Memory::Write<float>(m_basePointer + Offsets::m_viewAngles + sizeof(float), angle);
}

void LocalPlayer::addLastViewAngles(float curtime){
    m_LastViewAngles.push_back({getViewAngle(), curtime});
}

void LocalPlayer::purgeLastView(float curtime){
    if(m_LastViewAngles.empty())
    return;

    for(int i = 0; i < m_LastViewAngles.size(); i++)
    {
        LastViewAngles t = m_LastViewAngles.at(i);

        if(fabsf(t.time-curtime) > 0.1f)
        {
            m_LastViewAngles.erase(m_LastViewAngles.begin()+i);
        }
    }
}

void LocalPlayer::Update() {
    m_basePointer = Memory::Read<unsigned long>(
        Memory::GetLastModuleBase() + Offsets::LocalPlayer
    );
}