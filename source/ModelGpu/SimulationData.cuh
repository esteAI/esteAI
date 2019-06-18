#pragma once

#include "Base.cuh"
#include "Definitions.cuh"

struct SimulationData
{
    int2 size;

    Cell **cellMap;
    Particle **particleMap;

    ArrayController<Cluster*> clusterPointers;
    ArrayController<Cluster*> clusterPointersTemp;
    ArrayController<Cluster> clusters;
    ArrayController<Cell*> cellPointers;
    ArrayController<Cell*> cellPointersTemp;
    ArrayController<Cell> cells;
    ArrayController<Cell> cellsTemp;
    ArrayController<Token*> tokenPointers;
    ArrayController<Token*> tokenPointersTemp;
    ArrayController<Token> tokens;
    ArrayController<Token> tokensTemp;
    ArrayController<Particle> particles;
    ArrayController<Particle> particlesNew;

    CudaNumberGenerator numberGen;
};

