#pragma once
#include <vector>
#include "Entity.h"

namespace EntityList {
    extern std::vector<Entity*>* entityList;

    Entity* GetEntity(int index);
    int GetSize();

    void Populate();
    void Update();
}