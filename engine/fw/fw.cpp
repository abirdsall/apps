#include "fw.h"

namespace fw
{
    void Init()
    {
        InitSceneNodes();
        InitDrawBatches();
        InitQuad2dBatches();
        
        SystemFontInit();
    }
    
    void Kill()
    {
        KillSceneNodes();
        KillQuad2dBatches();
        KillDrawBatches();
    }
}

