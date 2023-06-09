#pragma once

class Module {
    bool running;
    const char* name;
public:
    // Default Module constructor
    Module(bool defaultRunning, const char* name);

    // Functions for accessing private variables
    const char* GetName();
    bool IsRunning();
    void SetRunning(bool running);

    // Virtual functions to override
    virtual void OnEnable();
    virtual void OnDisable();
    virtual void Update();
};