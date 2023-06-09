#pragma once
#include "../Module.h"
#include "../../Utils/Utils.h"
#include "../Manager/ModuleManager.h"

class Glow : public Module {
public:
    Glow();
    void OnEnable() override;
    void OnDisable() override;
    void Update() override;
};