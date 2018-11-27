#include "Enemy.h"
Enemy::Enemy()
{
	//Create the enemy's model
	enemyModel = new Model();
	
	//Load it
	enemyModel->Load("HarryPotter\\Boo.FBX");
	
	//Set the enemy's rigidbody
	enemyRB = new MyRigidBody(enemyModel->GetVertexList());

	UpdatePosition(vector3(0));
}


Enemy::~Enemy()
{
}

void Enemy::Update() 
{
	/*if (velocity.length > maxVel) {
		velocity /= velocity.length;
		velocity *= maxVel;
	}*/

}

matrix4 Enemy::UpdatePosition(vector3 basePoint)
{
	//make enemies face direction theyre moving in
	matrix4 mEnemyMatrix = glm::translate(vector3(basePoint.x, basePoint.y, basePoint.z));
	//mEnemyMatrix = glm::lookAt(basePoint,rigidBody->GetCenterGlobal(),vector3(0,1,0));


	enemyModel->SetModelMatrix(mEnemyMatrix);
	enemyRB->SetModelMatrix(mEnemyMatrix);
	return mEnemyMatrix;
}
