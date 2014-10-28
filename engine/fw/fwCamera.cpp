#include "fw.h"

namespace fw
{
	static Camera sCamera[ kCameraLimit ];
	static CameraHandle sCameraActive;

	void InitCameras()
	{
		for( u32 i = 0; i < kCameraLimit; i++ )
		{
			sCamera[ i ].mActive = false;
		}
		sCameraActive = kCameraInvalid;
	}
	
	void KillCameras()
	{
		for( u32 i = 0; i < kCameraLimit;  i++ )
		{
			CameraDelete( i );
		}
	}
	
	static void CameraTickFly(Camera& camera, f32 dt)
	{
		v3 position = camera.mMatrixModel.getPosition();
		if( os::MouseButtonDown( os::eMouseButtonRight ) ) os::MouseSetVisibile( false );
		if( os::MouseButtonUp( os::eMouseButtonRight ) ) os::MouseSetVisibile( true );
		if( os::MouseButtonHeld( os::eMouseButtonRight ) )
		{
			v2 vel = os::MouseVelocity();
            if(core::abs(vel.x) > 0.00f || core::abs(vel.y) > 0.00f)
			{
				camera.mMatrixModel.setPosition(v3(0.0f, 0.0f, 0.0f));
				if(core::abs(vel.x) > 0.0f) camera.mMatrixModel = multiply33(camera.mMatrixModel, r4(V3UnitY, dt * 5e2f * vel.x));
				if(core::abs(vel.y) > 0.0f) camera.mMatrixModel = multiply33(camera.mMatrixModel, r4(camera.mMatrixModel.rows[0].xyz(), dt * 5e2f * -vel.y));
				camera.mMatrixModel.stabilise();
			}
		}
		
		if( os::KeyHeld( os::eKeyA ) ){ position -= camera.mMatrixModel.rows[0].xyz() * dt * 2.0f; printf(" A"); }
		if( os::KeyHeld( os::eKeyD ) ){ position += camera.mMatrixModel.rows[0].xyz() * dt * 2.0f; printf(" D"); }
		if( os::KeyHeld( os::eKeyS ) ){ position -= camera.mMatrixModel.rows[2].xyz() * dt * 2.0f; printf(" S"); }
		if( os::KeyHeld( os::eKeyW ) ){ position += camera.mMatrixModel.rows[2].xyz() * dt * 2.0f; printf(" W"); }
		camera.mMatrixModel.setPosition(position);
	}
	
	static void CameraTickOrbit(Camera& camera, f32 dt)
	{
		if( os::KeyHeld( os::eKeySpace ) )
		{
			if( os::MouseButtonHeld( os::eMouseButtonLeft ) )
			{
				v2 vel = os::MouseVelocity();
				camera.mMatrixModel = multiply33(camera.mMatrixModel, r4(V3UnitY, -4.0f * vel.x));
				camera.mMatrixModel = multiply33(camera.mMatrixModel, r4(camera.mMatrixModel.rows[0].xyz(), -4.0f * vel.y));
				if(dot(camera.mMatrixModel.rows[1].xyz(), V3UnitY) < 0.0f)
				{
					v3 limit = dot(camera.mMatrixModel.rows[2].xyz(), V3UnitY) > 0.0f ? V3UnitY : -V3UnitY;
					camera.mMatrixModel = multiply33(camera.mMatrixModel, r4(limit, camera.mMatrixModel.rows[2].xyz()));
				}
			}
			if( os::MouseButtonHeld( os::eMouseButtonRight ) )
			{
				camera.mOrbitDistance += os::MouseVelocity().y * 40.0f;
				if(camera.mOrbitDistance < 0.0f)
				{
					camera.mOrbitDistance = 0.0f;
				}
			}
			camera.mMatrixModel.setPosition(camera.mFocus - camera.mMatrixModel.rows[2].xyz() * camera.mOrbitDistance);
		}
	}
	
	void TickCameras( f32 dt )
	{
		for( int i = 0; i < kCameraLimit; i++ )
		{
			Camera& camera = sCamera[ i ];
			if( camera.mActive )
			{
				CameraTickOrbit( camera, dt );
			}
		}
	}
	
	void DrawCameras()
	{
		for( int i = 0; i < kCameraLimit; i++ )
		{
			Camera& camera = sCamera[ i ];
			if( camera.mActive )
			{
				if( camera.mDraw )
				{
					gs::Put();
					gs::SetMatrixP(camera.mMatrixProject);
					gs::SetMatrixM(camera.mMatrixModel);
					//ABX needs to cope with screen resizing gs::SetViewport(camera.mWindow.Min().x, camera.mWindow.Min().y, camera.mWindow.Max().x, camera.mWindow.Max().y);
					camera.mDraw();
					gs::Pop();
				}
			}
		}
	}
	
	CameraHandle CameraNew( const Rect& window, void ( *draw )( void ) )
	{
		for( u32 i = 0; i < kCameraLimit; i++ )
		{
			if( !sCamera[ i ].mActive )
			{
				CameraHandle handle = i;
				Camera& camera = sCamera[ handle ];
				camera.mMatrixModel = identity4();
				camera.mMatrixModel = look(v3(0.0f, 0.0f, 2.0f), v3(0.0f, 0.0f, 0.0f), v3(0.0f, 1.0f, 0.0f));
				camera.mWindow = window;
				camera.mDraw = draw;
				camera.mOrbitDistance = 2.0f;
				camera.mFocalDistance = 0.5f;
				camera.mActive = true;
				CameraSetFocalLength( handle, 85.0f);
				return handle;
			}
		}
		ASSERT(0);
		return kCameraInvalid;
	}
	
	void CameraDelete( const CameraHandle handle )
	{
		if( sCamera[ handle ].mActive )
		{
			sCamera[ handle ].mActive = false;
		}
	}
	
	void CameraSet( const CameraHandle handle )
	{
		sCameraActive = handle;
	}
	
	const Camera& CameraGet( const CameraHandle handle )
	{
		return sCamera[ handle ];
	}
	
	u32 CameraActive() // todo rename to applied?
	{
		return sCameraActive;
	}
	
	f32 CameraGetFocalLength(const CameraHandle handle)
	{
		Camera& camera = sCamera[ handle ];
		return 0.5f * ( 36.0f / core::tan( 0.5f * camera.mFovX ) );
	}
	
	v3 CameraGetMouseDir(const CameraHandle handle)
	{
		Camera& camera = sCamera[ handle ];
		v2 npos = ( os::MousePositionPixels() - camera.mWindow.Min() ) / camera.mWindow.Size();
		v2 pos = npos - 0.5f;
		f32 frustrumWidthAtZ1 = 2.0f * core::tan(camera.mFovX * 0.5f);
		v3 dir = camera.mMatrixModel.rows[0].xyz() * pos.x * frustrumWidthAtZ1 -
		camera.mMatrixModel.rows[1].xyz() * pos.y *(frustrumWidthAtZ1 * os::WindowAspect() ) +
		camera.mMatrixModel.rows[2].xyz();
		return normalise(dir);
	}
	
	v3 CameraGetMouseVel(const CameraHandle handle)
	{
		Camera& camera = sCamera[ handle ];
		v2 vel2d = os::MouseVelocity();
		v3 vel3d = camera.mMatrixModel.rows[0].xyz() * vel2d.x - camera.mMatrixModel.rows[1].xyz() * vel2d.y;
		return vel3d;
	}
	
	v3 CameraGetDir(const CameraHandle handle)
	{
		Camera& camera = sCamera[ handle ];
		return camera.mMatrixModel.rows[2].xyz();
	}
	
	v3 CameraGetPos(const CameraHandle handle)
	{
		Camera& camera = sCamera[ handle ];
		return camera.mMatrixModel.getPosition();
	}
	
	void CameraSetFov(const CameraHandle handle, const f32 fov)
	{
		Camera& camera = sCamera[ handle ];
		camera.mFovX = fov;
		camera.mMatrixProject = projection(fov, os::WindowAspect(), ZNear, ZFar);
	}
	
	void CameraSetFocalLength(const CameraHandle handle, const f32 focalLength)
	{
		// 35mm format is 36mm x 24mm
		f32 fov = 2.0f * core::atan(36.0f / (2.0f * focalLength)); // fovX in 35mm format
		//f32 fov = 2.0f * core::atan(24.0f / (2.0f * focalLength)); // fovY in 35mm format
		CameraSetFov( handle, fov );
	}
	
	void CameraSetWindow(const CameraHandle handle, const Rect& window)
	{
		Camera& camera = sCamera[ handle ];
		camera.mWindow = window;
	}
	
	void CameraSetPlanes(const CameraHandle handle, const Rect& window)
	{
		Camera& camera = sCamera[ handle ];
		plane* p = camera.mFrustum.mPlanes;
		line* l = camera.mFrustum.mLines;
		Rect w = window.Corrected();
		v2 wMin = w.Min();
		v2 wMid = w.Mid();
		v2 wMax = w.Max();
		
		v3 p0 = CameraUnproject(handle, v3(wMin.x, wMin.y, 0.0f));
		v3 p1 = CameraUnproject(handle, v3(wMin.x, wMin.y, 1.0f));
		v3 p2 = CameraUnproject(handle, v3(wMin.x, wMax.y, 0.0f));
		v3 p3 = CameraUnproject(handle, v3(wMin.x, wMax.y, 1.0f));
		v3 p4 = CameraUnproject(handle, v3(wMax.x, wMax.y, 0.0f));
		v3 p5 = CameraUnproject(handle, v3(wMax.x, wMax.y, 1.0f));
		v3 p6 = CameraUnproject(handle, v3(wMax.x, wMin.y, 0.0f));
		v3 p7 = CameraUnproject(handle, v3(wMax.x, wMin.y, 1.0f));
		v3 pn = CameraUnproject(handle, v3(wMid.x, wMid.y, 0.0f));
		v3 pf = CameraUnproject(handle, v3(wMid.x, wMid.y, 1.0f));
		
		l[0].setPath(p0, p1);
		l[1].setPath(p2, p3);
		l[2].setPath(p4, p5);
		l[3].setPath(p6, p7);
		
		p[0] = plane(normalise(pf - pn), pn);
		p[1] = plane(normalise(pn - pf), pf);
		p[2] = plane(normalise(cross(p0 - p1, p2 - p3)), p0); // left
		p[3] = plane(normalise(cross(p4 - p5, p6 - p7)), p4); // right
		p[4] = plane(normalise(cross(p2 - p3, p4 - p5)), p4); // top
		p[5] = plane(normalise(cross(p6 - p7, p0 - p1)), p0); // bottom
	}
	
	void CameraSetModel(const CameraHandle handle, const m4& model)
	{
		Camera& camera = sCamera[ handle ];
		camera.mMatrixModel = model;
	}
	
	void CameraSetFocus(const CameraHandle handle, const v3& focus)
	{
		Camera& camera = sCamera[ handle ];
		camera.mFocus = focus;
	}
	
	v3 CameraUnproject(const CameraHandle handle, const v3& position)
	{
		Camera& camera = sCamera[ handle ];
		v4 pos(position, 1.0f);
		v2 posXY = v2(pos.x, pos.y);
		posXY = camera.mWindow.Fraction( posXY );
		v3 posXYZ = v3(posXY.x, posXY.y, pos.z);
		posXYZ = posXYZ * 2.0f - 1.0f;
		posXYZ.y = -posXYZ.y;
		pos.x = posXYZ.x;
		pos.y = posXYZ.y;
		pos.z = posXYZ.z;
		pos	= !(camera.mMatrixProject * camera.mMatrixModel) * pos;
		return pos.w == 0.0f ? v3(0.0f, 0.0f, 0.0f) : pos.xyz() / pos.w;
	}
	
	bool CameraVisible(const CameraHandle handle, const v3& position)
	{
		Camera& camera = sCamera[ handle ];
		return intersect(position, camera.mFrustum);
	}
	
	bool CameraVisible(const CameraHandle handle, const aabb& box)
	{
		Camera& camera = sCamera[ handle ];
		return intersect(box, camera.mFrustum);
	}
	
	bool CameraVisibleFast(const CameraHandle handle, const aabb& box)
	{
		Camera& camera = sCamera[ handle ];
		return intersectFast(box, camera.mFrustum);
	}
	
	bool CameraVisible(const CameraHandle handle, const line& line, f32& enter, f32& exit)
	{
		Camera& camera = sCamera[ handle ];
		return intersect(line, camera.mFrustum, enter, exit);
	}
}