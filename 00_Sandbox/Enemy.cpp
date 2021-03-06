#include "Enemy.h"

Enemy::Enemy(vector3 curPos)
{
	//Creating the entity 
	enemy = new MyEntity("HarryPotter\\Boo.fbx");

	mEnemyMatrix = IDENTITY_M4;	//initial enemy position/rotation/scale

	this->curPos = curPos;

	UpdatePosition(curPos);
	time(&prevTime);
}


Enemy::~Enemy()
{
	//Deleting the entity
	SafeDelete(enemy);
	enemy = nullptr;
}

void Enemy::Update(vector3 playerPos) 
{
	//Getting the time since last frame
	deltaTime = (clock()-begin_time)/(CLOCKS_PER_SEC*1.00000);

	//Getting a new initial time
	begin_time = clock();

	//If the ghost is moving faster than max velocity, slow it down
	if (glm::length(velocity) > maxVel) {
		velocity = glm::normalize(velocity);
		velocity *= maxVel;
	}

	//If the enemy is not shrinking, follow the player around
	if (!shrinking) 
	{
		////If the player is within range, chace it, else, wander aimlessly
		if (glm::length(curPos - playerPos) < chaseDis)
			Approach(playerPos);
		else
			Wander();

		//mEnemyMatrix *= glm::rotate( glm::radians(spinDir), AXIS_Z);
		spinDir = 0;
		//enemy->SetModelMatrix(mEnemyMatrix);
		UpdatePosition(curPos + velocity*deltaTime);
	}

	//If the enemy is shrinking
	else {
		//Reduce the amount of time left to shrink
		shrinkTimer -= deltaTime;
		sizeMulti = 0.975f;
		velocity *= 0.25f;

		// shrink
		mEnemyMatrix *= glm::rotate(IDENTITY_M4, glm::radians(7.0f), AXIS_Z);
		mEnemyMatrix *= glm::scale(vector3(sizeMulti));
		enemy->SetModelMatrix(mEnemyMatrix);

		if (shrinkTimer <= .1f)
		{
			isDead = true;
		}

	}
	//std::printf("%f, %f \n", deltaTime, curPos.y);

	
	
}

void Enemy::Wander()
{
	//If the wander position is zero, or is too small, generate a new position to wander to 
	if (wanderPos == ZERO_V3 || glm::length(wanderPos - curPos) < 1) {
		wanderPos = RandomUnitSphere()*(rand() % 100);
		wanderPos.z = wanderPos.z - 100;
	}

	Approach(wanderPos);
}

void Enemy::Approach(vector3 target)
{
	//Getting the distance between the target and the enemy
	vector3 desVel = target - curPos;

	//Normalizing the vector before increasing it to the maximum velocity
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

void Enemy::UpdatePosition(vector3 basePoint)
{
	//Creating a matrix to manipulate
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
	
	//Choosing the direction of the boos to move
	lookDir.x = -lookDir.x;
	mEnemyMatrix *= glm::lookAt(vector3(0), lookDir, up);

	//Rotating the enemy
	mEnemyMatrix *= glm::rotate(IDENTITY_M4, glm::radians(0.0f), AXIS_X);
	mEnemyMatrix *= glm::rotate(IDENTITY_M4, glm::radians(180.0f), AXIS_Y);

	//Scaling it down
	mEnemyMatrix *= glm::scale(vector3(sizeMulti));

	//Setting the entity's matrix
	enemy->SetModelMatrix(mEnemyMatrix);
}

void Enemy::Shrink() 
{
	shrinking = true;
	shrinkTimer = 3;
}

void Enemy::PushAway(vector3 target, boolean half)
{
	float radius = 1.8;
	float multiplier = radius * 2 - glm::length(curPos - target);

	if (half)
	{
		curPos += glm::normalize(curPos - target)*multiplier/2;
	}
	else {
		curPos += glm::normalize(curPos - target) *multiplier;
	}

}


