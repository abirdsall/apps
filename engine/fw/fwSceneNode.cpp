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
    
    SceneNode* SceneNodeNew()
    {
        return _sceneNodes.New();
    }
    
    void SceneNodeDelete( SceneNode* node )
    {
        _sceneNodes.Delete( node );
    }
}
