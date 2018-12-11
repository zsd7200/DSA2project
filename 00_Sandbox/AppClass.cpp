#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	numOfEnemies = 10;

#pragma region Music_and_Sounds
	//Music
	String sRoute = m_pSystem->m_pFolder->GetFolderData();
	sRoute += m_pSystem->m_pFolder->GetFolderAudio();
	
	//Background music
	m_soundBGM.openFromFile(sRoute + "bustin.ogg");
	//m_soundBGM.openFromFile(sRoute + "Music.ogg");

	m_soundBGM.play();
	m_soundBGM.setLoop(true);

	//sound effect
	m_soundBuffer.loadFromFile(sRoute + "Expecto Patronum.ogg");
	//m_soundBuffer.loadFromFile(sRoute + "12C.wav");
	//m_soundBuffer.loadFromFile(sRoute + "Magic.ogg");

	m_sound.setBuffer(m_soundBuffer);
#pragma endregion

	// instantiate entities for walls
	walls.push_back(new MyEntity("HarryPotter\\Cube.obj"));
	walls.push_back(new MyEntity("HarryPotter\\TransCube.obj"));
	walls.push_back(new MyEntity("HarryPotter\\TransCube.obj"));
	walls.push_back(new MyEntity("HarryPotter\\TransCube.obj"));
	walls.push_back(new MyEntity("HarryPotter\\TransCube.obj"));
	walls.push_back(new MyEntity("HarryPotter\\TransCube.obj"));

	mainPlayer = new Player();
	//firstEnemy = new Enemy(vector3(10, 0, 0));
	//m_pEntityMngr->AddEntity(firstEnemy->enemy);

	for (int i = 0; i < numOfEnemies; i++) {
		enemies.push_back(new Enemy(vector3(rand()%50-25, rand() % 50 + 5, rand() % 50 - 25)));
		m_pEntityMngr->AddEntity(enemies[i]->enemy);
	}

	m_pOctant = new MyOctant(2, numOfEnemies-1);
	m_pEntityMngr->Update();

	// load hogwarts bg
	m_pHogwarts = new MyEntity("HarryPotter\\hog_color.fbx");

	//mainPlayer->CreatePlayer();
	/*m_pPlayerModel = new Model();
	//m_pPlayerModel->Load("HarryPotter\\Br	oom.obj");
	m_pPlayerModel->Load("Mario\\Bowser.obj");

	m_pPlayerRB = new MyRigidBody(m_pPlayerModel->GetVertexList());
	*/
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

#pragma region Bounding_Walls
	// grass and walls
	matrix4 botAxis, topAxis, backAxis, frontAxis, rightAxis, leftAxis;

	// grass
	botAxis = glm::translate(vector3(0, -1, -25)) * glm::scale(vector3(100, 2, 100));
	walls[0]->SetModelMatrix(botAxis);
	walls[0]->AddToRenderList();

	// ceiling
	topAxis = glm::translate(vector3(0, 97, -25)) * glm::scale(vector3(100, 2, 100));
	m_pMeshMngr->AddWireCubeToRenderList(topAxis, C_BLUE);
	walls[1]->SetModelMatrix(topAxis);
	//walls["top"]->AddToRenderList();

	// walls
	backAxis = glm::translate(vector3(0, 48, -76)) * glm::scale(vector3(100, 100, 2));
	m_pMeshMngr->AddWireCubeToRenderList(backAxis, C_RED);
	walls[2]->SetModelMatrix(backAxis);
	//walls["back"]->AddToRenderList();

	frontAxis = glm::translate(vector3(0, 48, 26)) * glm::scale(vector3(100, 100, 2));
	m_pMeshMngr->AddWireCubeToRenderList(frontAxis, C_RED);
	walls[3]->SetModelMatrix(frontAxis);
	//walls["front"]->AddToRenderList();

	rightAxis = glm::translate(vector3(51, 48, -25)) * glm::scale(vector3(2, 100, 100));
	m_pMeshMngr->AddWireCubeToRenderList(rightAxis, C_PURPLE);
	walls[4]->SetModelMatrix(rightAxis);
	//walls["right"]->AddToRenderList();

	leftAxis = glm::translate(vector3(-51, 48, -25)) * glm::scale(vector3(2, 100, 100));
	m_pMeshMngr->AddWireCubeToRenderList(leftAxis, C_PURPLE);
	walls[5]->SetModelMatrix(leftAxis);
	//walls["left"]->AddToRenderList();
#pragma endregion

	static bool renderModel = true;
	static bool renderColModel = true;
	uint bulletIndexToDelete = -1;
	//static std::vector<std::vector<vector3>> localMinMax = m_pOctant->GetMinMaxList();
	//static std::vector<MyRigidBody*> octantBodies = m_pOctant->GetRigidBodies();
	static int score = 0;

	//Looping through each bullet in the field
	//Messing with, pls no deletarino
	for (size_t i = 0; i < mainPlayer->bullets.size(); i++)
	{
		mainPlayer->bullets[i]->UpdatePosition();
		mainPlayer->bullets[i]->bulletEntity->AddToRenderList();
		mainPlayer->bullets[i]->bulletEntity->GetRigidBody()->AddToRenderList();

		for (int j = 0; j < m_pOctant->GetOctantCount(); j++)
		{
			MyOctant* temp = m_pOctant->GetChild(j);

			//for (size_t k = 0; k < mainPlayer->bullets.size(); k++)
			
			bool tempBool = mainPlayer->bullets[i]->bulletEntity->GetRigidBody()->IsColliding(temp->GetRigidBody());
			//bool tempBool = enemies[i]->enemy->IsColliding(mainPlayer->bullets[j]->bulletEntity);
			if (tempBool)
			{
				mainPlayer->bullets[i]->bulletEntity->ClearDimensionSet();
				mainPlayer->bullets[i]->bulletEntity->AddDimension(j);
				//SafeDelete(mainPlayer->bullets[i]);
				//mainPlayer->bullets[i] = nullptr;
				//mainPlayer->bullets[i]->isTimedOut = true;
				
			}
		}

		/*for (size_t j = 0; j < octantBodies.size(); j++)
		{
			if (octantBodies[i]->IsColliding(mainPlayer->bullets[i]->bulletEntity->GetRigidBody()))
			{
				mainPlayer->bullets[i]->bulletEntity->ClearDimensionSet();
				mainPlayer->bullets[i]->bulletEntity->AddDimension(j);
			}
		}

		*/
		//uint* tempArray = mainPlayer->bullets[i]->bulletEntity->GetDimensionArray();
		//std::cout << tempArray[i] << std::endl;
	}
	
	/*if (mainPlayer->bullets.size() > 0)
	{
		int tempSize = mainPlayer->bullets.size();
		for (size_t i = tempSize; i > 0; i--)
		{
			if (tempSize == 1)
			{
				if (mainPlayer->bullets[0]->isTimedOut)
				{
					mainPlayer->bullets.erase(mainPlayer->bullets.end() - (tempSize - i));
				}
			}
			else
			{
				if (mainPlayer->bullets[i]->isTimedOut)
				{
					mainPlayer->bullets.erase(mainPlayer->bullets.end() - (tempSize - i));
				}
			}
		}
		/*std::vector<Bullet*> tempBullets;
		for (size_t i = 0; i < mainPlayer->bullets.size(); i++)
		{
			if (mainPlayer->bullets[i] != nullptr)
				tempBullets.push_back(mainPlayer->bullets[i]);
		}

		mainPlayer->bullets = tempBullets;
		
	}
	*/
	/*for (size_t i = 0; i < mainPlayer->bullets.size(); i++)
	{
		//Adding to the render list
		//m_pMeshMngr->AddAxisToRenderList(mainPlayer->bullets[i]->UpdatePosition());
		mainPlayer->bullets[i]->UpdatePosition();

		//mainPlayer->bullets[i]->bulletModel->AddToRenderList();
		//mainPlayer->bullets[i]->bulletRB->AddToRenderList();
		mainPlayer->bullets[i]->bulletEntity->AddToRenderList();

		for (size_t j = 0; j < localMinMax.size(); j++)
		{
			if (mainPlayer->bullets[i]->currentPosition.x < localMinMax[j][0].x)
				if (mainPlayer->bullets[i]->currentPosition.x > localMinMax[j][1].x)
					if (mainPlayer->bullets[i]->currentPosition.y < localMinMax[j][0].y)
						if (mainPlayer->bullets[i]->currentPosition.y > localMinMax[j][1].y)
							if (mainPlayer->bullets[i]->currentPosition.z < localMinMax[j][0].z)
								if (mainPlayer->bullets[i]->currentPosition.z > localMinMax[j][1].z)
								{
									mainPlayer->bullets[i]->bulletEntity->ClearDimensionSet();
									mainPlayer->bullets[i]->bulletEntity->AddDimension(j);
								}
			// check y
				//return false;
				//return false;

			// check z
				//return false;
				//return false;
			/*if (mainPlayer->bullets[i]->currentPosition.x < localMinMax[j][0].x && mainPlayer->bullets[i]->currentPosition.y < localMinMax[j][0].y && mainPlayer->bullets[i]->currentPosition.z < localMinMax[j][0].z)
				if (mainPlayer->bullets[i]->currentPosition.x > localMinMax[j][1].x && mainPlayer->bullets[i]->currentPosition.y > localMinMax[j][1].y && mainPlayer->bullets[i]->currentPosition.z > localMinMax[j][1].z)
				{
					mainPlayer->bullets[i]->bulletEntity->ClearDimensionSet();
					mainPlayer->bullets[i]->bulletEntity->AddDimension(j);
				}
				*/
		//}
		

		/*bool tempBool = m_pModelRB->IsColliding(mainPlayer->bullets[i]->bulletRB);
		if (tempBool)
		{
			renderModel = false;
			if (mainPlayer->bullets[i]->isTimedOut == false)
				indexesToDelete.push_back(i);
				
		}
		tempBool = m_pCollisionModelRB->IsColliding(mainPlayer->bullets[i]->bulletRB);
		if (tempBool)
		{
			renderColModel = false;
			if (mainPlayer->bullets[i]->isTimedOut == false)
				indexesToDelete.push_back(i);
				
		}
		if (mainPlayer->bullets[i]->isTimedOut)
			indexesToDelete.push_back(i);
			*/
	//}

	/*if (indexesToDelete.size() > 0)
	{
		std::vector<Bullet*> tempVec;
		for (size_t i = 0; i < indexesToDelete.size(); i++)
		{
			//SafeDelete(mainPlayer->bullets[indexesToDelete[i]]);

			//mainPlayer->bullets.erase(mainPlayer->bullets.begin() + indexesToDelete[i]);
			//mainPlayer->bullets[indexesToDelete[i]] = mainPlayer->bullets[mainPlayer->bullets.size() - 1];
			//mainPlayer->bullets.pop_back();
			for (size_t j = 0; j < mainPlayer->bullets.size(); j++)
			{
				if (j = indexesToDelete[i])
				{
					SafeDelete(mainPlayer->bullets[indexesToDelete[i]]);
				}
				else
				{
					tempVec.push_back(mainPlayer->bullets[j]);
				}
			}
		}

		mainPlayer-> bullets = tempVec;
		tempVec.clear();
		//mainPlayer->bullets.shrink_to_fit();
		indexesToDelete.clear();
	}
	*/
	
	//Rendering the player in the world
	mainPlayer->UpdatePosition(m_pCameraMngr->GetPosition(), m_pCameraMngr->GetForward());	//player moves with camera
	mainPlayer->playerModel->AddToRenderList();

	//firstEnemy->enemy->AddToRenderList();

	uint enemyIndexToDelete = -1;
	for (int i = 0; i < enemies.size(); i++) 
	{
		enemies[i]->enemy->AddToRenderList();
		enemies[i]->enemy->GetRigidBody()->AddToRenderList();

		for (size_t j = 0; j < mainPlayer->bullets.size(); j++)
		{
			bool tempBool = mainPlayer->bullets[j]->bulletEntity->IsColliding(enemies[i]->enemy);
			//bool tempBool = enemies[i]->enemy->IsColliding(mainPlayer->bullets[j]->bulletEntity);
			
			if (tempBool)
			{
				std::cout << "Real Collision" << std::endl;
				//std::cout << "enemy[" << i << "] and bullets[" << j << "]" << std::endl;
				//std::cout << glm::to_string(enemies[i]->enemy->GetModelMatrix()) << std::endl;
				bulletIndexToDelete = j;
				enemyIndexToDelete = i;
				score++;
			}

		}
	}	

	if (enemyIndexToDelete != -1)
	{
		for (size_t i = 0; i < m_pEntityMngr->GetEntityCount() - 1; i++)
			if (enemies[enemyIndexToDelete]->enemy == m_pEntityMngr->GetEntityList()[i])
			{
				std::vector<MyEntity*> temp = m_pEntityMngr->GetEntityList();
				temp.erase(temp.begin() + i);
				m_pEntityMngr->SetEntityList(temp);
			}
		enemies[enemyIndexToDelete]->Shrink();
		SafeDelete(enemies[enemyIndexToDelete]);
		enemies.erase(enemies.begin() + enemyIndexToDelete);
		enemyIndexToDelete = -1;
	}
	
	if (bulletIndexToDelete != -1)
	{
		SafeDelete(mainPlayer->bullets[bulletIndexToDelete]);
		mainPlayer->bullets.erase(mainPlayer->bullets.begin() + bulletIndexToDelete);
		bulletIndexToDelete = -1;
		/*for (size_t i = indexesToDelete.size(); i >= 0; i--)
		{
			if (indexesToDelete[i] == 0)
			{
				SafeDelete(mainPlayer->bullets[0]);
				mainPlayer->bullets.erase(mainPlayer->bullets.begin());
			}
			else
			{
				SafeDelete(mainPlayer->bullets[indexesToDelete[i]]);
				mainPlayer->bullets.erase(mainPlayer->bullets.begin() + (indexesToDelete[i] - 1));
			}
		}
		*/
	}

	/*for (int i = 0; i < m_pOctant->GetOctantCount(); i++) 
	{
		MyOctant* temp = m_pOctant->GetChild(i);

		for (size_t j = 0; j < mainPlayer->bullets.size(); j++)
		{
			bool tempBool = mainPlayer->bullets[j]->bulletEntity->GetRigidBody()->IsColliding(temp->GetRigidBody());
			//bool tempBool = enemies[i]->enemy->IsColliding(mainPlayer->bullets[j]->bulletEntity);
			
			if (tempBool)
			{
				std::cout << "Real Collision" << std::endl;
				std::cout << "octant[" << i << "] and bullets[" << j << "]" << std::endl;
				//m_pMeshMngr->AddWireCubeToRenderList(enemies[i]->enemy->GetModelMatrix(), C_RED);
				//std::cout << glm::to_string(enemies[i]->enemy->GetModelMatrix()) << std::endl;

				score++;
			}

		}
	}
	*/
	/*m_pMeshMngr->Print("Colliding: ");
	if (bColliding)
		m_pMeshMngr->PrintLine("YES!", C_RED);
	else
		m_pMeshMngr->PrintLine("no", C_YELLOW);
		*/
	m_pMeshMngr->Printf(C_BLACK, "Current Score: %i", score);
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

	for (size_t i = 0; i < walls.size(); i++)
		SafeDelete(walls[i]);

	for (auto & x : enemies)
		SafeDelete(x);

	SafeDelete(m_pModel);
	SafeDelete(m_pModelRB);
	SafeDelete(m_pCollisionModel);
	SafeDelete(m_pCollisionModelRB);
	SafeDelete(m_pHogwarts);
	SafeDelete(mainPlayer);
	//SafeDelete(firstEnemy);
}