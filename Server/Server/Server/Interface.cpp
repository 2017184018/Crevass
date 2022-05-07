#include "Interface.h"

void Interface::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

DirectX::XMFLOAT3 Interface::GetPos()
{
	return m_pos;
}

