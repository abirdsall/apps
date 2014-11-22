#ifndef FW_TEXTURE_VIEWER
#define FW_TEXTURE_VIEWER

namespace fw
{
	class TextureViewer
	{
	private:
		enum NavLevel
		{
			NavLevelTexture,
			NavLevelLayer,
			NavLevelLod,
			NavLevelCount
		};
		
		enum Channel
		{
			ChannelR = 0,
			ChannelG = 1,
			ChannelB = 2,
			ChannelA = 3,
			ChannelNone = 4,
			ChannelTypes = 5,
			ChannelCount = ChannelNone
		};
		
		const c8* sChannelLookupNames[ ChannelTypes ] =
		{
			"sample.r",
			"sample.g",
			"sample.b",
			"sample.a",
			"0"
		};
		
		static const u32 ChannelMixerLimit = ChannelTypes * ChannelTypes * ChannelTypes;
		gs::ShaderHandle _texture2dShaders[ ChannelMixerLimit ];
		gs::ShaderHandle _texture3dShaders[ ChannelMixerLimit ];
		bool _active;
		NavLevel _navLevel;
		s32 _navIndex[ NavLevelCount ];
		
		u32 MakeShaderIndex( Channel outR, Channel outG, Channel outB );
		gs::ShaderHandle MakeShader( Channel outR, Channel outG, Channel outB, bool is2d );
		void DrawHighlight( const Rect& rect );
		void DrawTexture( const Rect& rect, gs::Canvas& canvas, s32 textureIndex, s32 layerIndex, s32 lodIndex, bool highlighted );
        void DrawTexture( const Rect& rect, gs::TextureHandle textureHandle, s32 layerIndex, s32 lodIndex, bool highlighted );

	public:
		TextureViewer();
		
		void Init();
		void Tick();
		void Draw();
		void Kill();
	
		bool Active();
		void SetActive( bool active );
	};
}

#endif
