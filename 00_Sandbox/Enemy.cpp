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

matrix4 Enemy::UpdatePosition(vector3)
{
	return matrix4();
}
