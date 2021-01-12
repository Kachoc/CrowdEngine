#include "Camera.h"

namespace Crowd
{

//
//	Camera
//

Camera::Camera() {}

Camera::~Camera() {}

bool Camera::IsVisible(const AABB &box) const 
{
	//Vector3 P;
	Vector3 Q;

	for(unsigned int i = 0; i < 6; i++)
	{
		for(unsigned int j = 0; j < 3; j++)
		{
			if(m_frustumPlanes[i][j] > 0)
			{
				//P[j] = box.MinPoint[j];
				Q[j] = box.MaxPoint[j];
			}
			else
			{
				//P[j] = box.MaxPoint[j];
				Q[j] = box.MinPoint[j];
			}
		}

		if(Plane::Dot(m_frustumPlanes[i], Q) < 0)
		{
			return false;
		}
	}
	return true;
}

Matrix Camera::GetProjection() const
{
	return m_projection;
}

Matrix Camera::GetView() const
{
	return m_view;
}

Matrix Camera::GetViewProj() const
{
	return m_viewProj;
}

//
//	PerspectiveCamera
//

PerspectiveCamera::PerspectiveCamera()
{
	SetParameters((float) CRWD_PI*0.25, 800.0f/640.0f, 1.0f, 1000.0f);
	m_view = Matrix::Identity();
	m_viewProj = m_view*m_projection;
	ExtractFrustumPlanes();
}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::SetParameters(float fovY, float aspect, float zn, float zf)
{
	m_parameters.fovY = fovY;
	m_parameters.aspectRatio = aspect;
	m_parameters.nearZ = zn;
	m_parameters.farZ = zf;
	Matrix::BuildProjectionLH(m_projection, fovY, aspect, zn, zf);

	//m_viewProj = m_view*m_projection;
	//ExtractFrustumPlanes();
}

void PerspectiveCamera::SetOrientation(Vector3 &cameraPosition, float xAngle, float yAngle, float zAngle)
{
	m_position = cameraPosition;

	yAngle = yAngle/180.0f*(float)CRWD_PI;
	xAngle = xAngle/180.0f*(float)CRWD_PI;
	zAngle = zAngle/180.0f*(float)CRWD_PI;

	Vector4 xAxis(1.0f, 0, 0, 0), yAxis(0, 1.0f, 0, 0), zAxis(0, 0, 1.0f, 0);
	
	Matrix zRotation(	cos(zAngle), sin(zAngle), 0, 0,
						-sin(zAngle), cos(zAngle), 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
					);

	Matrix xRotation(	1, 0, 0, 0,
						0, cos(xAngle), sin(xAngle), 0,
						0, -sin(xAngle), cos(xAngle), 0,
						0, 0, 0, 1
					);

	Matrix yRotation(	cos(yAngle), 0, -sin(yAngle), 0,
						0, 1, 0, 0,
						sin(yAngle), 0, cos(yAngle), 0,
						0, 0, 0, 1
					);

	Matrix rotation = yRotation*xRotation*zRotation;
	rotation.Transpose();

	xAxis = xAxis*rotation;
	yAxis = yAxis*rotation;
	zAxis = zAxis*rotation;

	m_view = Matrix(
	 xAxis.x,								yAxis.x,									zAxis.x,									0,
	 xAxis.y,								yAxis.y,									zAxis.y,									0,
	 xAxis.z,								yAxis.z,									zAxis.z,									0,
	-Vector3::Dot(xAxis, cameraPosition),	-Vector3::Dot(yAxis, cameraPosition),		-Vector3::Dot(zAxis, cameraPosition),		1
	);

	m_viewProj = m_view*m_projection;

	ExtractFrustumPlanes();
}

void PerspectiveCamera::SetOrientation(Vector3 &cameraPosition, Vector3 &cameraTarget, Vector3 &cameraUpVector)
{
	m_position = cameraPosition;

	Vector3 zAxis = Vector3::Normalize(cameraTarget - cameraPosition);
	Vector3 xAxis = Vector3::Normalize(Vector3::Cross(cameraUpVector, zAxis));
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

	m_view = Matrix(
	 xAxis.x,								yAxis.x,									zAxis.x,									0,
	 xAxis.y,								yAxis.y,									zAxis.y,									0,
	 xAxis.z,								yAxis.z,									zAxis.z,									0,
	-Vector3::Dot(xAxis, cameraPosition),	-Vector3::Dot(yAxis, cameraPosition),		-Vector3::Dot(zAxis, cameraPosition),		1
	);


	m_viewProj = m_view*m_projection;

	ExtractFrustumPlanes();
}

Vector3 PerspectiveCamera::GetPosition() const
{
	return m_position;
}

void PerspectiveCamera::GetParameters(Parameters &param) const
{
	param = m_parameters;
}

void PerspectiveCamera::ExtractFrustumPlanes()
{
	Vector4 col0 (m_viewProj(0, 0), m_viewProj(1, 0), m_viewProj(2, 0), m_viewProj(3, 0));
	Vector4 col1 (m_viewProj(0, 1), m_viewProj(1, 1), m_viewProj(2, 1), m_viewProj(3, 1));
	Vector4 col2 (m_viewProj(0, 2), m_viewProj(1, 2), m_viewProj(2, 2), m_viewProj(3, 2));
	Vector4 col3 (m_viewProj(0, 3), m_viewProj(1, 3), m_viewProj(2, 3), m_viewProj(3, 3));

	m_frustumPlanes[0] = (Plane&) (col2);			// near
	m_frustumPlanes[1] = (Plane&) (col3 - col2);	// far
	m_frustumPlanes[2] = (Plane&) (col3 + col0);	// left
	m_frustumPlanes[3] = (Plane&) (col3 - col0);	// right
	m_frustumPlanes[4] = (Plane&) (col3 - col1);	// top
	m_frustumPlanes[5] = (Plane&) (col3 + col1);	// bottom

	for(unsigned int i = 0; i < 6; i++)
	{
		m_frustumPlanes[i].Normalize();
	}
}


//
//	OrthographicCamera
//
OrthographicCamera::OrthographicCamera()
{
	SetParameters(100.0f, 1.0f, 1.0f);
	m_view = Matrix::Identity();
	m_viewProj = m_view*m_projection;
}

OrthographicCamera::~OrthographicCamera()
{

}

void OrthographicCamera::SetOrientation(Vector3 &cameraPosition, float xAngle, float yAngle, float zAngle)
{
	m_position = cameraPosition;

	yAngle = yAngle/180.0f*(float)CRWD_PI;
	xAngle = xAngle/180.0f*(float)CRWD_PI;
	zAngle = zAngle/180.0f*(float)CRWD_PI;

	Vector4 xAxis(1.0f, 0, 0, 0), yAxis(0, 1.0f, 0, 0), zAxis(0, 0, 1.0f, 0);
	
	Matrix zRotation(	cos(zAngle), sin(zAngle), 0, 0,
						-sin(zAngle), cos(zAngle), 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
					);

	Matrix xRotation(	1, 0, 0, 0,
						0, cos(xAngle), sin(xAngle), 0,
						0, -sin(xAngle), cos(xAngle), 0,
						0, 0, 0, 1
					);

	Matrix yRotation(	cos(yAngle), 0, -sin(yAngle), 0,
						0, 1, 0, 0,
						sin(yAngle), 0, cos(yAngle), 0,
						0, 0, 0, 1
					);

	Matrix rotation = yRotation*xRotation*zRotation;
	rotation.Transpose();

	xAxis = xAxis*rotation;
	yAxis = yAxis*rotation;
	zAxis = zAxis*rotation;

	m_view = Matrix(
	 xAxis.x,								yAxis.x,									zAxis.x,									0,
	 xAxis.y,								yAxis.y,									zAxis.y,									0,
	 xAxis.z,								yAxis.z,									zAxis.z,									0,
	-Vector3::Dot(xAxis, cameraPosition),	-Vector3::Dot(yAxis, cameraPosition),		-Vector3::Dot(zAxis, cameraPosition),		1
	);

	m_viewProj = m_view*m_projection;

	ExtractFrustumPlanes();
}

void OrthographicCamera::SetOrientation(Vector3 &cameraPosition, Vector3 &cameraTarget, Vector3 &cameraUpVector)
{
	m_position = cameraPosition;

	Vector3 zAxis = Vector3::Normalize(cameraTarget - cameraPosition);
	Vector3 xAxis = Vector3::Normalize(Vector3::Cross(cameraUpVector, zAxis));
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

	m_view = Matrix(
	 xAxis.x,								yAxis.x,									zAxis.x,									0,
	 xAxis.y,								yAxis.y,									zAxis.y,									0,
	 xAxis.z,								yAxis.z,									zAxis.z,									0,
	-Vector3::Dot(xAxis, cameraPosition),	-Vector3::Dot(yAxis, cameraPosition),		-Vector3::Dot(zAxis, cameraPosition),		1
	);


	m_viewProj = m_view*m_projection;

	ExtractFrustumPlanes();
}

Vector3 OrthographicCamera::GetPosition() const
{
	return m_position;
}

void OrthographicCamera::SetParameters(float width, float height, float depth)
{
	m_parameters.width = width;
	m_parameters.height = height;
	m_parameters.depth = depth;

	m_projection = Matrix(
	 2.0f/width,	0,			0,			0,
	 0,			2.0f/height,	0,			0,
	 0,			0,			1.0f/depth,		0,
	 0,			0,			0,				1.0f
	);

	m_viewProj = m_view*m_projection;

	ExtractFrustumPlanes();
}

void OrthographicCamera::GetParameters(Parameters &param) const
{
	param = m_parameters;
}

void OrthographicCamera::ExtractFrustumPlanes()
{
	/*Vector3 lookDir(m_view._13, m_view._23, m_view._33);
	Vector3 lookUp(m_view._12, m_view._22, m_view._32);
	Vector3 lookRight(m_view._11, m_view._21, m_view._31);
	lookDir.Normalize();
	lookUp.Normalize();
	lookRight.Normalize();

	m_frustumPlanes[0] = Plane(m_position, lookDir);											// near
	m_frustumPlanes[1] = Plane(m_position + lookDir*m_parameters.depth, -lookDir);				// far
	m_frustumPlanes[2] = Plane(m_position - lookRight*m_parameters.width/2.0f, lookRight);		// left
	m_frustumPlanes[3] = Plane(m_position + lookRight*m_parameters.width/2.0f, -lookRight);		// right
	m_frustumPlanes[4] = Plane(m_position + lookUp*m_parameters.height/2.0f, -lookUp);			// top
	m_frustumPlanes[5] = Plane(m_position - lookUp*m_parameters.height/2.0f, +lookUp);			// bottom

	for(unsigned int i = 0; i < 6; i++)
	{
		m_frustumPlanes[i].Normalize();
	}*/

	Vector4 col0 (m_viewProj(0, 0), m_viewProj(1, 0), m_viewProj(2, 0), m_viewProj(3, 0));
	Vector4 col1 (m_viewProj(0, 1), m_viewProj(1, 1), m_viewProj(2, 1), m_viewProj(3, 1));
	Vector4 col2 (m_viewProj(0, 2), m_viewProj(1, 2), m_viewProj(2, 2), m_viewProj(3, 2));
	Vector4 col3 (m_viewProj(0, 3), m_viewProj(1, 3), m_viewProj(2, 3), m_viewProj(3, 3));

	m_frustumPlanes[0] = (Plane&) (col2);			// near
	m_frustumPlanes[1] = (Plane&) (col3 - col2);	// far
	m_frustumPlanes[2] = (Plane&) (col3 + col0);	// left
	m_frustumPlanes[3] = (Plane&) (col3 - col0);	// right
	m_frustumPlanes[4] = (Plane&) (col3 - col1);	// top
	m_frustumPlanes[5] = (Plane&) (col3 + col1);	// bottom

	for(unsigned int i = 0; i < 6; i++)
	{
		m_frustumPlanes[i].Normalize();
	}
}


//
//	CameraFunct
//
CameraFunct::CameraFunct(const Camera &camera) : m_camera(camera)
{
	
}

bool CameraFunct::operator()(const AABB &box)
{
	return m_camera.IsVisible(box);
}

} // namespace Crowd