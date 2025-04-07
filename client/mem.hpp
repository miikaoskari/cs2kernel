#pragma once

#include <Windows.h>


template<typename T> T ReadMemory(HANDLE hDriver, ULONG pid, uintptr_t address);
