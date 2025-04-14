#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "offsets.hpp"

HANDLE hDriver;

typedef struct _KERNEL_READ_REQUEST {
    ULONG pid;
    ULONG64 address;
    PVOID buffer;
    SIZE_T size;
} KERNEL_READ_REQUEST;

#define IOCTL_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0701, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

template<typename T> T ReadMemory(HANDLE hDriver, ULONG pid, uintptr_t address) {
    T value{};
    KERNEL_READ_REQUEST req = { pid, address, &value, sizeof(T) };
    DWORD bytesReturned;
    DeviceIoControl(hDriver, IOCTL_READ_REQUEST, &req, sizeof(req), &req, sizeof(req), &bytesReturned, nullptr);

    return value;
}

// function to find cs2.exe process id
ULONG FindProcessId(const char* processName) {
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (Process32FirstW(snapshot, &entry) == TRUE) {
        do {
            if (_wcsicmp(entry.szExeFile, std::wstring(processName, processName + strlen(processName)).c_str()) == 0) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32NextW(snapshot, &entry) == TRUE);
    }

    CloseHandle(snapshot);
    return 0;
}

uintptr_t GetModuleBaseAddress(DWORD pid, CONST WCHAR* target_module)
{
    HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (snapshotHandle == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    MODULEENTRY32W moduleEntry = { };
    moduleEntry.dwSize = sizeof(MODULEENTRY32W);

    if (Module32FirstW(snapshotHandle, &moduleEntry)) {

        do {

            if (_wcsicmp(moduleEntry.szModule, target_module) == 0) {
                CloseHandle(snapshotHandle);
                return reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
            }
        } while (Module32NextW(snapshotHandle, &moduleEntry));
    }
    CloseHandle(snapshotHandle);
    return NULL;
}

int main() {	
    const WCHAR* deviceName = L"\\\\.\\IoctlTest";

    hDriver = CreateFile(deviceName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    std::cout << "Driver handle: " << hDriver << std::endl;

    ULONG pid = FindProcessId("cs2.exe");

	if (pid == 0) {
		std::cerr << "Failed to find cs2.exe process." << std::endl;
		return 1;
	}

	std::cout << "Found cs2.exe process with PID: " << pid << std::endl;

    uintptr_t clientBaseAddress = GetModuleBaseAddress(pid, L"client.dll");
    if (clientBaseAddress == 0)
    {
        std::cerr << "Failed to find client.dll base address." << std::endl;
        return 1;
    }

    std::cout << "client.dll base address: " << std::hex << clientBaseAddress << std::endl;

    for(;;)
    {
        uintptr_t plantedC4Address = clientBaseAddress + cs2_dumper::offsets::client_dll::dwPlantedC4;
        uintptr_t isPlanted = ReadMemory<uintptr_t>(hDriver, pid, plantedC4Address);
		std::cout << "isPlanted: " << isPlanted << std::endl;
		
		Sleep(1000);
    }


    return 0;
}