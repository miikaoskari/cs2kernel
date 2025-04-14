#pragma once

#include <Windows.h>

class Process
{
public:
	ULONG FindProcessId(const char* processName);
	uintptr_t GetModuleBaseAddress(DWORD pid, CONST WCHAR* target_module);
};

