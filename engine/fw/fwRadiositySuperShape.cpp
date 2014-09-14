#include "fw.h"

namespace fw
{
    static Pool<RadiositySuperShape> _radiositySuperShapes;
    
    void InitRadiositySuperShapes()
    {
        _radiositySuperShapes.Init( 256 );
    }
    
    void KillRadiositySuperShapes()
    {
    }
    
    RadiositySuperShape* RadiositySuperShapeNew()
    {
        return _radiositySuperShapes.New();
    }
    
    void RadiositySuperShapeDelete( RadiositySuperShape* shape )
    {
        _radiositySuperShapes.Delete( shape );
    }
}
