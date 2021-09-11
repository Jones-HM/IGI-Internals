#pragma
#define GT_BUILD_DLL
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1
#define _CRT_SECURE_NO_DEPRECATE
#define DLL_EXPORT __declspec( dllexport )

#include "logger.hpp"
#include "Console.hpp"
#include "GTLibc.hpp"
#include "DllMain.hpp"
#include "AutoMsgBox.hpp"
#include "ThreadsEx.hpp"
#include "General.hpp"
#include "MinHook.hpp"
#include "NativeCaller.hpp"
#include "NativeHelper.hpp"
#include "Natives.hpp"

using namespace IGI;

//Include all static libraries for project.
#if defined _M_IX86
#if defined(DBG_x86)
#pragma comment(lib,"hook/libMinHook-x86-mdd.lib")
#pragma comment(lib,"libs/GTLibc-x86-Debug.lib")
#pragma comment(lib,"libs/GTConsole-x86-Debug.lib")
#elif defined(RLS_x86)
#pragma comment(lib,"hook/libMinHook-x86-md.lib")
#pragma comment(lib,"libs/GTLibc-x86-Release.lib")
#pragma comment(lib,"libs/GTConsole-x86-Release.lib")
#endif
#else
#error This project supports only 32bit builds.
#endif

typedef int(__cdecl* IGI_StatusTimer)();
typedef int(__cdecl* IGI_ParseWeaponConfig)(int index, char* cfgFile);
typedef int(__cdecl* IGI_HumanPlayer_LoadParameters)();
typedef int(__cdecl* IGI_EnableInput)(int* fileName);
typedef int(__cdecl* IGI_DisableInput)(int* fileName);
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
void LevelStartPatch();

int __cdecl LevelLoadDetour(int levelAddr, int levelLen);
int __cdecl HashInitDetour(char a1);
void __cdecl HashResetDetour();
void __cdecl UpdateQTaskDetour();
void __cdecl LoadLevelMenuDetour();
void LevelLoadRestart();
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
IGI_StartLevel StartLevel;
IGI_StartLevelParam StartLevelParam;
IGI_QuitLevelParam QuitLevelParam;
IGI_MenuManager MenuManager;
IGI_LoadLevelMenu LoadLevelMenu, LoadLevelMenuOut;
IGI_LevelStartInit LevelStartInit;
IGI_LevelStartMain LevelStartMain;
IGI_LevelStartCaller LevelStartCaller;
IGI_SetFramesVar SetFramesVar;
IGI_HashReset HashReset, HashResetOut = nullptr;
IGI_StartLevelLbl StartLevelLbl;
bool GetThreadState(DWORD tid);
DWORD GetMainThreadId();
void StartThreadTimer(HANDLE, DWORD, DWORD);
DWORD exec_tid = 0;
//Global vars.
int timerID;
HookProc hookProc;
TimerProc hookTimer;
uint32_t hookTimeout;
string hookCaption;
HHOOK hHook;
int captionLen;


void DllCleanup() {
	//#ifdef _DEBUG
	if (GetConsole()->IsAllocated()) {
		GetConsole()->DeAllocate();
	}
	//#endif
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

BOOL WINAPI  DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
	BOOL result;

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		SetOurModuleHandle(hModule);
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

void CreateAllHooks() {
	auto mh_status = MH_CreateHookEx(ParseConfig, &ParseConfigDetour, &ParseConfigOut);

	if (mh_status == MH_OK) {
		LOG_INFO("ParseConfig Createhook success %p", mh_status);
	}
	else {
		LOG_ERROR("Minhook Createhook error : %s", MH_StatusToString(mh_status));
	}

	mh_status = MH_CreateHookEx(CreateConfig, &CreateConfigDetour, &CreateConfigOut);

	if (mh_status == MH_OK) {
		LOG_INFO("CreateConfig Createhook success %p", mh_status);
	}
	else {
		LOG_ERROR("Minhook Createhook error : %s", MH_StatusToString(mh_status));
	}

	mh_status = MH_CreateHookEx(ParseWeaponConfig, &ParseWeaponConfigDetour, &ParseWeaponConfigOut);
	if (mh_status == MH_OK) {
		LOG_INFO("ParseWeaponConfig Createhook success %p", mh_status);
	}
	else {
		LOG_ERROR("Minhook Createhook error : %s", MH_StatusToString(mh_status));
	}

	mh_status = MH_CreateHookEx(LoadLevelMenu, &LoadLevelMenuDetour, &LoadLevelMenuOut);
	if (mh_status == MH_OK) {
		LOG_INFO("LoadLevelMenu Createhook success %p", mh_status);
	}
	else {
		LOG_ERROR("LoadLevelMenu Createhook error : %s", MH_StatusToString(mh_status));
	}
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
	MenuManager = reinterpret_cast<IGI_MenuManager>(0x418B00);
	LevelStartInit = reinterpret_cast<IGI_LevelStartInit>(0x004012A0);
	LevelStartMain = reinterpret_cast<IGI_LevelStartMain>(0x00402070);
	LevelStartCaller = reinterpret_cast<IGI_LevelStartCaller>(0x00416900);
	SetFramesVar = reinterpret_cast<IGI_SetFramesVar>(0x00402820);
	StartLevelLbl = reinterpret_cast<IGI_StartLevelLbl>(0x004869B0);
	LevelLoad = (IGI_LevelLoad)0x4F0E10;
	LoadLevelMenu = (IGI_LoadLevelMenu)0x00416fe0;
}


DWORD WINAPI MainThread(LPVOID lpThreadParameter) {
	//#ifdef _DEBUG
	GetConsole()->Allocate();
	GetConsole()->Clear();
	//#endif

	LOG_INFO("[+]Dll Attached");
	LOG_INFO("Initializing Pointers...");
	InitPointers();
	LOG_INFO("Initializing Pointers DONE...");

	// Initialize MinHook.
	MH_STATUS mh_status = MH_Initialize();
	if (mh_status != MH_OK)
	{
		LOG_ERROR("Minhook init error : %p", MH_StatusToString(mh_status));
		return 1;
	}
	else
	{
		LOG_INFO("MH_Init success : %p", mh_status);
		CreateAllHooks();
	}


	//if (MH_CreateHookEx(HashInit), &HashInitDetour, reinterpret_cast<void**>(&HashInitOut)) == MH_OK) {
	//	GT_ShowInfo("HashInit Createhook success");
	//}

	//if (MH_CreateHookEx(HashReset), &HashResetDetour, reinterpret_cast<void**>(&HashResetOut)) == MH_OK) {
	//	GT_ShowInfo("HashReset Createhook success");
	//}

	//if (MH_CreateHookEx(UpdateQTask), &UpdateQTaskDetour, reinterpret_cast<void**>(&UpdateQTaskOut)) == MH_OK) {
	//	GT_ShowInfo("UpdateQTask Createhook success");
	//}

	//IGINativeCaller::g_IGINativeCaller = new IGINativeCaller();

	DllMainLoop();
	FreeLibraryAndExitThread((HMODULE)lpThreadParameter, 0);
}

void DllMainLoop() {
	char* resultCompile = NULL;
	BOOL enableInput = FALSE, bypassSymbol = FALSE, loadRes = FALSE;

	while (!GT_IsKeyPressed(VK_END)) {

		if (GT_IsKeyToggled(VK_F1)) {
			Invoke((int)HASH::CONFIG_PARSE, NATIVE_CONFIG_FILE);
			//NATIVE_CONFIG::CONFIG_PARSE(NATIVE_CONFIG_FILE);
		}

		else if (GT_IsKeyToggled(VK_F2)) {
			NATIVE_CONFIG::CONFIG_CREATE(NATIVE_CONFIG_FILE);
		}

		if (GT_IsKeyToggled(VK_F3)) {
			NATIVE_CONFIG::WEAPON_CONFIG_PARSE(NATIVE_WEAPON_CONFIG_FILE);
		}


		//if (GT_IsKeyPressed(VK_MENU) && GT_IsKeyToggled('Q')) {
		//	LOG_INFO("Quit game main loop");
		//	*(PINT)0x005c8de8 = 0;
		//}

		//else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('Q')) {
		//	//*(PINT)0x00936274 = 0;//Disable Warnings.

		//	HashInit('0');
		//	//UpdateQTask();
		//	//char menuSymbol1[] = "MenuManager_RequestScreen";
		//	//RemoveSymbol((int*)0x00A43E88,(int)menuSymbol1);

		//	char mainMenuSystem[] = "LOCAL:menusystem\\mainmenu.qsc";
		//	DWORD mainMenuPtr = *(PDWORD)(*(PDWORD)0x00567c8c + 0x20);
		//	MenuManager(mainMenuPtr, mainMenuSystem, '1', '1', 1);
		//	HashReset();

		//	//char buf[0x32] = { NULL };
		//	//wsprintf(buf, "iVar2 is 0x%X\n", iVar2);
		//	//GT_ShowInfo(buf);

		//	//AutoMsgBox::Show("");
		//	//StartLevelMain(12, true, false, '0');

		//	//LevelStartInit(*(PINT)0x00567C8C, *(PINT)0x0057b1a2, 0, *(PINT)0x004026A9);
		//	//SetFramesVar(0x1e);

		//}


		//else if (GT_IsKeyToggled(VK_F1)) {
		//	StartLevelMain((*(int*)0x00539560) + 1, true, false, '\x1');
		//	//StartLevelLbl((*(int*)0x005C8A48));
		//}

		//else if (GT_IsKeyToggled(VK_F2)) {
		//	*(PINT)0x00539560 += 1; //Game level.
		//	*(PINT)0x00936274 = 0;//Disable Warnings.
		//	//*(PINT)0x00936268 = 0; //Disable Errors.

		//	HashInit('\x1');
		//	UpdateQTask();
		//	LevelStartInit(*(PINT)0x00567C8C, *(PINT)0x0057b1a2, 0, *(PINT)0x004026A9);
		//	StartLevelLbl(*(int*)0x005C8A48);
		//	SetFramesVar(0x1e);
		//	HashReset();
		//}


		else if (GT_IsKeyToggled(VK_HOME)) {

			*(PINT)0x00936274 = 0;//Disable Warnings.
			//*((PINT)0x539560) = 13;
			//StartLevelLbl(*(int*)0x005C8A48);

			//HashInit('1');
			//UpdateQTask();

			LoadLevelMenu();
			//SetFramesVar(0x1e);
			*(INT*)(*((PINT)0x00567C8C) + 0x28) = 4;
			//HashReset();
		}

		else if (GT_IsKeyToggled(VK_PRIOR)) {
			if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
				LOG_ERROR("Error enabling hook");
			else
				LOG_WARNING("[+]Hooks enabled");
		}

		else if (GT_IsKeyToggled(VK_NEXT)) {
			if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
				LOG_ERROR("Error disabling hook");
			else
				LOG_WARNING("[-]Hooks disabled");
		}

		else if (GT_IsKeyToggled(VK_SNAPSHOT)) {

			auto tid = GetMainThreadId();
			LOG_INFO("Main Thread Id : %p", tid);
			static bool state_action = false;
			auto h_thread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, tid);
			LOG_INFO("Remote thread :%p", h_thread);

			//bool thread_state;
			if (h_thread) {
				SuspendThread(h_thread);
				LOG_WARNING("SuspendThread done");
				LevelLoadRestart();
				LOG_INFO("LevelLoadRestart done");
				StartThreadTimer(h_thread, 3, tid);
			}
			//if (WaitForSingleObject(h_thread, 0x7D0u) == WAIT_OBJECT_0) 
			DWORD wait_time = 0x7D0u;
			//WaitForSingleObject(h_thread, wait_time);
			//do
			//{
			//	thread_state = GetThreadState(tid);
			//	LOG_INFO("WaitingForSingleObject.....");

			//} while (thread_state);


		}

		//else if (GT_IsKeyToggled(VK_F10)) {

		//	//LevelLoadRestart();
		//	LevelStartPatch();
		//	//PVOID mainFiber = ConvertThreadToFiber(NULL);
		//	//LOG_INFO("mainFiber : %p", mainFiber);
		//	//PVOID createdFiber = CreateFiber(NULL, (LPFIBER_START_ROUTINE)LevelLoadRestart, NULL);
		//	//SwitchToFiber(createdFiber);
		//	//LOG_INFO("createdFiber : %p", createdFiber);

		//	/*auto h_thread = OpenThread(THREAD_ACCESS, FALSE, exec_tid);
		//	ResumeThread(h_thread);
		//	ExecuteThread(lpThreadParameter);
		//	CloseHandle(h_thread);*/



		//	//auto mh_status = DLL_FreezeThread(reinterpret_cast<void**>(LevelLoad), TRUE);
		//	//if (mh_status != MH_OK) {
		//	//	LOG_INFO("Error Freezing Threads : %p",MH_StatusToString(mh_status));
		//	//}
		//}

		//else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('H')) {
		//	HumanPlayer_LoadParameters();
		//	ShowStatusMsgBox("HumanPlayer Loaded");
		//}

		//else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('W')) {
		//	char configFile[] = "LOCAL:weapons/weaponconfig.qsc";
		//	ParseWeaponConfig(0, configFile);
		//	ShowStatusMsgBox("Weapon Config Parsed");
		//}


		//else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('E')) {
		//	enableInput = !enableInput;
		//	if (enableInput) {
		//		char enableStr[0x32] = { NULL };
		//		memcpy(enableStr, (LPVOID)0x00539608, 23);
		//		EnableInput((int*)enableStr);
		//		ShowStatusMsgBox("EnableInput Loaded");
		//	}
		//	else {
		//		char disableStr[0x32] = { NULL };
		//		memcpy(disableStr, (LPVOID)0x00539620, 24);
		//		DisableInput((int*)disableStr);
		//		ShowStatusMsgBox("DisableInput Loaded");
		//	}
		//}

		//else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('F')) {
		//	BypassSymbolCheck(TRUE);
		//	char gameFrequencyStr[0x32] = { NULL };
		//	memcpy(gameFrequencyStr, (LPVOID)0x00539670, 15);

		//	TaskTypeSet((int*)gameFrequencyStr, 60);
		//	ShowStatusMsgBox("Game Frequency Loaded");
		//	BypassSymbolCheck(FALSE);
		//}

		//else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('C')) {
		//	char aLocalConfigQs[] = "LOCAL:config.qsc";
		//	ParseConfig(aLocalConfigQs);
		//	ShowStatusMsgBox("Config Parsed");
		//}


		//else if (GT_IsKeyToggled(VK_F6)) {
		//	LOG_INFO("Level restarting");
		//	RestartLevel();
		//	HWND hwnd = GT_FindGameWindow("IGI");
		//	SetForegroundWindow(hwnd);
		//	LOG_INFO("Level restarted");
		//}

		Sleep(10);
	}
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
	Sleep(0x7D0u);
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
	Sleep(0x7D0u);
	return status;
}

void StartLevelMain(int level = 1, bool disableWarn = true, bool disableErr = false, char hashVar = '1') {
	int* game_level = (int*)0x00539560;
	*(PINT)game_level = level;

	*(PINT)0x00936274 = !disableWarn;//Disable Warnings.
	*(PINT)0x00936268 = !disableErr; //Disable Errors.

	HashInit(hashVar);
	UpdateQTask();
	StartLevel(*(PINT)0x0057BABC, 00000000, *(PINT)0x00C28C5C, *(PINT)0x049E7434);
	HashReset();
}

int __cdecl LevelLoadDetour(int levelAddr, int levelLen) {

	*(PINT)0x00936274 = 0;//Disable Warnings.
	//int gameLevel = *((PDWORD)0x539560);
	//HWND hwnd = GT_FindGameWindow("IGI");

	char buf[0x50] = { NULL };
	wsprintf(buf, "levelAddr is 0x%X levelLen: %d\n", levelAddr, levelLen);
	GT_ShowInfo(buf);

	HashInit('1');
	UpdateQTask();

	//SetForegroundWindow(hwnd);
	int ret = LevelLoadOut(levelAddr, levelLen, *(PINT)0x04A27C18, 0x1);
	HashReset();
	return ret;
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
	SetFramesVar(0x1e);
	*(INT*)(*((PINT)0x00567C8C) + 0x28) = 4;
	LOG_INFO("LoadLevelMenuDetour Done");
	//LoadLevelMenuOut();
}

void LevelLoadRestart() {
	*(PINT)0x00936274 = 0;//Disable Warnings.
	//HWND hwnd = GT_FindGameWindow("IGI");
	LOG_INFO("LevelLoadRestart");
	LPVOID aLevelPathAddr = (LPVOID)0x0057B568;

	//HashInit('1');
	//LOG_INFO("LevelLoadRestart : HashInit");
	UpdateQTask();
	LOG_INFO("LevelLoadRestart : UpdateQTask");
	LevelLoad((int)aLevelPathAddr, 0x1E, *(PINT)0x04A27C18, 0x1);
	LOG_INFO("LevelLoadRestart : LevelLoad");
	//HashReset();
	//LOG_INFO("LevelLoadRestart : HashReset");
	//SetForegroundWindow(hwnd);
}

//MH_STATUS DLL_FreezeThread(LPVOID pTarget, BOOL enable) {
//	MH_STATUS mh_status = (MH_STATUS)MH_OK;
//
//	//if (mh_status != MH_OK) return mh_status;
//
//	EnterSpinLock();
//
//	if (g_hHeap != NULL)
//	{
//		if (pTarget == MH_ALL_HOOKS)
//		{
//			mh_status = EnableAllHooksLL(enable);
//		}
//		else
//		{
//			UINT pos = FindHookEntry(pTarget);
//			LOG_INFO("FindHookEntry pos : %p", pos);
//
//			if (pos != INVALID_HOOK_POS)
//			{
//				LOG_INFO("FindHookEntry Valid pos : ");
//				if (g_hooks.pItems[pos].isEnabled != enable)
//				{
//					FROZEN_THREADS threads;
//					mh_status = Freeze(&threads, pos, ACTION_ENABLE);
//					if (mh_status == MH_OK)
//					{
//						LOG_INFO("Freeze thread at pos : %p", pos);
//						LevelLoadRestart();
//						Unfreeze(&threads);
//						LOG_INFO("Freeze MH_OK ");
//					}
//				}
//				else
//				{
//					mh_status = enable ? (MH_STATUS)MH_ERROR_ENABLED : (MH_STATUS)MH_ERROR_DISABLED;
//				}
//			}
//			else
//			{
//				mh_status = (MH_STATUS)MH_ERROR_NOT_CREATED;
//			}
//		}
//	}
//	else
//	{
//		mh_status = (MH_STATUS)MH_ERROR_NOT_INITIALIZED;
//	}
//
//	LeaveSpinLock();
//
//	return (MH_STATUS)mh_status;
//}

void LevelStartPatch() {
	*(PINT)0x00936274 = 0;//Disable Warnings.
	int gameLevel = *((PDWORD)0x539560);
	HWND hwnd = GT_FindGameWindow("IGI");
	LOG_INFO("LevelStartPatch Run");
	//SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	LPVOID aLevelPathAddr = (LPVOID)0x0057B568;

	//HashInit('1');
	UpdateQTask();
	LevelLoad((int)aLevelPathAddr, 0x1E, NULL, NULL);
	//HashReset();
	SetForegroundWindow(hwnd);
}


DWORD GetMainThreadId() {
	const std::shared_ptr<void> hThreadSnapshot(
		CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0), CloseHandle);
	if (hThreadSnapshot.get() == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("GetMainThreadId failed");
	}
	THREADENTRY32 tEntry;
	tEntry.dwSize = sizeof(THREADENTRY32);
	DWORD result = 0;
	DWORD currentPID = GetCurrentProcessId();
	for (BOOL success = Thread32First(hThreadSnapshot.get(), &tEntry);
		!result && success && GetLastError() != ERROR_NO_MORE_FILES;
		success = Thread32Next(hThreadSnapshot.get(), &tEntry))
	{
		if (tEntry.th32OwnerProcessID == currentPID) {
			result = tEntry.th32ThreadID;
		}
	}
	return result;
}

bool GetThreadState(DWORD tid) {
	cProcInfo i_Proc;
	DWORD u32_Error = i_Proc.Capture();

	if (u32_Error)
	{
		LOG_INFO("Error 0x%X capturing processes.%p", std::to_string(u32_Error));
		return false;
	}

	SYSTEM_PROCESS* pk_Proc = i_Proc.FindProcessByPid(GetCurrentProcessId());
	if (!pk_Proc)
	{
		LOG_INFO("The process does not exist.\n");
		return false;
	}

	SYSTEM_THREAD* pk_Thread = i_Proc.FindThreadByTid(pk_Proc, tid);
	if (!pk_Thread)
	{
		LOG_INFO("The thread does not exist.\n");
		return false;
	}

	BOOL b_Suspend;
	i_Proc.IsThreadSuspended(pk_Thread, &b_Suspend);
	LOG_INFO("GetThreadState Suspended : %d,", b_Suspend);
	return b_Suspend;
}

class Timer
{
	std::thread th;
	bool running = false;

public:
	typedef std::chrono::milliseconds Interval;
	typedef std::function<void(void)> Timeout;

	void start(const Interval& interval,
		const Timeout& timeout)
	{
		running = true;

		th = std::thread([=]()
			{
				while (running == true) {
					std::this_thread::sleep_for(interval);
					timeout();
				}
			});

		// [*]
		th.join();
	}

	void stop()
	{
		running = false;
	}
};

void StartThreadTimer(HANDLE h_thread, DWORD seconds, DWORD tid) {

	bool thread_state = GetThreadState(tid);
	LOG_INFO("StartThreadTimer called");
	Timer tHello;
	tHello.start(std::chrono::seconds(seconds), [&] {
		if (thread_state) {
			ResumeThread(h_thread);
			CloseHandle(h_thread);
			LOG_INFO("WaitForSingleObject done");
			thread_state = !thread_state;
		}
		else
		{
			tHello.stop();
		}
		});
	//if (!thread_state)
	//	tHello.stop();
}