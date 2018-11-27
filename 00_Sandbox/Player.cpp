#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

void Player::CreatePlayer()
{
	//Create the player's model
	playerModel = new Model();

	//Load it
	playerModel->Load("Mario\\Bowser.obj");

	//Set the player's rigidbody
	playerRB = new MyRigidBody(playerModel->GetVertexList());
}

//Can add in more parameters to 
matrix4 Player::UpdatePosition(vector3 basePoint)
{
	//Setting the location of the player
	matrix4 mPlayerMatrix = glm::translate(vector3(basePoint.x, basePoint.y - 3.0f,basePoint.z));

	//Seting the model matrixes 
	playerModel->SetModelMatrix(mPlayerMatrix);
	playerRB->SetModelMatrix(mPlayerMatrix);
	return mPlayerMatrix;
}
