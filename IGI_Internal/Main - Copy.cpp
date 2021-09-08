#pragma
#define GT_BUILD_DLL
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1
#define _CRT_SECURE_NO_DEPRECATE
#pragma comment(lib, "winmm.lib")
#define DLL_EXPORT __declspec( dllexport )

#include "GTLibc.c"
#include "dllmain.h"
#include "AutoCloseMessageBox.cpp"
#include "hook.c"
#include "ThreadsEx.cpp"
#include "General.h"
#include "Log.h"

typedef int(__cdecl* IGI_StatusTimer)();
typedef int(__cdecl* IGI_ParseWeaponConfig)(int index, char* cfgFile);
typedef int(__cdecl* IGI_HumanPlayer_LoadParameters)();
typedef int(__cdecl* IGI_EnableInput)(int* fileName);
typedef int(__cdecl* IGI_DisableInput)(int* fileName);
typedef int(__cdecl* IGI_Error)(char* err, ...);
typedef int(__cdecl* IGI_Warning)(char* warn, ...);
typedef char* (__cdecl* IGI_Compile)(char* fileName);
typedef int(__cdecl* IGI_CompileConfig)(int a1, char* qFile, int a3);
typedef int(__cdecl* IGI_ParseConfig)(char* qFile);
typedef int(__cdecl* IGI_Assemble)(char* fileIn, char* fileOut);
typedef int* (__cdecl* IGI_LoadQVM)(char* fileName);
typedef int(__cdecl* IGI_CompileQVM)(char* fileName);
typedef int(__cdecl* IGI_CompileCleanUp)(char* fileName);
typedef int(__cdecl* IGI_LevelLoad)(int levelAddr, int levelLen, int param3, int param4);
typedef int(__cdecl* IGI_LevelLoadParam)(char* fileName, int fileLen);
typedef int(__cdecl* IGI_Script_SetSymbolContext)(char symbolName, int symbolBuf);
typedef int(__cdecl* IGI_TaskTypeSet)(int* taskName, int taskParam);
typedef char(__cdecl* IGI_ScriptInit)(int a1, char a2, int a3, char a4);
typedef int(__cdecl* IGI_GraphOpen)(int level, char* graphStr);
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
typedef int(__cdecl* IGI_ResourceFlush)(int param);
typedef int(__cdecl* IGI_MenuManager)(int a1, char* a2, char a3, char a4, int a5);
typedef int(__cdecl* IGI_LevelStartCaller)(int param);
typedef void(__cdecl* IGI_SetFramesVar)(int frameCount);

typedef int(__cdecl* IGI_Sub_416690)();
typedef int(__cdecl* IGI_Sub_416660)();
typedef int(__cdecl* IGI_Sub_46B310)();
typedef int(__cdecl* IGI_Sub_4015F0)(int a1);


typedef void(__cdecl* IGI_LoadLevelMenu)();
typedef int(__cdecl* IGI_Sub_4028D0)();
typedef int(__cdecl* IGI_Sub_453270)();
typedef int(__cdecl* IGI_Sub_48A620)(DWORD* a1, int a2);
typedef int(__cdecl* IGI_Sub_4EDCC0)();
typedef int(__cdecl* IGI_Sub_4E7130)();
typedef int(__cdecl* IGI_Sub_4868D0)(int a1);
typedef int(__cdecl* IGI_Sub_416920)();
typedef int(__cdecl* IGI_Sub_4CE830)(DWORD a1, DWORD a2);
typedef int(__cdecl* IGI_Sub_4B0D50)();
typedef int(__cdecl* IGI_Sub_4028F0)();
typedef int(__cdecl* IGI_Sub_487180)();
typedef int(__cdecl* IGI_Sub_00401fC0)();
typedef int(__cdecl* IGI_Sub_48A5D0)(int a1);
typedef int(__cdecl* IGI_UpdateQTask)();
typedef int(__cdecl* IGI_StartLevelParam)(unsigned int a1, int a2, int a3);
typedef int* (__cdecl* IGI_LevelStartInit)(int param_1, short param_2, int param_3, int param_4);
typedef int(__cdecl* IGI_QuitLevelParam)(unsigned int a1, int a2, int a3);
typedef int(__cdecl* IGI_UpdateQTaskList)(int a1);
typedef int(__cdecl* IGI_StatusMsg)(int sendStatus, const char* buffer, const char* msgSprite, const char* v6);
typedef int(__cdecl* IGI_FUN_00401900)(int param_1, int param_2, short param_3, int param_4);
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
DWORD WINAPI ExecuteThread(LPVOID lpThreadParameter);
int StatusMsgBox(const char* statusMsg, const char* msgSprite, char msgType);
int BypassSymbolCheck(BOOL bypass);

//Global vars.
int timerID;
HookProc hookProc;
TimerProc hookTimer;
uint32_t hookTimeout;
string hookCaption;
HHOOK hHook;
int captionLen;


IGI_StatusTimer GetStatusTimer;
IGI_StatusMsg StatusMsg;
IGI_Error ShowError;
IGI_Warning ShowWarning;
IGI_Compile CompileResource;
IGI_CompileConfig CompileConfig;
IGI_Assemble AssembleQVM;
IGI_CompileQVM CompileQVM;
IGI_CompileCleanUp CompileCleanUp;
IGI_LevelLoad LevelLoad, LevelLoadOut = nullptr;
IGI_LevelLoadParam LevelLoadParam;
IGI_ScriptInit ScriptInit;
IGI_GraphOpen GraphOpen;
IGI_HumanPlayer_LoadParameters HumanPlayer_LoadParameters;
IGI_ParseWeaponConfig ParseWeaponConfig;
IGI_DisableInput DisableInput;
IGI_EnableInput EnableInput;
IGI_TaskTypeSet TaskTypeSet;
IGI_Script_SetSymbolContext Script_SetSymbolContext;
IGI_CreateConfig CreateConfig;
IGI_ParseConfig ParseConfig;
IGI_LoadResource LoadResourceFile;
IGI_UnLoadResource UnLoadResource;
IGI_RestartLevel RestartLevel;
IGI_RestartLevelParam RestartLevelParam;
IGI_UpdateQTaskList UpdateQTaskList;
IGI_HashInit HashInit, HashInitOut = nullptr;
IGI_RemoveSymbol RemoveSymbol;
IGI_Sub_4028D0 Sub_4028D0;
IGI_Sub_453270 Sub_453270;
IGI_Sub_48A620 Sub_48A620;
IGI_Sub_4EDCC0 Sub_4EDCC0;
IGI_Sub_4E7130 Sub_4E7130;
IGI_Sub_4868D0 Sub_4868D0;
IGI_Sub_416920 Sub_416920;
IGI_Sub_4CE830 Sub_4CE830;
IGI_Sub_4B0D50 Sub_4B0D50;
IGI_Sub_48A5D0 Sub_48A5D0;
IGI_Sub_4028F0 Sub_4028F0;
IGI_Sub_487180 Sub_487180;
IGI_QuitLevel QuitLevel;
IGI_UpdateQTask UpdateQTask, UpdateQTaskOut = nullptr;
IGI_StartLevel StartLevel;
IGI_StartLevelParam StartLevelParam;
IGI_QuitLevelParam QuitLevelParam;
IGI_ResourceFlush ResourceFlush;
IGI_MenuManager MenuManager;
IGI_Sub_416690 Sub_416690;
IGI_Sub_416660 Sub_416660;
IGI_Sub_46B310 Sub_46B310;
IGI_Sub_4015F0 Sub_4015F0;
IGI_Sub_00401fC0 Sub_00401fC0;
IGI_LoadLevelMenu LoadLevelMenu, LoadLevelMenuOut;
IGI_LoadQVM LoadQVM;
IGI_LevelStartInit LevelStartInit;
IGI_LevelStartMain LevelStartMain;
IGI_FUN_00401900 FUN_00401900;
IGI_LevelStartCaller LevelStartCaller;
IGI_SetFramesVar SetFramesVar;
IGI_HashReset HashReset, HashResetOut = nullptr;
IGI_StartLevelLbl StartLevelLbl;
bool GetThreadState(DWORD tid);
DWORD GetMainThreadId();
void StartThreadTimer(HANDLE, DWORD, DWORD);
DWORD exec_tid = 0;

void DllCleanup() {
	if (GetConsole()->IsAllocated()) {
		GetConsole()->DeAllocate();
	}
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
		LOG_DEBUG("[-]Dll Deattached");
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

DWORD WINAPI MainThread(LPVOID lpThreadParameter) {
#ifdef _DEBUG
	GetConsole()->Allocate();
	GetConsole()->Clear();
#endif // DEBUG

	LOG_DEBUG("Initializing...");

	GetStatusTimer = (IGI_StatusTimer)0x4F0EE0;
	StatusMsg = (IGI_StatusMsg)0x485970;
	ShowWarning = (IGI_Warning)0x4AF810;
	ShowError = (IGI_Error)0x4AF7B0;
	CompileResource = (IGI_Compile)0x4B80B0;
	CompileConfig = (IGI_CompileConfig)0x4F0CF0;
	CompileQVM = (IGI_CompileQVM)0x4B85B0;
	AssembleQVM = (IGI_Assemble)0x4BB270;
	CompileCleanUp = (IGI_CompileCleanUp)0x4B83D0;
	LevelLoadParam = reinterpret_cast<IGI_LevelLoadParam>(0x4F0D40);
	//LevelLoad = reinterpret_cast<IGI_LevelLoad>(0x0041703E);
	LevelLoad = (IGI_LevelLoad)0x4F0E10;
	ScriptInit = (IGI_ScriptInit)0x4F0E50;
	GraphOpen = (IGI_GraphOpen)0x4F9FF0;
	HumanPlayer_LoadParameters = (IGI_HumanPlayer_LoadParameters)0x4137A0;
	ParseWeaponConfig = (IGI_ParseWeaponConfig)0x4071E0;
	EnableInput = (IGI_EnableInput)0x415830;
	DisableInput = (IGI_DisableInput)0x4157D0;
	TaskTypeSet = (IGI_TaskTypeSet)0x4B8810;
	Script_SetSymbolContext = (IGI_Script_SetSymbolContext)0x4B8930;
	CreateConfig = (IGI_CreateConfig)0x405BD0;
	LoadResourceFile = (IGI_LoadResource)0x4B5F00;
	UnLoadResource = (IGI_UnLoadResource)0x4B6380;
	RestartLevel = (IGI_RestartLevel)0x416FE0;
	RestartLevelParam = (IGI_RestartLevelParam)0x48A440;
	UpdateQTaskList = (IGI_UpdateQTaskList)0x401B20;
	HashInit = (IGI_HashInit)0x4B0D60;
	HashReset = (IGI_HashReset)0x004B0D90;
	RemoveSymbol = (IGI_RemoveSymbol)0x4C0460;
	StartLevel = (IGI_StartLevel)0x415B30;
	StartLevelParam = (IGI_StartLevelParam)0x401400;
	QuitLevelParam = (IGI_QuitLevelParam)0x401530;
	ParseConfig = (IGI_ParseConfig)0x405850;

	Sub_4028D0 = (IGI_Sub_4028D0)0x4028D0;
	Sub_453270 = (IGI_Sub_453270)0x453270;
	Sub_48A620 = (IGI_Sub_48A620)0x48A620;
	Sub_4EDCC0 = (IGI_Sub_4EDCC0)0x4EDCC0;
	Sub_4E7130 = (IGI_Sub_4E7130)0x4E7130;
	Sub_4868D0 = (IGI_Sub_4868D0)0x4868D0;
	UpdateQTask = (IGI_UpdateQTask)0x4F0E90;
	Sub_416920 = (IGI_Sub_416920)0x416920;
	Sub_4CE830 = (IGI_Sub_4CE830)0x4CE830;
	Sub_4B0D50 = (IGI_Sub_4B0D50)0x4B0D50;
	Sub_48A5D0 = (IGI_Sub_48A5D0)0x48A5D0;
	Sub_4028F0 = (IGI_Sub_4028F0)0x4028F0;
	Sub_487180 = (IGI_Sub_487180)0x487180;
	ResourceFlush = (IGI_ResourceFlush)0x4B63D0;
	Sub_416690 = (IGI_Sub_416690)0x416690;
	Sub_416660 = (IGI_Sub_416660)0x416660;
	Sub_46B310 = (IGI_Sub_46B310)0x46B310;
	Sub_4015F0 = (IGI_Sub_4015F0)0x4015F0;
	QuitLevel = (IGI_QuitLevel)0x416550;
	MenuManager = (IGI_MenuManager)0x418B00;
	LoadLevelMenu = (IGI_LoadLevelMenu)0x00416fe0;
	Sub_00401fC0 = (IGI_Sub_00401fC0)0x00401fC0;
	LoadQVM = (IGI_LoadQVM)0x004b80b0;
	LevelStartInit = (IGI_LevelStartInit)0x004012A0;
	LevelStartMain = (IGI_LevelStartMain)0x00402070;
	FUN_00401900 = (IGI_FUN_00401900)0x00401900;
	LevelStartCaller = (IGI_LevelStartCaller)0x00416900;
	SetFramesVar = (IGI_SetFramesVar)0x00402820;
	StartLevelLbl = (IGI_StartLevelLbl)0x004869B0;

	LOG_DEBUG("[+]Dll Attached");

	// Initialize MinHook.
	MH_STATUS mh_status = MH_Initialize();
	if (mh_status != MH_OK)
	{
		LOG_ERROR("Minhook init error : %p", MH_StatusToString(mh_status));
		return 1;
	}
	else
	{
		LOG_DEBUG("MH_Init success : %p", mh_status);
	}


	if (MH_CreateHook(reinterpret_cast<void**>(LoadLevelMenu), &LevelLoadRestart, reinterpret_cast<void**>(&LoadLevelMenuOut)) == MH_OK) {
		LOG_DEBUG("LoadLevelMenu Createhook success %p", mh_status);
	}

	//if (MH_CreateHook(reinterpret_cast<void**>(LevelLoad), &LevelLoadDetour, reinterpret_cast<void**>(&LevelLoadOut)) == MH_OK) {
	//	std::cout << "["  << GT_GetCurrentTime() << "] %p","LevelLoad Createhook success");
	//}

	//if (MH_CreateHook(reinterpret_cast<void**>(HashInit), &HashInitDetour, reinterpret_cast<void**>(&HashInitOut)) == MH_OK) {
	//	GT_ShowInfo("HashInit Createhook success");
	//}

	//if (MH_CreateHook(reinterpret_cast<void**>(HashReset), &HashResetDetour, reinterpret_cast<void**>(&HashResetOut)) == MH_OK) {
	//	GT_ShowInfo("HashReset Createhook success");
	//}

	//if (MH_CreateHook(reinterpret_cast<void**>(UpdateQTask), &UpdateQTaskDetour, reinterpret_cast<void**>(&UpdateQTaskOut)) == MH_OK) {
	//	GT_ShowInfo("UpdateQTask Createhook success");
	//}

	DllMainLoop();
	FreeLibraryAndExitThread((HMODULE)lpThreadParameter, 0);
}

void DllMainLoop() {
	char* resultCompile = NULL;
	BOOL enableInput = FALSE, bypassSymbol = FALSE, loadRes = FALSE;

	while (!GT_IsKeyPressed(VK_END)) {

		if (GT_IsKeyPressed(VK_KANA)) {

			char levelName[0x32] = { NULL };
			LPVOID aLevelPathAddr = (LPVOID)0x0057B568;
			LPVOID aObjResourceAddr = (LPVOID)0x0054E090;

			wsprintfA(levelName, (char*)aObjResourceAddr, aLevelPathAddr);

			char aTaskNew[10] = { NULL };
			memcpy(aTaskNew, (LPVOID)0x00539ABC, 9);
			int scriptBuf = *(PINT)(0xA758A4);

			AutoCloseMessageBox::Show("IGI", 750);
			int* v2 = (int*)&*(PDWORD)(0xA71890);
			do
			{
				if (*v2)
				{
					UpdateQTaskList(*v2);
					//AutoCloseMessageBox::Show("QTask", 50);
					*v2 = 0;
				}
				++v2;
			} while ((signed int)v2 < (signed int)&*(PDWORD)(0xA75890));

			AutoCloseMessageBox::Show("IGI", 1500);
			Script_SetSymbolContext((unsigned int)aTaskNew, scriptBuf);
			AutoCloseMessageBox::Show("IGI", 750);

			char* result = CompileResource(levelName);
			if (*result) {
				AutoCloseMessageBox::Show(result, 750);

				ScriptInit((int)levelName, 1, 0, 0);
				AutoCloseMessageBox::Show("ScriptInit success");

				/*LPVOID hashArrAddr = (LPVOID)0x9422E8;
				memset(hashArrAddr, 0, 0x1Cu);*/

				if (!*((PDWORD)0x9422FD))
					*((PDWORD)0x9422FD) = 1;//Hash init

				int qResult = CompileQVM(result);
				AutoCloseMessageBox::Show("CompileQVM success");
				char buf[0x32] = { NULL };
				wsprintf(buf, "Result is %d\n", qResult);
				AutoCloseMessageBox::Show(buf);

				CompileCleanUp(levelName);
				AutoCloseMessageBox::Show("CompileCleanUp success");

				*(PDWORD)(0xA70C98) = *(PDWORD)(0xA758A8) = 0;

			}
		}

		else if (GT_IsKeyPressed(VK_MENU) && GT_IsKeyToggled('Q')) {
			*(PINT)0x005c8de8 = 0;
		}

		else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('Q')) {
			//*(PINT)0x00936274 = 0;//Disable Warnings.

			HashInit('0');
			//UpdateQTask();
			//char menuSymbol1[] = "MenuManager_RequestScreen";
			//RemoveSymbol((int*)0x00A43E88,(int)menuSymbol1);

			char mainMenuSystem[] = "LOCAL:menusystem\\mainmenu.qsc";
			DWORD mainMenuPtr = *(PDWORD)(*(PDWORD)0x00567c8c + 0x20);
			MenuManager(mainMenuPtr, mainMenuSystem, '1', '1', 1);
			HashReset();

			//char buf[0x32] = { NULL };
			//wsprintf(buf, "iVar2 is 0x%X\n", iVar2);
			//GT_ShowInfo(buf);

			//AutoCloseMessageBox::Show("");
			//StartLevelMain(12, true, false, '0');

			//LevelStartInit(*(PINT)0x00567C8C, *(PINT)0x0057b1a2, 0, *(PINT)0x004026A9);
			//SetFramesVar(0x1e);

		}

		else if (GT_IsKeyToggled(VK_F9)) {
			HashInit('1');
			UpdateQTask();
			HashReset();
		}

		else if (GT_IsKeyToggled(VK_F1)) {
			StartLevelMain((*(int*)0x00539560) + 1, true, false, '\x1');
			//StartLevelLbl((*(int*)0x005C8A48));
		}

		else if (GT_IsKeyToggled(VK_F2)) {
			*(PINT)0x00539560 += 1; //Game level.
			*(PINT)0x00936274 = 0;//Disable Warnings.
			//*(PINT)0x00936268 = 0; //Disable Errors.

			HashInit('\x1');
			UpdateQTask();
			LevelStartInit(*(PINT)0x00567C8C, *(PINT)0x0057b1a2, 0, *(PINT)0x004026A9);
			StartLevelLbl(*(int*)0x005C8A48);
			SetFramesVar(0x1e);
			HashReset();
		}

		else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('L')) {
			if (!*((PDWORD)0x9422FD))
				*((PDWORD)0x9422FD) = 1;
			HashInit('1');
			UpdateQTask();
			HashReset();
			char symbolAk[] = "WEAPON_ID_AK47";
			int* local_604 = (int*)0x00a43e88;
			//RemoveSymbol(&*(PDWORD)0x00a43e88,(int)symbolAk);
			//RemoveSymbol(local_604, (int)symbolAk);
			BypassSymbolCheck(TRUE);
			short uVar1;
			int uVar2;

			uVar2 = 0;
			uVar1 = *(PSHORT)0x0054f928;
			uVar2 = FUN_00401900((int)&*(PDWORD)00536274, 0x54, uVar1, uVar2);
			*(PINT)0x00567C7C = (short)uVar2;
			LevelStartMain(0x006D0072);
		}

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
			if (MH_EnableHook(reinterpret_cast<void**>(LoadLevelMenu)) != MH_OK)
				LOG_ERROR("Error enabling hook");
			else
				LOG_DEBUG("[+]Hooks enabled");
		}

		else if (GT_IsKeyToggled(VK_NEXT)) {
			if (MH_DisableHook(reinterpret_cast<void**>(LoadLevelMenu)) != MH_OK)
				LOG_ERROR("Error disabling hook");
			else
				LOG_DEBUG("[-]Hooks disabled");
		}

		else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('P')) {


			//StartLevelParam(*((PDWORD)0x57B1A2), 0, (int)StartLevel);
			LoadLevelMenu();
			AutoCloseMessageBox::Show("Start", 1500);

			AutoCloseMessageBox::Show("");
			char levelName[0x32] = { NULL };
			LPVOID aLevelPathAddr = (LPVOID)0x0057B568;
			LPVOID aObjResourceAddr = (LPVOID)0x0054E090;

			wsprintfA(levelName, (char*)aObjResourceAddr, aLevelPathAddr);
			GT_ShowInfo(levelName);

			if (!*((PDWORD)0x9422FD))
				*((PDWORD)0x9422FD) = 1;

			UpdateQTask();

			char aTaskNew[10] = { NULL };
			memcpy(aTaskNew, (LPVOID)0x00539ABC, 9);
			int scriptBuf = *(PINT)(0xA758A4);

			UpdateQTask();
			AutoCloseMessageBox::Show("UpdateQTask success");
			Script_SetSymbolContext((unsigned int)aTaskNew, scriptBuf);


			char* qvmFile = (char*)LoadQVM(levelName);
			GT_ShowInfo(qvmFile);
			if (qvmFile != NULL) {
				CompileQVM(qvmFile);
				CompileCleanUp(qvmFile);
			}
		}

		else if (GT_IsKeyPressed(VK_F3)) {
			//AutoCloseMessageBox::Show("QuitLevel", 2500);
			//QuitLevel(0x0ABF849C);
			if (!*((PDWORD)0x9422FD))
				*((PDWORD)0x9422FD) = 1;

			AutoCloseMessageBox::Show("Sub_4E7130", 250);
			Sub_4E7130();
			AutoCloseMessageBox::Show("UpdateQTask", 250);
			UpdateQTask();
			AutoCloseMessageBox::Show("", 100);
			int qTaskList = *(DWORD*)(*((PDWORD)0x57BABC) + 84);
			if (qTaskList)
				UpdateQTaskList(qTaskList);
			AutoCloseMessageBox::Show("QuitLevelParam", 250);
			QuitLevelParam(*((PDWORD)0x57B1A2), 2, (int)QuitLevel);

			if (*(DWORD*)(0x57BABC + 72))
				Sub_4015F0(*(DWORD*)(0x57BABC + 72));
			AutoCloseMessageBox::Show("Sub_4015F0", 250);
			if (*(DWORD*)(0x57BABC + 68))
				Sub_4015F0(*(DWORD*)(0x57BABC + 68));
			AutoCloseMessageBox::Show("Sub_4015F0", 250);
			Sub_416690();
			AutoCloseMessageBox::Show("Sub_416690", 250);
			//if (*(DWORD*)(0x57BABC + 232))
				//ResourceFlush(*(DWORD*)(0x57BABC + 232));
			AutoCloseMessageBox::Show("ResourceFlush", 750);
			Sub_416660();
			AutoCloseMessageBox::Show("Sub_416660", 250);
			//if (*(DWORD*)(0x57BABC + 236))
				//ResourceFlush(*(DWORD*)(0x57BABC + 236));
			AutoCloseMessageBox::Show("QUIT-LVL", 1000);

			*(PINT)0x00936274 = 0;//Disable Warnings.
			HashInit('0');
			UpdateQTask();

			//AutoCloseMessageBox::Show("QUIT-", 1000);
			//QuitLevel(0x0ABF849C);
			//Sub_4015F0(*(DWORD*)(0x57BABC + 72));
			//AutoCloseMessageBox::Show("QuitLevel", 1500);

			//StartLevel(0x0ABF849C);
			//StartLevelParam(*((PDWORD)0x57B1A2), 0, (int)StartLevel);
			//AutoCloseMessageBox::Show("Start", 1500);

			//Menu Manager.
			char aLocalMenusystem[30] = { NULL };
			LPVOID menuResourceAddr = (LPVOID)0x00536308;

			wsprintfA(aLocalMenusystem, (char*)menuResourceAddr);
			AutoCloseMessageBox::Show(aLocalMenusystem, 1500);
			//char pcVar8[] = "LOCAL:menusystem\\mainmenu.qsc";
			DWORD iVar2 = *(PDWORD)(*(PDWORD)0x567C8C + 0x20);
			MenuManager(iVar2, aLocalMenusystem, '1', '1', 1);

			//int v33 = *(DWORD*)(0x567C8C + 32);
			//MenuManager(v33, (char*)"LOCAL:menusystem\\mainmenu.qsc", 1, 1, 1);
			//AutoCloseMessageBox::Show("MenuSystem", 1500);
		}

		else if (GT_IsKeyToggled(VK_SNAPSHOT)) {

			auto tid = GetMainThreadId();
			LOG_DEBUG("Main Thread Id : %p", tid);
			static bool state_action = false;
			auto h_thread = OpenThread(THREAD_ACCESS, FALSE, tid);
			LOG_DEBUG("Remote thread :%p", h_thread);

			//bool thread_state;

			SuspendThread(h_thread);
			LOG_WARNING("SuspendThread done");
			LevelLoadRestart();
			LOG_DEBUG("LevelLoadRestart done");
			StartThreadTimer(h_thread, 3, tid);
			//if (WaitForSingleObject(h_thread, 0x7D0u) == WAIT_OBJECT_0) 
			DWORD wait_time = 0x7D0u;
			//WaitForSingleObject(h_thread, wait_time);
			//do
			//{
			//	thread_state = GetThreadState(tid);
			//	LOG_DEBUG("WaitingForSingleObject.....");

			//} while (thread_state);


		}

		else if (GT_IsKeyToggled(VK_F10)) {

			//LevelLoadRestart();
			LevelStartPatch();
			//PVOID mainFiber = ConvertThreadToFiber(NULL);
			//LOG_DEBUG("mainFiber : %p", mainFiber);
			//PVOID createdFiber = CreateFiber(NULL, (LPFIBER_START_ROUTINE)LevelLoadRestart, NULL);
			//SwitchToFiber(createdFiber);
			//LOG_DEBUG("createdFiber : %p", createdFiber);

			/*auto h_thread = OpenThread(THREAD_ACCESS, FALSE, exec_tid);
			ResumeThread(h_thread);
			ExecuteThread(lpThreadParameter);
			CloseHandle(h_thread);*/



			//auto mh_status = DLL_FreezeThread(reinterpret_cast<void**>(LevelLoad), TRUE);
			//if (mh_status != MH_OK) {
			//	LOG_DEBUG("Error Freezing Threads : %p",MH_StatusToString(mh_status));
			//}
		}

		else if (GT_IsKeyPressed(VK_KANJI)) {
			LPVOID aLevelPathAddr = (LPVOID)0x0057B568;
			DWORD* v0;
			if (!*((PDWORD)0x9422FD))
				*((PDWORD)0x9422FD) = 1;//Hash init

			v0 = (DWORD*)RestartLevelParam(5);
			//*((PDWORD)0x5BDD6C) = 0;
			Sub_48A620(v0, 1);
			UpdateQTask();
			Sub_48A620(v0, 1);
			//LevelLoad((int)aLevelPathAddr, 30);
		}

		else if (GT_IsKeyPressed(VK_KANA)) {
			LPVOID aLevelPathAddr = (LPVOID)0x0057B568;

			DWORD* v0; // esi
			int v1 = *((PDWORD)0x5C8A48); // eax
			signed int v2; // edi
			int v3; // eax

			ShowStatusMsgBox("Run");
			Sub_4028D0();
			*((PDWORD)0x9422FD) = 1;//Hash init
			//v0 = (DWORD*)RestartLevelParam(5);
			Sub_4E7130();
			AutoCloseMessageBox::Show("");
			Sub_453270();
			AutoCloseMessageBox::Show("");
			//Sub_48A620(v0, 1);
			v1 = Sub_487180();
			//Sub_4868D0(*(DWORD*)(v1 + 1252));
			*((PDWORD)0x5BDD6C) = 0;
			//Sub_48A620(v0, 1);
			UpdateQTask();
			AutoCloseMessageBox::Show("");
			//Sub_48A620(v0, 1);
			Sub_4EDCC0();
			AutoCloseMessageBox::Show("");
			//LevelLoad((int)aLevelPathAddr, 30);
			//Sub_48A620(v0, 1);
			v2 = 0;
			do
			{
				v3 = Sub_416920();
				Sub_4CE830(v3, v2++);

			} while (v2 < 6);
			Sub_4B0D50();
			//Sub_48A620(v0, 1);
			//Sub_48A5D0((int)v0);
			Sub_4028F0();
			ShowStatusMsgBox("Level Loaded");
			Sleep(5000);
		}

		else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('H')) {
			HumanPlayer_LoadParameters();
			ShowStatusMsgBox("HumanPlayer Loaded");
		}

		else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('W')) {
			char configFile[] = "LOCAL:weapons/weaponconfig.qsc";
			ParseWeaponConfig(0, configFile);
			ShowStatusMsgBox("Weapon Config Parsed");
		}

		else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('P')) {
			char configFile[] = "LOCAL:config.qsc";
			CreateConfig(configFile);
			ShowStatusMsgBox("Cofing created");
		}


		else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('E')) {
			enableInput = !enableInput;
			if (enableInput) {
				char enableStr[0x32] = { NULL };
				memcpy(enableStr, (LPVOID)0x00539608, 23);
				EnableInput((int*)enableStr);
				ShowStatusMsgBox("EnableInput Loaded");
			}
			else {
				char disableStr[0x32] = { NULL };
				memcpy(disableStr, (LPVOID)0x00539620, 24);
				DisableInput((int*)disableStr);
				ShowStatusMsgBox("DisableInput Loaded");
			}
		}

		else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('F')) {
			BypassSymbolCheck(TRUE);
			char gameFrequencyStr[0x32] = { NULL };
			memcpy(gameFrequencyStr, (LPVOID)0x00539670, 15);

			TaskTypeSet((int*)gameFrequencyStr, 120);
			ShowStatusMsgBox("Game Frequency Loaded");
			BypassSymbolCheck(FALSE);
		}

		else if (GT_IsKeyPressed(VK_CONTROL) && GT_IsKeyToggled('C')) {
			char aLocalConfigQs[] = "LOCAL:config.qsc";
			ParseConfig(aLocalConfigQs);
			ShowStatusMsgBox("Config Parsed");
		}

		else if (GT_IsKeyToggled(VK_F11))
		{
			char levelName[0x32] = { NULL };
			LPVOID aLevelPathAddr = (LPVOID)0x0057B568;
			LPVOID aObjResourceAddr = (LPVOID)0x0054E090;

			wsprintfA(levelName, (char*)aObjResourceAddr, aLevelPathAddr);
			GT_ShowInfo(levelName);
			int qResult = AssembleQVM(levelName, levelName);
			AutoCloseMessageBox::Show("AssembleQVM success");
			char buf[0x32] = { NULL };
			wsprintf(buf, "Result is %d\n", qResult);
			AutoCloseMessageBox::Show(buf);
		}

		else if (GT_IsKeyToggled(VK_F12))
		{
			char levelName[0x32] = { NULL };
			LPVOID aLevelPathAddr = (LPVOID)0x0057B568;
			LPVOID aObjResourceAddr = (LPVOID)0x0054E090;
			wsprintfA(levelName, (char*)aObjResourceAddr, aLevelPathAddr);
			GT_ShowInfo(levelName);

			char aTaskNew[10] = { NULL };
			memcpy(aTaskNew, (LPVOID)0x00539ABC, 9);
			int scriptBuf = *(PINT)(0xA758A4);

			UpdateQTask();
			AutoCloseMessageBox::Show("UpdateQTask success");
			Script_SetSymbolContext((unsigned int)aTaskNew, scriptBuf);

			ScriptInit((int)levelName, 1, 0, 0);
			AutoCloseMessageBox::Show("ScriptInit success");

			//char* qResult = CompileResource(levelName);
			char qResult[] = "LOCAL:missions/location0/level1//objects.qvm";
			//char qResultStr[45] = { NULL };
			//strcpy_s(qResultStr, qResult);
			AutoCloseMessageBox::Show("CompileResource success");
			AutoCloseMessageBox::Show(qResult, 3000);

			int result = CompileQVM(qResult);
			if (result) {
				AutoCloseMessageBox::Show("CompileQVM success", 2500);
				char buf[0x64] = { NULL };
				wsprintf(buf, "Result is %d\n", result);
				AutoCloseMessageBox::Show(buf, 5000);
			}
			else
				GT_ShowError("Error with CompileQVM");
		}

		else if (GT_IsKeyToggled(VK_F5))
		{
			char resFile[0x32] = { NULL };
			LPVOID aLevelPathAddr = (LPVOID)0x0057B568;
			LPVOID aObjResourceAddr = (LPVOID)0x0054E090;
			int v5;
			wsprintfA(resFile, (char*)aObjResourceAddr, aLevelPathAddr);
			ShowStatusMsgBox(resFile);
			loadRes = !loadRes;

			if (loadRes) {
				char* resResult = LoadResourceFile(resFile, (int)&v5);
				ShowStatusMsgBox(resResult);
			}
			else {
				int resLen = lstrlen(resFile);
				resFile[resLen - 3] = 'q';
				resFile[resLen - 2] = 'v';
				resFile[resLen - 1] = 'm';
				ShowStatusMsgBox(resFile);
				int result = CompileQVM(resFile);
				if (result) {
					ShowStatusMsgBox("CompileQVM success");
					char buf[0x32] = { NULL };
					wsprintf(buf, "Result is %d\n", result);
					ShowStatusMsgBox(buf);
				}
				else
					GT_ShowError("Error with CompileQVM");

				ShowStatusMsgBox(resFile);
				int resResult = UnLoadResource(resFile);
				char buf[0x32] = { NULL };
				wsprintf(buf, "Result is %d\n", resResult);
				ShowStatusMsgBox(buf);
			}
		}

		else if (GT_IsKeyToggled(VK_F6)) {
			ShowStatusMsgBox("Level restart");
			RestartLevel();
			HWND hwnd = GT_FindGameWindow("IGI");
			SetForegroundWindow(hwnd);
			ShowStatusMsgBox("Level restart");
		}

		else if (GT_IsKeyToggled(VK_F7)) {

			char levelName[0x32] = { NULL };
			LPVOID aLevelPathAddr = (LPVOID)0x0057B568;
			LPVOID aObjResourceAddr = (LPVOID)0x0054E090;
			wsprintfA(levelName, (char*)aObjResourceAddr, aLevelPathAddr);
			GT_ShowInfo(levelName);
			char* v0; // eax
			char* v1; // esi
			AutoCloseMessageBox::Show("UpdateQTask", 1500);
			UpdateQTask();
			AutoCloseMessageBox::Show("UpdateQTask success", 2500);
			v0 = CompileResource(levelName);
			v1 = v0;
			if (!v0)
			{
				GT_ShowError("Error in Compiling");
			}
			AutoCloseMessageBox::Show("CompileQVM Init");
			int v2 = CompileQVM(v0);
			if (!v2)
				AutoCloseMessageBox::Show("CompileQVM Success");
			else
				AutoCloseMessageBox::Show("CompileQVM Failed");
			int v3 = CompileCleanUp(v1);
			(v3) ? AutoCloseMessageBox::Show("DONE", 5000) : AutoCloseMessageBox::Show("FAILED", 5000);

		}

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
	wsprintf(buf, "levelAddr is %p levelLen: %d\n", levelAddr, levelLen);
	AutoCloseMessageBox::Show(buf);

	HashInit('1');
	UpdateQTask();

	//SetForegroundWindow(hwnd);
	int ret = LevelLoadOut(levelAddr, levelLen, *(PINT)0x04A27C18, 0x1);
	HashReset();
	return ret;
}

int __cdecl HashInitDetour(char hashVal) {
	//AutoCloseMessageBox::Show("HashInitDetour",1);
	return HashInitOut(hashVal);
}

void __cdecl HashResetDetour() {
	//AutoCloseMessageBox::Show("HashResetDetour",1);
	HashResetOut();
}

void __cdecl UpdateQTaskDetour() {
	//AutoCloseMessageBox::Show("UpdateQTaskDetour",1);
	UpdateQTaskOut();
}

void __cdecl LoadLevelMenuDetour() {

	LPVOID aLevelPathAddr = (LPVOID)0x0057B568;
	LOG_DEBUG("LoadLevelMenuDetour ");

	HashInit('1');
	UpdateQTask();
	LevelLoad((int)aLevelPathAddr, 0x1E, *(PINT)0x04A27C18, 0x1);
	SetFramesVar(0x1e);
	*(INT*)(*((PINT)0x00567C8C) + 0x28) = 4;
	LOG_DEBUG("LoadLevelMenuDetour Done");
	//LoadLevelMenuOut();
}

void LevelLoadRestart() {
	*(PINT)0x00936274 = 0;//Disable Warnings.
	HWND hwnd = GT_FindGameWindow("IGI");
	LOG_DEBUG("LevelLoadRestart");
	LPVOID aLevelPathAddr = (LPVOID)0x0057B568;

	//HashInit('1');
	//LOG_DEBUG("LevelLoadRestart : HashInit");
	UpdateQTask();
	LOG_DEBUG("LevelLoadRestart : UpdateQTask");
	LevelLoad((int)aLevelPathAddr, 0x1E, *(PINT)0x04A27C18, 0x1);
	LOG_DEBUG("LevelLoadRestart : LevelLoad");
	//HashReset();
	//LOG_DEBUG("LevelLoadRestart : HashReset");
	SetForegroundWindow(hwnd);
}

MH_STATUS DLL_FreezeThread(LPVOID pTarget, BOOL enable) {
	MH_STATUS mh_status = (MH_STATUS)MH_OK;

	//if (mh_status != MH_OK) return mh_status;

	EnterSpinLock();

	if (g_hHeap != NULL)
	{
		if (pTarget == MH_ALL_HOOKS)
		{
			mh_status = EnableAllHooksLL(enable);
		}
		else
		{
			UINT pos = FindHookEntry(pTarget);
			LOG_DEBUG("FindHookEntry pos : %p", pos);

			if (pos != INVALID_HOOK_POS)
			{
				LOG_DEBUG("FindHookEntry Valid pos : ");
				if (g_hooks.pItems[pos].isEnabled != enable)
				{
					FROZEN_THREADS threads;
					mh_status = Freeze(&threads, pos, ACTION_ENABLE);
					if (mh_status == MH_OK)
					{
						LOG_DEBUG("Freeze thread at pos : %p", pos);
						LevelLoadRestart();
						Unfreeze(&threads);
						LOG_DEBUG("Freeze MH_OK ");
					}
				}
				else
				{
					mh_status = enable ? (MH_STATUS)MH_ERROR_ENABLED : (MH_STATUS)MH_ERROR_DISABLED;
				}
			}
			else
			{
				mh_status = (MH_STATUS)MH_ERROR_NOT_CREATED;
			}
		}
	}
	else
	{
		mh_status = (MH_STATUS)MH_ERROR_NOT_INITIALIZED;
	}

	LeaveSpinLock();

	return (MH_STATUS)mh_status;
}

void LevelStartPatch() {
	*(PINT)0x00936274 = 0;//Disable Warnings.
	int gameLevel = *((PDWORD)0x539560);
	HWND hwnd = GT_FindGameWindow("IGI");
	LOG_DEBUG("LevelStartPatch Run");
	//SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	LPVOID aLevelPathAddr = (LPVOID)0x0057B568;

	//HashInit('1');
	UpdateQTask();
	LevelLoad((int)aLevelPathAddr, 0x1E, NULL, NULL);
	//HashReset();
	SetForegroundWindow(hwnd);
}


DWORD GetMainThreadId() {
	const std::tr1::shared_ptr<void> hThreadSnapshot(
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
		LOG_DEBUG("Error 0x%X capturing processes.%p", std::to_string(u32_Error));
		return false;
	}

	SYSTEM_PROCESS* pk_Proc = i_Proc.FindProcessByPid(GetCurrentProcessId());
	if (!pk_Proc)
	{
		LOG_DEBUG("The process does not exist.\n");
		return false;
	}

	SYSTEM_THREAD* pk_Thread = i_Proc.FindThreadByTid(pk_Proc, tid);
	if (!pk_Thread)
	{
		LOG_DEBUG("The thread does not exist.\n");
		return false;
	}

	BOOL b_Suspend;
	i_Proc.IsThreadSuspended(pk_Thread, &b_Suspend);
	LOG_DEBUG("GetThreadState Suspended : %d,", b_Suspend);
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
	LOG_DEBUG("StartThreadTimer called");
	Timer tHello;
	tHello.start(std::chrono::seconds(seconds), [&] {
		if (thread_state) {
			ResumeThread(h_thread);
			CloseHandle(h_thread);
			LOG_DEBUG("WaitForSingleObject done");
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