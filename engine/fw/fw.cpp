#include "fw.h"

namespace fw
{
    void Init()
    {
        InitDrawBatches();
        InitQuad2dBatches();
        
        SystemFontInit();
    }
    
    void Kill()
    {
        KillQuad2dBatches();
        KillDrawBatches();
    }
}

