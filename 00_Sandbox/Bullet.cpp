#include "Bullet.h"

//Constructor which will create the bullet with all the information it needs
Bullet::Bullet(MyEntity* tempEnt, vector3 tempPos, vector3 tempForward)
{
	//Setting the values passed in
	bulletEntity = tempEnt;
	startVector = tempPos;
	forward = tempForward;
}

Bullet::~Bullet()
{
	//Deleting the bullet entity
	SafeDelete(bulletEntity);
	bulletEntity = nullptr;

	//Resetting the vectors
	startVector = vector3(0.0f);
	forward = vector3(0.0f);
}

void Bullet::UpdatePosition()
{
	//Getting the current position of the bullet
	currentPosition = (glm::normalize(forward) * timesMoved) + startVector;

	//Moving the bullet position
	matrix4 mBulletMatrix = glm::translate(vector3(currentPosition.x, currentPosition.y, currentPosition.z)) * glm::scale(vector3(.2f));

	//Seting the model matrixes 
	bulletEntity->SetModelMatrix(mBulletMatrix);

	//Moving the position forward consecutievely
	timesMoved += .2f;
	
	//If the bullet has moved far enough, time it out and delete it form the scene
	if (timesMoved > 40.0f)
		isTimedOut = true;
}
