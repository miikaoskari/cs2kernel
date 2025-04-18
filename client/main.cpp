#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>
#include <d3d11.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Driver.h"
#include "Process.h"
#include "PlayerPawn.h"
#include "Game.h"



int main()
{
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


    //uintptr_t entityListAddress = process.clientBaseAddress + cs2_dumper::offsets::client_dll::dwEntityList;


    for(;;)
    {
		
		
		
		    
		Sleep(1000);
    }


    return 0;
}