#pragma once
#include "../Module.h"
#include "../Manager/ModuleManager.h"

class NoRecoil : public Module {
    double m_previousPunchPitch = 0;
    double m_previousPunchYaw = 0;
public:
    NoRecoil();
    void OnEnable() override;
    void OnDisable() override;
    void Update() override;
};