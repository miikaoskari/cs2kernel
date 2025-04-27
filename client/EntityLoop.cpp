#include "EntityLoop.h"
#include "PlayerPawn.h"
#include "Game.h"
#include "Driver.h"
#include "Process.h"
#include "Entity.h"
#include "Draw.h"
#include "View.h"

#include "offsets.hpp"
#include "client_dll.hpp"

Entity entities[PLAYER_ENTITY_COUNT];

using namespace cs2_dumper::schemas::client_dll;

void EntityLoop::runPlayerLoop()
{
	PlayerPawn playerPawn;
	Game game;

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

	playerPawn.ReadData();
	game.ReadData();

	for (int i = 0; i < PLAYER_ENTITY_COUNT; i++)
	{
		uintptr_t entity = d.ReadMemory<uintptr_t>(p.pid, game.entityList + ((8 * (i & 0x7FFF) >> 9) + 16));
		if(entity == 0)
		{
			continue;
		}

		uintptr_t entityController = d.ReadMemory<uintptr_t>(p.pid, entity + (120) * (i & 0x1FF));
		if (entityController == 0)
		{
			continue;
		}

		uintptr_t entityControllerPawn = d.ReadMemory<uintptr_t>(p.pid, entityController + CCSPlayerController::m_hPlayerPawn);
		if (entityControllerPawn == 0)
		{
			continue;
		}

		uintptr_t listEntity = d.ReadMemory<uintptr_t>(p.pid, game.entityList + (0x8 * ((entityControllerPawn & 0x7FFFF) >> 9) + 16));
		if (listEntity == 0)
		{
			continue;
		}

		uintptr_t entityPawn = d.ReadMemory<uintptr_t>(p.pid, listEntity + (120) * (entityControllerPawn & 0x1FF));
		if (entityPawn == 0)
		{
			continue;
		}

		entities[i].ReadData(entityPawn, entityController);

		
		Vector3 headPos = entities[i].position + Vector3(0.0f, 0.0f, 72.0f);

		Vector2 entityScreenPos;

        if (project3DWorldTo2D(entities[i].position, entityScreenPos, game.viewMatrix))
        {
			Draw::drawPlayerBox(entityScreenPos, entityScreenPos, ImColor(255, 0, 0));
        }

	}
}

void EntityLoop::runWorldLoop()
{

}
