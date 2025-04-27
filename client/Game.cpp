#include "Game.h"
#include "Driver.h"
#include "Process.h"
#include "Vector.h"
#include "ViewMatrix.h"

#include "offsets.hpp"
#include "client_dll.hpp"

using namespace cs2_dumper::offsets::client_dll;
using namespace cs2_dumper::offsets::engine2_dll;
using namespace cs2_dumper::schemas::client_dll;


void Game::ReadData()
{
	auto& d = Driver::getInstance();

	if (!d.isDriverLoaded())
	{
		std::cerr << "Driver not loaded." << std::endl;
		return;
	}

	auto& p = Process::getInstance();

	if (p.pid == 0)
	{
		std::cerr << "Process ID is not set." << std::endl;
		return;
	}

	buildNumber = d.ReadMemory<uintptr_t>(p.pid, p.engineBaseAddress + dwBuildNumber);
	plantedC4 = d.ReadMemory<uintptr_t>(p.pid, p.clientBaseAddress + dwPlantedC4);
	weaponC4 = d.ReadMemory<uintptr_t>(p.pid, p.clientBaseAddress + dwWeaponC4);
	entityList = d.ReadMemory<uintptr_t>(p.pid, p.clientBaseAddress + dwEntityList);
	gameRules = d.ReadMemory<uintptr_t>(p.pid, p.clientBaseAddress + dwGameRules);
	globalVars = d.ReadMemory<uintptr_t>(p.pid, p.clientBaseAddress + dwGlobalVars);
	viewMatrix = d.ReadMemory<ViewMatrix>(p.pid, p.clientBaseAddress + dwViewMatrix);
	viewAngles = d.ReadMemory<Vector3>(p.pid, p.clientBaseAddress + dwViewAngles);
}