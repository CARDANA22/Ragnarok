#pragma once
#include <iostream>
#include <cmath>
#include "LocalPlayer.cpp"
#include "Utils.cpp"
#include "Offsets.cpp"
#include "Memory.cpp"

float getDistance2(Vector3 myPos, Vector3 targetPos)
{
    Vector3 delta { targetPos.x - myPos.x , targetPos.y -myPos.y, targetPos.z - myPos.z};
    return sqrt(delta.x*delta.x + delta.y*delta.y + delta.z*delta.z);
};

Vector3 playerPrediction(Vector3 localPos, Vector3 targetPos, Vector3 targetVel, float bulletSpeed, float bulletGrav)
{
	float distance = getDistance2(localPos, targetPos);
	float timeToTarget = distance / bulletSpeed; // In seconds
	Vector3 bulletGravChange = { 0, bulletGrav * timeToTarget, 0 };
	Vector3 movementChange = targetVel * timeToTarget;
	Vector3 newPos = targetPos - movementChange;// +bulletGravChange;
	return newPos;
};



struct Color
{
	float r,
    g,
    b;
};

struct GlowMode {
    uint8_t GeneralGlowMode, // 0x0
            BorderGlowMode, // 0x8
            BorderSize, //0x10
            TransparentLevel;  //0x18
};

class Player
{
private:
    int m_entityListIndex;
    float m_lastVisibleTime;
    long m_basePointer = 0;
    long getUnresolvedBasePointer()
    {
        long unresolvedBasePointer = offsets::REGION + offsets::ENTITY_LIST + (m_entityListIndex  << 5);
        return unresolvedBasePointer;
    }
    long getBasePointer()
    {
        if (m_basePointer == 0)
            m_basePointer = Memory::Read<long>(getUnresolvedBasePointer());
        return m_basePointer;
    }

public:
    Player(int entityListIndex)
    {
        m_entityListIndex = entityListIndex;
    }
    void markForPointerResolution()
    {
        m_basePointer = 0;
    }
    bool isDead()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LIFE_STATE;
        short result = Memory::Read<short>(ptrLong);
        return result > 0;
    }
    bool isItem()
    {
        auto SigAddress = Memory::Read<int>(getBasePointer() + offsets::OFFSET_SIGNIFIER);
        char Buffer[64] = { 0 };
        Memory::ReadRaw(SigAddress, &Buffer, 64);

        if (Buffer == "prop_survival")
        return true;

        return false;
    }
    int getHealth()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::OFFSET_HEALTH;
        int result = Memory::Read<int>(ptrLong);
        return result;
    }
    int getShield()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::OFFSET_SHIELD;
        int result = Memory::Read<int>(ptrLong);
        return result;
    }
    int getItemId()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::OFFSET_ITEM_ID;
        int result = Memory::Read<int>(ptrLong);
        return result;
    }
    bool isKnocked()
    {
        return Memory::Read<bool>(m_basePointer + offsets::BLEEDOUT_STATE);
    }
    std::string getName()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::NAME;
        std::string result = Memory::ReadString(ptrLong);
        return result;
    }
    bool isValid()
    {
        return getBasePointer() > 0 && !isDead();
    }
    bool isPlayer()
    {
        return (Memory::Read<uintptr_t>(getBasePointer() + offsets::NAME) == 125780153691248);
    }
    bool isDummy()
    {
        if(getTeamNumber()== 97)
        return true;

        return false;
    }
    void lobaGlow()
    {
        Memory::Write<int>(getBasePointer() + 0x02c0, 1363184265);
    }
    void lobaDisable()
    {
        Memory::Write<int>(getBasePointer() + 0x02c0, 1797292288);
    }
    std::string getInvalidReason()
    {
        if (getBasePointer() == 0)
            return "Unresolved base pointer";
        else if (isDead())
            return "Player is dead";
        else if (getName().empty())
            return "Name is empty";
        else
            return "Player is valid";
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
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN + sizeof(float) + sizeof(float);
        float result = Memory::Read<float>(ptrLong);
        return result;
    }
    bool isTeam()
    {
        uintptr_t mp_gamemode = Memory::Read<uintptr_t>(offsets::REGION + offsets::mp_gamemode);
        int buffer = Memory::Read<int>(mp_gamemode);

        uintptr_t local = Memory::Read<uintptr_t>(offsets::REGION+ offsets::LOCAL_PLAYER);
        int localTeam = Memory::Read<int>(local + offsets::TEAM_NUMBER);

        if (buffer == 1078406224) {
            if((getTeamNumber() & 1) == (localTeam & 1))
            {
                return true;
            }
        }
        else {
            if(getTeamNumber() == localTeam)
            {
                return true;
            }
        }
        return false;
    }
    int getTeamNumber()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::TEAM_NUMBER;
        int result = Memory::Read<int>(ptrLong);
        return result;
    }
    int getGlowEnable()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_ENABLE;
        int result = Memory::Read<int>(ptrLong);
        return result;
    }
    void setGlowEnable(int glowEnable)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_ENABLE;
        Memory::Write<int>(ptrLong, glowEnable);
    }

    void setGlowDistance(float glowDistance)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::OFFSET_GLOW_DISTANCE;
       Memory::Write<float>(ptrLong, glowDistance);
    }

    void setGlowMode(GlowMode pMode)
    {
        long basePointer = getBasePointer();
        Memory::Write<GlowMode>(basePointer + offsets::OFFSET_GLOW_MODE, pMode);
    }

    float GetYaw()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::OFFSET_YAW;
    	float yaw = 0;
        yaw = Memory::Read<float>(ptrLong);
    	if (yaw < 0)
        	yaw += 360;
    	yaw += 90;
    	if (yaw > 360)
        	yaw -= 360;

    	return yaw;
    }

   Vector3 getPos()
   {
        return {getLocationX(), getLocationY(), getLocationZ()};
   }

    Vector3 getBonePos(int id)
    {
        uintptr_t basePointer = Memory::Read<uintptr_t>(offsets::REGION + offsets::LOCAL_PLAYER);
        Vector3 pos ={getPos()};
        std::cout << "basePointer" << std::endl;
        uintptr_t bones = Memory::Read<uintptr_t>(basePointer + offsets::bones);
        Vector3 bone = {};
        uint32_t boneloc = (id * 0x30);
        bone_t boneOffset = {};
        boneOffset = Memory::Read<bone_t>(bones + boneloc);

        bone.x = boneOffset.x + pos.x;
        bone.y = boneOffset.y + pos.y;
        bone.z = boneOffset.z + pos.z;
        return bone;
    }

    auto getSignifier() -> std::string
    {
        auto SigAddress = Memory::Read<int>(getBasePointer() + offsets::OFFSET_SIGNIFIER);
        char Buffer[64] = { 0 };
        Memory::ReadRaw(SigAddress, &Buffer, 64);
        return Buffer;
    }

    void setGlowColor(Vector3 pColor)
    {
        long basePointer = getBasePointer();
        Memory::Write<Vector3>(basePointer + offsets::OFFSET_GLOW_COLOR, pColor);
    }

    int getGlowThroughWall()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_THROUGH_WALL;
        int result = Memory::Read<int>(ptrLong);
        return result;
    }
    void setGlowThroughWall(int glowThroughWall)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_THROUGH_WALL;
        Memory::Write<int>(ptrLong, glowThroughWall);
    }
    float getLastVisibleTime()
    {
        return Memory::Read<float>(getBasePointer() + offsets::LAST_VISIBLE_TIME);
    }
    bool isVisible()
    {
        float curtime  = Memory::Read<float>(offsets::REGION + offsets::GlobalVars + 0x10);
        float Time1 = getLastVisibleTime();
        return (Time1 + 0.2) >= curtime;
        //const float lastVisibleTime = getLastVisibleTime();
        //const bool isVisible = lastVisibleTime > 0.0f && fabsf(lastVisibleTime - curtime) < 0.1f;

        //return isVisible;

        //const float lastVisibleTime = getLastVisibleTime();
        //const bool isVisible = lastVisibleTime > m_lastVisibleTime;
        //m_lastVisibleTime = lastVisibleTime;
        //return isVisible;
    }

    Vector3 GetPosition()
    {
        return Memory::Read<Vector3>(getBasePointer() + offsets::LOCAL_ORIGIN);
    }


    Vector3 PredictPos(Vector3 bonePos)
    {
        Vector3 aimPos;
        Vector3 null {0.f, 0.f, 0.f};
    	uintptr_t local = Memory::Read<uintptr_t>(offsets::REGION + offsets::LOCAL_PLAYER);
        short actWeaponID = Memory::Read<short>(local+ offsets::OFFSET_CURRENT_WEAPON);
    	uintptr_t currentWeapon = Memory::Read<uintptr_t>(offsets::REGION + offsets::ENTITY_LIST +(actWeaponID << 5));

    	if (currentWeapon != 0)
    	{
    		float bulletSpeed = Memory::Read<float>(currentWeapon + offsets::OFFSET_BULLET_SPEED);
    		float bulletGravity = Memory::Read<float>(currentWeapon + offsets::OFFSET_BULLET_SPEED + 0x8);

    		if (bulletSpeed > 1.f)
    		{
    			Vector3 localPos = Memory::Read<Vector3>(offsets::REGION + offsets::LOCAL_PLAYER + offsets::CameraPos);
    			Vector3 targetVel =Memory::Read<Vector3>(getBasePointer() + offsets::ABS_VELOCITY);

                if(targetVel == null)
                return bonePos;

               aimPos = playerPrediction(localPos, bonePos, targetVel, bulletSpeed, bulletGravity);
    		}
    	}
        return aimPos;
    }

   /* bool fov_check() const {
        info.pitch = info.aim.x - local->camera_angles.x;
        info.yaw = info.aim.y - local->camera_angles.y;
        while (info.yaw <= -180.0f) info.yaw += 360.0f;
        while (info.yaw > 180.0f) info.yaw -= 360.0f;
        info.angle = sqrt(info.pitch * info.pitch + info.yaw * info.yaw);
        const float fov = get_fov() * get_fov_scale(state, local);
        return info.angle < fov;
    }*/
};

Vector3 GetPosition2(uintptr_t ent)
{
    return Memory::Read<Vector3>(ent + offsets::LOCAL_ORIGIN);
}

Vector3 getBonePos(uintptr_t ent, int id)
    {
        long basePointer = ent;
        Vector3 pos =GetPosition2(ent);
        uintptr_t bones = Memory::Read<uintptr_t>(basePointer + offsets::bones);
        Vector3 bone = {};
        uint32_t boneloc = (id * 0x30);
        bone_t boneOffset = {};
        boneOffset = Memory::Read<bone_t>(bones + boneloc);

        bone.x = boneOffset.x + pos.x;
        bone.y = boneOffset.y + pos.y;
        bone.z = boneOffset.z + pos.z;
        return bone;
    }
double calculateDistance2D(float x1, float y1, float x2, float y2)
    {
        float dx = (x1 - x2);
        float dy = (y1 - y2);
        float distance = sqrt(pow(dx, 2) + pow(dy, 2));
        return distance;
    }


float calculateDesiredYaw(Vector3 local, Vector3 enemy)
    {
		Vector3 locationDelta {enemy.x - local.x, enemy.y - local.y};

        const float yawInRadians = atan2(locationDelta.y, locationDelta.x);
        const float yawInDegrees = yawInRadians * (180 / M_PI);
        return yawInDegrees;
    }
double calculateDesiredPitch(Vector3 local, Vector3 enemy)
    {
        const float locationDeltaZ = enemy.z - local.z;
        const float distanceBetweenPlayers = calculateDistance2D(enemy.x, enemy.y, local.x, local.y);
        const float pitchInRadians = atan2(-locationDeltaZ, distanceBetweenPlayers);
        const float pitchInDegrees = pitchInRadians * (180 / M_PI);
        return pitchInDegrees;
    }