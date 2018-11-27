#pragma once
#include "MyRigidBody.h"
#include "Definitions.h"

using namespace Simplex;
class Bullet
{
public:
	Bullet(Model*, MyRigidBody*);
	~Bullet();
	Model* bulletModel = nullptr;
	MyRigidBody* bulletRB = nullptr;
	vector3 forward = vector3(0.0f);

	//void SetModel(Model*);
	//void SetRigidBody(MyRigidBody*);
	void SetForward(vector3);

	matrix4 UpdatePosition();
};

