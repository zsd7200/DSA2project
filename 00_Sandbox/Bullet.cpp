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

}

matrix4 Bullet::UpdatePosition()
{
	//vector3 tempForward = startVector;

	vector3 tempForward = (glm::normalize(forward) * timesMoved) + startVector;
	//std::cout << "bullet : " << tempForward.x << " " << tempForward.y << " " << tempForward.z << std::endl;
	//Setting the location of the player
	matrix4 mBulletMatrix = glm::translate(vector3(tempForward.x, tempForward.y, tempForward.z)) * glm::scale(vector3(5.0f));

	//Seting the model matrixes 
	bulletModel->SetModelMatrix(mBulletMatrix);
	bulletRB->SetModelMatrix(mBulletMatrix);

	//Moving the position forward consecutievely
	timesMoved += .1f;
	return mBulletMatrix;
}
