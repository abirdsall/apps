#ifndef FW_SCENE_NODE
#define FW_SCENE_NODE

namespace fw
{
    struct SceneNode;

    void InitSceneNodes();
    void KillSceneNodes();
    SceneNode* SceneNodeNew();
    void SceneNodeDelete( SceneNode* node );

    struct SceneNode
    {
        SceneNode* _parent;

        Array<SceneNodeComponent*> _components;
        Array<SceneNode*> _children;

        m4 _worldTransform;
        
        m4 _localTransform;
        
        v3 _modelScale;//?
        
        SceneNode()
        {
            _parent = Null;
        }
        
        ~SceneNode()
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

        void AddComponent( SceneNodeComponent* component )
        {
            _components.Add( component );
        }

        void AddChild( SceneNode* child )
        {
            _children.Add( child );
        }
        
        void Tick( float dt )
        {
            if( _parent != Null )
            {
                SceneNode* parent = ( SceneNode* )_parent;
                _worldTransform = parent->_worldTransform * _localTransform;
            }
            else
            {
                _worldTransform = _localTransform;
            }
            
            for( s32 i = 0; i < _components.Count(); i++ )
            {
                _components[ i ]->Tick( dt );
            }
            
            for( s32 i = 0; i < _children.Count(); i++ )
            {
                _children[ i ]->Tick( dt );
            }
        }
        
        void Render( Renderer& renderer )
        {
            for( s32 i = 0; i < _components.Count(); i++ )
            {
                _components[ i ]->Render( renderer, *this );
            }
            
            for( s32 i = 0; i < _children.Count(); i++ )
            {
                _children[ i ]->Render( renderer );
            }
        }
    };
}

#endif
