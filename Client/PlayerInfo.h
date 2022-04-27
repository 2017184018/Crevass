#pragma once
#include <DirectXMath.h>
class PlayerInfo
{
public:
	bool MYInfo;

public:
	PlayerInfo();
	~PlayerInfo();

public:
	void Initialize();
	void Release();

public:
	void					SetID(char id, bool isMe);
	int						GetID()const;

	void					SetReadyState(bool readyState);
	bool					GetReadyState()const;

	void SetPosition(DirectX::XMFLOAT3 PlayerPos);
	DirectX::XMFLOAT3 GetPosition();
private:
	int						m_ID;
	bool					m_ReadyState;
	DirectX::XMFLOAT3		m_Pos;
};
