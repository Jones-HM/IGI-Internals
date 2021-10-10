#pragma 
//ich liebe es zu programmieren und Deustchland <3 
#define USE_STACKTRACE_LIB 
#define USE_MINHOOK_LIB 
#define USE_GTLIBC_LIB 

#include "DllMain.hpp" 
#include <AutoMsgBox.hpp>
#include "Utils/Fibers.hpp"

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
void SaveMesh_MEF(string);

int delay_ms = 1000;


void ThreadCallerDelay(int delay) {
	std::this_thread::sleep_for(std::chrono::microseconds(delay));
}

template<typename RT = Void, typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void>
RT ThreadCallerExec(void* func_ptr, T1 param1 = nullptr, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {

	//std::thread th{ [&]() {
	//	std::this_thread::sleep_for(std::chrono::microseconds(delay_ms));
	//	} };

	std::thread th(ThreadCallerDelay, delay_ms);

	//g_AutoMsgBox->Show("",500);
	auto ret_val = NATIVE_INVOKE<Void>(func_ptr, param1, param2, param3, param4);
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

					auto igi_font = LR"(
╔═══╦═══╦═══╗ ╔╦═══╦═══╦════╗  ╔══╦═══╦══╗                 
║╔═╗║╔═╗║╔═╗║░║║╔══╣╔═╗║╔╗╔╗║░░╚╣╠╣╔═╗╠╣╠╝	▄▌			▄ 
║╚═╝║╚═╝║║░║║░║║╚══╣║░╚╩╝║║╚╝░░░║║║║░╚╝║║	▓▓▓▓▓▓▓▓▓▓▓▓▄
║╔══╣╔╗╔╣║░║╠╗║║╔══╣║░╔╗░║║░╔══╗║║║║╔═╗║║	▓▓▓▓▓▓▓▓▓▓▓▓▄ 
║║░░║║║╚╣╚═╝║╚╝║╚══╣╚═╝║░║║░╚══╬╣╠╣╚╩═╠╣╠╬			▀▐▓▓▓▓
╚╝  ╚╝╚═╩═══╩══╩═══╩═══╝ ╚╝    ╚══╩═══╩══╝ 			   ▐▓▓▌
)";

					//LOG_RAW(igi_font);
					LOG_WARNING("Logger initialized.");

					auto native_instance = std::make_unique<Natives>();
					LOG_WARNING("Natives initialized.");

					auto memory_instance = std::make_unique<Memory>(true);
					LOG_WARNING("Memory initialized.");

					auto auto_msg_box = std::make_unique<AutoMsgBox>();
					LOG_WARNING("AutoMsgBox initialized.");

#ifdef  USE_MINHOOK_LIB 
					// Initialize Hooking. 
					auto hook_instance = std::make_unique<Hook>(true);
					LOG_WARNING("Hook initialized.");
#endif //USE_MINHOOK_LIB 

#ifdef  USE_STACKTRACE_LIB
					auto dbg_instance = std::make_unique<DbgHelper>(true);
					LOG_WARNING("DbgHelper initialized.");
#endif//USE_STACKTRACE_LIB 

					//Invincible-Jones.
					GT_WriteNOP((LPVOID)0x00416D85, 6);
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
#ifdef  USE_MINHOOK_LIB
					hook_instance.reset();
					LOG_WARNING("Hook uninitialized.");
#endif
#ifdef  USE_STACKTRACE_LIB
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
					g_DbgHelper->StackTrace(true);
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

	//string m_str(32, '\0');
	//strcpy(m_str.data(), (char*)0x00A4EC6C);
	//LOG_INFO("Model: %s", m_str.data());

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
		g_Console->Clear();
	}

	else if (g_menu_screen == MENU_SCREEN_INGAME) {
	}

	else if (g_menu_screen == MENU_SCREEN_RESTART) {

	}

	auto GameTextPrint = (void(__cdecl*)(int**, char*))0x4B6E90;
	char msg[] = "SNIP3RZ";

	//if (g_Enabled) GameTextPrint((int**)0x12A37A9C, msg);

	if (GT_IsKeyToggled(VK_F1)) {
		CONFIG::READ();
		SFX::VOLUME_SET(0.5f);
		MISC::STATUS_MESSAGE_SHOW("MUSIC_SET_VOLUME", IGI_CONST_STATUSSCREEN_WEAPON);
	}

	else if (GT_IsKeyToggled(VK_F2)) {
		CONFIG::WEAPON_CONFIG_READ();
		//SFX::VOLUME_SFX_SET(0.8f);
		MISC::STATUS_MESSAGE_SHOW("WEAPON_CONFIG_READ");
	}

	else if (GT_IsKeyToggled(VK_KANJI)) {
	}

	else if (GT_IsKeyToggled(VK_F3)) {


		g_Enabled = !g_Enabled;

		//LOG_INFO("GameTextPrint run");
		//ThreadCallerExec<Void>(WEAPON::WEAPON_PICKUP, WEAPON_ID_JACKHAMMER);
		//LOG_INFO("WeaponPickup Run : Count %d", WEAPON::TOTAL_COUNT());
	}
	else if (GT_IsKeyToggled(VK_F4)) {

	}

	else if (GT_IsKeyToggled(VK_F5)) {
		//LEVEL::RESTART();
		delay_ms = 5000;
		ThreadCallerExec<Void>(LEVEL::RESTART);
	}

	else if (GT_IsKeyToggled(VK_F6)) {
		auto LoadResourceFunc = (char* (__cdecl*)(const char*, char**))0x4B5F00;
		auto ret_val = LoadResourceFunc("LOCAL:hconfig.qsc", NULL);
		LOG_INFO("LoadResource Run '%s'\n", ret_val);
	}

	else if (GT_IsKeyToggled(VK_F7)) {
		const char* cfg_file = "LOCAL:objects.qsc";
		QFILE::QSC_COMPILE(cfg_file);
		LOG_INFO("Config Objects Run");
	}

	else if (GT_IsKeyToggled(VK_F8)) {
		QTASK::UPDATE();
		QTASK::HASH_INIT(0);
		auto ammo_type_open = (void(__cdecl*)())0x47CAB0;
		ammo_type_open();
		LOG_INFO("AmmoType Run");
	}

	else if (GT_IsKeyToggled(VK_F9)) {
		QTASK::UPDATE();
		QTASK::HASH_INIT(0);
		auto weapon_type_open = (void(__cdecl*)())0x413B70;
		weapon_type_open();
		LOG_INFO("WeaponType Run");
	}

	else if (GT_IsKeyToggled(VK_RETURN)) {

		//HumanSoldier::PrintSoldierDataList();
		//soldier_t soldier_id = 2000;
		//auto human = HumanSoldier::FindSoldier(soldier_id);
		//human.PrintSoldierData();

		//for (const auto& soldier : soldiers) {
		//	if (soldier.soldier_id != 0) {
		//		string ai_data_info = "Model: " + soldier.model_id + " Id: " + std::to_string(soldier.soldier_id) + " " + soldier.weapon;
		//		MISC::STATUS_MESSAGE_SHOW(ai_data_info.c_str());
		//		std::this_thread::sleep_for(3s);
		//	}
		//}
		//soldiers.clear();

		//auto SoldierDead = (void(__cdecl*)(int, int))0x0045C440;

		//for (auto& soldier : soldiers) {
		//	int32_t soldier_addr = soldier.GetAddress();
		//	int32_t soldier_id = soldier.GetSoldierId();
		//	if (soldier_addr != 0 && soldier_id != -1) {
		//		if (soldier_id == 0) {
		//			LOG_ERROR("HumanSoldier_%d cannot be Executed!", soldier_id);
		//			continue;
		//		}

		//		int soldier_ptr = (soldier_addr + 0x2EC);
		//		char* dead_expr = (char*)(soldier_addr + 0x2EC + 0x107C);
		//		if (strlen(dead_expr) > 3)
		//			LOG_FILE("Expression %s", dead_expr);

		//		SoldierDead(soldier_ptr, soldier.GetAddress());
		//		LOG_FILE("HumanSoldier_%d Executed!", soldier_id);
		//	}
		//	else
		//		LOG_ERROR("Soldier address is invalid");
		//}
		soldiers.clear();
	}


	else if (GT_IsKeyToggled(VK_F11)) {
		DEBUG::TEXT_INIT(IGI_CONST_FONT_BIG);
		DEBUG::TEXT_ENABLE(g_Enabled);

		//Bypass Regular DebugPrint to Humanplayer.
		std::vector<uint8_t> vec{ {0x75},{0x47} };
		g_Memory->WriteMemory((LPVOID)0x00460C8F, vec);
	}

	else if (GT_IsKeyToggled(VK_F12)) {
		for (const auto& model_id : mef_files) {
			LOG_FILE("MEF: '%s'", model_id.c_str());
			SaveMesh_MEF(model_id);
		}
	}

	else if (GT_IsKeyToggled(VK_HOME)) {
		StartLevelMain(4);
	}

	else if (GT_IsKeyToggled(VK_PRIOR)) {
		static int hcam_val = 0;
		HUMAN::CAM_VIEW_SET(hcam_val);
		hcam_val = (++hcam_val > 6) ? 0 : hcam_val;
	}

	else if (GT_IsKeyToggled(VK_SNAPSHOT)) {
		g_Console->Clear();
	}
}


void SaveMesh_MEF(string model_id) {
	char sym_buf[0x64] = { NULL };
	binary_t mef_buf(0x30D40, '\0');
	auto LoadGameData = (int(__cdecl*)(char*, const char*, const char*))0x004A53B3;
	auto ResourceLoad = (int* (__cdecl*)(const char*, char**))0x004B5F00;

	//LoadGameData(sym_buf, "LOCAL:models/%s.mef", model_id.c_str());
	//LOG_CONSOLE("Symbol: %s", sym_buf);
	auto res_addr = ResourceLoad(model_id.data(), NULL);
	LOG_CONSOLE("ResourceLoaded at address %p", (int)res_addr);

	std::memcpy(mef_buf.data(), (void*)res_addr, mef_buf.capacity());
	binary_t mef_end{ 'N', 'A', 'M', 'E' };
	auto it = std::search(mef_buf.begin(), mef_buf.end(), mef_end.begin(), mef_end.end());
	mef_buf.resize(it - mef_buf.begin());

	model_id = model_id.substr(model_id.find_last_of("/"));
	string mesh_file = g_Utility.GetModuleFolder() + "\\" + model_id + "_res.mef";
	WriteFileType(mesh_file, mef_buf, BINARY_FILE);
	LOG_CONSOLE("Mesh File %s saved successfully!", mesh_file.c_str());
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