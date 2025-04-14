#pragma once

#include <cstddef>

class PlayerPawn
{
public:
	uintptr_t pawn;
	uintptr_t controller;

	char name[128] = { 0 };
	int team;
	int health;
	int armor;
	int ping;

	float aimPunchAngle;
	float sensitivity;

	bool isScoped;

	void ReadData();

};