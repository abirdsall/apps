#include "fw.h"

namespace fw
{
    void Init()
    {
        InitDrawBatches();
        InitQuad2dBatches();
        
        DrawInit();
        SystemFontInit();
    }
    
    void Kill()
    {
        KillQuad2dBatches();
        KillDrawBatches();
        
        DrawKill();
    }    
}

