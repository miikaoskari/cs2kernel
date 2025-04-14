#include "Process.h"

#include <TlHelp32.h>
#include <string>


ULONG Process::FindProcessId(const char* processName) {
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

uintptr_t Process::GetModuleBaseAddress(DWORD pid, CONST WCHAR* target_module)
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
