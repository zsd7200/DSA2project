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
	SafeDelete(bulletModel);
	bulletModel = nullptr;
	SafeDelete(bulletRB);
	bulletRB = nullptr;

	SafeDelete(bulletEntity);
	bulletEntity = nullptr;

	startVector = vector3(0.0f);
	forward = vector3(0.0f);
}

matrix4 Bullet::UpdatePosition()
{
	//Getting the current position of the bullet
	currentPosition = (glm::normalize(forward) * timesMoved) + startVector;

	//Moving the bullet position
	matrix4 mBulletMatrix = glm::translate(vector3(currentPosition.x, currentPosition.y, currentPosition.z)) * glm::scale(vector3(.5f));

	//Seting the model matrixes 
	bulletEntity->SetModelMatrix(mBulletMatrix);

	//Moving the position forward consecutievely
	timesMoved += .1f;
	
	if (timesMoved > 40.0f)
		isTimedOut = true;

	return mBulletMatrix;
}
