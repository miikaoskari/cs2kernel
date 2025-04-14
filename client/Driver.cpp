#include "Driver.h"
#include <iostream>

#define IOCTL_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0701, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_READ_REQUEST {
    ULONG pid;
    ULONG64 address;
    PVOID buffer;
    SIZE_T size;
} KERNEL_READ_REQUEST;

Driver::Driver() : hDriver(INVALID_HANDLE_VALUE) { }

Driver::~Driver() {
    closeDriver();
}

template<typename T> T Driver::ReadMemory(ULONG pid, uintptr_t address) {
	T value{};

	if (!isDriverLoaded()) {
		std::cerr << "Driver not loaded." << std::endl;
		return false;
	}
    
    KERNEL_READ_REQUEST req = { pid, address, &value, sizeof(T) };
    DWORD bytesReturned;
    DeviceIoControl(hDriver, IOCTL_READ_REQUEST, &req, sizeof(req), &req, sizeof(req), &bytesReturned, nullptr);

    return value;
}

void Driver::initDriver(const wchar_t* deviceName)
{
	hDriver = CreateFile(deviceName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hDriver == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to open driver handle." << std::endl;
		return;
	}
}

void Driver::closeDriver()
{
	if (hDriver != INVALID_HANDLE_VALUE) {
		CloseHandle(hDriver);
		hDriver = INVALID_HANDLE_VALUE;
	}
}

bool Driver::isDriverLoaded()
{
	return hDriver != INVALID_HANDLE_VALUE;
}
