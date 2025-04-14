#include "PlayerPawn.h"
#include "Driver.h"
#include "Process.h"

#include "offsets.hpp"
#include "client_dll.hpp"

using namespace cs2_dumper::offsets::client_dll;
using namespace cs2_dumper::schemas::client_dll;

inline void PlayerPawn::ReadData()
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

	uintptr_t playerPawn = d.ReadMemory<uintptr_t>(p.pid, p.clientBaseAddress + dwLocalPlayerPawn);
	
	if (playerPawn != NULL)
	{
		team = d.ReadMemory<int>(p.pid, (playerPawn + C_BaseEntity::m_iTeamNum));
		health = d.ReadMemory<int>(p.pid, (playerPawn + C_BaseEntity::m_iHealth));
		armor = d.ReadMemory<int>(p.pid, (playerPawn + C_CSPlayerPawn::m_ArmorValue));
		aimPunchAngle = d.ReadMemory<float>(p.pid, (playerPawn + C_CSPlayerPawn::m_aimPunchAngle));
		isScoped = d.ReadMemory<bool>(p.pid, (playerPawn + C_CSPlayerPawn::m_bIsScoped));
	}

	controller = d.ReadMemory<uintptr_t>(p.pid, p.clientBaseAddress + dwLocalPlayerController);
	if (controller != NULL)
	{
		uintptr_t nameAddress = d.ReadMemory<uintptr_t>(p.pid, (controller + CCSPlayerController::m_sSanitizedPlayerName));
		if (nameAddress != NULL)
		{
			// TODO: implement ReadMemoryArray to read the name
			// d.ReadMemory<char>(p.pid, nameAddress);
		}
		ping = d.ReadMemory<int>(p.pid, (controller + CCSPlayerController::m_iPing));
	}

	return;
}
