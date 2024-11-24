#pragma once

class CApplication;

class CLibraryHandler
{
public:

	CLibraryHandler(void) {}
	~CLibraryHandler(void) {}

	bool Create(CApplication* application);
	void Destroy(void);

};