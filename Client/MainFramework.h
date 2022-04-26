#pragma once
class Network;
class GameInfo;
class MainFramework
{
public:
	MainFramework();
	virtual ~MainFramework();

public:
	void Initialize();

private:
	Network* m_pNetwork;
	
};
