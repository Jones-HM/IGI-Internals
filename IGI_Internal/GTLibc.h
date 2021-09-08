#ifndef _GTLIBC_H_
#define _GTLIBC_H_

/*
BRIEF : GTLibc is library to make game trainer in c/c++ it provide all the necessary methods to make simple game trainer in
windows using win32-API with ease.
It uses only WIN32 API methods instead of CRT method because this is intended to work on Windows system only
and not shall be portable or to target other OS like Linux,MAC OS etc.

From the beginning of trainer development till end, it provides all necessary methods needed for game trainer.

*****************************
*******Main components.******
*****************************

1)Finding game --> use GT_FindGameProcess()  or GT_FindGameWindow() method.
2)Reading values Health,XP,Ammos from game --> use GT_ReadAddress() or GT_ReadAddressoffset() methods.
3)Writing values Health,XP,Ammos to  game --> use GT_WriteAddress() or GT_WriteAddressOffset() methods.
4)Creating Hot-keys for trainer --> use GT_HotKeysPressed() MACRO or GT_IsKeyPressed()/GT_IsKeyToggled() methods.

*****************************
****Additional components.***
*****************************

5)Additional Automation of scripting for trainer --> use GT_DoMousePress() and GT_DoKeyPress() methods.
6)Cheat code applying tool included in this library --> use GT_SetCheatCode() method.
7)Offset area searching tool included in this library --> use GT_SearchOffsetArea() method.

***********************************************
****Advanced components for Game Hacking.*****
**********************************************

8)Opcode injection tool included in this library --> use GT_InjectOpcode()/GT_InjectOpcodes() methods.
9)NOP instruction Filling tool included in this library --> use GT_WriteNOP()/GT_WriteNOPs() methods.
10)Write JMP/CALL assembly instruction --> use  GT_WriteJmpOrCall() method.
11)Shellcode injection tool included in this library --> use GT_InjectShellCode() method.
12)DLL injection tool included in this library --> use GT_InjectDLL() method.
13)Inject Assembly directly into process using --> use GT_InjectAsm() with utility GT_GetJmpBackAddress().

NOTE : This ain't memory scanning,hooking,analyzing library, it won't provide methods for scanning/signature or dumping RAW memory.

AIM : The aim of this library is only to provide the most efficient way of creating game trainer
and to provide a layer on top of WIN-32 API cumbersome methods and to make reading/writing ,finding Game process easier and convenient.

DOCUMENTATION INFO :
All Public and Semi-Private methods are well documented.
but private methods are not documented as it was not necessary to do so.

VERSION INFO :
GTLIBC Version : V 1.6.1

Written by Ha5eeB Mir (haseebmir.hm@gmail.com)
*/

/*Undefine UNICODE if already defined*/
#ifdef UNICODE
#undef UNICODE
#endif

#ifdef GT_BUILD_CLI
#include "GTConsole.c"
#endif

/*Defining WIN32 Constants*/
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501
#undef WIN32_LEAN_AND_MEAN  

/*Including WIN32 libraries*/
#include <windows.h>
#include <tlhelp32.h>

/*Including Conditional Process library*/
#ifdef GT_USE_PROC_MODULES
#include <psapi.h>
#endif

/*Defining NULL and NAN constants*/
#define GT_NIL 0x0          /*Integer NULL*/
#define GT_NUL '\0'         /*Character NULL*/
#define GT_NULL NULL /*Pointer NULL*/
#define GT_NULLF 0.0f       /*Floating NULL*/
#define GT_HotKeysPressed(...) GT_HotKeysDown(__VA_ARGS__, NULL)
typedef double* PDOUBLE;

/*Re-Defining standard constants*/
#if !defined(FILE_NAME) && !defined(LINE_NO) && !defined(FUNC_NAME)
#define FILE_NAME __FILE__
#define LINE_NO __LINE__
#define FUNC_NAME __func__
#endif

/*define date and time regardless if it will bind with GTConsole or not*/
#define CURR_DATE __DATE__
#define CURR_TIME __TIME__

/*Defining Architecture Build Type*/
#if defined(_WIN64)
#define BUILD_ARCH_64
static int gt_x64 = 1;
static int gt_x86 = 0;

#elif defined(_WIN32)
#define BUILD_ARCH_32
static int gt_x64 = 0;
static int gt_x86 = 1;
#endif

/*Defining exception handling constants*/
#if !defined(gt_try) && !defined(gt_catch) && !defined(gt_throw)
#define gt_try BOOL GT_HadError = FALSE;
#define gt_catch(x)  GT_ExitJump: if (GT_HadError)
#define gt_throw(x) GT_HadError = TRUE; goto GT_ExitJump;
#endif

/*Enum for OPCODE type*/
typedef enum GT_OPCODE
{
    GT_OP_SHORT_JUMP = 0x1,
    GT_OP_NEAR_JUMP = 0x2,
    GT_OP_CALL = 0x3
} GT_OPCODE;

/*Enum for SHELLCODE type*/
typedef enum GT_SHELL
{
    GT_ORIGINAL_SHELL,
    GT_PATCHED_SHELL
} GT_SHELL;

/*Enum for ASMInject type*/
typedef enum GT_ASM_TYPE
{
    GT_ORIGINAL_ASM,
    GT_PATCHED_ASM
} GT_ASM_TYPE;

/*Enum for ProcAction type*/
typedef enum GT_PROC_ACTION
{
    GT_PROC_RESUME,
    GT_PROC_SUSPEND
} GT_PROC_ACTION;

/****************************************************************************/
/*********************-PUBLIC-METHODS-***************************************/
/****************************************************************************/
/*Public methods to find Game process/window.*/
HANDLE GT_FindGameProcess(LPCSTR);
HWND GT_FindGameWindow(LPCSTR);

/*Public methods to Read/Write values from/at Address.*/
LPVOID GT_ReadAddress(LPVOID, SIZE_T);
LPVOID GT_ReadAddressOffset(LPVOID, DWORD, SIZE_T);
LPVOID GT_ReadAddressOffsets(LPVOID, DWORD *, SIZE_T, SIZE_T);
BOOL GT_WriteAddress(LPVOID, LPVOID, SIZE_T);
BOOL GT_WriteAddressOffset(LPVOID, DWORD, LPVOID, SIZE_T);
BOOL GT_WriteAddressOffsets(LPVOID, DWORD *, SIZE_T, LPVOID, SIZE_T);

/*Public methods to Read/Write pointer from/at Address.*/
BOOL GT_WritePointer(LPVOID,LPVOID, SIZE_T);
LPVOID GT_ReadPointer(LPVOID,SIZE_T);
LPVOID GT_ReadPointerOffset(LPVOID, DWORD, SIZE_T);
LPVOID GT_ReadPointerOffsets(LPVOID, DWORD *, SIZE_T, SIZE_T);
BOOL GT_WritePointerOffset(LPVOID, DWORD, LPVOID, SIZE_T);
BOOL GT_WritePointerOffsets(LPVOID, DWORD *, SIZE_T, LPVOID, SIZE_T);

/*Public methods to Read/Write different type of values from/at Address.*/
BYTE GT_ReadByte(LPVOID);
BOOL GT_WriteByte(LPVOID, BYTE);
UINT16 GT_Read2Bytes(LPVOID);
BOOL GT_Write2Bytes(LPVOID, UINT16);
UINT32 GT_Read4Bytes(LPVOID);
BOOL GT_Write4Bytes(LPVOID, UINT32);
UINT64 GT_Read8Bytes(LPVOID);
BOOL GT_Write8Bytes(LPVOID, UINT64);
FLOAT GT_ReadFloat(LPVOID);
BOOL GT_WriteFloat(LPVOID, FLOAT);
double GT_ReadDouble(LPVOID);
BOOL GT_WriteDouble(LPVOID, double);
PCHAR GT_ReadString(LPVOID, SIZE_T);
BOOL GT_WriteString(LPVOID, PCHAR);

/*Public getter methods to get Game Name,Handle,Process ID,base address,static address.*/
LPCSTR GT_GetGameName(VOID);
DWORD GT_GetProcessID(VOID);
HANDLE GT_GetGameHandle4mHWND(HWND);
DWORD GT_GetProcessID4mHWND(HWND);
LPVOID GT_GetGameBaseAddress(DWORD);
LPVOID GT_GetStaticAddress(DWORD64, DWORD *, SIZE_T, DWORD, SIZE_T);

/*Public methods for Process and modules*/
LPSTR GT_GetProcessModule(LPSTR);
void GT_SuspendResumeProcess(DWORD, GT_PROC_ACTION,INT);
BOOL GT_Is64bitGame(HANDLE);
void SetGameFocus();

/*Public methods for creating hot-keys*/
BOOL GT_HotKeysDown(INT, ...);
BOOL GT_IsKeyPressed(CONST INT);
BOOL GT_IsKeyToggled(CONST INT);

/****************************************************************************/
/****************-SEMI-PRIVATE-METHODS-**************************************/
/****************************************************************************/
/*Semi-private methods for pressing Keyboard and Mouse keys*/
VOID GT_DoMousePress(INT);
VOID GT_DoKeyPress(INT);

/*Semi-private Tool for Applying cheat codes*/
VOID GT_SetCheatCode(LPCSTR);

/*Semi-private Tool for searching in offset area*/
LPSTR GT_SearchOffsetArea(LPVOID, CONST size_t, CONST size_t, DWORD, SIZE_T);

/*Semi-private Tool for Injecting custom code/DLL*/
BOOL GT_InjectOpcode(LPVOID, LPCVOID, SIZE_T);
BOOL GT_InjectOpcodes(LPVOID[], LPBYTE[], SIZE_T[], SIZE_T);
BOOL GT_InjectDLL(LPCSTR, LPCSTR);

#ifdef GT_BUILD_DLL
BOOL GT_InjectAsm(LPVOID, LPVOID, INT, GT_ASM_TYPE);
DWORD GT_GetJmpBackAddress(LPVOID, DWORD, LPCSTR);
#endif

/*Semi-private Tool for writing assembly NOP instruction*/
BOOL GT_WriteNOP(LPVOID, SIZE_T);
BOOL GT_WriteNOPs(LPVOID[], SIZE_T[], SIZE_T);

/*Semi-private Tool for writing assembly JMP or CALL instruction*/
BOOL GT_WriteJmpOrCall(LPVOID, LPVOID, GT_OPCODE, UINT);

/*Semi-private Tool for injecting custom shellcode into game*/
LPVOID GT_InjectShellCode(LPVOID, LPCVOID, SIZE_T, UINT, GT_SHELL, GT_OPCODE);

/*Semi private method for enabling/disabling Logs*/
BOOL GT_EnableLogs(VOID);
BOOL GT_DisableLogs(VOID);

/*Semi-private getter methods to get Game,Handle,HWND.*/
HANDLE GT_GetGameHandle(VOID);
HWND GT_GetGameHWND(VOID);

/*Semi-private methods for showing info/warning/error*/
VOID GT_ShowInfo(LPCSTR);
VOID GT_ShowWarning(LPCSTR);
VOID GT_ShowError(LPCSTR);

/****************************************************************************/
/****************-PRIVATE-METHODS-*******************************************/
/****************************************************************************/
/*Private methods for showing error*/
static VOID GT_ThrowError(DWORD, LPCSTR, DWORD);
static DWORD GT_GetError(VOID);

/*Private setter methods for setting Game Name, ID,Handle,HWND etc*/
static VOID GT_SetProcessID(DWORD);
static VOID GT_SetGameHandle(HANDLE);
static VOID GT_SetGameName(LPCSTR);
static VOID GT_SetGameHWND(DWORD);

/*Private utility methods for Logs*/
static VOID GT_AddLog(LPCSTR, ...);
static BOOL GT_IsLogEnabled();
static BOOL GT_FileExist(LPCSTR);

/*Private memory allocation wrapper methods*/
LPVOID GT_MemAlloc(DWORD, SIZE_T);
BOOL GT_MemFree(LPVOID);

/*Private miscellaneous methods*/
static VOID GT_DoVirtualKeyPress(INT, INT, INT);
static BOOL CALLBACK GT_EnumAllWindows(HWND, LPARAM);
static BOOL GT_IsPrivateMethod(BOOL, LPCSTR, INT);
static LPCSTR GT_BoolAlpha(BOOL);
static VOID GT_GetValueType(LPVOID, PSIZE_T, LPCSTR);
static BOOL IsWow64(HANDLE);
static VOID GT_CheckGameTrainerArch(HANDLE);
static LPSTR GT_GetCurrentTime();
BOOL GT_IsElevatedProcess();
/*Private core method for injecting shell.*/
static LPVOID GT_InjectShell(LPVOID, LPCVOID, SIZE_T, GT_SHELL, GT_OPCODE);

/*Global variables for storing game information*/
extern DWORD gt_process_id;
extern HANDLE gt_game_handle;
extern CHAR gt_game_name[MAX_PATH];
extern HWND gt_game_hwnd;

/*Global variable for storing error code*/
extern DWORD gt_error_code;

/*Global variable for Setting private methods inaccessible*/
extern BOOL gt_private_method;

/*Global variable for enabling/disabling logs*/
extern BOOL gt_logs_enabled;

#endif /* _GTLIBC_H_ */
