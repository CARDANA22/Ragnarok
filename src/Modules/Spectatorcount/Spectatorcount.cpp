#include "Spectatorcount.h"

#include "../../Memory/Memory.h"
#include "../../Offsets.h"
#include "../../sdk/Types/LastViewAngles.h"

#include "../../sdk/Level.h"
#include "../../sdk/LocalPlayer.h"
#include "../../sdk/EntityList.h"

Spectatorcount::Spectatorcount() : Module(true, "Spectatorcount") {}

void Spectatorcount::OnEnable() {
}

void Spectatorcount::OnDisable() {
}

void Spectatorcount::Update() {
    m_Spectatorcount = 0;

    if (!Level::isPlayable())
        return;

    if (!LocalPlayer::IsAlive())
        return;
    
    currentTime = Memory::Read<float>(Memory::GetLastModuleBase() + Offsets::GlobalVars + 0x10);
    LocalPlayer::addLastViewAngles(currentTime);
    LocalPlayer::purgeLastView(currentTime);

    for (int i = 0; i < EntityList::GetSize(); i++)
    {
        
        Entity *ent = EntityList::GetEntity(i);
        if (ent == nullptr)
                continue;

        float targetangle = ent->getViewAngle().y;
        float targetyaw = -targetangle; // yaw is inverted
        if (targetyaw < 0)
            targetyaw += 360;
        targetyaw += 90; // yaw is off by 90
        if (targetyaw > 360)
            targetyaw -= 360;

        for(int i = 0; i < LocalPlayer::m_LastViewAngles.size(); i++)
        {
            LastViewAngles t = LocalPlayer::m_LastViewAngles.at(i);
            
            float localangle = t.angle.y;
            float localyaw = -localangle; // yaw is inverted
            if (localyaw < 0)
                localyaw += 360;
            localyaw += 90; // yaw is off by 90
            if (localyaw > 360)
                localyaw -= 360;
            
            if (targetyaw == localyaw && ent->getHealth() == 0)
                m_Spectatorcount++;
        }
    }

    ModuleManager::spectatorcount = m_Spectatorcount;

}