#ifndef FW_FONT
#define FW_FONT

#include "fw.h"

namespace fw
{
    void SystemFontInit();
    void SystemFontDraw( core::String& text, v2 position, v4 colour );
}

#endif