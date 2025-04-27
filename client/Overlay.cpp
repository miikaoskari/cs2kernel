#include "Overlay.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>


LRESULT CALLBACK window_procedure(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, w_param, l_param))
	{
		return 0L;
	}
	
	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0L;
	}

	return DefWindowProc(hwnd, message, w_param, l_param);
}

void Overlay::render()
{
	bool running = true;
	while (running)
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				running = false;
			}
		}
		
		if (!running)
		{
			break;
		}
		

		// rendering here!
		// (Your code process and dispatch Win32 messages)
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		

		ImGui::GetBackgroundDrawList()->AddCircleFilled({ 500,500 }, 10.f, ImColor(1.f, 0.f, 0.f));


		// Rendering
		// (Your code clears your framebuffer, renders your other stuff etc.)
		ImGui::Render();

		constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };
		deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
		deviceContext->ClearRenderTargetView(renderTargetView, color);


		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swapChain->Present(1, 0); // Present with vsync
	}
	
	PostQuitMessage(0);
}

void Overlay::initWindowContext()
{
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = window_procedure;
	wc.hInstance = instance;
	wc.lpszClassName = L"external overlay";
	
	RegisterClassExW(&wc);

	hwnd = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wc.lpszClassName,
		L"external overlay",
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	RECT client_area = { 0 };
	GetClientRect(hwnd, &client_area);

	RECT window_area = { 0 };
	GetWindowRect(hwnd, &window_area);

	POINT diff = { 0 };
	ClientToScreen(hwnd, &diff);

	const MARGINS margins = {
		window_area.left + (diff.x - window_area.left),
		window_area.top + (diff.y - window_area.top),
		client_area.right,
		client_area.bottom,
	};

	DwmExtendFrameIntoClientArea(hwnd, &margins);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
}

void Overlay::cleanupWindowContext()
{
	DestroyWindow(hwnd);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

void Overlay::initD3D11Context()
{
	DXGI_SWAP_CHAIN_DESC scd = { 0 };

	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.SampleDesc.Count = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevel;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		levels,
		2,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		&featureLevel,
		&deviceContext
	);

	swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
	backBuffer->Release();
}

void Overlay::cleanupD3D11Context()
{
	if (backBuffer)
	{
		backBuffer->Release();
	}

	if (deviceContext)
	{
		deviceContext->Release();
	}

	if (device)
	{
		device->Release();
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
	}
}

void Overlay::initImGuiContext()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);

}

void Overlay::cleanupImGuiContext()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
