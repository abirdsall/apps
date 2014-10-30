#ifndef FW_H
#define FW_H

#include "os.h"
#include "gs.h"

#include "intersect.h"
#include "frustum.h"
#include "aabb.h"

#include "fwRect.h"
#include "fwShader.h"

#include "fwCamera.h"
#include "fwTextureViewer.h"
#include "fwFont.h"
#include "fwMesh.h"
#include "fwLight.h"

namespace fw
{
    struct Renderer;
}

#include "fwSceneNodeComponent.h"
#include "fwSceneNode.h"

#include "fwRenderer.h"
#include "fwCube.h"
#include "fwCubeBatch.h"
#include "fwSuperShape.h"
#include "fwRadiosityRenderer.h"
#include "fwRadiosityRendererMono.h"
#include "fwRadiosityCube.h"
#include "fwRadiositySuperShape.h"

#include "fwDrawBatch.h"
#include "fwQuad2d.h"
#include "fwQuad2dBatch.h"

namespace fw
{
    void Init();
    void Kill();
}

#endif