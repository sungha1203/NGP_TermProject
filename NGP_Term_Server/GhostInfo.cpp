#include"GhostInfo.h"
#include <random>

using namespace std;

GhostInfo::GhostInfo(int num)
{
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> urd(0.01, 0.02);

	speed = urd(dre);

	for (int i = 0; i < 10; ++i) {
		if (num == i) {
			x = -10.0f + 2 * (i + 1) - 0.5f;
			y = 0.2f;
			z = -11.0;
			type = 1; //zÃà
		}
	}
	for (int i = 10; i < 20; ++i) {
		if (num == i) {
			x = 11.0f;
			y = 0.2f;
			z = -10.0 + 2 * ((i - 10) + 1) - 0.5;
			type = 2; //xÃà
		}
	}
	direction = 1;
}

GhostInfo::~GhostInfo()
{

}

float GhostInfo::GetCoordX()
{
	return x;
}

float GhostInfo::GetCoordY()
{
	return y;
}

float GhostInfo::GetCoordZ()
{
	return z;
}

int GhostInfo::GetDirection()
{
	return direction;
}

void GhostInfo::move(int movetype)
{
	if (movetype == 1) {
		z += direction * speed;
		if (z > 11.0 || z < -11.0)
		{
			direction *= -1;
		}
	}
	else
	{
		x += direction * speed;
		if (x > 11.0 || x < -11.0)
		{
			direction *= -1;
		}
	}
}
