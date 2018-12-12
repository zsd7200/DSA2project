#include "Player.h"
#include "MyEntity.h"
Player::Player()
{
	//Create the player's model
	playerModel = new Model();

	//Load it
	playerModel->Load("HarryPotter\\Stick.obj");

	//Set the player's rigidbody
	playerRB = new MyRigidBody(playerModel->GetVertexList());
}


Player::~Player()
{
	//Deleting objects
	SafeDelete(playerModel);
	//playerModel = nullptr;	//<< SafeDelete does this already
	SafeDelete(playerRB);
	//playerRB = nullptr;

	for (size_t i = 0; i < bullets.size(); i++)
	{
		SafeDelete(bullets[i]);
	}
	bullets.clear();
}

void Player::CreateProjectile(vector3 tempForward, vector3 currentPos)
{
	//Loading in the model
	Model* tempMod = new Model();
	tempMod->Load("HarryPotter\\Blast.obj");
	MyEntity* tempEnt = new MyEntity("HarryPotter\\Blast.obj");
	//Creating the rigid body
	MyRigidBody* tempRB = new MyRigidBody(tempMod->GetVertexList());

	//Creating the bullet
	Bullet* tempBul = new Bullet(tempEnt, currentPos, tempForward);

	//Adding the bullet to the list of bullets
	bullets.push_back(tempBul);
}

//Can add in more parameters to make movement better
matrix4 Player::UpdatePosition(vector3 basePoint, vector3 forward)
{
	//Needed to reverse the forward
	forward.z = -forward.z;

	//Increasing the upwards position of the forward vector
	forward.y += 0.1f;

	//Rotation by looking at a specific position
	quaternion rotQuat = glm::lookAt(basePoint,basePoint-forward,vector3(0,1,0));
	matrix4 rotMat = glm::toMat4(rotQuat);

	//Setting the location of the player
	matrix4 mPlayerMatrix = glm::translate(vector3(basePoint.x, basePoint.y - 1, basePoint.z))*rotMat;

	//Seting the model matrixes 
	playerModel->SetModelMatrix(mPlayerMatrix);
	playerRB->SetModelMatrix(mPlayerMatrix);


	return mPlayerMatrix;
}
