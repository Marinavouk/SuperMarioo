#pragma once

#include "State.h"

class CQuitState final : public CState
{
public:

	 CQuitState(void) {}
	 CQuitState(CApplication* application) : CState(application) {}
	~CQuitState(void) {}

	virtual bool OnEnter(void) override;

};