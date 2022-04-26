#pragma once
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

private:
	int						m_ID;
	bool					m_ReadyState;

};
