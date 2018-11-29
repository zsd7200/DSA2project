#pragma once

#include "MyRigidBody.h"
#include "Definitions.h"
#include <string>
using namespace Simplex;

class Enemy
{
public:
	Enemy();
	~Enemy();

	Model* enemyModel = nullptr;
	MyRigidBody* enemyRB = nullptr;
	matrix4 mEnemyMatrix = IDENTITY_M4;

	float acceleration = 2.0f;
	float maxVel = 4;
	vector3 velocity = vector3();

	void Update();
	matrix4 UpdatePosition(vector3);
};

