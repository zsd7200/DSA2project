#include "Enemy.h"
Enemy::Enemy(vector3 curPos)
{
	enemy = new MyEntity("HarryPotter\\Boo.FBX");

	mEnemyMatrix = IDENTITY_M4;	//initial enemy position/rotation/scale

	this->curPos = curPos;

	std::printf("%f, %f",curPos.x,curPos.y);

	UpdatePosition(curPos);
	time = GetCurrentTime();
}


Enemy::~Enemy()
{
	SafeDelete(enemy);
}

void Enemy::Update() 
{
	float deltaTime = GetCurrentTime() - time;

	if (glm::length(velocity) > maxVel) {
		velocity = glm::normalize(velocity);
		velocity *= maxVel;
	}
	
	UpdatePosition(curPos+velocity*deltaTime);
}

void Enemy::Wander()
{

}

void Enemy::Approach(vector3 target)
{
	float deltaTime = GetCurrentTime() - time;

	vector3 desVel = target - curPos;
	desVel = glm::normalize(desVel);
	desVel *= maxVel;

	vector3 changeNeeded = (desVel - velocity);
	if (glm::length(changeNeeded) > acceleration)
	{
		changeNeeded = glm::normalize(changeNeeded)*acceleration;
	}
	if (glm::length(changeNeeded) > 0.1f) 
	{
		velocity += changeNeeded*deltaTime;
	}
}

vector3 Enemy::RandomUnitSphere()
{
	return glm::normalize(vector3(rand(),rand(),rand()));
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
	mEnemyMatrix *= glm::lookAt(vector3(0), lookDir, up);
	//mEnemyMatrix *= glm::rotate(IDENTITY_M4, glm::radians(90.0f), AXIS_X);
	

	enemy->SetModelMatrix(mEnemyMatrix);
	//m_pMeshMngr->AddAxisToRenderList(enemyModel); //<<??
	return mEnemyMatrix;
}


