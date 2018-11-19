#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Large Patronus Energy: Dementor Adventure";

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);

	//init the camera
	m_pCamera = new MyCamera();
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 3.0f, 13.0f), //Position
		vector3(0.0f, 3.0f, 12.0f),	//Target
		AXIS_Y);					//Up

	//Get the singleton
	m_pMyMeshMngr = MyMeshManager::GetInstance();
	m_pMyMeshMngr->SetCamera(m_pCamera);

	////creeper
	//m_pCreeper = new Model();
	//m_pCreeper->Load("Minecraft\\Creeper.obj");
	//m_pCreeperRB = new MyRigidBody(m_pCreeper->GetVertexList());

	////steve
	//m_pSteve = new Model();
	//m_pSteve->Load("Minecraft\\Steve.obj");
	//m_pSteveRB = new MyRigidBody(m_pSteve->GetVertexList());
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Add objects to the Manager
	for (int j = -50; j < 50; j += 2)
	{
		for (int i = -50; i < 50; i += 2)
		{
			m_pMyMeshMngr->AddConeToRenderList(glm::translate(vector3(i, 0.0f, j)));
		}
	}

	////Set model matrix to the creeper
	//matrix4 mCreeper = glm::translate(m_v3Creeper) * ToMatrix4(m_qCreeper) * ToMatrix4(m_qArcBall);
	//m_pCreeper->SetModelMatrix(mCreeper);
	//m_pCreeperRB->SetModelMatrix(mCreeper);
	//m_pMeshMngr->AddAxisToRenderList(mCreeper);

	////Set model matrix to Steve
	//matrix4 mSteve = glm::translate(vector3(2.25f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, glm::radians(-55.0f), AXIS_Z);
	//m_pSteve->SetModelMatrix(mSteve);
	//m_pSteveRB->SetModelMatrix(mSteve);
	//m_pMeshMngr->AddAxisToRenderList(mSteve);

	//bool bColliding = m_pCreeperRB->IsColliding(m_pSteveRB);

	//m_pCreeper->AddToRenderList();
	//m_pCreeperRB->AddToRenderList();

	//m_pSteve->AddToRenderList();
	//m_pSteveRB->AddToRenderList();

	m_pMeshMngr->Print("Colliding: ");
	if (false)
		m_pMeshMngr->PrintLine("YES!", C_RED);
	else
		m_pMeshMngr->PrintLine("no", C_YELLOW);
}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//Render the list of MyMeshManager
	m_pMyMeshMngr->Render();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the MyMeshManager list
	m_pMyMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the singleton
	MyMeshManager::ReleaseInstance();

	//release the model
	SafeDelete(m_pCreeper);

	//release the rigid body for the model
	SafeDelete(m_pCreeperRB);

	//release the model
	SafeDelete(m_pSteve);

	//release the rigid body for the model
	SafeDelete(m_pSteveRB);

	//release the camera
	SafeDelete(m_pCamera);

	//release GUI
	ShutdownGUI();
}
