#ifndef FW_RADIOSITY_RENDERER_MONO
#define FW_RADIOSITY_RENDERER_MONO

namespace fw
{
    struct RadiosityRendererMono : Renderer
    {
        aabb _bounds;
        
        bool _voxelising;
        
        s32 _voxelCountX;
        s32 _voxelCountY;
        s32 _voxelCountZ;
        
        s32 _passesPacked;
        s32 _passesUnpacked;
        
        Array<gs::CanvasHandle> _voxelCanvasesHdPacked;
        Array<gs::CanvasHandle> _voxelCanvasesHdPackedScratch;
        Array<gs::CanvasHandle> _voxelCanvasesSdPacked;
        Array<gs::CanvasHandle> _voxelCanvasesSdPackedScratch;
        Array<gs::CanvasHandle> _voxelCanvasesSdPackedLod1;
        Array<gs::CanvasHandle> _voxelCanvasesSdPackedLod1Scratch;
        Array<gs::CanvasHandle> _voxelCanvasesSdPackedLod2;
        Array<gs::CanvasHandle> _voxelCanvasesSdPackedLod2Scratch;
        Array<gs::CanvasHandle> _voxelCanvasesHd;
        Array<gs::CanvasHandle> _voxelCanvasesSd;
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
        gs::ShaderHandle _shaderUnswizzle;
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
        
        bool Voxelising()
        {
            return _voxelising;
        }
    };
}

#endif