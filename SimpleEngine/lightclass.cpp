#include "lightclass.h"

LightClass::LightClass()
{

}

LightClass::LightClass(const LightClass &other)
{

}

LightClass::~LightClass()
{

}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{

}

void LightClass::SetDirection(float x, float y, float z)
{

}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

