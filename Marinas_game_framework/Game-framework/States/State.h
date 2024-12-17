#pragma once

class CApplication;

// The base class that every state in the application/game inherits from
class CState
{
public:

			 CState(void) {}
			 CState(CApplication* application) : m_pApplication(application) {}
	virtual ~CState(void) {}

	virtual bool OnEnter(void) { return true; }
	virtual void OnExit(void) {}
	virtual void Update(const float deltaTime) {}
	virtual void Render(void) {}
	virtual void RenderDebug(void) {}

protected:

	CApplication* m_pApplication = nullptr;

};