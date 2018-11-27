#pragma once

#include "MyRigidBody.h"
#include "Definitions.h"
#include "Bullet.h"
#include <string>
using namespace Simplex;

class Player
{
public:
	Player();
	~Player();

	Model* playerModel = nullptr;
	MyRigidBody* playerRB = nullptr;
	//void CreatePlayer();
	void CreateProjectile(vector3);

	//std::vector<Model*> bullets;
	//std::vector<MyRigidBody*> bulletsRB;
	std::vector<Bullet*> bullets;
	matrix4 UpdatePosition(vector3);

};

