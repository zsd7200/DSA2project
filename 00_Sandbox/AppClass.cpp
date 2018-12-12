#include "AppClass.h"

using namespace Simplex;
void Application::InitVariables(void)
{
	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Setting the number of enemies to spawn initally
	numOfEnemies = 20;

#pragma region Music_and_Sounds
	//Music
	String sRoute = m_pSystem->m_pFolder->GetFolderData();
	sRoute += m_pSystem->m_pFolder->GetFolderAudio();
	
	//Background music
	m_soundBGM.openFromFile(sRoute + "hedwig.ogg");
	m_soundBGM.play();
	m_soundBGM.setLoop(true);

	//sound effect
	m_soundBuffer.loadFromFile(sRoute + "Magic.ogg");
	m_sound.setBuffer(m_soundBuffer);
#pragma endregion

	//Create the player object
	mainPlayer = new Player();

	//Spawning in boos randomly
	for (int i = 0; i < numOfEnemies; i++) {
		enemies.push_back(new Enemy(vector3(rand()%50-25, rand() % 50 + 5, rand() % 50 - 25)));
		m_pEntityMngr->AddEntity(enemies[i]->enemy);
	}

	//Create the ooctant
	m_uOctantLevels = 2;
	m_pOctant = new MyOctant(m_uOctantLevels, 3);
	m_pEntityMngr->Update();

	// load hogwarts bg
	m_pHogwarts = new MyEntity("HarryPotter\\hog_color.fbx");

	// load grass
	grass = new MyEntity("HarryPotter\\Cube.obj");
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	matrix4 mHogwarts = glm::translate(vector3(0, 0, -25)) * glm::scale(vector3(0.1));
	m_pHogwarts->SetModelMatrix(mHogwarts);
	m_pHogwarts->AddToRenderList();
	
	//Set model matrix to the model			//ENEMY (BOO)
	//MODEL MATRIX SET IN ENEMY'S UpdatePosition METHOD
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->Update(m_pCameraMngr->GetPosition());
		m_pMeshMngr->AddAxisToRenderList(enemies[i]->mEnemyMatrix);	//accesses enemy's model matrix, which is based off of m_v3Model (which isnt set anywhere how does this even work)
	}

	// grass
	matrix4 grassAxis = glm::translate(vector3(0, -1, -25)) * glm::scale(vector3(100, 2, 100));
	grass->SetModelMatrix(grassAxis);
	grass->AddToRenderList();


	//Storing the index of a bullet to be destroyed (if applicable)
	uint bulletIndexToDelete = -1;

	//Score for the player
	//static int score = 0;

	//Looping through each bullet in the field
	for (size_t i = 0; i < mainPlayer->bullets.size(); i++)
	{
		//Updating the position of the bullet so it moves forward
		mainPlayer->bullets[i]->UpdatePosition();

		//Drawing the bullet
		mainPlayer->bullets[i]->bulletEntity->AddToRenderList();

		//Clearing out the dimensions of the bullet
		mainPlayer->bullets[i]->bulletEntity->ClearDimensionSet();
		if (m_uOctantLevels != 0)
		{
			for (size_t j = 0; j < 8; j++)
			{
				CheckCollision(mainPlayer->bullets[i]->bulletEntity, m_pOctant->GetChild(j));
			}
		}
		//If there is no bullet that has been set to delete yet, check if a bullet has timed out of bounds
		if (bulletIndexToDelete == -1)
		{
			if (mainPlayer->bullets[i]->isTimedOut)
			{
				bulletIndexToDelete = i;
			}
		}
	}

	//If a bullet should be deleted
	if (bulletIndexToDelete != -1)
	{
		//Deleting the object
		SafeDelete(mainPlayer->bullets[bulletIndexToDelete]);

		//Removing the bullet from the list
		mainPlayer->bullets.erase(mainPlayer->bullets.begin() + bulletIndexToDelete);

		//Resetting the index
		bulletIndexToDelete = -1;
	}
	
	//Rendering the player in the world
	mainPlayer->UpdatePosition(m_pCameraMngr->GetPosition(), m_pCameraMngr->GetForward());	//player moves with camera
	mainPlayer->playerEntity->AddToRenderList();

	//A temporary int which will store the index of an enemy to be deleted (if applicable)
	uint enemyIndexToDelete = -1;

	//Vector to hold the index of enemies which are about to be deleted
	std::vector<int> shrinkingEnemies;

	//Looping through the list of enemies and checking for collisions with the bullets
	for (int i = 0; i < enemies.size(); i++) 
	{
		//Rendering the enemies 
		enemies[i]->enemy->AddToRenderList();

		//If the enemy is not in the process of shrinking after it's been hit, check for collisions
		if (enemies[i]->shrinking == false)
		{
			//Checking collision between the bullets
			for (size_t j = 0; j < mainPlayer->bullets.size(); j++)
			{
				//Using the MyEntity collision check which uses dimension
				bool tempBool = mainPlayer->bullets[j]->bulletEntity->IsColliding(enemies[i]->enemy);

				//If there is collision
				if (tempBool)
				{
					//Setting a specific bullet to be deleted
					bulletIndexToDelete = j;

					//Setting the enemy to start shrinking
					enemies[i]->Shrink();

					//Incrementing score
					score++;
				}
			}
		}
		//If the enemies are shrinking, add them to the list of currently shrinking enemies
		if (enemies[i]->shrinking)
		{
			shrinkingEnemies.push_back(i);
		}
	}	

	//Looping through the list of enemies which are shrinking and seeing if any have finished shrinking
	for (size_t i = 0; i < shrinkingEnemies.size(); i++)
		if (enemies[shrinkingEnemies[i]]->isDead)
			enemyIndexToDelete = shrinkingEnemies[i];

	//Clear the list of shrinking enemies to be checked next update
	shrinkingEnemies.clear();

	//Checking if an enemy should be deleted
	if (enemyIndexToDelete != -1)
	{
		//Looping through the entity manager to remove the enemy from the list of entities
		for (size_t i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
			if (enemies[enemyIndexToDelete]->enemy == m_pEntityMngr->GetEntityList()[i])
				m_pEntityMngr->RemoveEntity(i);

		//Removing it from the list of eneies
		enemies.erase(enemies.begin() + enemyIndexToDelete);

		//Resetting the index
		enemyIndexToDelete = -1;

		// decrement num of enemies
		numOfEnemies--;
	}
	
	//If a bullet should be deleted
	if (bulletIndexToDelete != -1)
	{
		//Deleting the object
		SafeDelete(mainPlayer->bullets[bulletIndexToDelete]);

		//Removing the bullet from the list
		mainPlayer->bullets.erase(mainPlayer->bullets.begin() + bulletIndexToDelete);

		//Resetting the index
		bulletIndexToDelete = -1;
	}
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	m_pOctant->Display();

	//// set the model matrix of the model
	//m_pModel->SetModelMatrix(ToMatrix4(m_qArcBall));
	////play the default sequence of the model
	//m_pModel->PlaySequence();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();
	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}

void Application::Release(void)
{
	//release GUI
	ShutdownGUI();

	//release variables

	for (auto & x : enemies)
		SafeDelete(x);

	SafeDelete(grass);
	SafeDelete(m_pHogwarts);
	SafeDelete(mainPlayer);
}