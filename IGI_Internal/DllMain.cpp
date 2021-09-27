#pragma 
//ich liebe es zu programmieren und Deustchland <3 
#define USE_STACKTRACE_LIB 
#define USE_MINHOOK_LIB 
#define USE_GTLIBC_LIB 

#include "DllMain.hpp" 



//Include all static libraries for project. 
#if defined(_M_IX86) 
#if defined(DBG_x86)
#ifdef USE_MINHOOK_LIB 
#pragma comment(lib,"hook/libMinHook-x86-Debug.lib")
#endif 
#ifdef USE_GTLIBC_LIB 
#pragma comment(lib,"libs/GTLibc-x86-Debug.lib")
#pragma comment(lib,"libs/GTConsole-x86-Debug.lib")
#endif 

#elif defined(RLS_x86)
#ifdef USE_MINHOOK_LIB 
#pragma comment(lib,"hook/libMinHook-x86-Release.lib")
#endif 
#ifdef USE_GTLIBC_LIB 
#pragma comment(lib,"libs/GTLibc-x86-Release.lib")
#pragma comment(lib,"libs/GTConsole-x86-Release.lib")
#endif 
#endif 
#else 
#error This project supports only x86 (32-Bit) builds. 
#endif 

auto ParseConfig = (int(__cdecl*)(char* q_file))0x405850;
auto ParseWeaponConfig = (int(__cdecl*)(int index, char* cfg_file))0x4071E0;
auto CompileConfig = (int(__cdecl*)(int a1, char* q_file, int a3))0x4F0CF0;
auto CreateConfig = (int(__cdecl*)(char* file_name))0x405BD0;
auto LevelLoad = (int(__cdecl*)(int level_addr, int level_len, int param3, int param4))0x4F0E10;
auto LoadLevelMenu = (void(__cdecl*)(void))0x416fe0;
auto LoadResourceFile = (int* (__cdecl*)(const char* res_file, char** buffer))0x4B5F00;
auto MenuManager = (int(__cdecl*)(int a1, char* a2, char a3, char a4, int a5))0x418B00;
auto QuitLevel = (int(__cdecl*)(int param))0x416550;
auto QuitLevelParam = (int(__cdecl*)(unsigned int a1, int a2, int a3))0x401530;
auto RemoveSymbol = (char* (__cdecl*)(int* a1, char symbol))0x4C0460;
auto ScriptInit = (char(__cdecl*)(int a1, char a2, int a3, char a4))0x4F0E50;
auto Script_SetSymbolContext = (int(__cdecl*)(char symbol_name, int symbol_buf))0x4B8930;
auto SetFramesVar = (void(__cdecl*)(int frame_count))0x402820;
auto StartLevel = (int(__cdecl*)(int, int, int, int))0x415B30;
auto StatusMsg = (int(__cdecl*)(int send_status, const char* buffer, const char* msg_sprite, const char* v6))0x485970;
auto TaskTypeSet = (int(__cdecl*)(int* task_name, int task_param))0x4B8810;
auto UnLoadResource = (int(__cdecl*)(char* res_file))0x4B6380;
auto UpdateQTaskList = (int(__cdecl*)(int a1))0x401B20;


decltype(ParseConfig) ParseConfigOut;
decltype(ParseWeaponConfig) ParseWeaponConfigOut;
decltype(CreateConfig) CreateConfigOut;
decltype(SetFramesVar) SetFramesVarOut;
decltype(StartLevel) StartLevelOut;
decltype(QuitLevel) QuitLevelOut;
decltype(LevelLoad) LevelLoadOut;

#define ShowStatusMsgBox(msg)  StatusMsgBox(msg, NULL, 1)
#define ShowStatusDlg(msg)  StatusMsgBox(msg, NULL, 0)
void StartLevelMain(int, bool, bool, char);

DWORD WINAPI MainThread(LPVOID lp_thread_parameter);
int StatusMsgBox(const char* status_msg, const char* msg_sprite, char msg_type);
int BypassSymbolCheck(BOOL bypass);

//Global vars. 
int timer_i_d;
HookProc hook_proc;
TimerProc hook_timer;
uint32_t hook_timeout;
string hook_caption;
HHOOK h_hook;
int caption_len;


void DllCleanup() {
#ifdef  USE_MINHOOK_LIB 
	if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
		LOG_ERROR("Error disabling hook");
	else
		LOG_WARNING("[-]All Hooks disabled");
#endif //  USE_MINHOOK_LIB 

#ifdef _DEBUG 
	if (GetConsole()->IsAllocated()) {
		GetConsole()->DeAllocate();
	}
#endif 
}

void ReadWholeFile(LPCSTR file_name, LPCSTR file_mode = "rb") {
	LOG_WARNING("%s File : %s Mode : %s", FUNC_NAME, file_name, file_mode);
	try {
		if (string(file_mode) == "rb") {
			auto file_string = string(file_name);
			std::ifstream ifs(file_name, std::ios::binary);
			if (ifs.good()) {
				std::ofstream ofs(file_string.insert(file_string.length() - 4, "_open"), std::ios::binary);
				std::vector<byte> buffer(std::istreambuf_iterator<char>(ifs), {});
				std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<char>(ofs, ""));
			}
			else
			{
				LOG_WARNING("%s File : %s doesn't exist", FUNC_NAME, file_name);
			}
		}
	}
	catch (const std::exception& ex) {
		std::cout << "Exception: " << ex.what() << std::endl;
	}
}

int __cdecl ParseConfigDetour(char* q_file) {
	LOG_WARNING("%s : q_file : %s\n", FUNC_NAME, q_file);
	Sleep(100);
	return ParseConfigOut(q_file);
}

int __cdecl CreateConfigDetour(char* q_file) {
	LOG_WARNING("%s : q_file : %s\n", FUNC_NAME, q_file);
	Sleep(100);
	int ret_val = CreateConfigOut(q_file);

	auto stack_trace = DbgHelper::StackTraceWalk(true);
	DbgHelper::StackTracePrint(stack_trace, true);

	return ret_val;
}

int __cdecl ParseWeaponConfigDetour(int index, char* cfg_file) {
	LOG_WARNING("%s index : %d cfg_file : %s\n", FUNC_NAME, index, cfg_file);
	Sleep(100);
	return ParseWeaponConfigOut(index, cfg_file);
}

void __cdecl SetFramesDetour(int frames) {
	LOG_WARNING("%s : Frames : %d\n", FUNC_NAME, frames);
	Sleep(100);
	return SetFramesVarOut(frames);
}

int __cdecl  StartLevelDetour(int param1, int param2, int param3, int param4) {
	LOG_WARNING("%s param1 : %p param2 : %p param3 : %p param4 : %p\n", FUNC_NAME, param1, param2, param3, param4);
	Sleep(100);
	return StartLevelOut(param1, param2, param3, param4);
}

int __cdecl  QuitLvlDetour(int param1, int param2, int param3, int param4) {
	LOG_WARNING("%s param1 : %p param2 : %p param3 : %p param4 : %p\n", FUNC_NAME, param1, param2, param3, param4);
	Sleep(100);
	return QuitLevelOut(param1);
}

//Dummies detours. 
decltype(LoadResourceFile) LoadResourceOut;

auto CompileQVM = (void(__cdecl*)(char*))0x4B85B0;
decltype(CompileQVM) CompileQVMOut;

auto GameOpenFile = (FILE * (__cdecl*)(char*, char*))0x4A5350;
decltype(GameOpenFile) GameOpenFileOut;

auto LoadQVM = (int* (__cdecl*)(LPCSTR))0x4B80B0;
decltype(LoadQVM) LoadQVMOut;

auto AssembleQAS = (int(__cdecl*)(char*, char*))0x4BB270;
decltype(AssembleQAS) AssembleQASOut;

auto AmmoTypeOpen = (void(__cdecl*)(int))0x47CA40;
decltype(AmmoTypeOpen) AmmoTypeOpenOut;

auto WeaponTypeOpen = (void(__cdecl*)(int))0x413A90;
decltype(WeaponTypeOpen) WeaponTypeOpenOut;

void __cdecl AmmoTypeOpenDetour(int param1) {
	LOG_INFO("%s param1 : %p", FUNC_NAME, param1);
	AmmoTypeOpenOut(param1);
}

void __cdecl WeaponTypeOpenDetour(int param1) {
	LOG_INFO("%s param1 : %p", FUNC_NAME, param1);
	WeaponTypeOpenOut(param1);
}

int __cdecl AssembleQASDetour(char* file_out, char* file_in) {
	int qas_ret = AssembleQASOut(file_out, file_in);
	LOG_INFO("%s file_in : %s file_out : %s  Status : %d", "AssembleQAS", file_in, file_out, qas_ret);
	return qas_ret;
}

int* __cdecl LoadResourceDetour(char* param1, char** param2) {

	int* res_addr = LoadResourceOut(param1, param2);
	LOG_FILE("%s File '%s'\t Address %p", "LoadResource", param1, (int)res_addr);

	auto stack_trace = DbgHelper::StackTraceWalk(true);
	DbgHelper::StackTracePrint(stack_trace, true);

	return res_addr;
}

void __cdecl CompileQVMDetour(char* file_name) {
	//LOG_INFO("%s file_name : %s", "CompileQVM", file_name); 

	*(PDWORD64)(0x0201B1) += 1;
	strcpy((char*)0x943606, file_name);
	Sleep(100);
	CompileQVMOut(file_name);
}

int* __cdecl LoadQVMDetour(LPCSTR file_name) {
	LOG_FILE("%s file_name : %s", "LoadQVM", file_name);

	auto CompileQVM = (int(__cdecl*)(int))0x4B85B0;
	auto CompileCleanUp = (int(__cdecl*)(int*))0x4B83D0;
	auto qvm_file = LoadQVMOut(file_name);

	//ReadWholeFile(file_name); 
	return qvm_file;
}

FILE* __cdecl GameOpenFileDetour(char* file_name, char* file_mode) {
	//if (string(file_mode) == "wb")
	LOG_FILE("%s File : '%s' Mode : '%s'", "OpenFile", file_name, file_mode);

	//ReadWholeFile(file_name, file_mode); 
	auto stack_trace = DbgHelper::StackTraceWalk(true);
	DbgHelper::StackTracePrint(stack_trace, true);

	Sleep(100);
	return GameOpenFileOut(file_name, file_mode);
}

int __cdecl LevelLoadDetour(int param1, int param2, int param3, int param4) {
	LOG_INFO("%s param1 : %s param2 : %p param3 : %p  param4 : %p", "LevelLoad", param1, param2, param3, param4);
	Sleep(100);
	auto stack_trace = DbgHelper::StackTraceWalk(true);
	DbgHelper::StackTracePrint(stack_trace, true);
	return LevelLoadOut(param1, param2, param3, param4);
}



BOOL WINAPI  DllMain(HINSTANCE h_module, DWORD fdw_reason, LPVOID lpv_reserved)
{
	BOOL result;

	switch (fdw_reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(h_module);
		CreateThread(0, 0, MainThread, h_module, 0, 0);
		SetModuleHandle(h_module);
		result = 1;
		break;
	case DLL_PROCESS_DETACH:
		LOG_INFO("[-]Dll Deattached");
		DllCleanup();
		result = 1;
		break;
	default:
		result = 1;
		break;
	}
	return result;
}

bool WriteMemory(LPVOID address, std::vector<byte>& v_bytes)
{
	if (address == NULL || v_bytes.size() == 0) {
		GT_ShowError("Error occurred while writing data to memory.");
		return false;
	}

	DWORD old_protection = NULL;
	const SIZE_T write_len = v_bytes.size() * sizeof(byte);

	VirtualProtect(address, write_len, PAGE_EXECUTE_READWRITE, &old_protection);
	std::memcpy(address, &v_bytes[0], write_len);
	VirtualProtect(address, write_len, old_protection, &old_protection);

	return true;
};

#ifdef  USE_MINHOOK_LIB 
int CreateAllHooks() {

	auto mh_status = MH_CreateHookEx(ParseConfig, &ParseConfigDetour, &ParseConfigOut);
	if (mh_status != MH_OK)
		LOG_ERROR("ParseConfig Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_CreateHookEx(CreateConfig, &CreateConfigDetour, &CreateConfigOut);
	if (mh_status != MH_OK)
		LOG_ERROR("CreateConfig Hooking error : %s", MH_StatusToString(mh_status));

	//mh_status = MH_CreateHookEx(ParseWeaponConfig, &ParseWeaponConfigDetour, &ParseWeaponConfigOut); 
	//if (mh_status != MH_OK)
	// LOG_ERROR("ParseWeaponConfig Hooking error : %s", MH_StatusToString(mh_status)); 

	//mh_status = MH_CreateHookEx(StartLevel, &StartLevelDetour, &StartLevelOut); 
	//if (mh_status != MH_OK)
	// LOG_ERROR("StartLevel Createhook error : %s", MH_StatusToString(mh_status)); 

	//mh_status = MH_CreateHookEx(QuitLvl, &QuitLvlDetour, &QuitLvlOut); 
	//if (mh_status == MH_OK)
	// LOG_INFO("QuitLvl Hooking DONE!"); 
	//else 
	// LOG_ERROR("QuitLvl Createhook error : %s", MH_StatusToString(mh_status)); 

	mh_status = MH_CreateHookEx(LoadResourceFile, &LoadResourceDetour, &LoadResourceOut);
	if (mh_status != MH_OK)
		LOG_ERROR("GameResourcesLoad Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_CreateHookEx(LevelLoad, &LevelLoadDetour, &LevelLoadOut);
	if (mh_status != MH_OK)
		LOG_ERROR("LevelLoad Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_CreateHookEx(GameOpenFile, &GameOpenFileDetour, &GameOpenFileOut);
	if (mh_status != MH_OK)
		LOG_ERROR("GameOpenFile Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_CreateHookEx(LoadQVM, &LoadQVMDetour, &LoadQVMOut);
	if (mh_status != MH_OK)
		LOG_ERROR("LoadQVM Createhook error : %s", MH_StatusToString(mh_status));

	//mh_status = MH_CreateHookEx(CompileQVM, &CompileQVMDetour, &CompileQVMOut); 
	//if (mh_status != MH_OK)
	// LOG_ERROR("CompileQVM Createhook error : %s", MH_StatusToString(mh_status)); 

	mh_status = MH_CreateHookEx(AssembleQAS, &AssembleQASDetour, &AssembleQASOut);
	if (mh_status != MH_OK)
		LOG_ERROR("AssembleQAS Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_CreateHookEx(AmmoTypeOpen, &AmmoTypeOpenDetour, &AmmoTypeOpenOut);
	if (mh_status != MH_OK)
		LOG_ERROR("AmmoTypeOpen Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_CreateHookEx(WeaponTypeOpen, &WeaponTypeOpenDetour, &WeaponTypeOpenOut);
	if (mh_status != MH_OK)
		LOG_ERROR("WeaponTypeOpen Createhook error : %s", MH_StatusToString(mh_status));

	return mh_status;
}

int EnableAllHooks() {
	auto mh_status = MH_EnableHook(MH_ALL_HOOKS);
	if (mh_status == MH_OK)
		LOG_WARNING("[+]All Hooks enabled");
	else
		LOG_ERROR("Error enabling Hooks Reason: %s", MH_StatusToString(mh_status));
	return mh_status;
}
#endif //USE_MINHOOK_LIB 

#ifdef USE_STACKTRACE_LIB 
HANDLE InitStackTrace() {
	auto g_Handle = GT_GetGameHandle4mHWND(GetForegroundWindow());
	Utility::SetHandle(g_Handle);

	if (SymInitialize(g_Handle, NULL, TRUE) == FALSE)
	{
		DBG_TRACE(__FUNCTION__ ": Failed to call SymInitialize.");
		return nullptr;
	}
	return g_Handle;
}
#endif 

DWORD WINAPI MainThread(LPVOID h_module) {

	try {
#ifdef _DEBUG 
		GetConsole()->Allocate();
		GetConsole()->Clear();
#endif 

#ifdef USE_STACKTRACE_LIB 
		auto g_Handle = InitStackTrace();
#endif 

		GT_InitTrainerWindow("IGI-Base", 50, 50, 1200, 700);// BG_OLIVE, FG_BROWN); 
		//GT_SetConsoleTextColor(FG_WHITE); 

		auto logger_instance = std::make_unique<Log>();
		LOG_INFO("Logger initialized!");

		auto native_instance = std::make_unique<Natives>();
		LOG_INFO("Natives initialized!");

		auto native_caller_instance = std::make_unique<NativeCaller>();
		LOG_INFO("NativeCaller initialized!");

		auto memory_instance = std::make_unique<Memory>();
		LOG_INFO("Memory initialized!");

		LOG_INFO("[+]Dll Attached");

		//Init Signature patterns. 
		string sig_err_reason;
		bool init_sigs = g_Memory->SignatureScan(sig_err_reason);
		if (init_sigs)
			LOG_INFO("Signatures Scanning DONE!");
		else {
			auto sig_error = "Pattern signatures not found!\nReason: " + sig_err_reason;
			LOG_ERROR(sig_error.c_str());
			GT_ShowError(sig_error.c_str());
			DllCleanup();
			FreeLibraryAndExitThread((HMODULE)h_module, 0);
			return EXIT_FAILURE;
		}

#ifdef  USE_MINHOOK_LIB 

		// Initialize MinHook. 
		MH_STATUS mh_status = MH_Initialize();
		if (mh_status != MH_OK)
		{
			LOG_ERROR("Minhook Init Error : %s", MH_StatusToString(mh_status));
			DllCleanup();
			FreeLibraryAndExitThread((HMODULE)h_module, 0);
			return EXIT_FAILURE;
		}
		else
		{
			LOG_INFO("Hooking Init DONE!");
			auto mh_status = CreateAllHooks();

			if (mh_status == MH_OK) {
				LOG_INFO("Creating Hooks DONE!");
				mh_status = EnableAllHooks();
				if (mh_status == MH_OK) {
					LOG_INFO("Enabling Hooks DONE!");
				}
			}
		}
#endif //USE_MINHOOK_LIB 

		//Main loop of DLL. 

		while (!GT_IsKeyPressed(VK_END)) {
			DllMainLoop();
			std::this_thread::sleep_for(10ms);
		}

		logger_instance.reset();
		native_instance.reset();
		native_caller_instance.reset();
		memory_instance.reset();


#ifdef USE_STACKTRACE_LIB 
		if (g_Handle)
			SymCleanup(g_Handle);
#endif //USE_STACKTRACE_LIB 

		//Fiber::fiber_handle = CreateFiber(NULL, Fiber::fiber_thread, nullptr); 
		//Fiber::fiber_thread(NULL); 
		FreeLibraryAndExitThread((HMODULE)h_module, 0);
	}
	catch (const std::exception ex) {
		GT_ShowError(ex.what());
		auto stack_trace = DbgHelper::StackTraceWalk(true);
		DbgHelper::StackTracePrint(stack_trace, true);
	}
}

void DllMainLoop() {

	if (GT_IsKeyToggled(VK_F1)) {
		CONFIG::CONFIG_READ(NATIVE_CONST_CONFIG_FILE);
		g_Natives->FindNativeName(0x004B1FCF);
	}

	else if (GT_IsKeyToggled(VK_F2)) {

		const char* qsc_file = "LOCAL:hconfig.qsc";
		auto AssembleQVM = (int(__cdecl*)(const char*, const char*))0x4BB270;
		//auto LoadResourceFunc = (char* (__cdecl*)(const char*, char**))0x4B5F00; 
		auto OpenFileFunc = (char* (__cdecl*)(const char*, const char*))0x4A5350;

		string qas_file = string(qsc_file);
		size_t lastindex = qas_file.find_last_of(".");
		auto qvm_file = qas_file.substr(0, lastindex).append(".qvm");
		qas_file = qas_file.substr(0, lastindex).append(".qas");

		LoadResourceFile(qsc_file, NULL);
		OpenFileFunc(qas_file.c_str(), "wb");

		auto assemble_ret = AssembleQVM(qvm_file.c_str(), qas_file.c_str());
		(assemble_ret == 0) ? LOG_INFO("AssembleQVM Success") : LOG_ERROR("AssembleQVM Error");
		//auto MainRestart = (int(__cdecl*)(int, int, int, int))0x416FE0; 
		//MainRestart(*(PINT)0x54F8E8, *(PINT)0xA972D4, *(PINT)0x54F8E0, *(PINT)0x54F8E4); 
	}

	else if (GT_IsKeyToggled(VK_F3)) {
		GFX::GRAPHICS_RESET();
		//QuitLvl(*(PINT)0x57BABC, *(PINT)0xC28C5C, *(PINT)(*(PINT)0x57BABC), *(PINT)0x567C8C); 

	}
	else if (GT_IsKeyToggled(VK_F4)) {
		StartLevelMain(4, true, true, '1');
	}

	else if (GT_IsKeyToggled(VK_F5)) {

		//MISC::ERRORS_DISABLE(); 
		//MISC::WARNINGS_DISABLE(); 

		auto CompileQVM = (int(__cdecl*)(const char*))0x4B85B0;
		CompileQVM("LOCAL:config.qvm");
		LOG_INFO("CompileQVM Run");
	}

	else if (GT_IsKeyToggled(VK_F6)) {
		auto ret_val = LoadResourceFile("LOCAL:hconfig.qsc", NULL);
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

	else if (GT_IsKeyToggled(VK_F11)) {
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

int StatusMsgBox(const char* status_msg, const char* msg_sprite, char msg_type)
{
	const char* status_sprite;
	int status_timer;
	int status_type;
	LPVOID status_byte = (LPVOID)0x567C74;

	status_sprite = msg_sprite;
	*(PDWORD)status_byte = msg_type;
	status_timer = *(PINT)0xA758AC;
	status_type = *(PINT)(status_byte);
	if (msg_sprite == NULL)
		status_sprite = (char*)&status_type;

	return StatusMsg(status_timer, status_msg, status_sprite, (char*)&status_byte);
}

int BypassSymbolCheck(BOOL bypass) {

	LPVOID symbol_chk_addr = (LPVOID)0x4C04D6;
	std::vector<BYTE>(v_bytes);

	if (bypass) {
		v_bytes.push_back(0x75);
		v_bytes.push_back(0x3A);
	}
	else {
		v_bytes.push_back(0x74);
		v_bytes.push_back(0x3A);
	}
	int status = WriteMemory(symbol_chk_addr, v_bytes);

	return status;
}

void StartLevelMain(int level = 1, bool disable_warn = true, bool disable_err = false, char hash_var = '1') {
	int* game_level = (int*)0x539560;
	*(PINT)game_level = level;

	*(PINT)0x936274 = !disable_warn;//Disable Warnings. 
	*(PINT)0x936268 = !disable_err; //Disable Errors. 

	//HashInit(hash_var); 
	QTASK::UPDATE();
	StartLevel(*(PINT)0x57BABC, 00000000, *(PINT)0xC28C5C, *(PINT)(*(PINT)0x57BABC));
	QTASK::RESET();
}


void __cdecl LoadLevelMenuDetour() {

	LPVOID a_level_path_addr = (LPVOID)0x57B568;
	LOG_INFO("LoadLevelMenuDetour ");

	QTASK::HASH_INIT(1);
	QTASK::UPDATE();
	LevelLoad((int)a_level_path_addr, 0x1E, *(PINT)0x04A27C18, 0x1);
	//SetFramesVar(0x1e); 
	*(INT*)(*((PINT)0x567C8C) + 0x28) = 4;
	LOG_INFO("LoadLevelMenuDetour Done");
	//LoadLevelMenuOut(); 
}