#include "fw.h"

#define DEMO_LIGHTING 1

#if DEMO_LIGHTING

#include "lighting.h"

int main( int argc, char *argv[] )
//int main( int argc, char *const argv[] )
{
	return os::Main( argc, argv, &lightingInit, &lightingTick, Null, &lightingDraw, Null,
#if OsIos
                    os::WindowFormat()
#else
                    os::WindowFormat( 512, 512, 8, 8, 8, 8, 24, false, false )
#endif
                    );
}

#endif
