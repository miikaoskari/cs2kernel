#pragma once

#include <d3d11.h>
#include <Windows.h>

class Overlay
{
public:
	void init(HINSTANCE instance, int nCmdShow);
	void cleanup();
	void render();


};

