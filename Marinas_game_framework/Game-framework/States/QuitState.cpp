#include "Pch.h"
#include "QuitState.h"


#include "Application.h"


bool CQuitState::OnEnter(void)
{
	m_pApplication->Quit();


	return true;
}
