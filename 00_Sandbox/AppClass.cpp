#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Music
	String sRoute = m_pSystem->m_pFolder->GetFolderData();
	sRoute += m_pSystem->m_pFolder->GetFolderAudio();
	
#pragma region Sandbox
	//Background music
	m_soundBGM.openFromFile(sRoute + "elementary-wave-11.ogg");
	//m_soundBGM.openFromFile(sRoute + "Music.ogg");

	m_soundBGM.play();
	m_soundBGM.setLoop(true);

	//sound effect
	//m_soundBuffer.loadFromFile(sRoute + "12C.wav");
	m_soundBuffer.loadFromFile(sRoute + "Magic.ogg");

	m_sound.setBuffer(m_soundBuffer);

	//load model			//currently minecraft block
	m_pModel = new Simplex::Model();
	m_pModel->Load("Minecraft\\Cube.obj");
	m_pModelRB = new MyRigidBody(m_pModel->GetVertexList());

	//load collision model	//currently warp pipe
	m_pCollisionModel = new Model();
	m_pCollisionModel->Load("Mario\\WarpPipe.obj");
	m_pCollisionModelRB = new MyRigidBody(m_pCollisionModel->GetVertexList());

	// instantiate entities for walls
	walls.push_back(new MyEntity("HarryPotter\\Cube.obj"));
	walls.push_back(new MyEntity("HarryPotter\\TransCube.obj"));
	walls.push_back(new MyEntity("HarryPotter\\TransCube.obj"));
	walls.push_back(new MyEntity("HarryPotter\\TransCube.obj"));
	walls.push_back(new MyEntity("HarryPotter\\TransCube.obj"));
	walls.push_back(new MyEntity("HarryPotter\\TransCube.obj"));

	mainPlayer = new Player();
	firstEnemy = new Enemy(vector3(10, 0, 0));
	m_pEntityMngr->AddEntity(firstEnemy->enemy);

	for (int i = 0; i < 30; i++) {
		enemies.push_back(new Enemy(vector3(rand()%50-25, rand() % 50 + 5, rand() % 50 - 25)));
		m_pEntityMngr->AddEntity(enemies[i]->enemy);
	}

	m_pOctant = new MyOctant(2, 5);
	m_pEntityMngr->Update();

	// load hogwarts bg
	m_pHogwarts = new Model();
	m_pHogwarts->Load("HarryPotter\\hog_color.fbx");

	//mainPlayer->CreatePlayer();
	/*m_pPlayerModel = new Model();
	//m_pPlayerModel->Load("HarryPotter\\Br	oom.obj");
	m_pPlayerModel->Load("Mario\\Bowser.obj");

	m_pPlayerRB = new MyRigidBody(m_pPlayerModel->GetVertexList());
	*/
#pragma endregion
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Move light... just for fun...
	static double dTimer = 0.0f; //create a variable to store time
	static uint uClock = m_pSystem->GenClock(); //generate a clock to track time
	dTimer += m_pSystem->GetDeltaTime(uClock); //get the time difference since last time called
	double dAngle = MapValue(dTimer, 0.0, 5.0, 0.0, 360.0);//map the value so we do not need to wait 360 seconds, only 5
	
	static vector3 v3Color(C_WHITE); //color of the light
	vector3 v3Position(glm::sin(glm::radians(dAngle)) * 5.0f, 2.5f, glm::cos(glm::radians(dAngle)) * 5.0f);//holds position of light
	m_pLightMngr->SetPosition(v3Position, 1); //set the position of first light(0 is reserved for global light)
	m_pLightMngr->SetIntensity(5.0f, 1); //set the intensity of first light
	m_pLightMngr->SetColor(v3Color, 1); //set the color of first light
	m_pMeshMngr->AddSphereToRenderList(glm::translate(v3Position) * glm::scale(vector3(0.15f)), v3Color, RENDER_SOLID); //add a sphere to "see" it

	matrix4 mHogwarts = glm::translate(vector3(0, 0, -25)) * glm::scale(vector3(0.1));
	m_pHogwarts->SetModelMatrix(mHogwarts);
	m_pMeshMngr->AddAxisToRenderList(mHogwarts);

	// disable drawing the cube in the center of view

	////Set model matrix to the model			//minecraft block
	//matrix4 mModel = glm::translate(m_v3Model) * ToMatrix4(m_qModel) * ToMatrix4(m_qArcBall);	//WHERE ARE THESE VALUES BEING SET?
	//m_pModel->SetModelMatrix(mModel);
	//m_pModelRB->SetModelMatrix(mModel);
	//m_pMeshMngr->AddAxisToRenderList(mModel);

	//Set model matrix to CollisionModel	//WARP PIPE
	matrix4 mCollisionModel = glm::translate(vector3(2.25f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, glm::radians(-55.0f), AXIS_Z);
	m_pCollisionModel->SetModelMatrix(mCollisionModel);
	m_pCollisionModelRB->SetModelMatrix(mCollisionModel);
	m_pMeshMngr->AddAxisToRenderList(mCollisionModel);

	//Set model matrix to the model			//PLAYER (BOWSER)
	//matrix4 mPlayerMatrix = glm::translate(vector3( m_pCameraMngr->GetPosition().x, m_pCameraMngr->GetPosition().y - 3.0f, m_pCameraMngr->GetPosition().z));
	//m_pPlayerModel->SetModelMatrix(mPlayerMatrix);
	//m_pPlayerRB->SetModelMatrix(mPlayerMatrix);
	m_pMeshMngr->AddAxisToRenderList(mainPlayer->UpdatePosition(m_pCameraMngr->GetPosition(), m_pCameraMngr->GetForward()));	//player moves with camera
	
	//Set model matrix to the model			//ENEMY (BOO)
	//MODEL MATRIX SET IN ENEMY'S UpdatePosition METHOD
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->Update();
		m_pMeshMngr->AddAxisToRenderList(enemies[i]->mEnemyMatrix);	//accesses enemy's model matrix, which is based off of m_v3Model (which isnt set anywhere how does this even work)
	}


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

	static bool renderModel = true;
	static bool renderColModel = true;
	static std::vector<int> indexesToDelete;
	static std::vector<std::vector<vector3>> localMinMax = m_pOctant->GetMinMaxList();
	static int score = 0;

	//Looping through each bullet in the field
	for (size_t i = 0; i < mainPlayer->bullets.size(); i++)
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
		}

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
	}

	m_pEntityMngr->Update();
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
	
	bool bColliding = mainPlayer->playerRB->IsColliding(walls[0]->GetRigidBody());

	m_pHogwarts->AddToRenderList();

	if (renderModel)
	{
		m_pModel->AddToRenderList();
		m_pModelRB->AddToRenderList();
	}

	if (renderColModel)
	{
		m_pCollisionModel->AddToRenderList();
		m_pCollisionModelRB->AddToRenderList();
	}
	//Rendering the player in the world
	mainPlayer->playerModel->AddToRenderList();
	mainPlayer->playerRB->AddToRenderList();

	firstEnemy->enemy->AddToRenderList();

	for (int i = 0; i < enemies.size(); i++) 
	{
		enemies[i]->enemy->AddToRenderList();

		for (size_t j = 0; j < mainPlayer->bullets.size(); j++)
		{
			bool tempBool = mainPlayer->bullets[j]->bulletEntity->IsColliding(enemies[i]->enemy);
			//bool tempBool = enemies[i]->enemy->IsColliding(mainPlayer->bullets[j]->bulletEntity);
			
			if (tempBool)
			{
				std::cout << "Real Collision" << std::endl;
				std::cout << "enemy[" << i << "] and bullets[" << j << "]" << std::endl;
				m_pMeshMngr->AddWireCubeToRenderList(enemies[i]->enemy->GetModelMatrix(), C_RED);
				std::cout << glm::to_string(enemies[i]->enemy->GetModelMatrix()) << std::endl;

				score++;
			}

		}
	}

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
	SafeDelete(firstEnemy);
}