#pragma once


#ifndef CSE168_CAMERA_H
#define CSE168_CAMERA_H

#include "Matrix34.h"
#include "Bitmap.h"
#include "Scene.h"

class Camera
{
public:
	Camera();
	~Camera();

	void SetFOV(float f);
	void SetAspect(float a);
	void SetResolution(int x, int y);
	void LookAt(Vector3 &pos, Vector3 &target, Vector3 &up);

	void Render(Scene &s);
	void SaveBitmap(char *filename);


private:

	int XRes, YRes;
	Matrix34 WorldMatrix;
	float VerticalFOV;
	float Aspect;
	Bitmap BMP;
};

#endif

