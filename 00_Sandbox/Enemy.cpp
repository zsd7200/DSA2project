#include "Enemy.h"
Enemy::Enemy(vector3 curPos)
{
	enemyModel = new Model();								//Create the enemy's model
	enemyModel->Load("HarryPotter\\Boo.FBX");				//Load it
	enemyRB = new MyRigidBody(enemyModel->GetVertexList());	//Set the enemy's rigidbody

	mEnemyMatrix = IDENTITY_M4;	//initial enemy position/rotation/scale

	this->curPos = curPos;

	//UpdatePosition(curPos);
}


Enemy::~Enemy()
{
	SafeDelete(enemyModel);
	SafeDelete(enemyRB);
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
	//mEnemyMatrix = glm::normalize(mEnemyMatrix);
	

	mEnemyMatrix = IDENTITY_M4;
	mEnemyMatrix *= glm::translate(basePoint);
	mEnemyMatrix *= glm::scale(vector3(0.01f));

	//make enemies face direction theyre moving in

	vector3 tLookDir = basePoint - curPos;
	curPos = basePoint;
	
	vector3 up = vector3(0, -1, 0);

	if (tLookDir!=ZERO_V3) 
	{
		tLookDir = glm::normalize(tLookDir);

		if (tLookDir.x != 0 || tLookDir.z != 0)
			this->lookDir = tLookDir;
	}	
	mEnemyMatrix *= glm::lookAt(vector3(-this->lookDir.x, -this->lookDir.y, -this->lookDir.z), vector3(0), up);
	mEnemyMatrix *= glm::rotate(IDENTITY_M4, glm::radians(90.0f), AXIS_X);
	

	enemyModel->SetModelMatrix(mEnemyMatrix);
	enemyRB->SetModelMatrix(mEnemyMatrix);
	//m_pMeshMngr->AddAxisToRenderList(enemyModel); //<<??
	return mEnemyMatrix;
}
