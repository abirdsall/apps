#ifndef FW_LIGHT
#define FW_LIGHT

#include "os.h"
#include "gs.h"
#include "aabb.h"

namespace fw
{
	typedef ::u32 LightHandle;
	
	const u32 kLightInvalid = kMaxU32;
	const u32 kLightLimit = 64;
	
	struct Light
	{
		v3 mPosition;
		v3 mColour;
		bool mActive;
	};
	
	enum DrawPhase
	{
		DrawPhasePreVoxelPass,
		DrawPhaseMake,
		DrawPhaseFinal
	};

	const aabb& LightGetBounds();
	
	void DrawLights( DrawPhase phase );
	void InitLights( const aabb& bounds );
	void KillLights();
	
	LightHandle LightNew( const v3& position, const v3& colour );
	void LightDelete( LightHandle handle );
	void LightSetPosition( LightHandle handle, const v3& position );
	const v3& LightGetPosition( LightHandle handle );
	const v3& LightGetColour( LightHandle handle );
	s32 LightActiveCount();
	s32 LightActiveGet( LightHandle array[ kLightLimit ] );
}

#endif