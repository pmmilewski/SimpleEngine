#pragma once
//////////////////////////////////////
// Filename: modelclass.h		    //
//////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_



/////////////////////////////////////
// INCLUDES						  //
///////////////////////////////////
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

/////////////////////////////////////
// Class name: ModelClass		  //
///////////////////////////////////
class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};



#endif // !_MODELCLASS_H_