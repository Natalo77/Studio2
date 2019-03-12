////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <vector>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"

using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void CreateSphere(int LatLines, int LongLines);

	struct Vertex
	{
		XMVECTOR pos;
		DirectX::PackedVector::XMCOLOR colour;
		XMFLOAT3 Pos;
	};

	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;
	};

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;


	//-----SKYBOX WORK-----
	int NumSphereVertices;
	int NumSphereFaces;

	ID3D11Buffer* m_SphereIndexBuffer;
	ID3D11Buffer* m_SphereVertBuffer;

	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11VertexShader* VS;
	ID3D10Blob* VS_Buffer;
	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;

	ID3D11ShaderResourceView* m_smrv;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	ID3D11SamplerState* CubesTexSamplerState;

	ID3D11Buffer* cbPerObjectBuffer;

	D3DClass::cbPerObject cbPerObj;

public:
	void doThings();
	//-----SKYBOX WORK-----

	
};

#endif