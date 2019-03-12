#include "ModelHolder.h"



ModelHolder::ModelHolder(D3DClass* d3d)
{
	m_D3D = d3d;
	m_floor = 0;
	m_InnerWall = 0;
	m_Door = 0;
	m_Frame = 0;
	m_WindowFrameTop = 0;
	m_WindowFrameBot = 0;
	m_Table = 0;
	m_OuterWall = 0;
	m_Roof = 0;
}


ModelHolder::~ModelHolder()
{
	Shutdown();
}

void ModelHolder::Shutdown()
{
	if (m_floor)
	{
		m_floor->Shutdown();
		delete m_floor;
		m_floor = 0;
	}

	if (m_InnerWall)
	{
		m_InnerWall->Shutdown();
		delete m_InnerWall;
		m_InnerWall = 0;
	}

	if (m_Door)
	{
		m_Door->Shutdown();
		delete m_Door;
		m_Door = 0;
	}

	if (m_Frame)
	{
		m_Frame->Shutdown();
		delete m_Frame;
		m_Frame = 0;
	}

	if (m_WindowFrameTop)
	{
		m_WindowFrameTop->Shutdown();
		delete m_WindowFrameTop;
		m_WindowFrameTop = 0;
	}

	if (m_WindowFrameBot)
	{
		m_WindowFrameBot->Shutdown();
		delete m_WindowFrameBot;
		m_WindowFrameBot = 0;
	}

	if (m_Table)
	{
		m_Table->Shutdown();
		delete m_Table;
		m_Table = 0;
	}

	if (m_OuterWall)
	{
		m_OuterWall->Shutdown();
		delete m_OuterWall;
		m_OuterWall = 0;
	}

	if (m_Roof)
	{
		m_Roof->Shutdown();
		delete m_Roof;
		m_Roof = 0;
	}
}

bool ModelHolder::Initialize()
{
	bool result;

	// Create the model object.
	m_floor = new ModelClass;
	if (!m_floor)
	{
		return false;
	}

	// Initialize the floor object.
	result = m_floor->Initialize(m_D3D->GetDevice(), "../Engine/data/Floor.txt", L"../Engine/data/StoneSlab.dds");
	if (!result)
	{
		return false;
	}

	//Create the InnerWall object.
	m_InnerWall = new ModelClass;
	if (!m_InnerWall)
	{
		return false;
	}

	//Initialize the InnerWall object.
	result = m_InnerWall->Initialize(m_D3D->GetDevice(), "../Engine/data/InnerWall.txt", L"../Engine/data/Wall_Specular.dds");
	if (!result)
	{
		return false;
	}

	
	//Create the Door object.
	m_Door = new ModelClass();
	if (!m_Door) { return false; }

	//Initialize the Door object.
	result = m_Door->Initialize(m_D3D->GetDevice(), "../Engine/data/door.txt", L"../Engine/data/Door.dds");
	if (!result)
	{ 
		return false;
	}

	//Create the frame object.
	m_Frame = new ModelClass();
	if (!m_Frame) { return false; }

	//Initialise the frame object.
	result = m_Frame->Initialize(m_D3D->GetDevice(), "../Engine/data/frame.txt", L"../Engine/data/Frame.dds");
	if(!result)
	{ 
		return false; 
	}

	//Create the window frame object.
	m_WindowFrameTop = new ModelClass();
	if (!m_WindowFrameTop)
		return false;

	//Initialize the window frame object.
	result = m_WindowFrameTop->Initialize(m_D3D->GetDevice(), "../Engine/data/WindowFrameTop.txt", L"../Engine/data/BlackMarbleTop.dds");
	if (!result)
		return false;

	//Create the bottom of the windowframe object.
	m_WindowFrameBot = new ModelClass();
	if (!m_WindowFrameBot)
		return false;

	//Initialize the windowframebot object.
	result = m_WindowFrameBot->Initialize(m_D3D->GetDevice(), "../Engine/data/WindowFrameBot.txt", L"../Engine/data/BlackMarbleTop.dds");
	if (!result)
		return false;

	//Create the table object.
	m_Table = new ModelClass();
	if (!m_Table)
		return false;

	//Initialize the table object.
	result = m_Table->Initialize(m_D3D->GetDevice(), "../Engine/data/Table.txt", L"../Engine/data/Wood.dds");
	if (!result)
		return false;

	//Create the outerWall object.
	m_OuterWall = new ModelClass();
	if (!m_OuterWall)
		return false;

	//Initialize the outerwall object.
	result = m_OuterWall->Initialize(m_D3D->GetDevice(), "../Engine/data/OuterWall2.txt", L"../Engine/data/stone.dds");
	if (!result)
		return false;

	//Create the pillar object.
	m_Roof = new ModelClass();
	if (!m_Roof)
		return false;

	//Initialize the pillar object.
	result = m_Roof->Initialize(m_D3D->GetDevice(), "../Engine/data/RoofPlane.txt", L"../Engine/data/Metal.dds");
	if (!result)
		return false;

	return true;

	
}

ModelClass * ModelHolder::getFloor()
{
	return m_floor;
}

ModelClass* ModelHolder::getInnerWall()
{
	return m_InnerWall;
}

ModelClass* ModelHolder::getDoor()
{
	return m_Door;
}

ModelClass* ModelHolder::getFrame()
{
	return m_Frame;
}

ModelClass* ModelHolder::getWindowFrameTop()
{
	return m_WindowFrameTop;
}

ModelClass* ModelHolder::getWindowFrameBot()
{
	return m_WindowFrameBot;
}

ModelClass* ModelHolder::getTable()
{
	return m_Table;
}

ModelClass* ModelHolder::getOuterWall()
{
	return m_OuterWall;
}

ModelClass* ModelHolder::getRoof()
{
	return m_Roof;
}


