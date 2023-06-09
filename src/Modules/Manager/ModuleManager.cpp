#include "ModuleManager.h"
#include <iostream>
#include <string.h>
#include <vector>

int ModuleManager::spectatorcount = 0;

float ModuleManager::minRecoil = 40.f;
float ModuleManager::maxRecoil = 50.f;

Vector3 ModuleManager::visible = { 0.f, 255.f, 0.f };
Vector3 ModuleManager::hidden = { 255.f, 0.f, 0.f };

bool ModuleManager::healthBased = false;

/*
    //Recoil Values
    extern float minRecoil = 64.f;
    extern float maxRecoil = 70.f;

    //Glow Values
    extern Vector3 visible = { 255.f, 0.f, 0.f };
    extern Vector3 hidden = { 255.f, 0.f , 0.f };
    extern bool healthBased = false;

*/

// A vector that holds all the added Modules
std::vector<Module*> modules;

void ModuleManager::AddModule(Module* module) {
    // Push the given module to the vector
    modules.push_back(module);
}

void ModuleManager::RemoveModule(const char* name) {
    // Loop over all the module instances in modules
    for(int i = 0; i < modules.size(); i++) {
        Module* module = modules.at(i);

        // Check if the module's name is equal to the given name
        if(strcmp(module->GetName(), name) == 0) {
            // Remove the item at the index equal to the current itteration
            modules.erase(modules.begin() + i);
            break;
        }
    }
}

void ModuleManager::EnableModule(const char* name) {
    // Loop over all the module instances in modules
    for(Module* module : modules) {
        // Check if the module's name is not equal to the given name
        if(strcmp(module->GetName(), name) != 0)
            continue;

        // Check if it is already running
        if(module->IsRunning())
            return;

        // Else enable the module, and set running to true
        module->OnEnable();
        module->SetRunning(true);
    }
}

void ModuleManager::DisableModule(const char* name) {
    // Loop over all the module instances in modules
    for(Module* module : modules) {
        // Check if the module's name is not equal to the given name
        if(strcmp(module->GetName(), name) != 0)
            continue;

        // Check if the module is already disabled
        if(module->IsRunning() == false)
            return;

        // Else disable the module and set running to false
        module->OnDisable();
        module->SetRunning(false);
    }
}

void ModuleManager::ToggleModule(const char* name) {
    // Loop over all the module instances in modules
    for(Module* module : modules) {
        // Check if the module's name is not equal to the given name
        if(strcmp(module->GetName(), name) != 0)
            continue;
        
        // If the module is running, disable it, else enable it
        if(module->IsRunning()) {
            module->OnDisable();
            module->SetRunning(false);
        } else {
            module->OnEnable();
            module->SetRunning(true);
        }
    }
}

void ModuleManager::Update() {
    // Loop over all the modules
    for(Module* module : modules) {
        // If they're running, call their update method
        if(module->IsRunning())
            module->Update();
    }
}