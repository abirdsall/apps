#ifndef FW_H
#define FW_H

#include "os.h"
#include "gs.h"

#include "fwCamera.h"
#include "fwCanvasViewer.h"
#include "fwComponent.h"
#include "fwComponentFactory.h"
#include "fwCube.h"
#include "fwCubeComponent.h"
#include "fwDraw.h"
#include "fwLight.h"
#include "fwMaterial.h"
#include "fwMesh.h"
#include "fwObject.h"
#include "fwObjectFactory.h"
#include "fwRect.h"
#include "fwRenderer.h"
#include "fwString.h"

namespace fw
{
    void Init();
    void Kill();
}

#endif