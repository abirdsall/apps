#include "fw.h"

namespace fw
{
    void Init()
    {
        InitSceneNodes();
        InitLights();
        InitDrawBatches();
        InitQuad2dBatches();
        
        SystemFontInit();
    }
    
    void Kill()
    {
        KillSceneNodes();
        KillLights();
        KillQuad2dBatches();
        KillDrawBatches();
    }
}

