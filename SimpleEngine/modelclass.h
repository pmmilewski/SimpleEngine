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
// MY INCLUDES			     	  //
///////////////////////////////////
#include "textureclass.h"

/////////////////////////////////////
// Class name: ModelClass		  //
///////////////////////////////////
class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
};

#endif // !_MODELCLASS_H_