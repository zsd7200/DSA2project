#include "Bullet.h"

Bullet::Bullet(Model* tempMod, MyRigidBody* tempRB)
{
	bulletModel = tempMod;
	bulletRB = tempRB;
}

Bullet::~Bullet()
{

}

/*void Bullet::SetModel(Model* a_mModel)
{
	bulletModel = a_mModel;
}

void Bullet::SetRigidBody(MyRigidBody* a_mRB)
{
	bulletRB = a_mRB;
}
*/
void Bullet::SetForward(vector3 a_vForward)
{
	forward = a_vForward;
}

matrix4 Bullet::UpdatePosition()
{
	vector3 tempForward = glm::normalize(forward);
	//Setting the location of the player
	matrix4 mBulletMatrix = glm::translate(vector3(tempForward.x, tempForward.y, tempForward.z)) * glm::scale(vector3(100.0f));

	//Seting the model matrixes 
	bulletModel->SetModelMatrix(mBulletMatrix);
	bulletRB->SetModelMatrix(mBulletMatrix);
	return mBulletMatrix;
}
