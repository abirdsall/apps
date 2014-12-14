#ifndef FW_SCENE_NODE
#define FW_SCENE_NODE

namespace fw
{
    struct SceneNode
    {
        SceneNode* _parent;

        Array<SceneNodeComponent*> _components;
        Array<SceneNode*> _children;

        m4 _worldTransform;
        m4 _localTransform;
        
        SceneNode()
        {
            _parent = Null;
            _localTransform = identity4();
        }
        
        ~SceneNode();

        void AddComponent( SceneNodeComponent* component )
        {
            _components.Add( component );
        }

        void AddChild( SceneNode* child )
        {
            child->_parent = this;
            
            _children.Add( child );
        }
        
        void SetLocalPosition( const v3& position )
        {
            _localTransform.setPosition( position );
        }

        void SetLocalScale( const v3& scale )
        {
            _localTransform.setScale( scale );
            
            // if z has changed then update thickness values for self and all children
        }

        void Tick( f32 dt )
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
                SceneNodeComponent* component = _components[ i ];
                component->Tick( dt, *this );
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
                SceneNodeComponent* component = _components[ i ];
                component->Render( renderer, *this );
            }
            
            for( s32 i = 0; i < _children.Count(); i++ )
            {
                SceneNode* child = _children[ i ];
                child->Render( renderer );
            }
        }
    };
    
    void InitSceneNodes();
    void KillSceneNodes();
    SceneNode* SceneNodeNew();
    void SceneNodeDelete( SceneNode* node );
}

#endif
