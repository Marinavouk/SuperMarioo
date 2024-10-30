#include "Pch.h"
#include "Application.h"

int32_t main(int32_t argc, char* argv[])
{
	CApplication* application = new CApplication;

	if (application->Create())
	{
		application->Run();
		application->Destroy();
	}

	delete application;
	application = nullptr;

#if defined(_DEBUG)
//	system("pause");
#endif

	return 0;
}