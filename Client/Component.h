#pragma once
class Component abstract
{
public:
	explicit Component();
	virtual ~Component();

public:
	virtual HRESULT InitializeComponent() = 0;
	virtual void	UpdateComponent(const float& fTimeDelta) = 0;
	virtual void	ReleaseComponent() = 0;
};

