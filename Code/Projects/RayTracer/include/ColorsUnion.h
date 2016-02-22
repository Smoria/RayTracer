#pragma once
#include <cstdint>

union ColorsUnion
{
    uint32_t combined;
    uint8_t colors[4];
};