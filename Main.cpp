#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <thread>
#include "NoRecoile.cpp"
#include "Utils.cpp"
#include "Offsets.cpp"
#include "Level.cpp"
#include "LocalPlayer.cpp"
#include "Sense.cpp"
#include "Player.cpp"
#include "Aimbot.cpp"



int main(int argc, char *argv[])
{

    if (getuid())
    {
        printf("MUST RUN AS ROOT!\n");
        return -1;
    }
    if (Memory::GetProcessId("R5Apex.exe") == 0)
    {
        printf("GAME NOT FOUND. EXITING!\n");
        return -1;
    }
    Level *level = new Level();
    LocalPlayer *localPlayer = new LocalPlayer();
    std::vector<Player *> *players = new std::vector<Player *>;

    for (int i = 0; i < 60; i++)
    {
        players->push_back(new Player(i));
    }

    Aimbot *aimbot = new Aimbot(level, localPlayer, players);
    Sense *sens = new Sense(level, localPlayer, players);
    NoRecoil *no = new NoRecoil(level, localPlayer, players);

    // Main loop
    printf("MYAPEX STARTING MAIN LOOP\n");
    int counter = 0;
    while (1)
    {
        try
        {
            // resolve pointers
            localPlayer->markForPointerResolution();

            // run features
                aimbot->update();
                sens->update();
                no->update();
                sens->hand_model_glow();
                //sens->NightMode();

            // all ran fine
            if (counter % 500 == 0){
                system("clear");
                printf("UPDATE[%d] OK. \n", counter);
                sens->printSpec();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        catch (...)
        {
            printf("UPDATE[%d] ERROR (LOADING SCREEN?). SLEEPING FOR 10 SECONDS\n", counter);
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        counter++;
        if (counter > 1000)
            counter = 0;
    }
}


