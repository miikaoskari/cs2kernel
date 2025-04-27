#include <Windows.h>
#include <string>
#include <iostream>

#include "Driver.h"
#include "Process.h"
#include "PlayerPawn.h"
#include "Game.h"
#include "Overlay.h"
#include "EntityLoop.h"
#include "imgui.h"


static bool isRunning = true;

INT APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	const WCHAR* deviceName = L"\\\\.\\IoctlTest";

	auto& driver = Driver::getInstance();
	driver.initDriver(deviceName);

	if (!driver.isDriverLoaded()) {
		std::cerr << "Failed to load driver." << std::endl;
		return 1;
	}

	auto& process = Process::getInstance();
	process.getAllBaseAddresses();
	

    std::cout << "client.dll base address: " << std::hex << process.clientBaseAddress << std::endl;
	std::cout << "engine2.dll base address: " << std::hex << process.engineBaseAddress << std::endl;
	std::cout << "matchmaking.dll base address: " << std::hex << process.matchmakingBaseAddress << std::endl;


	Overlay overlay(hInstance, nShowCmd);
	EntityLoop entityLoop;

	while (isRunning)
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				return 0;
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow(&isRunning);

		entityLoop.runPlayerLoop();
		//overlay.render();

		ImGui::Render();
		overlay.deviceContext->OMSetRenderTargets(1, &overlay.renderTargetView, nullptr);

		constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };
		overlay.deviceContext->ClearRenderTargetView(overlay.renderTargetView, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		overlay.swapChain->Present(1, 0); // Present with vsync

	}

    return 0;
}