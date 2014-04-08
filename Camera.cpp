#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{ 
}


void Camera::SetFOV(float f) {
	VerticalFOV = f;
}
void Camera::SetAspect(float a) {
	Aspect = a;
}
void Camera::SetResolution(int x, int y) {
	XRes = x;
	YRes = y;
}
void Camera::LookAt(Vector3 &pos, Vector3 &target, Vector3 &up) {
	WorldMatrix.d = pos;
	WorldMatrix.c = WorldMatrix.d - target;
	WorldMatrix.c.Normalize();
	WorldMatrix.a.Cross(up, WorldMatrix.c);
	WorldMatrix.a.Normalize();
	WorldMatrix.b.Cross(WorldMatrix.c, WorldMatrix.a);
}

void Camera::Render(Scene &s) {
	for (int y = 0; y < YRes; ++y) {
		for (int x = 0; x < XRes; ++x) {
			RenderPixel(x, y, s);
		}
	}
}
void Camera::SaveBitmap(char *filename) {
	BMP.SaveBMP(filename);
}

void Camera::RenderPixel(int x, int y, Scene &s) {
	Ray sentRay;
	sentRay.Origin = WorldMatrix.d;
	
	//compute ray direction:
	Vector3 modifiedB = WorldMatrix.b;
	modifiedB.Scale(tan(VerticalFOV/2));
	Vector3 modifiedA = WorldMatrix.a;
	modifiedA.Scale(Aspect * tan(VerticalFOV / 2));
	Vector3 topLeft = WorldMatrix.d - WorldMatrix.c + modifiedB + modifiedA;

	Intersection hit;
	if (!s.Intersect(sentRay, hit)) {
		//no hits so skycolor:
		BMP.SetPixel(x, y, s.GetSkyColor().ToInt());
	}

	//compute color with lighting:
}
