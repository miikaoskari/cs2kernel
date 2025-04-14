#pragma once

#include <Windows.h>

class Process
{
public:
	static Process& getInstance() {
		static Process instance;
		return instance;
	}

	uintptr_t clientBaseAddress;
	uintptr_t engineBaseAddress;
	uintptr_t matchmakingBaseAddress;
	uintptr_t inputSystemBaseAddress;
	uintptr_t soundSystemBaseAddress;

	ULONG pid;

	bool getAllBaseAddresses();

private:
	Process();
	~Process();

	ULONG FindProcessId(const char* processName);
	uintptr_t GetModuleBaseAddress(DWORD pid, CONST WCHAR* target_module);
};

