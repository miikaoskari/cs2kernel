#pragma once

#include <d3d11.h>
#include <Windows.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
};

class Overlay
{
public:
	Overlay(HINSTANCE instance, int nCmdShow)
		: instance{instance}
		, nCmdShow{nCmdShow}
	{
		initWindowContext();
		initD3D11Context();
		initImGuiContext();
	}

	~Overlay()
	{
		cleanupImGuiContext();
		cleanupD3D11Context();
		cleanupWindowContext();
	}

	void render();

	void initWindowContext();
	void cleanupWindowContext();

	void initD3D11Context();
	void cleanupD3D11Context();

	void initImGuiContext();
	void cleanupImGuiContext();


private:
	HINSTANCE instance;
	int nCmdShow;

	HWND hwnd;
	WNDCLASSEXW wc;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;
	ID3D11Texture2D* backBuffer = nullptr;	




};

