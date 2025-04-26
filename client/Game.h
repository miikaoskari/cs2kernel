#pragma once

#include <cstddef>

class Game
{
public:
	char map_name[128] = { 0 };

	uintptr_t buildNumber;
	uintptr_t plantedC4;
	uintptr_t weaponC4;
	uintptr_t entityList;
	uintptr_t gameRules;
	uintptr_t globalVars;
	uintptr_t cameraServices;

	float flashDuration;

	int entityIndex;


	void ReadData();
};

