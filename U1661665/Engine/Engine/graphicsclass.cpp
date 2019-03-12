////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Input = 0;
	m_D3D = 0;
	m_Timer = 0;
	m_ShaderManager = 0;
	m_Light = 0;
	m_LightShader = 0;
	m_Position = 0;
	m_Camera = 0;
	m_ModelHolder = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass(){
}


bool GraphicsClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;

	//Create the input object.
	//The input object will be used to handle reading the keyboard and mouse input.
	m_Input = new InputClass();
	if (!m_Input)
	{
		return false;
	}

	//Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the shader manager object.
	m_ShaderManager = new ShaderManagerClass;
	if(!m_ShaderManager)
	{
		return false;
	}

	// Initialize the shader manager object.
	result = m_ShaderManager->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	//Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	//Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		return false;
	}

	//Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	//Set the initial position and rotation of the viewer.
	m_Position->SetPosition(0.0f, 0.0f, -10.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	//m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	//Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	
	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, -0.5f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(64.0f);

	//Create the light shader object.
	m_LightShader = new LightShaderClass();
	if (!m_LightShader)
		return false;

	//Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
	

	//Create the modelHolder object.
	m_ModelHolder = new ModelHolder(m_D3D);

	//Initialize the modelHolder object.
	result = m_ModelHolder->Initialize();
	if (!result)
	{
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_ModelHolder)
	{
		delete m_ModelHolder;
		m_ModelHolder = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the shader manager object.
	if(m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	//Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	//Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	//Update the system stats.
	m_Timer->Frame();

	//Read the user input.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	//Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	//Do the frame input processing.
	result = HandleMovementInput(m_Timer->GetTime());
	{
		if (!result)
		{
			return false;
		}
	}

	//Render the graphics.
	result = Render();
	if (!result)
	{
		return false;
	}


	return true;
}

bool GraphicsClass::HandleMovementInput(float frameTime)
{
	bool keyDown;
	bool sprintDown;
	float posX, posY, posZ, rotX, rotY, rotZ;

	//Set the frame time for calculating the updated position.
	m_Position->setFrameTime(frameTime);

	//Handle the input.
	sprintDown = m_Input->IsShiftPressed();

	

	keyDown = m_Input->IsWPressed();
	m_Position->MoveForward(keyDown, sprintDown);

	keyDown = m_Input->IsSPressed();
	m_Position->MoveBackward(keyDown, sprintDown);

	keyDown = m_Input->IsAPressed();
	m_Position->MoveLeft(keyDown, sprintDown);

	keyDown = m_Input->IsDPressed();
	m_Position->MoveRight(keyDown, sprintDown);

	keyDown = m_Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsRightPressed();
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsSpacePressed();
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsCtrlPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsDownPressed();
	m_Position->LookDownward(keyDown);

	//Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	//Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	return true;
}

bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	bool result;

	static float rotation = 0.0f;

	//Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.0005f * m_Timer->GetTime();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//Render the Floors.
	if (!RenderFloors(&worldMatrix, &viewMatrix, &projectionMatrix, &translateMatrix, &rotation))
		return false;

	//Render the inner walls.
	if (!RenderInnerWalls(&worldMatrix, &viewMatrix, &projectionMatrix, &translateMatrix, &rotation))
		return false;

	//Render the Interrogation room door
	if (!RenderDoor(&worldMatrix, &viewMatrix, &projectionMatrix, &translateMatrix, &rotation))
		return false;

	//Render the one way mirror frame.
	if (!RenderWindowFrame(&worldMatrix, &viewMatrix, &projectionMatrix, &translateMatrix, &rotation))
		return false;

	//Render the skybox.
	//RenderSkybox(&worldMatrix, &viewMatrix, &projectionMatrix);

	//Render the table.
	if (!RenderTable(&worldMatrix, &viewMatrix, &projectionMatrix, &translateMatrix, &rotation))
		return false;

	//Render the outerwalls
	if (!RenderOuterWalls(&worldMatrix, &viewMatrix, &projectionMatrix, &translateMatrix, &rotation))
		return false;

	//Render the pillars.
	if (!RenderRoof(&worldMatrix, &viewMatrix, &projectionMatrix, &translateMatrix, &rotation))
		return false;


	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}


bool GraphicsClass::RenderFloors(XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix, XMMATRIX* translateMatrix, float* rotation)
{
	bool result;
	ModelClass* floorObj = m_ModelHolder->getFloor();

	float xMod = 0.f;
	float zMod = 0.f; 
	for (int x = 0; x < 13; x++)
	{
		m_D3D->GetWorldMatrix(*worldMatrix);
		*translateMatrix = XMMatrixTranslation(0.0f + xMod, 0.0f, 0.0f + zMod);
		XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);
		

		//Render the model.


		// Render the first model using the texture shader.
		floorObj->Render(m_D3D->GetDeviceContext());

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), floorObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			floorObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		//This section loops through changing the x and z modifiers like a typewriter.
		if (x != 0)
		{
			if (x % 4 == 0)
			{
				zMod += 50;
				xMod = 0;
			}
			else { xMod += 50; }
		}
		


		if (!result)
		{
			return false;
		}	
	}

	return true;


}

bool GraphicsClass::RenderInnerWalls(XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix, XMMATRIX* translateMatrix, float* rotation)
{
	bool result;
	ModelClass* innerWallObj = m_ModelHolder->getInnerWall();

	XMMATRIX scaleMatrix = XMMatrixScaling(0.47575f, 0.5f, 0.5f);
	XMVECTOR axis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX rotationMatrix = XMMatrixRotationAxis(axis, XMConvertToRadians(0.0f));

	innerWallObj->Render(m_D3D->GetDeviceContext());

	//Render back wall.
	for (int x = 0; x < 8; x++)
	{
		m_D3D->GetWorldMatrix(*worldMatrix);
		*translateMatrix = XMMatrixTranslation(-12.5f + (x * 25.0f), 0.0f, 125.0f);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), innerWallObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			innerWallObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result) { return false; }
	}

	//Render left wall. 
	rotationMatrix = XMMatrixRotationAxis(axis, XMConvertToRadians(-90.0f));
	for (int x = 0; x < 3; x++)
	{
		m_D3D->GetWorldMatrix(*worldMatrix);
		*translateMatrix = XMMatrixTranslation(62.5f + x * 25.0f, 0.0f, 25.0f);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, rotationMatrix);

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), innerWallObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			innerWallObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result) { return false; }
	}

	//Render right wall.
	rotationMatrix = XMMatrixRotationAxis(axis, XMConvertToRadians(90.0f));
	for (int x = 0; x < 3; x++)
	{
		m_D3D->GetWorldMatrix(*worldMatrix);
		*translateMatrix = XMMatrixTranslation(-112.5f + x * 25.0f, 0.0f, 175.0f);
		
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, rotationMatrix);

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), innerWallObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			innerWallObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result) { return false; }
	}

	//Render forward wall.
	rotationMatrix = XMMatrixRotationAxis(axis, XMConvertToRadians(180.0f));
	int xMod = 0;
	for (int x = 0; x < 4; x++)
	{
		
		if (x == 1) { xMod = 41.75; }
		if (x == 2) { xMod = 66.75; }
		if (x == 3) { xMod = 175.0f; }
		m_D3D->GetWorldMatrix(*worldMatrix);
		*translateMatrix = XMMatrixTranslation(-162.5f + xMod, 0.0f, -50.0f);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, rotationMatrix);

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), innerWallObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			innerWallObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result) { return false; }
	}

	return true;
}

bool GraphicsClass::RenderDoor(XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix, XMMATRIX* translateMatrix, float* rotation)
{
	bool result;
	ModelClass* doorObj = m_ModelHolder->getDoor();

	m_D3D->GetWorldMatrix(*worldMatrix);

	//Set up the scaling and transformation
	XMMATRIX scaleMatrix = XMMatrixScaling(3.58f, 3.55f, 5.0f);
	*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

	//Translate the door away from the origin.
	*translateMatrix = XMMatrixTranslation(-4.0f, -0.075f, -0.5f);
	*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

	//Calculate the angle of rotation.
	float angle = sin(*rotation) * 10 / 11.25 /*22.5*/;

	//Rotate around the edge of the door.
	*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationY(-angle));

	//Calculate abitrary translation modifiers.
	float x1 = 4.0f;
	float z1 = 0.0f;
	float x2 = 4.0 * cos(angle);
	float z2 = 4.0 * sin(angle);

	float xDiff = x1 - x2;
	float zDiff = z1 - z2;

	//Translate into position.
	*translateMatrix = XMMatrixTranslation(146.0f + xDiff, 0.0f, 49.5 + zDiff);
	*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

	//Render the door.
	doorObj->Render(m_D3D->GetDeviceContext());

	//Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), doorObj->GetIndexCount(),
		*worldMatrix, *viewMatrix, *projectionMatrix,
		doorObj->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(),
		m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
		return false;

	//Render the frame
	//Frame operations
	ModelClass* frameObj = m_ModelHolder->getFrame();
	m_D3D->GetWorldMatrix(*worldMatrix);

	scaleMatrix = XMMatrixScaling(3.58f, 3.58f, 5.0f);

	*translateMatrix = XMMatrixTranslation(141.75f, -0.075f, 50.0f);
	*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);
	*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

	
	frameObj->Render(m_D3D->GetDeviceContext());

	//Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), frameObj->GetIndexCount(),
		*worldMatrix, *viewMatrix, *projectionMatrix,
		frameObj->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(),
		m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	return result;
	
}

bool GraphicsClass::RenderWindowFrame(XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix, XMMATRIX* translateMatrix, float* rotation)
{
	bool result;

	//Set up the scaling and transformation
	XMMATRIX scaleMatrix = XMMatrixScaling(0.4925f, 0.5235f, 1.5f);
	*translateMatrix = XMMatrixTranslation(42.0f, 0.0f, 50.0f);

	//Render the top of the windowframe.
	ModelClass* wFrameObj = m_ModelHolder->getWindowFrameTop();

	wFrameObj->Render(m_D3D->GetDeviceContext());

	m_D3D->GetWorldMatrix(*worldMatrix);

	*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);
	*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), wFrameObj->GetIndexCount(),
		*worldMatrix, *viewMatrix, *projectionMatrix,
		wFrameObj->GetTexture(), 
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
		m_Camera->GetPosition(), 
		m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	//Render the bottom of the window frame.
	wFrameObj = m_ModelHolder->getWindowFrameBot();

	wFrameObj->Render(m_D3D->GetDeviceContext());

	m_D3D->GetWorldMatrix(*worldMatrix);

	*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);
	*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

	//Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), wFrameObj->GetIndexCount(),
		*worldMatrix, *viewMatrix, *projectionMatrix,
		wFrameObj->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(),
		m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	return result;
}

bool GraphicsClass::RenderTable(XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix, XMMATRIX* translateMatrix, float* rotation)
{
	bool result;
	ModelClass* tableObj = m_ModelHolder->getTable();

	m_D3D->GetWorldMatrix(*worldMatrix);

	//Set up the scaling and transformation
	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.4f, 0.5f);
	*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

	//Rotate 90 degrees
	*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationY(XMConvertToRadians(90)));

	//Translate into position.
	*translateMatrix = XMMatrixTranslation(42.0f, 0.0f, 87.5f);
	*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

	//Render the table.
	tableObj->Render(m_D3D->GetDeviceContext());

	//Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), tableObj->GetIndexCount(),
		*worldMatrix, *viewMatrix, *projectionMatrix,
		tableObj->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(),
		m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
		return false;

	return true;
}

void GraphicsClass::RenderSkybox(XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix)
{
	//get the world matrix.
	m_D3D->GetWorldMatrix(*worldMatrix);

	//Define sphereWorld's world space matrix.
	XMMATRIX scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);

	//Make sure the sphere is always centered around the camera.
	XMMATRIX translation = XMMatrixTranslation(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);

	//Set sphere world's world space using the transformations.
	*worldMatrix = XMMatrixMultiply(*worldMatrix, scale);
	*worldMatrix = XMMatrixMultiply(*worldMatrix, translation);

	m_D3D->doThings();
}

bool GraphicsClass::RenderOuterWalls(XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix, XMMATRIX* translateMatrix, float* rotation)
{
	bool result;
	int mod;
	ModelClass* outerWallObj = m_ModelHolder->getOuterWall();


	//Scale the model.
	XMMATRIX scaleMatrix = XMMatrixScaling(0.005f, 0.00525f, 0.005f);
	outerWallObj->Render(m_D3D->GetDeviceContext());

	//Render mid wall.
	for (int x = 0; x < 4; x++)
	{
		//Change the x modifier based on the iteration of model.
		switch (x)
		{
		case 0:
			mod = 0;
			break;
		case 1:
			mod = 109.5;
			break;
		case 2:
			mod = 134.5;
			break;
		case 3:
			mod = 175;
			break;
		}

		//Get the world matrix.
		m_D3D->GetWorldMatrix(*worldMatrix);

		//Scale the model.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

		//Rotate the model.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationY(XMConvertToRadians(90)));

		//Translate the model into position using the position modifier previously calculated.
		*translateMatrix = XMMatrixTranslation(-12.5f + (float)mod, 0.0f, 48.5f);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), outerWallObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			outerWallObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result) { return false; }
	}

	
	//Render left wall. 
	for (int x = 0; x < 6; x++)
	{
		//Get the world matrix.
		m_D3D->GetWorldMatrix(*worldMatrix);

		//Scale the model.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

		//No rotation needed.

		//Translate the model into position using the x value as a modifier.
		*translateMatrix = XMMatrixTranslation(-26.25f, 0.0f, -12.5 + x * 25.0f);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), outerWallObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			outerWallObj->GetTexture(), 
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result) { return false; }
	}

	
	//Render right wall.
	for (int x = 0; x < 6; x++)
	{
		//Get the world matrix.
		m_D3D->GetWorldMatrix(*worldMatrix);

		//Scale the model.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

		//No rotation needed.

		//Translate the model into position using the x value as a modifier.
		*translateMatrix = XMMatrixTranslation(176.25f, 0.0f, -12.5 + x * 25.0f);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), outerWallObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			outerWallObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result) { return false; }
	}

	
	//Render back wall.
	for (int x = 0; x < 8; x++)
	{
		//Get the world matrix.
		m_D3D->GetWorldMatrix(*worldMatrix);

		//Scale the model.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

		//Rotate the model.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationY(XMConvertToRadians(90)));

		//Translate the model into position using the x value as a modifier.
		*translateMatrix = XMMatrixTranslation(-12.5f + x * 25.0f, 0.0f, 126.25f);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), outerWallObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			outerWallObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result) { return false; }
	}

	//Render Forward wall.
	for (int x = 0; x < 8; x++)
	{
		//Get the world matrix.
		m_D3D->GetWorldMatrix(*worldMatrix);

		//Scale the model.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

		//Rotate the model.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationY(XMConvertToRadians(90)));

		//Translate the model into position using the x value as a modifier.
		*translateMatrix = XMMatrixTranslation(-12.5f + x * 25.0f, 0.0f, -25.0f);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), outerWallObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			outerWallObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		
		if (!result) 
			return false;
	}

	return true;
}

bool GraphicsClass::RenderRoof(XMMATRIX* worldMatrix, XMMATRIX* viewMatrix, XMMATRIX* projectionMatrix, XMMATRIX* translateMatrix, float* rotation)
{
	bool result;
	ModelClass* roofObj = m_ModelHolder->getRoof();

	//Set up the scaling.
	XMMATRIX scaleMatrix = XMMatrixScaling(0.834f, 1.0f, 0.5897f);
	
	float zMod = 0;
	float xMod = 0;
	//Render the outofscreen rotated panels.
	for (int x = 0; x < 16; x++)
	{
		if (x == 8)
			zMod = -76.5;
		x > 7 ? xMod = x - 8 : xMod = x;

		//Get the world matrix.
		m_D3D->GetWorldMatrix(*worldMatrix);

		//Scale the model
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

		//Rotate the model.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationX(XMConvertToRadians(45)));

		//Translate into position.
		*translateMatrix = XMMatrixTranslation(-12.5f + (xMod * 25.0f), 32.25f, 118.75f + zMod);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

		//Render the RoofPlane.
		roofObj->Render(m_D3D->GetDeviceContext());

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), roofObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			roofObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result)
			return false;
	}

	zMod = 0;
	//Render the intoscreen rotated panels.
	for (int x = 0; x < 16; x++)
	{
		if (x == 8)
			zMod = -74.0f;
		x > 7 ? xMod = x - 8 : xMod = x;

		//Get the world matrix.
		m_D3D->GetWorldMatrix(*worldMatrix);

		//Scale the model
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

		//Rotate the model.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationZ(XMConvertToRadians(180)));
		*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationX(XMConvertToRadians(135)));

		//Translate into position.
		*translateMatrix = XMMatrixTranslation(-12.5f + (xMod * 25.0f), 32.25f, 56.5f + zMod);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

		//Render the RoofPlane.
		roofObj->Render(m_D3D->GetDeviceContext());

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), roofObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			roofObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result)
			return false;
	}

	//Render the Left and right walls.
	xMod = 0;
	zMod = 0;
	float rotator = -90;
	
	//Set up the new scaling.
	scaleMatrix = XMMatrixScaling(0.834f, 1.0f, 0.41f);

	for (int x = 0; x < 12; x++)
	{
		//Get the world matrix.
		m_D3D->GetWorldMatrix(*worldMatrix);

		//Scale the model
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

		//Rotate the model upright
		*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationX(XMConvertToRadians(90)));
		
		if (x == 6)
		{
			xMod = 200.0f;
			//Rotate the model around to fit the right wall.
			rotator = 90;
		}
		x > 5 ? zMod = x - 6 : zMod = x;

		//Rotate the model to fit either the left or right wall.
		*worldMatrix = XMMatrixMultiply(*worldMatrix, XMMatrixRotationY(XMConvertToRadians(rotator)));

		

		//Translate into position.
		*translateMatrix = XMMatrixTranslation(-25.0f + xMod, 32.4f, -12.5f + zMod * 25.0f);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

		//Render the RoofPlane.
		roofObj->Render(m_D3D->GetDeviceContext());

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), roofObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			roofObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result)
			return false;
	}

	//Render the roof panels.
	xMod = 0;
	zMod = 0;
	int multiple = 0;

	//Set up the new scaling.
	scaleMatrix = XMMatrixScaling(0.834f, 1.0f, 0.834f);

	for (int x = 0; x < 32; x++)
	{
		//Get the world matrix.
		m_D3D->GetWorldMatrix(*worldMatrix);

		//Scale the model
		*worldMatrix = XMMatrixMultiply(*worldMatrix, scaleMatrix);

		if (x == 8)
			zMod = -25.0f;
		if (x == 16)
			zMod = -75.0f;
		if (x == 24)
			zMod = -100.0f;

		multiple = x / 8;
		xMod = x - multiple * 8;

		//Translate into position.
		*translateMatrix = XMMatrixTranslation(-12.5f + xMod * 25.0f, 38.5f, 100.0f + zMod);
		*worldMatrix = XMMatrixMultiply(*worldMatrix, *translateMatrix);

		//Render the RoofPlane.
		roofObj->Render(m_D3D->GetDeviceContext());

		//Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), roofObj->GetIndexCount(),
			*worldMatrix, *viewMatrix, *projectionMatrix,
			roofObj->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(),
			m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result)
			return false;
	}

	return true;
}
