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
        
        SystemFontInit();
    }
    
    void Kill()
    {
        KillRadiosityCubes();
        KillQuad2dBatches();
        KillCubeBatches();
        KillDrawBatches();
        KillLights();
        KillSceneNodes();
    }
}

