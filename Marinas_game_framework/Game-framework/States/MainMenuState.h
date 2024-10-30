#pragma once

#include "State.h"

class CMainMenuState final : public CState
{
public:

	 CMainMenuState(void)												{}
	 CMainMenuState(CApplication* application) : CState(application)	{}
	~CMainMenuState(void)												{}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;
	virtual void RenderDebug(void) override;

public:

	// Place getters and setters here, in their own public field 

	// Example:
	/*
	* int	GetMyInteger(void) const					{return m_MyInteger;}
	* void	SetMyInteger(const int newInteger) const	{m_MyInteger = newInteger;}
	*/

private:

	// Declare the game objects here, create them in the OnEnter function and then finally destroy them in the OnExit function


};