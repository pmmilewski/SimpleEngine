#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Direct3D = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_Direct3D = new D3DClass;
	if (m_Direct3D == nullptr)
	{
		return false;
	}

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Cound not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}
	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render()
{
	m_Direct3D->BeginScene(1.0f, 1.0f, 0.0f, 1.0f);
	m_Direct3D->EndScene();

	return true;
}
