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

	//Entity for the bullet
	MyEntity* bulletEntity = nullptr;

	//The forward vector of the camera at the time the bullet is created
	vector3 forward = vector3(0.0f);

	//The starting position of the camera
	vector3 startVector = vector3(0.0f);

	vector3 currentPosition = vector3(0.0f);
	//The number to multiply movement by
	float timesMoved = 0.0f;

	//Used for moving the bullet
	void UpdatePosition();

	//Used to flag if the bullet should be deleted 
	bool isTimedOut;
};

