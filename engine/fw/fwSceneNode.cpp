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
    
    SceneNode::~SceneNode()
    {
        for( s32 i = 0; i < _components.Count(); i++ )
        {
            _components[ i ]->Delete();
        }
        
        for( s32 i = 0; i < _children.Count(); i++ )
        {
            SceneNodeDelete( _children[ i ] );
        }
    }
}
