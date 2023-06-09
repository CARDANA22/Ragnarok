#pragma once

#include <iostream>

#include "../sdk/Types/Vector.h"


class Entity
{
private:
    int m_entityListIndex;
    float m_lastVisibleTime;
    long m_basePointer = 0;

public:
    Entity(int entityListIndex);

    bool isDead();
    bool isKnocked();
    std::string getName();
    Vector3 getViewAngle();
    bool isValid();
    bool isVisible();

    uintptr_t getBase();

    std::string getSignifier();
    int getHealth();
    int getShield();
    int getItemId();
    Vector3 GetPosition();
    int GetTeamNumber();
    float getLastVisibleTime();
    Vector3 getBonePos(int id);
    
    void setGlow(int glowEnable, int glowThroughWall, Vector3 pColor, float glowDistance);
    
    void ResetPointer();
    void Update();
};
