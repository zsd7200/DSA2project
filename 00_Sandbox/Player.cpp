#include "Player.h"
Player::Player()
{
	//Create the player's model
	playerModel = new Model();

	//Load it
	playerModel->Load("Mario\\Bowser.obj");

	//Set the player's rigidbody
	playerRB = new MyRigidBody(playerModel->GetVertexList());
}


Player::~Player()
{
	//Deleting objects
	SafeDelete(playerModel);
	playerModel = nullptr;
	SafeDelete(playerRB);
	playerRB = nullptr;

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
	//Creating the rigid body
	MyRigidBody* tempRB = new MyRigidBody(tempMod->GetVertexList());

	//Creating the bullet
	Bullet* tempBul = new Bullet(tempMod, tempRB, currentPos, tempForward);

	//Adding the bullet to the list of bullets
	bullets.push_back(tempBul);
}

//Can add in more parameters to make movement better
matrix4 Player::UpdatePosition(vector3 basePoint)
{
	//Setting the location of the player
	matrix4 mPlayerMatrix = glm::translate(vector3(basePoint.x, basePoint.y - 3.0f,basePoint.z));

	//Seting the model matrixes 
	playerModel->SetModelMatrix(mPlayerMatrix);
	playerRB->SetModelMatrix(mPlayerMatrix);
	return mPlayerMatrix;
}
