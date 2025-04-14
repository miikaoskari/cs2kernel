#pragma once

#include <Windows.h>

class Driver
{
public:
	Driver();
	~Driver();
	
	
	void initDriver(const wchar_t* deviceName);
	void closeDriver();
	bool isDriverLoaded();

	template<typename T> T ReadMemory(ULONG pid, uintptr_t address);

private:
	HANDLE hDriver;
		
};

