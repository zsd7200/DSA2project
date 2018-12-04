#pragma once

#include "MyRigidBody.h"
#include "Definitions.h"
#include <string>
using namespace Simplex;

class Enemy
{
public:
	Enemy(vector3);
	~Enemy();

	Model* enemyModel = nullptr;
	MyRigidBody* enemyRB = nullptr;
	matrix4 mEnemyMatrix = IDENTITY_M4;

	float acceleration = 2.0f;
	float maxVel = 4;
	float time = 0;

	vector3 velocity = vector3();
	vector3 curPos;
	vector3 wanderPos;
	vector3 lookDir = vector3(1);

	void Update();

	void Wander();
	void Approach(vector3);
	static vector3 RandomUnitSphere();

	matrix4 UpdatePosition(vector3);
};

