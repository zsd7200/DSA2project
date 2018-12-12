#pragma once

#include "MyRigidBody.h"
#include "Definitions.h"
#include "Bullet.h"
#include <string>
using namespace Simplex;

class Player
{
public:
	//Constructor
	Player();
	~Player();

	// player entity
	MyEntity* playerEntity = nullptr;

	//void CreatePlayer();

	//Spawing in a projectile
	void CreateProjectile(vector3, vector3);

	//std::vector<Model*> bullets;
	//std::vector<MyRigidBody*> bulletsRB;

	//Keeping track of the bullets fired 
	std::vector<Bullet*> bullets;

	//Updateing the position of the player
	matrix4 UpdatePosition(vector3, vector3);
};

