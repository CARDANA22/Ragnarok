#include "Entity.h"

#include <iostream>

#include "../Memory/Memory.h"
#include "../Offsets.h"
#include "Types/Bone.h"
#include "LocalPlayer.h"

Entity::Entity(int entityListIndex)
{
    m_entityListIndex = entityListIndex;
    m_basePointer = Memory::Read<long>( Memory::GetLastModuleBase() + Offsets::cl_entitylist + ((m_entityListIndex + 1) << 5));
}

bool Entity::isDead()
{
    return Memory::Read<bool>(m_basePointer + Offsets::m_lifeState);
}

std::string Entity::getSignifier()
{
    char buffer[64] = { 0 };
    Memory::ReadArray(m_basePointer + Offsets::m_iSignifierName, &buffer, 64);
    return buffer;
}

int Entity::getHealth()
{
    return Memory::Read<int>(m_basePointer + Offsets::m_iHealth);
}  

int Entity::getShield()
{
    return Memory::Read<int>(m_basePointer + Offsets::m_shieldHealth);
}

int Entity::getItemId()
{
    return Memory::Read<int>(m_basePointer + Offsets::m_customScriptInt);
}

bool Entity::isKnocked()
{
    return Memory::Read<bool>(m_basePointer + Offsets::m_bleedoutState);
}

std::string Entity::getName()
{
    return Memory::ReadString(m_basePointer + Offsets::m_iName);
}

Vector3 Entity::getViewAngle()
{
    return Memory::Read<Vector3>(m_basePointer + Offsets::m_viewAngles);
}

bool Entity::isValid()
{
    return m_basePointer > 0 && !Entity::isDead();
}

Vector3 Entity::GetPosition() 
{
    return Memory::Read<Vector3>(m_basePointer + Offsets::m_localOrigin);
}

int Entity::GetTeamNumber() 
{
    return Memory::Read<int>(m_basePointer + Offsets::m_iTeamNum);
}

float Entity::getLastVisibleTime()
{
    return Memory::Read<float>(m_basePointer + Offsets::lastVisibleTime);
}

void Entity::setGlow(int glowEnable, int glowThroughWall, Vector3 pColor, float glowDistance)
{
    Memory::Write<int>(m_basePointer + Offsets::glowEnable, glowEnable);
    Memory::Write<int>(m_basePointer + Offsets::glowThroughWall, glowThroughWall);
    Memory::Write<Vector3>(m_basePointer + Offsets::glowColor, pColor);
    Memory::Write<float>(m_basePointer + Offsets::glowDistance, glowDistance);
}

bool Entity::isVisible()
{
    const float lastVisibleTime = Entity::getLastVisibleTime();
    const bool isVisible = lastVisibleTime > m_lastVisibleTime;
    m_lastVisibleTime = lastVisibleTime;
    return isVisible;
}

Vector3 Entity::getBonePos(int id)
{
    Vector3 pos = GetPosition();
    uintptr_t bones = Memory::Read<uintptr_t>(m_basePointer + Offsets::bones);
    Vector3 bone = {};
	uint32_t boneloc = (id * 0x30);
	bone_t boneOffset = {};
	boneOffset = Memory::Read<bone_t>(bones + boneloc);

	bone.x = boneOffset.x + pos.x;
	bone.y = boneOffset.y + pos.y;
	bone.z = boneOffset.z + pos.z;
	return bone;
}

uintptr_t Entity::getBase()
{
    return  Entity::m_basePointer;
}

void Entity::Update() {
    m_basePointer = Memory::Read<long>( Memory::GetLastModuleBase() + Offsets::cl_entitylist + ((m_entityListIndex + 1) << 5));
}