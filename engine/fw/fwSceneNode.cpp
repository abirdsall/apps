#include "fw.h"

namespace fw
{
    static Pool<SceneNode> _sceneNodes;
    
    void InitSceneNodes()
    {
        _sceneNodes.Init( 8192 );
    }
    
    void KillSceneNodes()
    {
        
    }
    
    SceneNodeHandle SceneNodeNew()
    {
        SceneNode* node = _sceneNodes.Alloc();

        return ( SceneNodeHandle )node;
    }
    
    void SceneNodeDelete( SceneNodeHandle handle )
    {
        SceneNode* node = ( SceneNode* )handle;
        
        _sceneNodes.Free( node );
    }
}
