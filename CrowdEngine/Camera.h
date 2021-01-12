#ifndef CRWD_CAMERA_H_INCLUDED
#define CRWD_CAMERA_H_INCLUDED

#include "CRWDMath.h"
#include "Export.h"

namespace Crowd
{

//
// Camera
//
class Camera
{
	public:
	CRWD_EXPORT Camera();
	CRWD_EXPORT virtual ~Camera();

	CRWD_EXPORT virtual bool IsVisible(const AABB &box) const;

	CRWD_EXPORT virtual Matrix GetProjection() const;
	CRWD_EXPORT virtual Matrix GetView() const;
	CRWD_EXPORT virtual Matrix GetViewProj() const;

	protected:
	Plane  m_frustumPlanes[6]; // near, far, left, right, top, bottom

	Matrix m_projection;
	Matrix m_view;
	Matrix m_viewProj;
};


//
//	PerspectiveCamera
//
class PerspectiveCamera : public Camera
{
	public:
	CRWD_EXPORT PerspectiveCamera();
	CRWD_EXPORT virtual ~PerspectiveCamera();

	CRWD_EXPORT virtual void SetParameters(float fovY, float aspect, float zn, float zf);
	CRWD_EXPORT virtual void SetOrientation(Vector3 &cameraPosition, float xAngle, float yAngle, float zAngle);
	CRWD_EXPORT virtual void SetOrientation(Vector3 &cameraPosition, Vector3 &cameraTarget, Vector3 &cameraUpVector);

	CRWD_EXPORT virtual Vector3 GetPosition() const;

	struct Parameters
	{
		float fovY;
		float aspectRatio;
		float nearZ;
		float farZ;
	};
	CRWD_EXPORT virtual void GetParameters(Parameters &param) const;

	protected:
	CRWD_EXPORT virtual void ExtractFrustumPlanes();

	protected:
	Vector3 m_position;

	Parameters m_parameters;
};


//
//	OrthogonalCamera
//
class OrthographicCamera : public Camera
{
	public:
	CRWD_EXPORT OrthographicCamera();
	CRWD_EXPORT virtual ~OrthographicCamera();

	CRWD_EXPORT virtual void SetOrientation(Vector3 &cameraPosition, float xAngle, float yAngle, float zAngle);
	CRWD_EXPORT virtual void SetOrientation(Vector3 &cameraPosition, Vector3 &cameraTarget, Vector3 &cameraUpVector);

	CRWD_EXPORT virtual Vector3 GetPosition() const;

	struct Parameters
	{
		float width;
		float height;
		float depth;
	};

	CRWD_EXPORT virtual void SetParameters(float width, float height, float depth);
	CRWD_EXPORT virtual void GetParameters(Parameters &param) const;

	protected:
	CRWD_EXPORT virtual void ExtractFrustumPlanes();

	protected:
	Vector3 m_position;

	Parameters m_parameters;
};


//
//	CameraFunct
//
class CameraFunct
{
	public:
	CRWD_EXPORT CameraFunct(const Camera &camera);

	CRWD_EXPORT bool operator()(const AABB &);

	protected:
	const Camera &m_camera;
};

} // namespace Crowd

#endif // CRWD_CAMERA_H_INCLUDED