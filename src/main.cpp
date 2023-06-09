#include <iostream>
#include <thread>

#include "Modules/Glow/Glow.h"
#include "Utils/Utils.h"
#include "Memory/Memory.h"
#include "Keys/KeyManager.h"
#include "Modules/Manager/ModuleManager.h"
#include "sdk/LocalPlayer.h"
#include "sdk/Entity.h"
#include "sdk/Level.h"
#include "sdk/EntityList.h"

#include "Modules/NoRecoil/NoRecoil.h"
#include "Modules/Glow/Glow.h"
#include "Modules/AimAssist/AimAssist.h"

#include "Menu/Menu.h"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main() {
    srand(time(NULL));

    // Check if the user is the root user
    RootCheck();

    // Check if it's safe to mess with Apex Legends
    SafetyCheck();

    // Wait for the R5Apex.exe process
    pid_t processId = WaitForProcess("R5Apex.exe"); //WaitForProcess("R5Apex.exe");
    std::cout << "Found R5Apex.exe with process id: " << processId << std::endl;

    // Retrieve the address for the r5apex.exe module
    unsigned long moduleBase = Memory::GetModuleBase(processId, "r5apex.exe");
    std::cout << "Found r5apex.exe module at address: " << std::hex << moduleBase << std::dec << std::endl;

    EntityList::Populate();

    std::cout << "Waiting 5 seconds (please move to Apex)..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    Menu menu = Menu("Ragnarok", SCR_WIDTH, SCR_HEIGHT);

    // Add some modules
    ModuleManager::AddModule(new NoRecoil());
    ModuleManager::AddModule(new Glow());
   //ModuleManager::AddModule(new AimAssist());

    while(menu.IsRunning()) {
        // Update global variables
        menu.Render();

        LocalPlayer::Update();
        EntityList::Update();

        KeyManager::Update();
        ModuleManager::Update();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}