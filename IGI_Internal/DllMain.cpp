#pragma 
//ich liebe es zu programmieren und Deustchland <3 
#define USE_STACKTRACE_LIB 
#define USE_MINHOOK_LIB 
#define USE_GTLIBC_LIB 
#include "DllMain.hpp" 
#include <AutoMsgBox.hpp>

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

BOOL WINAPI  DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hmod);
		SetModuleHandle(hmod);
		g_Hmodule = hmod;
		g_Main_Thread = CreateThread(nullptr, 0, [](PVOID)->DWORD
			{
				auto console_instance = std::make_unique<Console>();
				auto logger_instance = std::make_unique<Log>();
				try {
#ifdef _DEBUG 
					g_Console->Allocate();
					g_Console->Clear();
					GT_InitTrainerWindow(PROJECT_NAME, 50, 50, 1200, 700);
#endif 

					auto igi_font = LR"(
╔═══╦═══╦═══╗ ╔╦═══╦═══╦════╗  ╔══╦═══╦══╗                 
║╔═╗║╔═╗║╔═╗║░║║╔══╣╔═╗║╔╗╔╗║░░╚╣╠╣╔═╗╠╣╠╝	▄▌			▄ 
║╚═╝║╚═╝║║░║║░║║╚══╣║░╚╩╝║║╚╝░░░║║║║░╚╝║║	▓▓▓▓▓▓▓▓▓▓▓▓▄
║╔══╣╔╗╔╣║░║╠╗║║╔══╣║░╔╗░║║░╔══╗║║║║╔═╗║║	▓▓▓▓▓▓▓▓▓▓▓▓▄ 
║║░░║║║╚╣╚═╝║╚╝║╚══╣╚═╝║░║║░╚══╬╣╠╣╚╩═╠╣╠╬			▀▐▓▓▓▓
╚╝  ╚╝╚═╩═══╩══╩═══╩═══╝ ╚╝    ╚══╩═══╩══╝ 			   ▐▓▓▌
)";

					LOG_RAW(igi_font);
					LOG_WARNING("Logger initialized.");

					auto native_instance = std::make_unique<Natives>();
					LOG_WARNING("Natives initialized.");

					auto native_caller_instance = std::make_unique<NativeCaller>();
					LOG_WARNING("NativeCaller initialized.");

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

					//Main loop of DLL. 
					while (!GT_IsKeyPressed(VK_END)) {
						DllMainLoop();
						std::this_thread::sleep_for(10ms);
					}

					native_instance.reset();
					LOG_WARNING("Natives uninitialized.");

					native_caller_instance.reset();
					LOG_WARNING("NativeCaller uninitialized.");

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

void DllMainLoop() {

	if (GT_IsKeyToggled(VK_F1)) {
		CONFIG::CONFIG_READ(NATIVE_CONST_CONFIG_FILE);
		MISC::STATUS_MESSAGE_SHOW("CONFIG_READ", STATUSSCREEN_WEAPON);
	}

	else if (GT_IsKeyToggled(VK_F2)) {
		CONFIG::CONFIG_WRITE(NATIVE_CONST_CONFIG_FILE);
		MISC::STATUS_MESSAGE_SHOW("CONFIG_WRITE", STATUSSCREEN_GUN);
	}

	else if (GT_IsKeyToggled(VK_KANJI)) {

		const char* qsc_file = "LOCAL:hconfig.qsc";
		auto AssembleQVM = (int(__cdecl*)(const char*, const char*))0x4BB270;
		auto LoadResourceFunc = (char* (__cdecl*)(const char*, char**))0x4B5F00;
		auto OpenFileFunc = (char* (__cdecl*)(const char*, const char*))0x4A5350;

		string qas_file = string(qsc_file);
		size_t lastindex = qas_file.find_last_of(".");
		auto qvm_file = qas_file.substr(0, lastindex).append(".qvm");
		qas_file = qas_file.substr(0, lastindex).append(".qas");

		LoadResourceFunc(qsc_file, NULL);
		OpenFileFunc(qas_file.c_str(), "wb");
		auto assemble_ret = AssembleQVM(qvm_file.c_str(), qas_file.c_str());
		(assemble_ret == 0) ? LOG_INFO("AssembleQVM Success") : LOG_ERROR("AssembleQVM Error");
		//auto MainRestart = (int(__cdecl*)(int, int, int, int))0x416FE0; 
		//MainRestart(*(PINT)0x54F8E8, *(PINT)0xA972D4, *(PINT)0x54F8E0, *(PINT)0x54F8E4); 
	}

	else if (GT_IsKeyToggled(VK_F3)) {

		//char* msg = (char*)"This is text for printing.";
		//auto GameTextPrint = (void(__cdecl*)(int**, char*))0x4B6E90;
		//GameTextPrint((int**)0x11EA19DC, (char*)"DISTANCE: 154M");
		//GameTextPrint((int**)0x11EA8034, (char*)"ANYA: Very easily!.");
		//LOG_INFO("GameTextPrint run");

		MISC::WARNINGS_DISABLE();
		auto GunPickup = (void(__cdecl*)(int param_1, int* param_2))0x45FFC0;
		auto AmmoPickup = (void(__cdecl*)(int param_1, int* param_2))0x45FF80;

		uint32_t gun_ammo_pickup_ptr = READ_STATIC_PTR_OFF2(0x005BDC6C, 0x1B0, 0xCB4);

		//M16A2.
		g_AutoMsgBox->Show("", 10);
		*(PINT)0x19F720 = 4; GunPickup(READ_PTR(gun_ammo_pickup_ptr), (int*)0x19F720);
		g_AutoMsgBox->Show("", 10);
		*(PINT)0x19F820 = 8; AmmoPickup(READ_PTR(gun_ammo_pickup_ptr), (int*)0x19F820);
		g_AutoMsgBox->Show("", 10);
		*(PINT)0x19F820 = 2; AmmoPickup(READ_PTR(gun_ammo_pickup_ptr), (int*)0x19F820);

		//JackHammer.
		g_AutoMsgBox->Show("", 10);
		*(PINT)0x19F720 = 9; GunPickup(READ_PTR(gun_ammo_pickup_ptr), (int*)0x19F720);
		g_AutoMsgBox->Show("", 10);
		*(PINT)0x19F820 = 0; AmmoPickup(READ_PTR(gun_ammo_pickup_ptr), (int*)0x19F820);

		LOG_INFO("GunPickup Run : Count %d\n",WEAPON::WEAPONS_COUNT());
		SFX::MUSIC_SET_VOLUME(5);

		//GFX::GRAPHICS_RESET();
		//MISC::STATUS_MESSAGE_SHOW("GRAPHICS_RESET");

		//QuitLvl(*(PINT)0x57BABC, *(PINT)0xC28C5C, *(PINT)(*(PINT)0x57BABC), *(PINT)0x567C8C);
	}
	else if (GT_IsKeyToggled(VK_F4)) {
		auto LoadGameData = (int(__cdecl*)(char*, const char*))0x4A53B3;
		char buff[1024] = { NULL };
		//MISC::STATUS_MESSAGE_SHOW("LoadGameData run");
		LoadGameData(buff, "%s.nActiveID");
		LOG_INFO("local_buf '%s'", buff);

		LoadGameData(buff, "menu_k0%.1d");
		LOG_INFO("local_buf '%s'", buff);

		LoadGameData(buff, "%s.isOn");
		LOG_INFO("local_buf '%s'", buff);

	}

	else if (GT_IsKeyToggled(VK_F5)) {

		//MISC::ERRORS_DISABLE(); 
		//MISC::WARNINGS_DISABLE(); 

		auto StsMsg = (int(__cdecl*)())0x00485a40;
		StsMsg();
		LOG_INFO("StsMsg Run");
		// 
		//auto CompileQVM = (int(__cdecl*)(const char*))0x4B85B0;
		//CompileQVM("LOCAL:config.qvm");
		//LOG_INFO("CompileQVM Run");
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

	else if (GT_IsKeyToggled(VK_F10)) {
		auto ForceUpdateWindow = (int(__cdecl*)(int*, int, int))0x417880;
		auto update_wnd_addr = READ_STATIC_PTR_OFF3(0x568140, 0x14, 0x0, 0xA10);
		LOG_INFO("%s base_update_wnd_addr : %p", FUNC_NAME, update_wnd_addr);

		auto update_wnd_val = READ_PTR(update_wnd_addr);
		LOG_INFO("%s base_update_wnd_val : %p", FUNC_NAME, update_wnd_val);

		ForceUpdateWindow((int*)0x19F974, *(int*)0xA758A8, update_wnd_val);
		LOG_DEBUG("ForceUpdateWindow called");
	}

	else if (GT_IsKeyToggled(VK_HOME)) {
		StartLevelMain(4);
	}

	else if (GT_IsKeyToggled(VK_PRIOR)) {
	}

	else if (GT_IsKeyToggled(VK_F12)) {

		auto player_remove_profile = (int(__cdecl*)(char*, int, int))0x405400;
		auto remove_wnd_addr1 = READ_STATIC_PTR_OFF3(0x568168, 0xC, 0x0, 0xE08);
		auto remove_wnd_addr2 = READ_STATIC_PTR_OFF3(0x568140, 0xC, 0x0, 0xE40);

		LOG_INFO("%s remove_wnd_addr1 : %p", FUNC_NAME, remove_wnd_addr1);
		LOG_INFO("%s remove_wnd_addr2 : %p", FUNC_NAME, remove_wnd_addr2);

		auto remove_wnd_val1 = READ_PTR(remove_wnd_addr1);
		auto remove_wnd_val2 = READ_PTR(remove_wnd_addr2);
		LOG_INFO("%s base_update_wnd_val1 : %p", FUNC_NAME, remove_wnd_val1);
		LOG_INFO("%s base_update_wnd_val2 : %p", FUNC_NAME, remove_wnd_val2);

		//playerRemoveProfile((char*)0x19F948,0x0, remove_wnd_val); 
		LOG_DEBUG("playerRemoveProfile called");
	}

	else if (GT_IsKeyToggled(VK_HOME)) {

	}

	else if (GT_IsKeyToggled(VK_SNAPSHOT)) {
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