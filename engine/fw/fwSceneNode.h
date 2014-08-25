namespace fw
{
    typedef ::hwInt SceneNodeHandle;
    
    static const ::hwInt InvalidSceneNodeHandle = ( SceneNodeHandle )kNull;
    
    void InitSceneNodes();
    void KillSceneNodes();
    
    SceneNodeHandle SceneNodeNew();
    
    void SceneNodeDelete( SceneNodeHandle handle );

    struct SceneNode
    {
        SceneNodeHandle _parent;
        
        Array<SceneNodeHandle> _children;

        m4 _worldTransform;
        
        m4 _localTransform;
        
        v3 _modelScale;//?
        
        SceneNode()
        {
            _parent = InvalidSceneNodeHandle;
        }
        
        ~SceneNode()
        {
            for( s32 i = 0; i < _children.Count(); i++ )
            {
                SceneNodeDelete( _children[ i ] );
            }
        }
        
        void AddChild( SceneNodeHandle child )
        {
            _children.Add( child );
        }
        
        void Tick( float dt )
        {
            if( _parent != InvalidSceneNodeHandle )
            {
                SceneNode* parent = ( SceneNode* )_parent;
                _worldTransform = parent->_worldTransform * _localTransform;
            }
            else
            {
                _worldTransform = _localTransform;
            }
            
            for( s32 i = 0; i < _children.Count(); i++ )
            {
                SceneNode* child = ( SceneNode* )_children[ i ];
                child->Tick( dt );
            }
        }
    };
}
