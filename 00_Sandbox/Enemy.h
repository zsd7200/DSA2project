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
	MyRigidBody* rigidBody = nullptr;

	matrix4 UpdatePosition(vector3);
};

