#pragma once
#include "modelclass.h"
#include "d3dclass.h"

class ModelHolder
{
public:
	ModelHolder(D3DClass* d3d);
	~ModelHolder();

	void Shutdown();

	bool Initialize();

	ModelClass* getFloor();
	ModelClass* getInnerWall();
	ModelClass* getDoor();
	ModelClass* getFrame();
	ModelClass* getWindowFrameTop();
	ModelClass* getWindowFrameBot();
	ModelClass* getTable();
	ModelClass* getOuterWall();
	ModelClass* getRoof();

private:

	ModelClass * m_floor;
	ModelClass * m_InnerWall;
	ModelClass * m_Door;
	ModelClass * m_Frame;
	ModelClass * m_WindowFrameTop;
	ModelClass * m_WindowFrameBot;
	ModelClass * m_Table;
	ModelClass * m_OuterWall;
	ModelClass * m_Roof;

	D3DClass* m_D3D;

};

