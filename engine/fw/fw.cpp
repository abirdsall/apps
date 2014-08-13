#include "fw.h"

namespace fw
{
    void Init()
    {
        DrawInit();
        SystemFontInit();
    }
    
    void Kill()
    {
        DrawKill();
    }    
}

