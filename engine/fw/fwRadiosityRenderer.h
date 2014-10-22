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
        
        gs::CanvasHandle _voxelCanvasHd;
        gs::CanvasHandle _voxelCanvasHdScratch;
        gs::CanvasHandle _voxelCanvasSd;
        gs::CanvasHandle _voxelCanvasSdScratch;
        gs::CanvasHandle _voxelCanvasH2;
        gs::CanvasHandle _voxelCanvasH2Scratch;
        gs::CanvasHandle _voxelCanvasS2;
        gs::CanvasHandle _voxelCanvasS2Scratch;
        gs::CanvasHandle _voxelCanvasSdLod1;
        gs::CanvasHandle _voxelCanvasSdLod1Scratch;
        gs::CanvasHandle _voxelCanvasSdLod2;
        gs::CanvasHandle _voxelCanvasSdLod2Scratch;
        
        Array<gs::CanvasHandle> _lightCanvases;
        
        gs::TextureHandle _voxelTextureHd;
        gs::TextureHandle _voxelTextureHdScratch;
        gs::TextureHandle _voxelTextureSd;
        gs::TextureHandle _voxelTextureSdScratch;
        gs::TextureHandle _lightTextureColour;
        gs::TextureHandle _lightTextureDirection;
        
        gs::ShaderHandle _shaderBlurX;
        gs::ShaderHandle _shaderBlurY;
        gs::ShaderHandle _shaderBlurZ;
        gs::ShaderHandle _shaderBlu2X;
        gs::ShaderHandle _shaderBlu2Y;
        gs::ShaderHandle _shaderBlu2Z;
        gs::ShaderHandle _shaderBlurXHalf;
        gs::ShaderHandle _shaderBlurYHalf;
        gs::ShaderHandle _shaderBlurZHalf;
        gs::ShaderHandle _shaderBlurXQuarter;
        gs::ShaderHandle _shaderBlurYQuarter;
        gs::ShaderHandle _shaderBlurZQuarter;

        gs::ShaderHandle _shaderVoxelise;
        gs::ShaderHandle _shaderLuminise;
        gs::ShaderHandle _shaderForward;
        gs::ShaderHandle _shaderFillZ;

        void Init( const aabb& bounds, s32 voxelCountX, s32 voxelCountY, s32 voxelCountZ );
        
        gs::ShaderHandle MakeBlurShader( s32 axis, s32 targetCount, f32 zMin, f32 zStep );
        
        void Kill();
        
        void Render();
        
        void BlurLayers(gs::TextureHandle textureX,
                        gs::TextureHandle textureY,
                        gs::TextureHandle textureZ,
                        gs::CanvasHandle canvasX,
                        gs::CanvasHandle canvasY,
                        gs::CanvasHandle canvasZ,
                        gs::CanvasHandle canva2X,
                        gs::CanvasHandle canva2Y,
                        gs::CanvasHandle canva2Z,
                        gs::ShaderHandle shaderX,
                        gs::ShaderHandle shaderY,
                        gs::ShaderHandle shaderZ,
                        gs::ShaderHandle shade2X,
                        gs::ShaderHandle shade2Y,
                        gs::ShaderHandle shade2Z,
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