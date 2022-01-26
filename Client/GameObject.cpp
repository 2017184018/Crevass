#include "pch.h"
#include "GameObject.h"

static unsigned int s_currentIndex = 0;

GameObject::GameObject(RenderLayer layer, std::string type, std::string id):
	m_Layer(layer),
	m_Type(type),
	m_ID(id),
	m_Index(s_currentIndex++)
{
	World = MathHelper::Identity4x4();
	TexTransform = MathHelper::Identity4x4();

}

GameObject::~GameObject()
{
}

