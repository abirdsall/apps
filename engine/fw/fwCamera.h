#ifndef FW_CAMERA
#define FW_CAMERA

namespace fw
{
	typedef ::u32 CameraHandle;
	
	const u32 kCameraInvalid = kMaxU32;
	const u32 kCameraLimit = 4;
	
	struct Camera
	{
		void ( *mDraw )( void );
		m4 mMatrixProject;
		m4 mMatrixModel;
		m4 mMatrixModelInverse;
		m4 mMatrixModelProject;
		frustum mFrustum;
		Rect mWindow;
		v3 mFocus;
		f32 mFocalDistance;
		f32 mOrbitDistance;
		f32 mFovX;
		bool mActive;
	};
	
	void InitCameras();
	void KillCameras();
	void TickCameras( f32 dt );
	void DrawCameras();
	
	CameraHandle CameraNew( const Rect& window, void ( *draw )( void ) );
	void CameraDelete( const CameraHandle handle );
	void CameraSet( const CameraHandle handle );
	const Camera& CameraGet( const CameraHandle handle );
	
	f32 CameraGetFocalLength(const CameraHandle handle);
	v3 CameraGetMouseDir(const CameraHandle handle);
	v3 CameraGetMouseVel(const CameraHandle handle);
	v3 CameraGetDir(const CameraHandle handle);
	v3 CameraGetPos(const CameraHandle handle);
	void CameraSetFov(const CameraHandle handle, const f32 fov);
	void CameraSetFocalLength(const CameraHandle handle, const f32 focalLength);
	void CameraSetWindow(const CameraHandle handle, const Rect& window);
	void CameraSetPlanes(const CameraHandle handle, const Rect& window);
	void CameraSetModel(const CameraHandle handle, const m4& model);
	void CameraSetFocus(const CameraHandle handle, const v3& focus);
	v3 CameraUnproject(const CameraHandle handle, const v3& position);
	bool CameraVisible(const CameraHandle handle, const v3& position);
	bool CameraVisible(const CameraHandle handle, const aabb& box);
	bool CameraVisibleFast(const CameraHandle handle, const aabb& box);
	bool CameraVisible(const CameraHandle handle, const line& line, f32& enter, f32& exit);
}

#endif

/*
 
 f32 fov = mFovX;
 
 if(osKeyboard().getKeyHeld(OS_KEY_LEFT))
 {
 fov += 1.0f * dt;
 if(fov > kPi) fov = kPi;
 }
 else if(osKeyboard().getKeyHeld(OS_KEY_RIGHT))
 {
 fov -= 1.0f * dt;
 if(fov < 0.1f) fov = 0.1f;
 }
 
 mFocalDistance = 0.5f;
 if(abs(fov - mFovX) > 0.001f)
 {
 f32 targetDistance = mFocalDistance;
 f32 distanceToTarget = mOrbitDistance - targetDistance;
 if(distanceToTarget > 0.0f)
 {
 f32 frustrumWidthAtTarget = distanceToTarget * (2.0f * math::tan(mFovX * 0.5f));
 f32 distanceToTargetRequired = frustrumWidthAtTarget / (2.0f * math::tan(fov * 0.5f));
 f32 newZoom = targetDistance + distanceToTargetRequired;
 if(newZoom > targetDistance)
 {
 mOrbitDistance = newZoom;
 viewManager::get()->setFov(viewManager::get()->getHandle(this), fov);
 }
 }
 //viewManager::get()->setFov(viewManager::get()->getHandle(this), fov);
 //mMatrixModel.setPosition(mFocus - mMatrixModel.rows[2].xyz() * mOrbitDistance);
 }
 }
 
 void view :: draw(void)
 {
 if(mDraw)
 {
 gs::Put();
 gs::SetMatrixP(mMatrixProject);
 //m4 pj = orthogonal(-0.1f, 0.1f, 0.1f, -0.1f, 1.0f, -1.0f);
 //gsSetMatrixP(pj);
 gs::SetMatrixM(mMatrixModel);
 gs::SetViewport(mWindow);
 mDraw();
 #if 0
 gs::Put();
 gs::Set2d();
 gsFillText(rect(0.0f, 0.0f, osWindow().sizeX(), osWindow().sizeY()), GS_ALIGN_BOTTOM | GS_ALIGN_LEFT, v4(1.0f, 1.0f, 1.0f, 1.0f), v4(0.0f, 0.0f, 0.0f, 1.0f),
 osString( "focal length = %.0fmm\nfov = %.0fdeg\n%fm",
 viewManager::get()->getFocalLength(viewManager::get()->getHandle(this)),
 mFovX * (180.0f / kPi),
 mOrbitDistance ).getString(),
 1.0f);
 gs::Pop();
 #endif
 
 */