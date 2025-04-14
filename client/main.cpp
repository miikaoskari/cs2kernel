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

	Driver driver;
	driver.initDriver(deviceName);

	if (!driver.isDriverLoaded()) {
		std::cerr << "Failed to load driver." << std::endl;
		return 1;
	}

	Process process;
	ULONG pid = process.FindProcessId("cs2.exe");

	if (pid == 0) {
		std::cerr << "Failed to find cs2.exe process." << std::endl;
		return 1;
	}

	std::cout << "Found cs2.exe process with PID: " << pid << std::endl;

    uintptr_t clientBaseAddress = process.GetModuleBaseAddress(pid, L"client.dll");
    if (clientBaseAddress == 0)
    {
        std::cerr << "Failed to find client.dll base address." << std::endl;
        return 1;
    }

	uintptr_t engine2BaseAddress = process.GetModuleBaseAddress(pid, L"engine2.dll");
	if (engine2BaseAddress == 0)
	{
		std::cerr << "Failed to find engine2.dll base address." << std::endl;
		return 1;
	}

	uintptr_t matchmakingBaseAddress = process.GetModuleBaseAddress(pid, L"matchmaking.dll");
	if (matchmakingBaseAddress == 0)
	{
		std::cerr << "Failed to find matchmaking.dll base address." << std::endl;
		return 1;
	}

    std::cout << "client.dll base address: " << std::hex << clientBaseAddress << std::endl;
	std::cout << "engine2.dll base address: " << std::hex << engine2BaseAddress << std::endl;
	std::cout << "matchmaking.dll base address: " << std::hex << matchmakingBaseAddress << std::endl;


    uintptr_t playerPawnAddress = clientBaseAddress + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;
    uintptr_t entityListAddress = clientBaseAddress + cs2_dumper::offsets::client_dll::dwEntityList;


    for(;;)
    {
		uintptr_t playerPawn = driver.ReadMemory<uintptr_t>(pid, playerPawnAddress);
		uintptr_t entityList = driver.ReadMemory<uintptr_t>(pid, entityListAddress);

        if (playerPawn != NULL)
        {
            int team = driver.ReadMemory<int>(pid, (playerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum));
			int health = driver.ReadMemory<int>(pid, (playerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth));
			int armor = driver.ReadMemory<int>(pid, (playerPawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_ArmorValue));

			std::cout << "Player Team: " << team << std::endl;
			std::cout << "Player Health: " << health << std::endl;
			std::cout << "Player Armor: " << armor << std::endl;
        }
		
		Sleep(1000);
    }


    return 0;
}