#include "Enemy.h"
Enemy::Enemy()
{
	//Create the enemy's model
	enemyModel = new Model();
	
	//Load it
	enemyModel->Load("HarryPotter\\Boo.FBX");
	
	//Set the enemy's rigidbody
	enemyRB = new MyRigidBody(enemyModel->GetVertexList());

	mEnemyMatrix = IDENTITY_M4;

	//UpdatePosition(vector3(10, 10, 10));
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
	//set model matrix to the enemy model
	mEnemyMatrix = glm::translate(vector3(basePoint.x, basePoint.y, basePoint.z)) * glm::rotate(IDENTITY_M4, glm::radians(10.0f), AXIS_Z);
	//make enemies face direction theyre moving in
	//mEnemyMatrix = glm::lookAt(basePoint,rigidBody->GetCenterGlobal(),vector3(0,1,0));
	enemyModel->SetModelMatrix(mEnemyMatrix);
	enemyRB->SetModelMatrix(mEnemyMatrix);
	// m_pMeshMngr->AddAxisToRenderList(enemyModel); //<<??
	return mEnemyMatrix;
}
