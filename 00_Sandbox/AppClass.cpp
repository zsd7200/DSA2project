#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Music
	String sRoute = m_pSystem->m_pFolder->GetFolderData();
	sRoute += m_pSystem->m_pFolder->GetFolderAudio();

#pragma region Make some meshes, add them to a group and add the group to a model
	/*
	Mesh* pMesh1 = new Mesh();
	Mesh* pMesh2 = new Mesh();
	Mesh* pMesh3 = new Mesh();
	pMesh1->GenerateCone(1.0f, 1.0f, 10, C_RED);
	pMesh2->GenerateTorus(1.0f, 0.7f, 10, 10, C_MAGENTA);
	pMesh3->GenerateTube(1.0f, 0.7f, 1.0f, 10, C_BLUE);
	int nMesh1 = m_pMngr->AddMesh(pMesh1);
	int nMesh2 = m_pMngr->AddMesh(pMesh2);

	Group* pGroup1 = new Group();
	pGroup1->AddMesh(nMesh1, MeshOptions(glm::translate(AXIS_Y), WIRE));
	pGroup1->AddMesh(nMesh2, MeshOptions(glm::translate(-AXIS_Y), SOLID));
	Group* pGroup2 = new Group();
	pGroup2->AddMesh(pMesh3, MeshOptions(IDENTITY_M4, SOLID | WIRE));

	m_pModel1 = new Simplex::Model();
	m_pModel1->AddGroup(pGroup1);
	m_pModel1->AddGroup(pGroup2);
	*/
#pragma endregion
#pragma region Load Model and change its rendering properties
	/*
	m_pModel1 = new Simplex::Model();
	m_pModel1->Load("Macross\\YF19.BTO");
	m_pModel1->ChangeMeshOptions(MeshOptions(IDENTITY_M4, SOLID | WIRE), -1, -1);
	*/
#pragma endregion
#pragma region Load Model and change its materials
	/*
	m_pModel1 = new Simplex::Model();
	m_pModel1->Load("Macross\\YF19.BTO");
	//m_pModel1->ChangeMeshOptions(MeshOptions(IDENTITY_M4, SOLID | WIRE), -1, -1);
	m_pMatMngr->AddMaterial("Cloud.png", "Cloud.png");
	m_pModel1->ChangeMaterialOfGroup("Cloud.png", "ALL");
	*/
#pragma endregion
#pragma region Load Model disconect and change its materials
	/*
	m_pModel1 = new Simplex::Model();
	m_pModel1->Load("Macross\\YF19.BTO");
	//m_pModel1->ChangeMeshOptions(MeshOptions(IDENTITY_M4, SOLID | WIRE), -1, -1);
	m_pMatMngr->AddMaterial("Cloud.png", "Cloud.png");
	m_pModel1->DuplicateMaterialsUnique(-1, "_copy"); //dupliocate existing materials
	m_pModel1->CreateMaterials(); //duplicate one per mesh
	m_pModel1->ChangeMaterialOfGroup("Cloud.png", "ALL");
	*/
#pragma endregion
#pragma region Load Model and duplicate onto another one
	/*
	m_pModel3 = new Simplex::Model();
	m_pModel3->Load("Macross\\YF19.BTO");
	m_pModel1 = new Simplex::Model();
	m_pModel1->Instance(m_pModel3, "_copy_");
	*/
#pragma endregion
#pragma region Load Model
	/*
	m_pModel1 = new Simplex::Model();
	m_pModel1->Load("Macross\\YF19.BTO");
	*/
#pragma endregion
#pragma region Sandbox
	//Background music
	m_soundBGM.openFromFile(sRoute + "elementary-wave-11.ogg");
	m_soundBGM.play();
	m_soundBGM.setLoop(true);

	//sound effect
	m_soundBuffer.loadFromFile(sRoute + "12C.wav");
	m_sound.setBuffer(m_soundBuffer);



	//load model			//currently bowser (not the player sprite)
	m_pModel = new Simplex::Model();
	//m_pModel->Load("HarryPotter\\Hogwarts.fbx");
	m_pModel->Load("Mario\\Bowser.obj");
	m_pModelRB = new MyRigidBody(m_pModel->GetVertexList());

	//load collision model	//currently warp pipe
	m_pCollisionModel = new Model();
	m_pCollisionModel->Load("Mario\\WarpPipe.obj");
	m_pCollisionModelRB = new MyRigidBody(m_pCollisionModel->GetVertexList());


	mainPlayer = new Player();
	firstEnemy = new Enemy();

	// load hogwarts bg
	m_pHogwarts = new Model();
	m_pHogwarts->Load("HarryPotter\\hog_color.fbx");

	//mainPlayer->CreatePlayer();
	/*m_pPlayerModel = new Model();
	//m_pPlayerModel->Load("HarryPotter\\Broom.obj");
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

	//Set model matrix to the model			//BOWSER (NOT PLAYER)
	matrix4 mModel = glm::translate(m_v3Model) * ToMatrix4(m_qModel) * ToMatrix4(m_qArcBall);	//WHERE ARE THESE VALUES BEING SET?
	m_pModel->SetModelMatrix(mModel);
	m_pModelRB->SetModelMatrix(mModel);
	m_pMeshMngr->AddAxisToRenderList(mModel);

	//Set model matrix to CollisionModel	//WARP PIPE
	matrix4 mCollisionModel = glm::translate(vector3(2.25f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, glm::radians(-55.0f), AXIS_Z);
	m_pCollisionModel->SetModelMatrix(mCollisionModel);
	m_pCollisionModelRB->SetModelMatrix(mCollisionModel);
	m_pMeshMngr->AddAxisToRenderList(mCollisionModel);

	//Set model matrix to the model			//PLAYER (BOWSER)
	//matrix4 mPlayerMatrix = glm::translate(vector3( m_pCameraMngr->GetPosition().x, m_pCameraMngr->GetPosition().y - 3.0f, m_pCameraMngr->GetPosition().z));
	//m_pPlayerModel->SetModelMatrix(mPlayerMatrix);
	//m_pPlayerRB->SetModelMatrix(mPlayerMatrix);
	m_pMeshMngr->AddAxisToRenderList(mainPlayer->UpdatePosition(m_pCameraMngr->GetPosition()));	//player moves with camera
	
	//Set model matrix to the model			//ENEMY (BOO)
	//MODEL MATRIX SET IN ENEMY'S UpdatePosition METHOD
	//m_pMeshMngr->AddAxisToRenderList(firstEnemy->UpdatePosition(m_pCameraMngr->GetPosition())); // SETS BOO TO CAMERA, NOT WHAT WE WANT
	//m_pMeshMngr->AddAxisToRenderList(firstEnemy->UpdatePosition(m_pCameraMngr->GetPosition()));	//NEEDS A WAY TO ACCESS ENEMY'S MODEL MATRIX

	static bool renderModel = true;
	static bool renderColModel = true;
	//Looping through each bullet in the field
	for (size_t i = 0; i < mainPlayer->bullets.size(); i++)
	{
		//Adding to the render list
		m_pMeshMngr->AddAxisToRenderList(mainPlayer->bullets[i]->UpdatePosition());
		mainPlayer->bullets[i]->bulletModel->AddToRenderList();
		mainPlayer->bullets[i]->bulletRB->AddToRenderList();

		bool tempBool = m_pModelRB->IsColliding(mainPlayer->bullets[i]->bulletRB);
		if (tempBool)
			renderModel = false;
		tempBool = m_pCollisionModelRB->IsColliding(mainPlayer->bullets[i]->bulletRB);
		if (tempBool)
			renderColModel = false;
	}


	bool bColliding = m_pModelRB->IsColliding(m_pCollisionModelRB);

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

	//m_pPlayerModel->AddToRenderList();
	//m_pPlayerRB->AddToRenderList();
	mainPlayer->playerModel->AddToRenderList();
	mainPlayer->playerRB->AddToRenderList();

	//firstEnemy->enemyModel->AddToRenderList();
	//firstEnemy->enemyRB->AddToRenderList();

	//std::cout << "Forward: " << m_pCameraMngr->GetForward().x << " " << m_pCameraMngr->GetForward().y << " " << m_pCameraMngr->GetForward().z << std::endl;

	m_pMeshMngr->Print("Colliding: ");
	if (bColliding)
		m_pMeshMngr->PrintLine("YES!", C_RED);
	else
		m_pMeshMngr->PrintLine("no", C_YELLOW);

	//std::cout << "Cam Position: " << m_pCameraMngr->GetPosition().x <<  " " << m_pCameraMngr->GetPosition().y <<  " " << m_pCameraMngr->GetPosition().z << std::endl;
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

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
	SafeDelete(m_pModel);
	SafeDelete(m_pModelRB);
	SafeDelete(m_pCollisionModel);
	SafeDelete(m_pCollisionModelRB);
	SafeDelete(m_pHogwarts);
	SafeDelete(mainPlayer);
	SafeDelete(firstEnemy);
}