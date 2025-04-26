#include <Windows.h>
#include <string>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Driver.h"
#include "Process.h"
#include "PlayerPawn.h"
#include "Game.h"



INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow) {
    const WCHAR* deviceName = L"\\\\.\\IoctlTest";

	auto& driver = Driver::getInstance();
	driver.initDriver(deviceName);

	if (!driver.isDriverLoaded()) {
		std::cerr << "Failed to load driver." << std::endl;
		return 1;
	}

	auto& process = Process::getInstance();
	process.getAllBaseAddresses();
	

    std::cout << "client.dll base address: " << std::hex << process.clientBaseAddress << std::endl;
	std::cout << "engine2.dll base address: " << std::hex << process.engineBaseAddress << std::endl;
	std::cout << "matchmaking.dll base address: " << std::hex << process.matchmakingBaseAddress << std::endl;


    for(;;)
    {
		
		
		
		    
		Sleep(1000);
    }


    return 0;
}