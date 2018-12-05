#include "Bullet.h"

//Constructor which will create the bullet with all the information it needs
Bullet::Bullet(Model* tempMod, MyRigidBody* tempRB, vector3 tempPos, vector3 tempForward)
{
	bulletModel = tempMod;
	bulletRB = tempRB;
	startVector = tempPos;
	forward = tempForward;
}

Bullet::~Bullet()
{
	SafeDelete(bulletModel);
	bulletModel = nullptr;
	SafeDelete(bulletRB);
	bulletRB = nullptr;

	startVector = vector3(0.0f);
	forward = vector3(0.0f);
}

matrix4 Bullet::UpdatePosition()
{
	//Getting the initial position for the object to start
	vector3 tempForward = (glm::normalize(forward) * timesMoved) + startVector;

	//Setting the location of the player
	matrix4 mBulletMatrix = glm::translate(vector3(tempForward.x, tempForward.y, tempForward.z)) * glm::scale(vector3(1.0f));

	//Seting the model matrixes 
	bulletModel->SetModelMatrix(mBulletMatrix);
	bulletRB->SetModelMatrix(mBulletMatrix);

	//Moving the position forward consecutievely
	timesMoved += .1f;
	
	if (timesMoved > 10.0f)
		isTimedOut = true;
	return mBulletMatrix;
}
