#pragma once

#include "SimulationParameters.h"
#include "GpuSettings.h"

struct Settings
{
    int worldSizeX;
    int worldSizeY;
    SimulationParameters simulationParameters;
    GpuSettings gpuSettings;
};
