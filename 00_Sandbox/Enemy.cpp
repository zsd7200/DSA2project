#include "Enemy.h"


Enemy::Enemy(vector3 curPos)
{
	enemy = new MyEntity("HarryPotter\\Boo.FBX");

	mEnemyMatrix = IDENTITY_M4;	//initial enemy position/rotation/scale

	this->curPos = curPos;

	//std::printf("%f, %f",curPos.x,curPos.y);

	UpdatePosition(curPos);
	time(&prevTime);
}


Enemy::~Enemy()
{
	SafeDelete(enemy);
}

void Enemy::Update(vector3 playerPos) 
{
	deltaTime = (clock()-begin_time)/(CLOCKS_PER_SEC*1.00000);
	begin_time = clock();

	if (glm::length(velocity) > maxVel) {
		velocity = glm::normalize(velocity);
		velocity *= maxVel;
	}


	if (!shrinking) {
		if (glm::length(curPos - playerPos) < chaseDis)
			ChasePlayer(playerPos);
		else
			Wander();
	}
	else {
		shrinkTimer -= deltaTime;
		sizeMulti=glm::lerp(0.0f,0.01f,(float)(shrinkTimer/3.000));
		velocity *= 0.25f;
	}
	//std::printf("%f, %f \n", deltaTime, curPos.y);

	//UpdatePosition(curPos+velocity*deltaTime);
	
}

void Enemy::Wander()
{
	if (wanderPos == ZERO_V3 || glm::length(wanderPos - curPos) < 1) {
		wanderPos = RandomUnitSphere()*(rand() % 50);
		wanderPos.z = wanderPos.z - 25;
	}

	Approach(wanderPos);
}

void Enemy::ChasePlayer(vector3 playerPos) 
{
	Approach(playerPos);

}

void Enemy::Approach(vector3 target)
{
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
	mEnemyMatrix *= glm::translate(curPos);
	

	//make enemies face direction theyre moving in

	vector3 tLookDir = velocity;
	curPos = basePoint;
	
	vector3 up = vector3(0, 1, 0);

	if (tLookDir!=ZERO_V3) 
	{
		tLookDir = glm::normalize(tLookDir);

		//if (tLookDir.x != 0 || tLookDir.z != 0)
			this->lookDir = tLookDir;
	}	
	
	//messing with some axes values since the boo model axes don't match the global axes
	lookDir.x = -lookDir.x;
	//float holder = lookDir.y;
	//lookDir.y = lookDir.z;
	//lookDir.z = holder;

	mEnemyMatrix *= glm::lookAt(vector3(0), lookDir, up);
	//mEnemyMatrix *= glm::lookAt(vector3(0), vector3(-1,0,0), vector3(0,0,1));
	mEnemyMatrix *= glm::rotate(IDENTITY_M4, glm::radians(90.0f), AXIS_X);
	mEnemyMatrix *= glm::rotate(IDENTITY_M4, glm::radians(180.0f), AXIS_Y);

	mEnemyMatrix *= glm::scale(vector3(sizeMulti));

	enemy->SetModelMatrix(mEnemyMatrix);
	//m_pMeshMngr->AddAxisToRenderList(enemyModel); //<<??
	return mEnemyMatrix;
}

void Enemy::Shrink() {
	shrinking = true;
	shrinkTimer = 3;
}


