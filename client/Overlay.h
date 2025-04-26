#pragma once

#include <d3d11.h>
#include <Windows.h>

class Overlay
{
public:
	void init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void cleanup();
	void render();


};

