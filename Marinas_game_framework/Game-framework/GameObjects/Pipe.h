#pragma once

#include "GameObject.h"

class CPipe final : public CGameObject
{
public:

	 CPipe(void) {}
	 CPipe(CApplication* application) : CGameObject(application) {}
	~CPipe(void) {}

};


