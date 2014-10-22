#ifndef FW_CANVAS_VIEWER
#define FW_CANVAS_VIEWER

namespace fw
{
	class CanvasViewer
	{
	private:
		enum eNavLevel
		{
			eNavLevelCanvas,
			eNavLevelTexture,
			eNavLevelLayer,
			eNavLevelLod,
			eNavLevelCount
		};
		
		enum eChannel
		{
			eChannelR = 0,
			eChannelG = 1,
			eChannelB = 2,
			eChannelA = 3,
			eChannelNone = 4,
			eChannelTypes = 5,
			eChannelCount = eChannelNone
		};
		
		const c8* sChannelLookupNames[ eChannelTypes ] =
		{
			"sample.r",
			"sample.g",
			"sample.b",
			"sample.a",
			"0"
		};
		
		static const u32 kChannelMixerLimit = eChannelTypes * eChannelTypes * eChannelTypes;
		gs::ShaderHandle mTexture2dShaders[ kChannelMixerLimit ];
		gs::ShaderHandle mTexture3dShaders[ kChannelMixerLimit ];
		bool mActive;
		eNavLevel mNavLevel;
		s32 mNavIndex[ eNavLevelCount ];
		
		u32 MakeShaderIndex( eChannel outR, eChannel outG, eChannel outB );
		gs::ShaderHandle MakeShader( eChannel outR, eChannel outG, eChannel outB, bool is2d );
		void DrawHighlight( const Rect& rect );
		void DrawTexture( const Rect& rect, gs::Canvas& canvas, const s32 textureIndex, const s32 layerIndex, const s32 lodIndex, const bool highlighted );
		
	public:
		CanvasViewer();
		
		void Init();
		void Tick();
		void Draw();
		void Kill();
	
		bool Active();
		void SetActive( bool active );
	};
}

#endif
