#include "fw.h"

namespace fw
{
    static Pool<RadiosityCube> _radiosityCubes;
    
    void InitRadiosityCubes()
    {
        _radiosityCubes.Init( 32 );
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
    
    void RadiosityCube::Delete()
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
        
        RadiosityCubeDelete( this );
    }
}
