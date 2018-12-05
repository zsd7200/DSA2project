#pragma once
#include "MyRigidBody.h"
#include "Definitions.h"
#include "MyEntity.h"
using namespace Simplex;
class Bullet
{
public:

	//Constructor
	Bullet(MyEntity*, vector3, vector3);

	//Destructor
	~Bullet();

	MyEntity* bulletEntity = nullptr;

	//The model for the bullet
	Model* bulletModel = nullptr;
	
	//The bullet's Rigid body
	MyRigidBody* bulletRB = nullptr;

	//The forward vector of the camera at the time the bullet is created
	vector3 forward = vector3(0.0f);

	//The starting position of the camera
	vector3 startVector = vector3(0.0f);

	vector3 currentPosition = vector3(0.0f);
	//The number to multiply movement by
	float timesMoved = 0.0f;

	//Used for moving the bullet
	matrix4 UpdatePosition();

	bool isTimedOut;
};

