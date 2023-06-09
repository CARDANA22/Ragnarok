#pragma once
#include "../Module.h"
#include "../Manager/ModuleManager.h"

class Spectatorcount : public Module {
    int m_Spectatorcount = 0;
    float currentTime = 0;
public:
    Spectatorcount();
    void OnEnable() override;
    void OnDisable() override;
    void Update() override;
};