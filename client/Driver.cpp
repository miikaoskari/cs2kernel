#include "Driver.h"
#include <iostream>



Driver::Driver() : hDriver(INVALID_HANDLE_VALUE) { }

Driver::~Driver() {
    closeDriver();
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
