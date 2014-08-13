#ifndef FW_H
#define FW_H

#include "os.h"
#include "gs.h"

#include "intersect.h"
#include "vector.h"
#include "matrix.h"
#include "frustum.h"
#include "aabb.h"
#include "cMath.h"
#include "vector.h"

#include "fwRect.h"
#include "fwPool.h"
#include "fwString.h"

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

namespace fw
{
    void Init();
    void Kill();
}

#endif