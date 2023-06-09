#pragma once
#include <cstdint>

struct GlowMode {
    uint8_t GeneralGlowMode, // 0x0
            BorderGlowMode, // 0x8
            BorderSize, //0x10
            TransparentLevel;  //0x18
};