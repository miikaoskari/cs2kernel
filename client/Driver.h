#pragma once

#include <Windows.h>
#include <iostream>

#define IOCTL_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0701, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_READ_REQUEST {
	ULONG pid;
	ULONG64 address;
	PVOID buffer;
	SIZE_T size;
} KERNEL_READ_REQUEST;

class Driver
{
public:
	static Driver& getInstance() {
		static Driver instance;
		return instance;
	}
	
	void initDriver(const wchar_t* deviceName);
	void closeDriver();
	bool isDriverLoaded();

	template<typename T> 
	T ReadMemory(ULONG pid, uintptr_t address) {
		T value{};

		if (!isDriverLoaded()) {
			std::cerr << "Driver not loaded." << std::endl;
			return T{};
		}

		KERNEL_READ_REQUEST req = { pid, address, &value, sizeof(T) };
		DWORD bytesReturned;
		DeviceIoControl(hDriver, IOCTL_READ_REQUEST, &req, sizeof(req), &req, sizeof(req), &bytesReturned, nullptr);

		return value;
	}

private:
	HANDLE hDriver;
	Driver();
	~Driver();
		
};

