#include "Level.h"

#include <iostream>

#include "../Memory/Memory.h"
#include "../Offsets.h"

std::string Level::getName()
{
    return Memory::ReadString(Memory::GetLastModuleBase() + Offsets::LevelName);
}

bool Level::isPlayable()
{
    if (Level::getName().empty())
            return false;
    if (Level::getName().compare("mp_lobby") == 0)
        return false;
    return true;
}

bool Level::isTrainingArea()
{
    if (Level::getName().compare("mp_rr_canyonlands_staging") == 0)
        return true;
    return false;
}