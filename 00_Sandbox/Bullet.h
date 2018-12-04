#pragma once
#include "MyRigidBody.h"
#include "Definitions.h"

using namespace Simplex;
class Bullet
{
public:

	//Constructor
	Bullet(Model*, MyRigidBody*, vector3, vector3);

	//Destructor
	~Bullet();

	//The model for the bullet
	Model* bulletModel = nullptr;
	
	//The bullet's Rigid body
	MyRigidBody* bulletRB = nullptr;

	//The forward vector of the camera at the time the bullet is created
	vector3 forward = vector3(0.0f);

	//The starting position of the camera
	vector3 startVector = vector3(0.0f);

	//The number to multiply movement by
	float timesMoved = 0.0f;

	//Used for moving the bullet
	matrix4 UpdatePosition();

	bool isTimedOut;
};

