#include "Module.h"
#include <iostream>

// Set the private variables to those given in the constructor
Module::Module(bool defaultRunning, const char* name)
    : running(defaultRunning), name(name) {}

// Return the private name variable
const char* Module::GetName() {
    return name;
}

// Return if the module is running
bool Module::IsRunning() {
    return running;
}

// Set if the module should be run or not
void Module::SetRunning(bool running) {
    this->running = running;
}

// Default Module methods (should be overriden)
void Module::OnEnable() {
    std::cout << "Enabling " << name << "!" << std::endl;
}

void Module::OnDisable() {
    std::cout << "Disabling " << name << "!" << std::endl;
}

void Module::Update() {
    std::cout << "Update " << name << "!" << std::endl;
}