#pragma once
#include "../Module.h"
#include "../../Utils/Utils.h"
#include "../Manager/ModuleManager.h"

class Glow : public Module {
private:
    int oldDelay = 0;

public:
    Glow();
    void OnEnable() override;
    void OnDisable() override;
    void Update() override;

    void hand_model_glow();
    Vector3 Rainbow(float delay);
};