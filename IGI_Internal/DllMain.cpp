#pragma

#include "DllMain.hpp"
using namespace IGI;

//Include all static libraries for project.
#if defined _M_IX86
#if defined(DBG_x86)
#pragma comment(lib,"hook/libMinHook-x86-Debug.lib")
#pragma comment(lib,"libs/GTLibc-x86-Debug.lib")
#pragma comment(lib,"libs/GTConsole-x86-Debug.lib")
#ifdef STACK_WALKER
#pragma comment(lib,"libs/StackWalker-x86-Debug.lib")
#endif
#elif defined(RLS_x86)
#pragma comment(lib,"hook/libMinHook-x86-Release.lib")
#pragma comment(lib,"libs/GTLibc-x86-Release.lib")
#pragma comment(lib,"libs/GTConsole-x86-Release.lib")
#ifdef STACK_WALKER
#pragma comment(lib,"libs/StackWalker-x86-Release.lib")
#endif
#endif
#else
#error This project supports only x86 (32-Bit) builds.
#endif

#ifdef STACK_WALKER
//Extend stackwalker for Output.
class StackWalkerEx : public StackWalker
{
protected:
	virtual void OnOutput(LPCSTR stackText) { LOG_FILE("%s", stackText); }
};

//Place in Current method to get stack information.
void foo() {
	StackWalkerEx sw;
	sw.ShowCallstack();
}
#endif // STACK_WALKER


typedef int(__cdecl* IGI_StatusTimer)();
typedef int(__cdecl* IGI_ParseWeaponConfig)(int index, char* cfgFile);
typedef int(__cdecl* IGI_HumanPlayer_LoadParameters)();
typedef int(__cdecl* IGI_EnableInput)(char* fileName);
typedef int(__cdecl* IGI_DisableInput)(char* fileName);
typedef int(__cdecl* IGI_CompileConfig)(int a1, char* qFile, int a3);
typedef int(__cdecl* IGI_ParseConfig)(char* qFile);
typedef int(__cdecl* IGI_LevelLoad)(int levelAddr, int levelLen, int param3, int param4);
typedef int(__cdecl* IGI_LevelLoadParam)(char* fileName, int fileLen);
typedef int(__cdecl* IGI_Script_SetSymbolContext)(char symbolName, int symbolBuf);
typedef int(__cdecl* IGI_TaskTypeSet)(int* taskName, int taskParam);
typedef char(__cdecl* IGI_ScriptInit)(int a1, char a2, int a3, char a4);
typedef int(__cdecl* IGI_CreateConfig)(char* fileName);
typedef char* (__cdecl* IGI_LoadResource)(char* resFile, int a2);
typedef char* (__cdecl* IGI_RemoveSymbol)(int* a1, char symbol);
typedef int(__cdecl* IGI_UnLoadResource)(char* resFile);
typedef int(__cdecl* IGI_RestartLevel)();
typedef int(__cdecl* IGI_QuitLevel)(int param);
typedef int(__cdecl* IGI_RestartLevelParam)(int param);
typedef int(__cdecl* IGI_LevelStartMain)(int param);
typedef int(__cdecl* IGI_HashInit)(char a1);
typedef void(__cdecl* IGI_HashReset)();
typedef int(__cdecl* IGI_StartLevel)(int param1, int param2, int param3, int param4);
typedef int(__cdecl* IGI_QuitLvl)(int param1, int param2, int param3, int param4);
typedef void(__cdecl* IGI_StartLevelLbl)(int param);
typedef int(__cdecl* IGI_MenuManager)(int a1, char* a2, char a3, char a4, int a5);
typedef int(__cdecl* IGI_LevelStartCaller)(int param);
typedef void(__cdecl* IGI_SetFramesVar)(int frameCount);
typedef void(__cdecl* IGI_LoadLevelMenu)();
typedef int(__cdecl* IGI_UpdateQTask)();
typedef int(__cdecl* IGI_StartLevelParam)(unsigned int a1, int a2, int a3);
typedef int* (__cdecl* IGI_LevelStartInit)(int param_1, short param_2, int param_3, int param_4);
typedef int(__cdecl* IGI_QuitLevelParam)(unsigned int a1, int a2, int a3);
typedef int(__cdecl* IGI_UpdateQTaskList)(int a1);
typedef int(__cdecl* IGI_StatusMsg)(int sendStatus, const char* buffer, const char* msgSprite, const char* v6);

#define ShowStatusMsgBox(msg)  StatusMsgBox(msg, NULL, 1)
#define ShowStatusDlg(msg)  StatusMsgBox(msg, NULL, 0)
void StartLevelMain(int, bool, bool, char);

int __cdecl HashInitDetour(char a1);
void __cdecl HashResetDetour();
void __cdecl UpdateQTaskDetour();
void __cdecl LoadLevelMenuDetour();
void DllMainLoop();
MH_STATUS DLL_FreezeThread(LPVOID pTarget, BOOL enable);

DWORD WINAPI MainThread(LPVOID lpThreadParameter);
int StatusMsgBox(const char* statusMsg, const char* msgSprite, char msgType);
int BypassSymbolCheck(BOOL bypass);

IGI_StatusTimer GetStatusTimer;
IGI_StatusMsg StatusMsg;
IGI_CompileConfig CompileConfig;
IGI_LevelLoad LevelLoad, LevelLoadOut = nullptr;
IGI_LevelLoadParam LevelLoadParam;
IGI_ScriptInit ScriptInit;
IGI_HumanPlayer_LoadParameters HumanPlayer_LoadParameters;
IGI_ParseWeaponConfig ParseWeaponConfig, ParseWeaponConfigOut = nullptr;
IGI_DisableInput DisableInput;
IGI_EnableInput EnableInput;
IGI_TaskTypeSet TaskTypeSet;
IGI_Script_SetSymbolContext Script_SetSymbolContext;
IGI_CreateConfig CreateConfig, CreateConfigOut = nullptr;
IGI_ParseConfig ParseConfig, ParseConfigOut = nullptr;
IGI_LoadResource LoadResourceFile;
IGI_UnLoadResource UnLoadResource;
IGI_RestartLevel RestartLevel;
IGI_RestartLevelParam RestartLevelParam;
IGI_UpdateQTaskList UpdateQTaskList;
IGI_HashInit HashInit, HashInitOut = nullptr;
IGI_RemoveSymbol RemoveSymbol;
IGI_QuitLevel QuitLevel;
IGI_UpdateQTask UpdateQTask, UpdateQTaskOut = nullptr;
IGI_StartLevel StartLevel, StartLevelOut;
IGI_StartLevelParam StartLevelParam;
IGI_QuitLevelParam QuitLevelParam;
IGI_MenuManager MenuManager;
IGI_LoadLevelMenu LoadLevelMenu, LoadLevelMenuOut;
IGI_LevelStartInit LevelStartInit;
IGI_LevelStartMain LevelStartMain;
IGI_LevelStartCaller LevelStartCaller;
IGI_SetFramesVar SetFramesVar, SetFramesVarOut;
IGI_HashReset HashReset, HashResetOut = nullptr;
IGI_StartLevelLbl StartLevelLbl;
IGI_QuitLvl QuitLvl, QuitLvlOut;

//Global vars.
int timerID;
HookProc hookProc;
TimerProc hookTimer;
uint32_t hookTimeout;
string hookCaption;
HHOOK hHook;
int captionLen;


void DllCleanup() {
	if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
		LOG_ERROR("Error disabling hook");
	else
		LOG_WARNING("[-]All Hooks disabled");


#ifdef _DEBUG
	if (GetConsole()->IsAllocated()) {
		GetConsole()->DeAllocate();
	}
#endif
}

void ReadWholeFile(LPCSTR fileName, LPCSTR fileMode = "rb") {
	LOG_WARNING("%s File : %s Mode : %s", FUNC_NAME, fileName, fileMode);
	try {
		if (std::string(fileMode) == "rb") {
			auto fileString = std::string(fileName);
			std::ifstream ifs(fileName, std::ios::binary);
			if (ifs.good()) {
				std::ofstream ofs(fileString.insert(fileString.length() - 4, "_open"), std::ios::binary);
				std::vector<byte> buffer(std::istreambuf_iterator<char>(ifs), {});
				std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<char>(ofs, ""));
			}
			else
			{
				LOG_WARNING("%s File : %s doesn't exist", FUNC_NAME, fileName);
			}
		}
	}
	catch (const std::exception& ex) {
		std::cout << "Exception: " << ex.what() << std::endl;
	}
}

int __cdecl ParseConfigDetour(char* qFile) {
	LOG_WARNING("%s : qFile : %s\n", FUNC_NAME, qFile);
	Sleep(100);
	return ParseConfigOut(qFile);
}

int __cdecl CreateConfigDetour(char* qFile) {
	LOG_WARNING("%s : qFile : %s\n", FUNC_NAME, qFile);
	Sleep(100);
	return CreateConfigOut(qFile);
}

int __cdecl ParseWeaponConfigDetour(int index, char* cfgFile) {
	LOG_WARNING("%s index : %d cfgFile : %s\n", FUNC_NAME, index, cfgFile);
	Sleep(100);
	return ParseWeaponConfigOut(index, cfgFile);
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
	return QuitLvlOut(param1, param2, param3, param4);
}


//Dummies detours.
auto GameResourcesLoad = (int*(__cdecl*)(char*,char**))0x4B5F00;
decltype(GameResourcesLoad) GameResourcesLoadOut;

auto CompileQVM = (void(__cdecl*)(char*))0x4B85B0;
decltype(CompileQVM) CompileQVMOut;

auto GameOpenFile = (FILE * (__cdecl*)(char*, char*))0x4A5350;
decltype(GameOpenFile) GameOpenFileOut;

auto LoadQVM = (int*(__cdecl*)(LPCSTR))0x4B80B0;
decltype(LoadQVM) LoadQVMOut;

auto AssembleQAS = (int (__cdecl*)(char*, char*))0x4BB270;
decltype(AssembleQAS) AssembleQASOut;

int __cdecl AssembleQASDetour(char* fileOut, char* fileIn) {
	int qasRet = AssembleQASOut(fileOut, fileIn);
	LOG_INFO("%s fileIn : %s fileOut : %s  Status : %d", "AssembleQAS", fileIn, fileOut, qasRet);
	return qasRet;
}

int* __cdecl GameResourcesLoadDetour(char* param1, char** param2) {
	int* resAddr = GameResourcesLoadOut(param1, param2);
	LOG_FILE("%s File '%s'\t Address %p", "LoadResource", param1, (int)resAddr);
	return resAddr;
}

void __cdecl CompileQVMDetour(char* fileName) {
	//LOG_INFO("%s fileName : %s", "CompileQVM", fileName);
	
	*(PDWORD64)(0x000201B1) += 1;
	strcpy((char*)0x943606, fileName);
	Sleep(100);
	CompileQVMOut(fileName);
}

int* __cdecl LoadQVMDetour(LPCSTR fileName) {
	LOG_INFO("%s fileName : %s", "LoadQVM", fileName);

	auto CompileQVM = (int(__cdecl*)(int))0x4B85B0;
	auto CompileCleanUp = (int(__cdecl*)(int*))0x4B83D0;
	auto qvmFile = LoadQVMOut(fileName);

	//ReadWholeFile(fileName);
	return qvmFile;
}

FILE* __cdecl GameOpenFileDetour(char* fileName, char* fileMode) {
	if(std::string(fileMode) == "wb")
	LOG_INFO("%s File : '%s' Mode : '%s'", "OpenFile", fileName, fileMode);

	//ReadWholeFile(fileName, fileMode);
	Sleep(100);
	return GameOpenFileOut(fileName, fileMode);
}

int __cdecl LevelLoadDetour(int param1, int param2, int param3, int param4) {
	LOG_INFO("%s param1 : %s param2 : %p param3 : %p  param4 : %p", "LevelLoad", param1, param2, param3, param4);
	Sleep(100);
	return LevelLoadOut(param1, param2, param3, param4);
}

//0x4f0e10; LevelLoad.
//0x416FE0; LevelLoadMenu.
//0x004b8890; -GameDefine.
//0x4A5350 - OpenFile.


BOOL WINAPI  DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
	BOOL result;

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		SetModuleHandle(hModule);
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

BOOL WriteMemory(LPVOID address, std::vector< BYTE >& vBytes)
{
	if (address == NULL || vBytes.size() == 0) {
		GT_ShowError("Error occurred while writing data to memory.");
		return FALSE;
	}

	DWORD OldProtection = NULL;
	const SIZE_T writeLen = vBytes.size() * sizeof(BYTE);

	VirtualProtect(address, writeLen, PAGE_EXECUTE_READWRITE, &OldProtection);
	std::memcpy(address, &vBytes[0], writeLen);
	VirtualProtect(address, writeLen, OldProtection, &OldProtection);

	return TRUE;
};

int CreateAllHooks() {
	
	auto mh_status = MH_CreateHookEx(ParseConfig, &ParseConfigDetour, &ParseConfigOut);
	if (mh_status != MH_OK)
		LOG_ERROR("ParseConfig Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_CreateHookEx(CreateConfig, &CreateConfigDetour, &CreateConfigOut);
	if (mh_status != MH_OK)
		LOG_ERROR("CreateConfig Hooking error : %s", MH_StatusToString(mh_status));

	//mh_status = MH_CreateHookEx(ParseWeaponConfig, &ParseWeaponConfigDetour, &ParseWeaponConfigOut);
	//if (mh_status != MH_OK)
	//	LOG_ERROR("ParseWeaponConfig Hooking error : %s", MH_StatusToString(mh_status));

	//mh_status = MH_CreateHookEx(StartLevel, &StartLevelDetour, &StartLevelOut);
	//if (mh_status != MH_OK)
	//	LOG_ERROR("StartLevel Createhook error : %s", MH_StatusToString(mh_status));

	//mh_status = MH_CreateHookEx(QuitLvl, &QuitLvlDetour, &QuitLvlOut);
	//if (mh_status == MH_OK)
	//	LOG_INFO("QuitLvl Hooking DONE!");
	//else
	//	LOG_ERROR("QuitLvl Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_CreateHookEx(GameResourcesLoad, &GameResourcesLoadDetour, &GameResourcesLoadOut);
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
	//	LOG_ERROR("CompileQVM Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_CreateHookEx(AssembleQAS, &AssembleQASDetour, &AssembleQASOut);
	if (mh_status != MH_OK)
		LOG_ERROR("AssembleQAS Createhook error : %s", MH_StatusToString(mh_status));

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

void InitPointers() {

	GetStatusTimer = reinterpret_cast<IGI_StatusTimer>(0x4F0EE0);
	StatusMsg = reinterpret_cast<IGI_StatusMsg>(0x485970);
	CompileConfig = reinterpret_cast<IGI_CompileConfig>(0x4F0CF0);
	LevelLoadParam = reinterpret_cast<IGI_LevelLoadParam>(0x4F0D40);
	LevelLoad = reinterpret_cast<IGI_LevelLoad>(0x4F0E10);
	ScriptInit = reinterpret_cast<IGI_ScriptInit>(0x4F0E50);
	HumanPlayer_LoadParameters = reinterpret_cast<IGI_HumanPlayer_LoadParameters>(0x4137A0);
	ParseWeaponConfig = reinterpret_cast<IGI_ParseWeaponConfig>(0x4071E0);
	EnableInput = reinterpret_cast<IGI_EnableInput>(0x415830);
	DisableInput = reinterpret_cast<IGI_DisableInput>(0x4157D0);
	TaskTypeSet = reinterpret_cast<IGI_TaskTypeSet>(0x4B8810);
	Script_SetSymbolContext = reinterpret_cast<IGI_Script_SetSymbolContext>(0x4B8930);
	CreateConfig = reinterpret_cast<IGI_CreateConfig>(0x405BD0);
	LoadResourceFile = reinterpret_cast<IGI_LoadResource>(0x4B5F00);
	UnLoadResource = reinterpret_cast<IGI_UnLoadResource>(0x4B6380);
	RestartLevel = reinterpret_cast<IGI_RestartLevel>(0x416FE0);
	RestartLevelParam = reinterpret_cast<IGI_RestartLevelParam>(0x48A440);
	UpdateQTaskList = reinterpret_cast<IGI_UpdateQTaskList>(0x401B20);
	HashInit = reinterpret_cast<IGI_HashInit>(0x4B0D60);
	HashReset = reinterpret_cast<IGI_HashReset>(0x004B0D90);
	RemoveSymbol = reinterpret_cast<IGI_RemoveSymbol>(0x4C0460);
	StartLevel = reinterpret_cast<IGI_StartLevel>(0x415B30);
	StartLevelParam = reinterpret_cast<IGI_StartLevelParam>(0x401400);
	QuitLevelParam = reinterpret_cast<IGI_QuitLevelParam>(0x401530);
	ParseConfig = reinterpret_cast<IGI_ParseConfig>(0x405850);
	UpdateQTask = reinterpret_cast<IGI_UpdateQTask>(0x4F0E90);
	QuitLevel = reinterpret_cast<IGI_QuitLevel>(0x416550);
	QuitLvl = reinterpret_cast<IGI_QuitLvl>(0x416550);
	MenuManager = reinterpret_cast<IGI_MenuManager>(0x418B00);
	LevelStartInit = reinterpret_cast<IGI_LevelStartInit>(0x004012A0);
	LevelStartMain = reinterpret_cast<IGI_LevelStartMain>(0x00402070);
	LevelStartCaller = reinterpret_cast<IGI_LevelStartCaller>(0x00416900);
	SetFramesVar = reinterpret_cast<IGI_SetFramesVar>(0x00402820);
	StartLevelLbl = reinterpret_cast<IGI_StartLevelLbl>(0x004869B0);
	LoadLevelMenu = (IGI_LoadLevelMenu)0x00416fe0;
}


DWORD WINAPI MainThread(LPVOID hModule) {
#ifdef _DEBUG
	GetConsole()->Allocate();
	GetConsole()->Clear();
#endif

	GT_InitTrainerWindowEx("IGI-Base", 50, 50, 1200, 700, BG_TEAL, FG_YELLOW);
	GT_SetFontSize(0.2f, false);
	GT_SetConsoleTextColor(BG_TEAL | FG_YELLOW);

	LOG_INFO("[+]Dll Attached");
	LOG_INFO("Initializing Pointers...");
	InitPointers();
	LOG_INFO("Initializing Pointers DONE!");

	//Init Signature patterns.
	std::string sigErrorReason;
	bool initSigs = true;//InitSigPatterns(sigErrorReason);
	if (initSigs)
		LOG_INFO("Initializing Signatures Scanning DONE!");
	else {
		auto sigError = "Pattern signatures not found!\nReason: " + sigErrorReason;
		LOG_DEBUG(sigError.c_str());
		GT_ShowError(sigError.c_str());
		DllCleanup();
		FreeLibraryAndExitThread((HMODULE)hModule, 0);
		return EXIT_FAILURE;
	}

	// Initialize MinHook.
	MH_STATUS mh_status = MH_Initialize();
	if (mh_status != MH_OK)
	{
		LOG_ERROR("Minhook Init Error : %s", MH_StatusToString(mh_status));
		DllCleanup();
		FreeLibraryAndExitThread((HMODULE)hModule, 0);
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

	while (!GT_IsKeyPressed(VK_END)) {
		DllMainLoop();
	}

	//Fiber::fiber_handle = CreateFiber(NULL, Fiber::fiber_thread, nullptr);
	//Fiber::fiber_thread(NULL);
	FreeLibraryAndExitThread((HMODULE)hModule, 0);
}

void DllMainLoop() {

	if (GT_IsKeyToggled(VK_F1)) {
		CONFIG::CONFIG_READ(NATIVE_CONST_CONFIG_FILE);
	}

	else if (GT_IsKeyToggled(VK_F2)) {
		
		const char* qscFile = "LOCAL:hconfig.qsc";
		auto AssembleQVM = (int(__cdecl*)(const char*, const char*))0x4BB270;
		auto LoadResourceFunc = (char* (__cdecl*)(const char*, char**))0x4B5F00;
		auto OpenFileFunc = (char* (__cdecl*)(const char*, const char*))0x4A5350;

		std::string qasFile = std::string(qscFile);
		size_t lastindex = qasFile.find_last_of(".");
		auto qvmFile = qasFile.substr(0, lastindex).append(".qvm");
		qasFile = qasFile.substr(0, lastindex).append(".qas");

		LoadResourceFunc(qscFile, NULL);
		OpenFileFunc(qasFile.c_str(), "wb");

		auto assembleRet = AssembleQVM(qvmFile.c_str(), qasFile.c_str());
		(assembleRet == 0) ? LOG_INFO("AssembleQVM Success") : LOG_ERROR("AssembleQVM Error");
		//auto MainRestart = (int(__cdecl*)(int, int, int, int))0x416FE0;
		//MainRestart(*(PINT)0x0054F8E8, *(PINT)0x00A972D4, *(PINT)0x0054F8E0, *(PINT)0x0054F8E4);
	}

	else if (GT_IsKeyToggled(VK_F3)) {
		QuitLvl(*(PINT)0x0057BABC, *(PINT)0x00C28C5C, *(PINT)(*(PINT)0x0057BABC), *(PINT)0x00567C8C);

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
		auto LoadResourceFunc = (char* (__cdecl*)(const char*, char**))0x4B5F00;
		auto retVal = LoadResourceFunc("LOCAL:hconfig.qsc",NULL);

		LOG_INFO("LoadResource Run '%s'\n", retVal);
	}

	else if (GT_IsKeyToggled(VK_F7)) {
		const char* cfgFile = "LOCAL:objects.qsc";
		QFILE::QSC_COMPILE(cfgFile);
		LOG_INFO("Config Objects Run");
	}

	else if (GT_IsKeyToggled(VK_F8)) {
	}

	else if (GT_IsKeyToggled(VK_F9)) {
	}

	else if (GT_IsKeyToggled(VK_F10)) {
		auto ForceUpdateWindow = (int(__cdecl*)(int*, int, int))0x417880;
		auto updateWndAddr = READ_STATIC_PTR_OFF3(0x568140, 0x14, 0x0, 0xA10);
		LOG_INFO("%s baseUpdateWndAddr : %p", FUNC_NAME, updateWndAddr);

		auto updateWndVal = READ_PTR(updateWndAddr);
		LOG_INFO("%s baseUpdateWndVal : %p", FUNC_NAME, updateWndVal);

		ForceUpdateWindow((int*)0x19F974, *(int*)0xA758A8, updateWndVal);
		LOG_DEBUG("ForceUpdateWindow called");
	}

	else if (GT_IsKeyToggled(VK_F11)) {
	}

	else if (GT_IsKeyToggled(VK_F12)) {

		auto playerRemoveProfile = (int(__cdecl*)(char*, int, int))0x00405400;
		auto removeWndAddr1 = READ_STATIC_PTR_OFF3(0x00568168, 0xC, 0x0, 0xE08);
		auto removeWndAddr2 = READ_STATIC_PTR_OFF3(0x00568140, 0xC, 0x0, 0xE40);

		LOG_INFO("%s removeWndAddr1 : %p", FUNC_NAME, removeWndAddr1);
		LOG_INFO("%s removeWndAddr2 : %p", FUNC_NAME, removeWndAddr2);

		auto removeWndVal1 = READ_PTR(removeWndAddr1);
		auto removeWndVal2 = READ_PTR(removeWndAddr2);
		LOG_INFO("%s baseUpdateWndVal1 : %p", FUNC_NAME, removeWndVal1);
		LOG_INFO("%s baseUpdateWndVal2 : %p", FUNC_NAME, removeWndVal2);

		//playerRemoveProfile((char*)0x0019F948,0x0, removeWndVal);
		LOG_DEBUG("playerRemoveProfile called");
	}

	else if (GT_IsKeyToggled(VK_HOME)) {

	}

	else if (GT_IsKeyToggled(VK_SNAPSHOT)) {
	}


	Sleep(10);
}

int StatusMsgBox(const char* statusMsg, const char* msgSprite, char msgType)
{
	const char* statusSprite;
	int statusTimer;
	int statusType;
	LPVOID statusByte = (LPVOID)0x567C74;

	statusSprite = msgSprite;
	*(PDWORD)statusByte = msgType;
	statusTimer = GetStatusTimer();
	statusType = *(PINT)(statusByte);
	if (msgSprite == NULL)
		statusSprite = (char*)&statusType;

	return StatusMsg(statusTimer, statusMsg, statusSprite, (char*)&statusByte);
}

int BypassSymbolCheck(BOOL bypass) {

	LPVOID symbolChkAddr = (LPVOID)0x004C04D6;
	std::vector<BYTE>(vBytes);

	if (bypass) {
		vBytes.push_back(0x75);
		vBytes.push_back(0x3A);
	}
	else {
		vBytes.push_back(0x74);
		vBytes.push_back(0x3A);
	}
	int status = WriteMemory(symbolChkAddr, vBytes);

	return status;
}

void StartLevelMain(int level = 1, bool disableWarn = true, bool disableErr = false, char hashVar = '1') {
	int* game_level = (int*)0x00539560;
	*(PINT)game_level = level;

	*(PINT)0x00936274 = !disableWarn;//Disable Warnings.
	*(PINT)0x00936268 = !disableErr; //Disable Errors.

	//HashInit(hashVar);
	UpdateQTask();
	StartLevel(*(PINT)0x0057BABC, 00000000, *(PINT)0x00C28C5C, *(PINT)(*(PINT)0x0057BABC));
	HashReset();
}


int __cdecl HashInitDetour(char hashVal) {
	//AutoMsgBox::Show("HashInitDetour",1);
	return HashInitOut(hashVal);
}

void __cdecl HashResetDetour() {
	//AutoMsgBox::Show("HashResetDetour",1);
	HashResetOut();
}

void __cdecl UpdateQTaskDetour() {
	//AutoMsgBox::Show("UpdateQTaskDetour",1);
	UpdateQTaskOut();
}

void __cdecl LoadLevelMenuDetour() {

	LPVOID aLevelPathAddr = (LPVOID)0x0057B568;
	LOG_INFO("LoadLevelMenuDetour ");

	HashInit('1');
	UpdateQTask();
	LevelLoad((int)aLevelPathAddr, 0x1E, *(PINT)0x04A27C18, 0x1);
	//SetFramesVar(0x1e);
	*(INT*)(*((PINT)0x00567C8C) + 0x28) = 4;
	LOG_INFO("LoadLevelMenuDetour Done");
	//LoadLevelMenuOut();
}
