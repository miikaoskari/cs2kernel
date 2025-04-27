#pragma once

#include "imgui.h"

struct Vector2
{
	float x, y;

	Vector2(float x, float y) : x(x), y(y) {}
	Vector2() : x(0), y(0) {}

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}
};

struct Vector3
{
	float x, y, z;

	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3() : x(0), y(0), z(0) {}

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
};

struct Vector4
{
	float x, y, z, w;

	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vector4() : x(0), y(0), z(0), w(0) {}

	Vector4 operator+(const Vector4& other) const
	{
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}
};