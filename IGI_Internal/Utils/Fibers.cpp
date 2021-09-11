#include <windows.h>
//#include "dllmain.hpp"
#pragma comment(lib, "winmm.lib")
namespace Fiber {
    static HANDLE main_fiber = nullptr;
    static DWORD time_to_wake_up = 0;

    void wait_for(DWORD ms) {
        time_to_wake_up = timeGetTime() + ms;
        SwitchToFiber(main_fiber);
    }

    void __stdcall fiber_thread(LPVOID params) {
        srand(GetTickCount64());
        while (true) {
            //DllMainLoop();
            wait_for(0);
        }
    }

    void on_tick() {
        if (!main_fiber)
            main_fiber = ConvertThreadToFiber(nullptr);

        if (time_to_wake_up > timeGetTime())
            return;

        static HANDLE fiber_handle = nullptr;
        if (fiber_handle) {
            SwitchToFiber(fiber_handle);
        }
        else {
            fiber_handle = CreateFiber(NULL, fiber_thread, nullptr);
        }
    }
}

