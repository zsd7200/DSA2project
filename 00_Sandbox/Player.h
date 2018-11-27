#pragma once

#include "MyRigidBody.h"
#include "Definitions.h"
#include <string>
using namespace Simplex;

class Player
{
public:
	Player();
	~Player();

	Model* playerModel = nullptr;
	MyRigidBody* playerRB = nullptr;
	void CreatePlayer();
	void CreateProjectile();

	matrix4 UpdatePosition(vector3);

};

