#ifndef FW_RADIOSITY_RENDERER
#define FW_RADIOSITY_RENDERER

namespace fw
{
    struct RadiosityRenderer : Xenderer
    {
        aabb _bounds;
        
        s32 _voxelCountX;
        s32 _voxelCountY;
        s32 _voxelCountZ;
        
        gs::CanvasHandle _voxelCanvasHd;
        gs::CanvasHandle _voxelCanvasHdScratch;
        gs::CanvasHandle _voxelCanvasSd;
        gs::CanvasHandle _voxelCanvasSdScratch;
        gs::CanvasHandle _voxelCanvasSdLod1;
        gs::CanvasHandle _voxelCanvasSdLod1Scratch;
        gs::CanvasHandle _voxelCanvasSdLod2;
        gs::CanvasHandle _voxelCanvasSdLod2Scratch;
        gs::CanvasHandle _lightCanvas;
        
        gs::TextureHandle _voxelTextureHd;
        gs::TextureHandle _voxelTextureHdScratch;
        gs::TextureHandle _voxelTextureSd;
        gs::TextureHandle _voxelTextureSdScratch;
        gs::TextureHandle _lightTextureColour;
        gs::TextureHandle _lightTextureDirection;
        
        gs::ShaderHandle _shaderBlurX;
        gs::ShaderHandle _shaderBlurY;
        gs::ShaderHandle _shaderBlurZ;
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
        
        gs::ShaderHandle MakeBlurShader( s32 axis, s32 layerCount );
        
        void Kill();
        
        void Render();
        
        void BlurLayers(gs::TextureHandle textureX,
                        gs::CanvasHandle canvasX,
                        gs::TextureHandle textureY,
                        gs::CanvasHandle canvasY,
                        gs::TextureHandle textureZ,
                        gs::CanvasHandle canvasZ,
                        gs::ShaderHandle shaderX,
                        gs::ShaderHandle shaderY,
                        gs::ShaderHandle shaderZ,
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
    };
}

#endif