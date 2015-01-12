#ifndef FW_RADIOSITY_RENDERER
#define FW_RADIOSITY_RENDERER

namespace fw
{
    struct RadiosityRenderer : Renderer
    {
        aabb _bounds;
        
        bool _voxelising;
        
        s32 _voxelCountX;
        s32 _voxelCountY;
        s32 _voxelCountZ;
        
        s32 _voxelCountPerPassZ;
        s32 _voxelCountPerPassHalfZ;
        s32 _voxelCountPerPassQuarterZ;
        s32 _passCountZ;
        s32 _passCountHalfZ;
        s32 _passCountQuarterZ;

        gs::TextureHandle _voxelTextureA;
        gs::TextureHandle _voxelTextureB;
        gs::TextureHandle _voxelTextureHalfA;
        gs::TextureHandle _voxelTextureHalfB;
        gs::TextureHandle _lightTextureColour;
        gs::TextureHandle _lightTextureDirection;

        Array<gs::CanvasHandle> _voxelCanvasesA;
        Array<gs::CanvasHandle> _voxelCanvasesB;
        Array<gs::CanvasHandle> _voxelCanvasesHalfA;
        Array<gs::CanvasHandle> _voxelCanvasesHalfB;
        Array<gs::CanvasHandle> _voxelCanvasesQuarterA;
        Array<gs::CanvasHandle> _voxelCanvasesQuarterB;
        Array<gs::CanvasHandle> _voxelCanvasesEighthA;
        Array<gs::CanvasHandle> _voxelCanvasesEighthB;
        Array<gs::CanvasHandle> _lightCanvases;
        
        gs::ShaderHandle _fillShader;

        Array<gs::ShaderHandle> _blurShadersX;
        Array<gs::ShaderHandle> _blurShadersY;
        Array<gs::ShaderHandle> _blurShadersZ;
        Array<gs::ShaderHandle> _blurShadersHalfX;
        Array<gs::ShaderHandle> _blurShadersHalfY;
        Array<gs::ShaderHandle> _blurShadersHalfZ;
        Array<gs::ShaderHandle> _blurShadersQuarterX;
        Array<gs::ShaderHandle> _blurShadersQuarterY;
        Array<gs::ShaderHandle> _blurShadersQuarterZ;

        gs::ShaderHandle _shaderVoxelise;
        gs::ShaderHandle _shaderLuminise;
        gs::ShaderHandle _shaderForward;
        
        bool _debugVoxels;
        s32 _debugVoxelLayer;
        gs::ShaderHandle _debugVoxelShader;

        void Init( const aabb& bounds, s32 voxelCountX, s32 voxelCountY, s32 voxelCountZ );
        
        gs::ShaderHandle MakeBlurShader( s32 axis, s32 targetCount, f32 zMin, f32 zStep );
        
        void Kill();
        
        void Tick( f32 dt );
        void Render( Camera* camera );
        
        void BlurLayers(gs::TextureHandle textureX,
                        gs::TextureHandle textureY,
                        gs::TextureHandle textureZ,
                        Array<gs::CanvasHandle>& canvasesX,
                        Array<gs::CanvasHandle>& canvasesY,
                        Array<gs::CanvasHandle>& canvasesZ,
                        Array<gs::ShaderHandle>& shadersX,
                        Array<gs::ShaderHandle>& shadersY,
                        Array<gs::ShaderHandle>& shadersZ,
                        s32 srcLod,
                        s32 dstLod,
                        u32 dstSizeX,
                        u32 dstSizeY,
                        u32 dstSizeZ,
                        f32 zBlur );
        
        void SetStageMatrices( bool tracing );
        
        const aabb Bounds()
        {
            return _bounds;
        }
        
        bool Voxelising()
        {
            return _voxelising;
        }
    };
}

#endif