#include "stdafx.h"
#include "OBB_check.h"

using namespace std;
using namespace glm;


bool CheckOBBCollision(ST_OBB* Box1, ST_OBB* Box2)
{
	double c[3][3];
	double absC[3][3];
	double d[3];
	double r0, r1, r;

	int i;
	const double cutoff = 0.999999;
	bool existsParallelPair = false;
	vec3 diff = Box1->vCenterPos - Box2->vCenterPos;

	for (i = 0; i < 3; ++i)
	{
		c[0][i] = dot(Box1->vAxisDir[0], Box2->vAxisDir[i]);
		absC[0][i] = abs(c[0][i]);
		if (absC[0][i] > cutoff)
			existsParallelPair = true;
	}
	d[0] = dot(diff, Box1->vAxisDir[0]);
	r = abs(d[0]);
	r0 = Box1->fAxisLen[0];
	r1 = Box2->fAxisLen[0] * absC[0][0] + Box2->fAxisLen[1] * absC[0][1] +
		Box2->fAxisLen[2] * absC[0][2];
	if (r > r0 + r1)
		return false;


	for (i = 0; i < 3; ++i)
	{
		c[1][i] = dot((Box1->vAxisDir[1]), (Box2->vAxisDir[i]));
		absC[1][i] = abs(c[1][i]);
		if (absC[1][i] > cutoff)
			existsParallelPair = true;
	}
	d[1] = dot((diff), (Box1->vAxisDir[1]));
	r = abs(d[1]);
	r0 = Box1->fAxisLen[1];
	r1 = Box2->fAxisLen[0] * absC[1][0] + Box2->fAxisLen[1] * absC[1][1] +
		Box2->fAxisLen[2] * absC[1][2];
	if (r > r0 + r1)
		return false;

	for (i = 0; i < 3; ++i)
	{
		c[2][i] = dot((Box1->vAxisDir[0]), (Box2->vAxisDir[i]));
		absC[2][i] = abs(c[2][i]);
		if (absC[2][i] > cutoff)
			existsParallelPair = true;
	}
	d[2] = dot((diff), (Box1->vAxisDir[2]));
	r = abs(d[2]);
	r0 = Box1->fAxisLen[2];
	r1 = Box2->fAxisLen[0] * absC[2][0] + Box2->fAxisLen[1] * absC[2][1] +
		Box2->fAxisLen[2] * absC[2][2];
	if (r > r0 + r1)
		return false;

	r = abs(dot((diff), (Box2->vAxisDir[0])));
	r0 = Box1->fAxisLen[0] * absC[0][0] + Box1->fAxisLen[1] * absC[1][0] +
		Box1->fAxisLen[2] * absC[2][0];
	r1 = Box2->fAxisLen[0];
	if (r > r0 + r1)
		return FALSE;


	r = abs(dot((diff), (Box2->vAxisDir[1])));
	r0 = Box1->fAxisLen[0] * absC[0][1] + Box1->fAxisLen[1] * absC[1][1] +
		Box1->fAxisLen[2] * absC[2][1];
	r1 = Box2->fAxisLen[1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(dot((diff), (Box2->vAxisDir[2])));
	r0 = Box1->fAxisLen[0] * absC[0][2] + Box1->fAxisLen[1] * absC[1][2] +
		Box1->fAxisLen[2] * absC[2][2];
	r1 = Box2->fAxisLen[2];
	if (r > r0 + r1)
		return FALSE;

	if (existsParallelPair == true)
		return true;

	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = Box1->fAxisLen[1] * absC[2][0] + Box1->fAxisLen[2] * absC[1][0];
	r1 = Box2->fAxisLen[1] * absC[0][2] + Box2->fAxisLen[2] * absC[0][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = Box1->fAxisLen[1] * absC[2][1] + Box1->fAxisLen[2] * absC[1][1];
	r1 = Box2->fAxisLen[1] * absC[0][2] + Box2->fAxisLen[2] * absC[0][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = Box1->fAxisLen[1] * absC[2][2] + Box1->fAxisLen[2] * absC[1][2];
	r1 = Box2->fAxisLen[1] * absC[0][2] + Box2->fAxisLen[2] * absC[0][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = Box1->fAxisLen[0] * absC[2][0] + Box1->fAxisLen[2] * absC[0][0];
	r1 = Box2->fAxisLen[1] * absC[1][2] + Box2->fAxisLen[2] * absC[1][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = Box1->fAxisLen[0] * absC[2][1] + Box1->fAxisLen[2] * absC[0][1];
	r1 = Box2->fAxisLen[0] * absC[1][2] + Box2->fAxisLen[2] * absC[1][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = Box1->fAxisLen[0] * absC[2][2] + Box1->fAxisLen[2] * absC[0][2];
	r1 = Box2->fAxisLen[0] * absC[1][1] + Box2->fAxisLen[2] * absC[1][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = Box1->fAxisLen[0] * absC[1][0] + Box1->fAxisLen[1] * absC[0][0];
	r1 = Box2->fAxisLen[1] * absC[2][2] + Box2->fAxisLen[2] * absC[2][1];
	if (r > r0 + r1)
		return false;

	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = Box1->fAxisLen[0] * absC[1][1] + Box1->fAxisLen[1] * absC[0][1];
	r1 = Box2->fAxisLen[0] * absC[2][2] + Box2->fAxisLen[2] * absC[2][0];
	if (r > r0 + r1)
		return false;

	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = Box1->fAxisLen[0] * absC[1][2] + Box1->fAxisLen[1] * absC[0][2];
	r1 = Box2->fAxisLen[0] * absC[2][1] + Box2->fAxisLen[1] * absC[2][0];
	if (r > r0 + r1)
		return false;

	return TRUE;
}