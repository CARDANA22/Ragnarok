#pragma once
#include "../Module.h"
#include "../../sdk/Entity.h"
#include "../../sdk/Types/Vector.h"

class AimAssist : public Module {
private:
    Vector3 aimPos;
    const float activationFOv = 10;
    const int smoothing = 10;
    const float range = 50.f;

    Entity* getClosestEnemy();
    void AimAt(Vector3 target);
    float calculateAngleDelta(float oldAngle, float newAngle);
    float flipYawIfNeeded(float angle);

public:
    AimAssist();
    void OnEnable() override;
    void OnDisable() override;
    void Update() override;

};