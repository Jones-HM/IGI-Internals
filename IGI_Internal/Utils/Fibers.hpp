#pragma once
#include <windows.h>
#include "dllmain.hpp"
#pragma comment(lib, "winmm.lib")
using namespace IGI;

namespace Fiber {
	static HANDLE main_fiber = nullptr;
	static DWORD time_to_wake_up = 0;
	static HANDLE fiber_handle = nullptr;
	void on_tick();

	void wait_for(DWORD ms) {
		time_to_wake_up = GetTickCount() + ms;
		if (IsThreadAFiber()) {
			SwitchToFiber(main_fiber);
			LOG_INFO("%s SwitchToFiber Main", FUNC_NAME);
		}
	}

	void __stdcall fiber_thread(LPVOID params) {
		srand(GetTickCount64());
		while (!GT_IsKeyPressed(VK_END)) {
			DllMainLoop();
			wait_for(10);
		}
	}

	void on_tick() {

		if (time_to_wake_up <= GetTickCount())
		{
			main_fiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(NULL);
			SwitchToFiber(fiber_handle);
			LOG_INFO("%s SwitchToFiber", FUNC_NAME);
		}

		else {
			fiber_handle = CreateFiber(NULL, fiber_thread, nullptr);
			LOG_INFO("%s CreateFiber called", FUNC_NAME);
		}
	}
}

