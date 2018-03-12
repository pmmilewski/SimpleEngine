////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightshaderclass.h"

LightShaderClass::LightShaderClass()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_sampleState = nullptr;
	m_matrixBuffer = nullptr;
	m_lightBuffer = nullptr;
}

LightShaderClass::LightShaderClass(const LightShaderClass &other)
{
}

LightShaderClass::~LightShaderClass()
{
}

bool LightShaderClass::Initialize(ID3D11Device *device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"light.vs", L"light.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

void LightShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

bool LightShaderClass::Render(ID3D11DeviceContext *deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView *texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{

	return true;
}
