

#include "Triangle.h"
#include <iostream>


Triangle::Triangle() {};

bool Triangle::Intersect(const Ray &ray, Intersection &hit) const {
	Matrix34 M = Matrix34();
	Vector3 triA, triB, triC;
	triA = Vtx[0]->Position;
	triB = Vtx[1]->Position;
	triC = Vtx[2]->Position;
	M.a = -ray.Direction;
	M.b = triB - triA;
	M.c = triC - triA;
	

	Vector3 bMa, cMa, crossResult, pMa;
	crossResult = bMa = triB - triA;
	cMa = triC - triA;
	pMa = ray.Origin - triA;

	//det(M) = -d dot ((b-a) cross (c-a))
	crossResult.Cross(crossResult, cMa);
	float DetM = (-ray.Direction).Dot(crossResult);

	//check if ray is parallel to the plane:
	if (DetM == 0) return false;

	//alpha = -d dot ((p - a) cross (c - a)) /det(M)
	crossResult = pMa;
	crossResult.Cross(crossResult, cMa);
	float alpha = (-ray.Direction).Dot(crossResult) / DetM;

	//check if alpha is in range
	if (alpha < 0 || alpha > 1) return false;

	//beta = -d dot (( b - a) cross (p - a)) / det(M)
	crossResult = bMa;
	crossResult.Cross(crossResult, pMa);
	float beta = (-ray.Direction).Dot(crossResult) / DetM;

	//check if beta is in range and if alpha+beta is in range
	if (beta < 0 || beta > 1 || (alpha+beta > 1)) return false;

	// t = (p - a) dot ((b - a) cross (c - a)) / det(M)
	crossResult = bMa;
	crossResult.Cross(crossResult, cMa);
	float t = pMa.Dot(crossResult) / DetM;

	//make sure that t is positive and it is less than hitdistance:
	if (t < 0 && t >= hit.HitDistance) return false;

	hit.HitDistance = t;
	hit.Mtl = Mtl;
	hit.Position = ray.Origin;
	hit.Position.AddScaled(ray.Direction, t);
	
	//normal:
	Vector3 nA = Vtx[0]->Normal;
	nA.Scale(1 - alpha - beta);
	Vector3 nB = Vtx[1]->Normal;
	nB.Scale(1 - alpha - beta);
	Vector3 nC = Vtx[2]->Normal;
	nC.Scale(1 - alpha - beta);
	hit.Normal = nA + nB + nC;
	//std::cout << "hit" << std::endl;
	return true;
}