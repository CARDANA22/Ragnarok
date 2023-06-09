#include "Utils.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <cmath>

#include "../Memory/Memory.h"
#include "../sdk/Types/Vector.h"

bool RootCheck() {
    // Return true if the user is root
    if(getuid() == 0)
        return true;

    // Let the user know that they have to run it as root
    std::cout << "Please run this binary as the root user!" << std::endl;
    
    // Exit the program
    exit(1);
}

bool SafetyCheck() {
    // Check if the process hiding rootkit is installed
    std::ifstream hyde("/usr/local/lib/RagnarokHyde.so");

    // If it isn't exit the program
    if (!hyde.good())
    {
        std::cout << "Please use the loader to start Ragnarok!" << std::endl;
        return false;
    }

    return true;
}

int WaitForProcess(const char* processName) {
    // Create a string to hold the ouput text
    std::string processStr = "Waiting for ";
    processStr.append(processName);

    // Output some information
    std::cout << processStr.c_str();

    // Wait for the process to exist
    do {
        // Output a dot to let the user know it's still waiting
        std::cout << ".";
        // Flush the stdout cache
        std::cout.flush();
        // Wait for a few seconds
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while(Memory::GetProcessId(processName) == 0);

    // Move on to the next line
    std::cout << std::endl;

    // Return the last found process id
    return Memory::GetLastProcessId();
}

float distanceToMeters(float distance)
{
    const float INCHES_TO_METER_RATE = 39.3701;
    return distance / INCHES_TO_METER_RATE;
}
