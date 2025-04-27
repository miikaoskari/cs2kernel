#pragma once

#include <cstddef>
#include "Vector.h"

class Entity
{
public:
	uintptr_t entity;
	uintptr_t controller;

	char name[128] = { 0 };
	int team;
	int health;
	int armor;
	int ping;

	float aimPunchAngle;
	float sensitivity;

	Vector3 position;
	Vector3 view;
	Vector3 velocity;

	bool isScoped;

	void ReadData(uintptr_t playerPawn, uintptr_t playerController);

};

