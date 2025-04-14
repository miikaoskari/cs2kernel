#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "offsets.hpp"
#include "client_dll.hpp"
#include "engine2_dll.hpp"

#include "Driver.h"
#include "Process.h"


int main() {	
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


    uintptr_t playerPawnAddress = process.clientBaseAddress + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;
    uintptr_t entityListAddress = process.clientBaseAddress + cs2_dumper::offsets::client_dll::dwEntityList;

    for(;;)
    {
		uintptr_t playerPawn = driver.ReadMemory<uintptr_t>(process.pid, playerPawnAddress);
		uintptr_t entityList = driver.ReadMemory<uintptr_t>(process.pid, entityListAddress);

        if (playerPawn != NULL)
        {
            int team = driver.ReadMemory<int>(process.pid, (playerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum));
			int health = driver.ReadMemory<int>(process.pid, (playerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth));
			int armor = driver.ReadMemory<int>(process.pid, (playerPawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_ArmorValue));

			std::cout << "Player Team: " << team << std::endl;
			std::cout << "Player Health: " << health << std::endl;
			std::cout << "Player Armor: " << armor << std::endl;
        }
		
		Sleep(1000);
    }


    return 0;
}