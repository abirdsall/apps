#include "fw.h"

namespace fw
{
    static Pool<RadiosityCube> _radiosityCubes;
    
    void InitRadiosityCubes()
    {
        _radiosityCubes.Init( 256 );
    }
    
    void KillRadiosityCubes()
    {
    }
    
    RadiosityCube* RadiosityCubeNew()
    {
        return _radiosityCubes.New();
    }
    
    void RadiosityCubeDelete( RadiosityCube* cube )
    {
        _radiosityCubes.Delete( cube );
    }
}
