#pragma once
#include "../Module.h"

#include "../../sdk/Types/Vector.h"

namespace ModuleManager {

    //Spectator
    extern int spectatorcount;

    //Recoil Values
    extern float minRecoil;
    extern float maxRecoil;

    //Glow Values
    extern Vector3 visible;
    extern Vector3 hidden;
    extern bool healthBased;

    // Functions for adding and removing stuff to/from the vector
    void AddModule(Module* module);
    void RemoveModule(const char* name);

    // Functions to change a Module's state
    void EnableModule(const char* name);
    void DisableModule(const char* name);
    void ToggleModule(const char* name);

    // Update function that updates all Modules
    void Update();
}