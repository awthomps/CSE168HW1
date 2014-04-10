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
	//TODO: make sure that the creation of these points is correct
	//TODO: make sure that the triangle intersection is happening correctly.
	//create the corners of the rendering window:
	Vector3 modifiedB = WorldMatrix.b;
	modifiedB.Scale(tan((VerticalFOV / 2) * PI / 180));
	Vector3 modifiedA = WorldMatrix.a;
	double hfov = 2 * atan(Aspect * tan((VerticalFOV / 2) * PI / 180)) * 180 / PI;
	modifiedA.Scale(atan(hfov / 2) * PI / 180);
	//modifiedA.Scale(Aspect * tan((VerticalFOV / 2) * PI / 180));

	topLeft = WorldMatrix.d - WorldMatrix.c + modifiedB - modifiedA;
	topLeft.Print();
	topRight = WorldMatrix.d - WorldMatrix.c + modifiedB + modifiedA;
	topRight.Print();
	bottomLeft = WorldMatrix.d - WorldMatrix.c - modifiedB - modifiedA;
	bottomLeft.Print();
	right = (topRight - topLeft);
	rightDelta = right.Magnitude() / XRes;
	right.Normalize();
	down = (bottomLeft - topRight);
	downDelta = down.Magnitude() / YRes;
	down.Normalize();

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

	sentRay.Direction = topRight;
	sentRay.Direction.AddScaled(right, (x + 0.5) * rightDelta);
	sentRay.Direction.AddScaled(down, (y + 0.5) * downDelta);
	sentRay.Direction = sentRay.Direction - WorldMatrix.d;
	sentRay.Direction.Normalize();
	//sentRay.Direction.Print();

	Intersection hit;
	if (!s.Intersect(sentRay, hit)) {
		//no hits so skycolor:
		BMP.SetPixel(x, y, s.GetSkyColor().ToInt());
	}
	else {
		Color newColor = Color(0.0,0.0,0.0);
		//compute color with lighting:
		for (int i = 0; i < s.GetNumLights(); ++i) {
			Color lightColor, matColor;
			Vector3 toLight, ItPos, in, out, currentColor;
			float intensity = s.GetLight(i).Illuminate(hit.Position, lightColor, toLight, ItPos);
			hit.Mtl->ComputeReflectance(matColor, in, out, hit);
			lightColor.Multiply(matColor);
			currentColor = lightColor.getInVector3();
			currentColor.Dot(-toLight);
			currentColor.Dot(hit.Normal);
			currentColor.Scale(intensity);
			newColor.Add(currentColor);
		}
		BMP.SetPixel(x, y, newColor.ToInt());
	}
}
