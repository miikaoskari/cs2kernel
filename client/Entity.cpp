#include "Entity.h"
#include "Driver.h"
#include "Process.h"

#include "offsets.hpp"
#include "client_dll.hpp"

using namespace cs2_dumper::offsets::client_dll;
using namespace cs2_dumper::schemas::client_dll;

void Entity::ReadData(uintptr_t playerPawn, uintptr_t playerController)
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

	if (playerPawn != NULL)
	{
		team = d.ReadMemory<int>(p.pid, (playerPawn + C_BaseEntity::m_iTeamNum));
		health = d.ReadMemory<int>(p.pid, (playerPawn + C_BaseEntity::m_iHealth));
		armor = d.ReadMemory<int>(p.pid, (playerPawn + C_CSPlayerPawn::m_ArmorValue));
		aimPunchAngle = d.ReadMemory<float>(p.pid, (playerPawn + C_CSPlayerPawn::m_aimPunchAngle));
		isScoped = d.ReadMemory<bool>(p.pid, (playerPawn + C_CSPlayerPawn::m_bIsScoped));
		position = d.ReadMemory<Vector3>(p.pid, (playerPawn + C_BasePlayerPawn::m_vOldOrigin));
		view = d.ReadMemory<Vector3>(p.pid, (playerPawn + C_BaseModelEntity::m_vecViewOffset));
		velocity = d.ReadMemory<Vector3>(p.pid, (playerPawn + C_BaseEntity::m_vecAbsVelocity));
	}

	if (playerController != NULL)
	{
		uintptr_t nameAddress = d.ReadMemory<uintptr_t>(p.pid, (playerController + CCSPlayerController::m_sSanitizedPlayerName));
		if (nameAddress != NULL)
		{
			// TODO: implement ReadMemoryArray to read the name
			// d.ReadMemory<char>(p.pid, nameAddress);
		}
		ping = d.ReadMemory<int>(p.pid, (playerController + CCSPlayerController::m_iPing));
	}

	return;
}