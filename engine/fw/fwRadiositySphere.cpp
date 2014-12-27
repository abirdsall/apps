#include "fw.h"

namespace fw
{
    static Pool<RadiositySphere> _radiositySpheres;
    
    void InitRadiositySpheres()
    {
        _radiositySpheres.Init( 32 );
    }
    
    void KillRadiositySpheres()
    {
    }
    
    RadiositySphere* RadiositySphereNew()
    {
        return _radiositySpheres.New();
    }
    
    void RadiositySphereDelete( RadiositySphere* sphere )
    {
        _radiositySpheres.Delete( sphere );
    }
    
    void RadiositySphere::Delete()
    {
        if(_batchFilled != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( _batchFilled );
            _batchFilled = InvalidDrawBatchHandle;
        }
        
        if(_batchRadiosity != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( _batchRadiosity );
            _batchRadiosity = InvalidDrawBatchHandle;
        }
        
        RadiositySphereDelete( this );
    }
}
