#pragma once
#include <vector>
#include <iostream>
#include "../sdk/Types/Vector.h"
#include "../sdk/Types/LastViewAngles.h"
#include <string>

namespace LocalPlayer
{
    extern unsigned long m_basePointer;
    extern std::vector<LastViewAngles> m_LastViewAngles;
    
    std::string GetName();
    Vector3 GetPosition();
    
    int GetHealth();
    int GetTeamNumber();
    
    bool IsAlive();
    bool IsKnocked();
    
    bool IsAttacking();
    bool IsZooming();
    
    void setViewAngle(Vector3 angle);
    void setYaw(float angle);

    Vector3 getViewAngle();
    Vector3 getPunchAngle();

    void addLastViewAngles(float curtime);
    void purgeLastView(float curtime);

    void Update();
}