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
        f32 testa;
        SceneNode* _parent;

        Array<SceneNodeComponent*> _components;
        Array<SceneNode*> _children;

        m4 _worldTransform;
        
        m4 _localTransform;
        
        v3 _modelScale;//?
        
        SceneNode()
        {
            testa = 3.0f;
            _parent = Null;
            testb = 4.0f;
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
//                printf("compr %f %f\n", component->testa, component->testb);
                component->Render( renderer, *this );
            }
            
            for( s32 i = 0; i < _children.Count(); i++ )
            {
                SceneNode* child = _children[ i ];
//                printf("child\n");
                child->Render( renderer );
            }
        }
        f32 testb;
    };
}

#endif
