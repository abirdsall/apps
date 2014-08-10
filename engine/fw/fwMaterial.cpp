#include "fw.h"

namespace fw
{
	Material::Material()
	{
		mColour = v4( 1.0f, 1.0f, 1.0f, 1.0f );
	}

	Material::Material( v4 colour )
	{
		mColour = colour;
	}
}