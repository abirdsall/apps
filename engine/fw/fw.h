#ifndef FW_H
#define FW_H

#include "os.h"
#include "gs.h"

#include "intersect.h"
#include "frustum.h"
#include "aabb.h"

#include "fwRect.h"

#include "fwComponent.h"
#include "fwObject.h"
#include "fwMaterial.h"

#include "fwCamera.h"
#include "fwCanvasViewer.h"
#include "fwComponentFactory.h"
#include "fwCube.h"
#include "fwCubeComponent.h"
#include "fwDraw.h"
#include "fwDrawLines.h"
#include "fwFont.h"
#include "fwLight.h"
#include "fwMesh.h"
#include "fwObjectFactory.h"

#include "fwRenderer.h"

#include "fwShader.h"
#include "fwDrawBatch.h"
#include "fwQuad2d.h"
#include "fwQuad2dBatch.h"

namespace fw
{
    void Init();
    void Kill();
}

#endif