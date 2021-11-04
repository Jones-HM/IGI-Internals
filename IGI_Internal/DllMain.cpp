//ich liebe es zu programmieren und Deustchland <3 
#define USE_STACKTRACE_LIB 
#define USE_MINHOOK_LIB 
#define USE_GTLIBC_LIB 

#include "DllMain.hpp" 
#include "AutoMsgBox.hpp"


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


void StartLevelMain(int = 1, bool = true, bool = true, int = 1);
void QuitLevelMain();
void DllMainLoop();

int delay_ms = 2500;

void ThreadCallerDelay(int delay) {
	std::this_thread::sleep_for(std::chrono::microseconds(delay));
}

template<typename RT = Void, typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void>
RT ThreadCallerExec(void* func_ptr, T1 param1 = nullptr, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
	std::thread th(ThreadCallerDelay, delay_ms);

	auto ret_val = NATIVE_INVOKE<RT>(func_ptr, param1, param2, param3, param4);
	th.join();

	if constexpr (!std::is_same_v<RT, void>)
		return ret_val;
}

//std::thread exec_delay_thread(DelayExecution, 1000);

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
					GT_InitTrainerWindowEx(PROJECT_NAME, 50, 50, 1200, 700, BG_GREEN, FG_GRAY);
#endif 
					auto GAME_font = LR"(
╔═══╦═══╦═══╗ ╔╦═══╦═══╦════╗  ╔══╦═══╦══╗                 
║╔═╗║╔═╗║╔═╗║░║║╔══╣╔═╗║╔╗╔╗║░░╚╣╠╣╔═╗╠╣╠╝	▄▌			▄ 
║╚═╝║╚═╝║║░║║░║║╚══╣║░╚╩╝║║╚╝░░░║║║║░╚╝║║	▓▓▓▓▓▓▓▓▓▓▓▓▄
║╔══╣╔╗╔╣║░║╠╗║║╔══╣║░╔╗░║║░╔══╗║║║║╔═╗║║	▓▓▓▓▓▓▓▓▓▓▓▓▄ 
║║░░║║║╚╣╚═╝║╚╝║╚══╣╚═╝║░║║░╚══╬╣╠╣╚╩═╠╣╠╬			▀▐▓▓▓▓
╚╝  ╚╝╚═╩═══╩══╩═══╩═══╝ ╚╝    ╚══╩═══╩══╝ 			   ▐▓▓▌
)";

					LOG_RAW(GAME_font);
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
					GT_WriteNOP(PLAYER_XPL_HIT_ADDR, 6);
					WEAPON::UNLIMITED_AMMO_SET(true);

					//Main loop of DLL. 
					while (!GT_IsKeyPressed(VK_END)) {
						DllMainLoop();
						std::this_thread::sleep_for(10ms);
					}

					//Fiber::fiber_handle = CreateFiber(NULL, Fiber::fiber_thread, nullptr);
					//Fiber::fiber_thread(NULL);

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

bool g_Enabled = true;
void DllMainLoop() {

	g_menu_screen = READ_PTR(menu_screen_ptr);
	g_game_level = LEVEL::GET();
	if (g_curr_level != g_game_level) {
		g_curr_level = g_game_level;
		g_level_changed = true;
	}

	if (g_level_changed) {
		g_level_changed ^= 1;
	}

	if (g_menu_screen == MENU_SCREEN_MAINMENU) {
		//g_Console->Clear();

		game_resources.clear();
		g_level_graphs.clear();
	}

	else if (g_menu_screen == MENU_SCREEN_INGAME) {

		if (GT_IsKeyToggled(VK_F1)) {
			DEBUG::INIT(GAME_FONT_BIG);
			DEBUG::ENABLE(g_Enabled);
			string dbg_msg = "Debug mode " + std::string((g_Enabled) ? "Enabled" : "Disabled");
			MISC::STATUS_MESSAGE_SHOW(dbg_msg.c_str());
			g_Enabled = !g_Enabled;

			//Bypass Regular DebugPrint to Humanplayer.
			//binary_t vec{ {0x75},{0x47} };
			//g_Memory->WriteMemory((LPVOID)0x00460C8F, vec);
		}

		else if (GT_IsKeyToggled(VK_F2)) {
			const char* cfg_file = "LOCAL:objects.qsc";
			SCRIPT::COMPILE(cfg_file);
			LOG_INFO("Compile '%s' done!", cfg_file);
			MISC::STATUS_MESSAGE_SHOW("Compile object.qsc done!");
		}

		else if (GT_IsKeyToggled(VK_F3)) {
			ThreadCallerExec<Void>(WEAPON::WEAPON_PICKUP, WEAPON_ID_JACKHAMMER);
			ThreadCallerExec<Void>(WEAPON::WEAPON_PICKUP, WEAPON_ID_DRAGUNOV);
			ThreadCallerExec<Void>(WEAPON::WEAPON_PICKUP, WEAPON_ID_UZIX2);
			LOG_INFO("WeaponPickup : Total Weapons Count %d", WEAPON::TOTAL_COUNT());
			MISC::STATUS_MESSAGE_SHOW("WeaponPickup done!");
		}

		else if (GT_IsKeyToggled(VK_F4)) {
			//g_Graph.DebugGraphInfo(true, true);
			//g_level_graphs.clear();
			g_Graph.DOT_SaveGraphs();
			LOG_CONSOLE("DOT Save graphs done");
			//static int hcam_val = 0;
			//HUMAN::CAM_VIEW_SET(hcam_val);
			//hcam_val = (++hcam_val > 6) ? 0 : hcam_val;
			//status_byte = 1;
			//NATIVE_INVOKE<Void>((Void)HASH::STATUS_MESSAGE_SHOW, *(PINT)0x00A758AC, "HumanViewCam changed!", NULL, (char*)&status_byte);

		}

		else if (GT_IsKeyToggled(VK_F5)) {
			SOLDIER::INIT();

			//for (auto& soldier : soldiers) {
			//	if (soldier.GetSoldierId() == 0 || soldier.GetAddress() == READ_PTR(humanplayer_ptr)) continue;
			//	else {
			//		SOLDIER::EXECUTE(soldier);
			//		LOG_INFO("Soldier_%d executed", soldier.GetSoldierId());
			//	}
			//}
			//soldiers.clear();

			//delay_ms = 5000;
			//ThreadCallerExec<Void>(LEVEL::RESTART);
		}

		else if (GT_IsKeyToggled(VK_F6)) {
			RESOURCE::ANIMATION_INFO_SAVE("IGI_Animations.txt");
			RESOURCE::FONT_INFO_SAVE("IGI_Fonts.txt");
			RESOURCE::SOUND_INFO_SAVE("IGI_Sound.txt");
			RESOURCE::MATERIAL_INFO_SAVE("IGI_Material.txt");
			RESOURCE::LIGHTMAP_INFO_SAVE("IGI_Lightmap.txt");
			RESOURCE::OBJECT_INFO_SAVE("IGI_Object.txt");
			RESOURCE::RESOURCE_INFO_SAVE("IGI_Resource.txt");
			RESOURCE::TERRAIN_INFO_SAVE("IGI_Terrain.txt");
			RESOURCE::TEXTURE_INFO_SAVE("IGI_Texture.txt");
			RESOURCE::GRAPHICS_2D_INFO_SAVE("IGI_2D_Graphics.txt");
			RESOURCE::GRAPHICS_3D_INFO_SAVE("IGI_3D_Graphics.txt");
			LOG_CONSOLE("Resource files saved");
		}

		else if (GT_IsKeyToggled(VK_F7)) {

			std::vector<string> res_list{
				"STATUSSCREEN:status.res","LANGUAGE:messages.res",
				"LOCAL:menusystem/missionsprites.res","LOCAL:common/sprites/sprites.res",
				"LOCAL:common/textures/textures.res" ,"COMPUTER:computer.res",
				"LANGUAGE:computer.res" ,"LANGUAGE:objectives.res",
				"LOCAL:language/ENGLISH/menusystem.res" ,"LOCAL:language/ENGLISH/missions.res",
			};

			for (const auto& res : res_list) {
				auto addr = RESOURCE::FIND(res.c_str());
				LOG_INFO("'%s': %p", res.c_str(), addr);
			}
			RESOURCE::UNLOAD(res_list);

			LOG_CONSOLE("Resource Run");
		}


		else if (GT_IsKeyToggled(VK_F8)) {
			std::vector<string> models_list{ "Watertower","Watchtower","Barracks" };
			RESOURCE::MEF_REMOVE_MODELS(models_list);

			LOG_INFO("MEF_RemoveModel run");
			//StartLevelMain(4);
		}

		else if (GT_IsKeyToggled(VK_F9)) {
			RESOURCE::MEF_RESTORE_MODELS();
			LOG_INFO("MEF_RestoreModel run");
		}


		else if (GT_IsKeyToggled(VK_F11)) {
			//Bypass Regular DebugPrint to Humanplayer.
			//std::vector<uint8_t> vec{ {0x75},{0x47} };
			//g_Memory->WriteMemory((LPVOID)0x00460C8F, vec);

			Camera::Controls controls;
			controls.UP(VK_SPACE);
			controls.DOWN(VK_MENU);
			controls.LEFT(VK_LEFT);
			controls.RIGHT(VK_RIGHT);
			controls.FORWARD(VK_UP);
			controls.BACKWARD(VK_DOWN);
			controls.CALIBRATE(VK_BACK);
			controls.QUIT(VK_RETURN);
			controls.AXIS_OFF(0.5f);

			CAMERA::FREECAM(controls);
		}

		else if (GT_IsKeyToggled(VK_F12)) {

			string qsc_file = "LOCAL:hconfig.qsc";
			string qas_file = "LOCAL:hconfig.qas";
			SCRIPT::PARSE(qsc_file, qas_file);

			LOG_INFO("Compile Run");
		}

		else if (GT_IsKeyToggled(VK_SNAPSHOT)) {
			g_Console->Clear();
		}
	}

	else if (g_menu_screen == MENU_SCREEN_RESTART) {
		soldiers.clear();
		//game_resources.clear();
	}

}

void StartLevelMain(int level, bool disable_warn, bool disable_err, int hash_val) {

	LEVEL::SET(level);
	if (disable_warn) MISC::WARNINGS_DISABLE();
	if (disable_err) MISC::ERRORS_DISABLE();

	//QuitLevelMain();
	//std::this_thread::sleep_for(10s);

	QTASK::HASH_INIT(1);
	QTASK::UPDATE();

	//auto StartLevel = (int(__cdecl*)(int, int, int, int))0x415B30;
	//StartLevel(*(PINT)0x57BABC,00000000, * (PINT)0xC28C5C, * (PINT)(*(PINT)0x57BABC));

	auto StartLevelCaller = (int(__cdecl*)(int))0x416900;
	StartLevelCaller(*(PINT)0x00567C8C);

	QTASK::RESET();
}

void QuitLevelMain() {
	auto sub_416d40 = (int(__cdecl*)())0x416d40;
	auto ppiVar4 = (int**)sub_416d40();

	auto sub_004015f0 = (int(__cdecl*)(int**))0x004015f0;
	sub_004015f0(ppiVar4);
	*(int*)(0x00567c8c + 0x28) = 3;

	auto sub_402890 = (int(__cdecl*)())0x402890;
	int iVar2 = sub_402890();

	auto MenuManager = (int(__cdecl*)(int, const char*, char, char, int))0x418b00;
	MenuManager(iVar2, "LOCAL:menusystem\\mainmenu.qsc", '\x1', '\x1', 1);
}