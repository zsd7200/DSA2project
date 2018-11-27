#include "Enemy.h"



Enemy::Enemy()
{
	enemyModel = new Model();
	enemyModel->Load("HarryPotter\\Boo.FBX");

	rigidBody = new MyRigidBody(enemyModel->GetVertexList());
}


Enemy::~Enemy()
{
}

void Enemy::Update() 
{

	

	if (velocity.length > maxVel) {
		velocity /= velocity.length;
		velocity *= maxVel;
	}

}

matrix4 Enemy::UpdatePosition(vector3 basePoint)
{
	matrix4 mEnemyMatrix = glm::translate(vector3(basePoint.x, basePoint.y, basePoint.z));
	mEnemyMatrix = glm::lookAt(basePoint,rigidBody->GetCenterGlobal(),vector3(0,1,0));


	enemyModel->SetModelMatrix(mEnemyMatrix);
	rigidBody->SetModelMatrix(mEnemyMatrix);
	return mEnemyMatrix;
}
