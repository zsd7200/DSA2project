#pragma once

#include "MyEntity.h"
#include "Definitions.h"
#include <string>
#include <ctime>
using namespace Simplex;

class Enemy
{
public:
	Enemy(vector3);
	~Enemy();

	MyEntity* enemy = nullptr;
	matrix4 mEnemyMatrix = IDENTITY_M4;

	float acceleration = 2.0f;
	float maxVel = 4;
	double deltaTime=0;
	time_t prevTime = 0;

	clock_t begin_time = clock();

	vector3 velocity = vector3();
	vector3 curPos;
	vector3 wanderPos=ZERO_V3;
	vector3 lookDir = vector3(1);

	void Update();

	void Wander();
	void ChasePlayer(vector3 playerPos);
	void Approach(vector3);
	static vector3 RandomUnitSphere();

	matrix4 UpdatePosition(vector3);
};

