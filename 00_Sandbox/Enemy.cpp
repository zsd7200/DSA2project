#include "Enemy.h"
Enemy::Enemy()
{
	enemyModel = new Model();								//Create the enemy's model
	enemyModel->Load("HarryPotter\\Boo.FBX");				//Load it
	enemyRB = new MyRigidBody(enemyModel->GetVertexList());	//Set the enemy's rigidbody

	mEnemyMatrix = IDENTITY_M4;	//initial enemy position/rotation/scale

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
	//mEnemyMatrix = glm::translate(vector3(basePoint.x, basePoint.y, basePoint.z)) * glm::rotate(IDENTITY_M4, glm::radians(10.0f), AXIS_Z);
	mEnemyMatrix = glm::translate(basePoint) /** ToMatrix4(model) * ToMatrix4(arcBall)*/;
	mEnemyMatrix = glm::scale(mEnemyMatrix, vector3(0.01f));

	//make enemies face direction theyre moving in
	mEnemyMatrix *= glm::lookAt(vector3(0,0,1),vector3(0),vector3(0,1,0));
	enemyModel->SetModelMatrix(mEnemyMatrix);
	enemyRB->SetModelMatrix(mEnemyMatrix);
	// m_pMeshMngr->AddAxisToRenderList(enemyModel); //<<??
	return mEnemyMatrix;
}
