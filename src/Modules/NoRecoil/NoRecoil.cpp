#include "NoRecoil.h"

#include <random>

#include "../../sdk/Level.h"
#include "../../sdk/LocalPlayer.h"
#include "../../sdk/Entity.h"

NoRecoil::NoRecoil() : Module(true, "NoRecoil") {}

void NoRecoil::OnEnable() {
}

void NoRecoil::OnDisable() {
}

float RandomFloat(float min, float max) {
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
} 

void NoRecoil::Update() {
    if (!Level::isPlayable())
        return;

    if (!LocalPlayer::IsAlive())
        return;

    if (LocalPlayer::IsKnocked())
        return;

    float punchPitchDelta = 0;
    float punchYawDelta = 0;

    Vector3 punchAngle = LocalPlayer::getPunchAngle();

    // adjust pitch
    if (punchAngle.x != 0)
    {
        punchPitchDelta = (punchAngle.x - m_previousPunchPitch) * RandomFloat(ModuleManager::minRecoil / 100.f, ModuleManager::maxRecoil / 100.f);
        m_previousPunchPitch = punchAngle.x;
        
        if(LocalPlayer::IsAttacking())
            LocalPlayer::setViewAngle({LocalPlayer::getViewAngle().x - punchPitchDelta, LocalPlayer::getViewAngle().y});
    }

    // adjust yaw
    if (punchAngle.y != 0)
    {
        punchYawDelta = (punchAngle.y - m_previousPunchYaw) * RandomFloat(ModuleManager::minRecoil / 100.f, ModuleManager::maxRecoil / 100.f);
        m_previousPunchYaw = punchAngle.y;
        
        if(LocalPlayer::IsAttacking())
            LocalPlayer::setViewAngle({LocalPlayer::getViewAngle().x, LocalPlayer::getViewAngle().y - punchYawDelta});
    }
}