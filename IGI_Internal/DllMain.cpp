//ich liebe es zu programmieren und Deustchland <3 
#define USE_STACKTRACE_LIB 
#define USE_MINHOOK_LIB 
#define USE_GTLIBC_LIB 

#include "DllMain.hpp" 

//Include all static libraries for project. 
#if defined(_M_IX86) 
#if defined(DBG_x86)
#pragma comment(lib,"hook/libMinHook-x86-Debug.lib")
#ifdef USE_GTLIBC_LIB 
#pragma comment(lib,"libs/GTLibc-x86-Debug.lib")
#pragma comment(lib,"libs/GTConsole-x86-Debug.lib")
#endif 

#elif defined(RLS_x86)
#pragma comment(lib,"hook/libMinHook-x86-Release.lib")
#ifdef USE_GTLIBC_LIB 
#pragma comment(lib,"libs/GTLibc-x86-Release.lib")
#pragma comment(lib,"libs/GTConsole-x86-Release.lib")
#endif 
#endif 
#else 
#error This project supports only x86 (32-Bit) builds. 
#endif 


BOOL WINAPI  DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hmod);
		g_Utility.SetModuleHandle(hmod);
		g_Hmodule = hmod;

		g_Main_Thread = CreateThread(nullptr, 0, [](PVOID)->DWORD
			{
				auto console_instance = std::make_unique<Console>();
				auto logger_instance = std::make_unique<Log>();
				try {
#ifdef _DEBUG 
					g_Console->Allocate();
					g_Console->Clear();
					GT_InitTrainerWindowEx(PROJECT_NAME, 50, 50, 1200, 700, BG_GREEN, FG_CYAN);
#endif 
					auto game_font = LR"(
╔═══╦═══╦═══╗ ╔╦═══╦═══╦════╗  ╔══╦═══╦══╗                 
║╔═╗║╔═╗║╔═╗║░║║╔══╣╔═╗║╔╗╔╗║░░╚╣╠╣╔═╗╠╣╠╝	▄▌			▄ 
║╚═╝║╚═╝║║░║║░║║╚══╣║░╚╩╝║║╚╝░░░║║║║░╚╝║║	▓▓▓▓▓▓▓▓▓▓▓▓▄
║╔══╣╔╗╔╣║░║╠╗║║╔══╣║░╔╗░║║░╔══╗║║║║╔═╗║║	▓▓▓▓▓▓▓▓▓▓▓▓▄ 
║║░░║║║╚╣╚═╝║╚╝║╚══╣╚═╝║░║║░╚══╬╣╠╣╚╩═╠╣╠╬			▀▐▓▓▓▓
╚╝  ╚╝╚═╩═══╩══╩═══╩═══╝ ╚╝    ╚══╩═══╩══╝ 			   ▐▓▓▌
)";

					LOG_RAW(game_font);
					LOG_WARNING("Logger initialized.");

					auto native_instance = std::make_unique<Natives>();
					LOG_WARNING("Natives initialized.");

					auto memory_instance = std::make_unique<Memory>(true);
					LOG_WARNING("Memory initialized.");

					auto auto_msg_box = std::make_unique<AutoMsgBox>();
					LOG_WARNING("AutoMsgBox initialized.");

					auto game_resources = std::make_unique<GameResource>();
					LOG_WARNING("GameResources initialized.");

#ifdef  USE_MINHOOK_LIB 
					// Initialize Hooking. 
					auto hook_instance = std::make_unique<Hook>(true);
					LOG_WARNING("Hook initialized.");
#endif //USE_MINHOOK_LIB 

#if  defined(USE_STACKTRACE_LIB) && defined(DBG_x86)
					auto dbg_instance = std::make_unique<DbgHelper>(true);
					LOG_WARNING("DbgHelper initialized.");
#endif//USE_STACKTRACE_LIB 

					//Invincible-Jones.
#ifdef DBG_x86
					//GT_WriteNOP(PLAYER_XPL_HIT_ADDR, 6);
					//WEAPON::UNLIMITED_AMMO_SET(true);20.5990
					MISC::STATUS_MESSAGE_SHOW((PROJECT_NAME + string(" v1.1 Attached")).c_str());
#else
					MISC::STATUS_MESSAGE_SHOW((PROJECT_NAME + string(" v1.1 Attached")).c_str());
#endif
					//Set Game handle for both Debug and Release builds.
					HANDLE g_handle = GT_GetGameHandle4mHWND(GT_FindGameWindow(GAME_NAME));
					LOG_INFO("Game handle 0x%p", g_handle);
					g_Utility.SetHandle(g_handle);

					//Main loop of DLL. 
					while (!GT_IsKeyPressed(VK_END)) {
						DllMainLoop();
						std::this_thread::sleep_for(10ms);
					}

					native_instance.reset();
					LOG_WARNING("Natives uninitialized.");

					memory_instance.reset();
					LOG_WARNING("Memory uninitialized.");

					auto_msg_box.reset();
					LOG_WARNING("AutoMsgBox uninitialized.");

					game_resources.reset();
					LOG_WARNING("GameResources uninitialized.");
#ifdef  USE_MINHOOK_LIB
					hook_instance.reset();
					LOG_WARNING("Hook uninitialized.");
#endif
#if  defined(USE_STACKTRACE_LIB) && defined(DBG_x86)
					dbg_instance.reset();
					LOG_WARNING("DbgHelper uninitialized.");
#endif

#ifdef _DEBUG 
					if (g_Console->IsAllocated()) {
						g_Console->DeAllocate();
					}
#endif 
				}
				catch (std::exception const& ex) {
					GT_ShowError(ex.what());
#if  defined(USE_STACKTRACE_LIB) && defined(DBG_x86)
					g_DbgHelper->StackTrace(true);
#endif
				}

				LOG_WARNING("Console uninitialized.");
				console_instance.reset();

				LOG_WARNING("Logger uninitialized.");
				logger_instance.reset();

				CloseHandle(g_Main_Thread);
				FreeLibraryAndExitThread(g_Hmodule, 0);
			}, nullptr, 0, &g_Main_Thread_Id);
	}
	return true;
}