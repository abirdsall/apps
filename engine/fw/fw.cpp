#include "fw.h"

namespace fw
{
    void Init()
    {
        InitSceneNodes();
        InitLights();
        InitDrawBatches();
        InitCubeBatches();
        InitQuad2dBatches();
        InitRadiosityCubes();
        InitRadiositySpheres();
        
        SystemFontInit();
    }
    
    void Kill()
    {
        InitRadiositySpheres();
        KillRadiosityCubes();
        KillQuad2dBatches();
        KillCubeBatches();
        KillDrawBatches();
        KillLights();
        KillSceneNodes();
    }
}

