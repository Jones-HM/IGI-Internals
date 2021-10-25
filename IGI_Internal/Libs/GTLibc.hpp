#ifndef _GTLIBC_H_ 
#define _GTLIBC_H_ 

#ifdef __cplusplus 
extern "C" {
#endif 

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
	****Advanced components for Game Trainer.*****
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
	GTLIBC Version : V 1.6.2

	Written by Ha5eeB Mir (haseebmir.hm@gmail.com)
	*/

	/*Undefine UNICODE if already defined*/
#ifdef UNICODE 
#undef UNICODE 
#endif 

#ifdef GT_BUILD_CLI 
#include "GTConsole.hpp" 
#endif 

/*Defining WIN32 Constants*/
#define WINVER 0x0500 
#define _WIN32_WINNT 0x0501 

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

/*Re-Defining standard constants*/
#if !defined(FILE_NAME) && !defined(LINE_NO) && !defined(FUNC_NAME)
#define FILE_NAME __FILE__ 
#define LINE_NO __LINE__ 
#define FUNC_NAME __func__ 
#endif 

/*define date and time regardless if it will bind with GTConsole or not*/
#define CURR_DATE __DATE__ 
#define CURR_TIME __TIME__ 

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
	/**
	* @description - Find game by process name.
	* @param - Game name in string format (CASE INSENSTIVE). and without '.exe' extension.
	* @return - If game found it returns HANDLE to game otherwise returns GT_NULL
	* NOTE : Process name is name of your .exe file loaded in Memory.
	*/

	HANDLE GT_FindGameProcess(LPCSTR game_name);

	/**
	* @description - Find game by window name.
	* @param - window name in string format (CASE INSENSITIVE).
	* @return - if game window found then it returns HWND to that window otherwise returns GT_NULL
	* NOTE : Window name is name of your Game Process Window not the .exe file.
	*/

	HWND GT_FindGameWindow(LPCSTR window_name);

	/**
	* @description - Read value from provided address.
	* @param - Address in VOID* format and size of value.
	* @return - If read succeeded then it returns LPVOID object otherwise returns NULL.
	*/

	LPVOID GT_ReadAddress(LPVOID address, SIZE_T lp_size);
	/**
	* @description - Write value at provided address.
	* @param - Address in VOID* format and Pointer to Value to be written.
	* @return - If write is succeeded then it returns TRUE otherwise returns FALSE
	*/

	BOOL GT_WriteAddress(LPVOID address, LPVOID lp_value, SIZE_T lp_size);

	/**
	* @description - Read value from provided address with offset.
	* @param - Address in VOID* format and offset in DWORD format.
	* @return - If read succeeded then it returns address of pointer otherwise returns NULL.
	*/

	LPVOID GT_ReadAddressOffset(LPVOID lp_address, DWORD dw_offset, SIZE_T lp_size);

	/**
	* @description - Write value at provided address with offset.
	* @param - Address in VOID* format and offset in DWORD format,_and Pointer to value.
	* @return - If write succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_WriteAddressOffset(LPVOID lp_address, DWORD dw_offset, LPVOID value, SIZE_T lp_size);

	/**
	* @description - Read value from provided address with provided offsets.
	* @param - Address in VOID* format, offsets in DWORD* format and size of offsets and size of each element.
	* @return - If read succeeded then it returns list of values otherwise returns GT_NULL
	* NOTE : This will be useful in reading multiple values at a time like multiple Ammos/Clips from Ammos/Clips offsets list.
	* PS : FREE this memory after using it to avoid memory leaks use HeapFree() Method from (windows.h).
	*/

	LPVOID GT_ReadAddressOffsets(LPVOID lp_address, DWORD* dw_offsets, SIZE_T sz_offsets, SIZE_T lp_size);

	/**
	* @description - Write value at provided address with provided offsets.
	* @param - Address in VOID* format and offset in DWORD* format, Size in SIZE_T format
	* and Pointer to value to be written.
	* @return - If write succeeded then it returns TRUE otherwise returns FALSE.
	* NOTE : This will be useful in writing multiple values at a time like multiple ammo/clips values at Ammos/Clips offsets list.
	*/

	BOOL GT_WriteAddressOffsets(LPVOID lp_address, DWORD* dw_offsets, SIZE_T sz_offsets, LPVOID value, SIZE_T lp_size);
	/**
	* @description - Read pointer's address from provided address.
	* @param - Address in VOID* format and offset in DWORD format and size of pointer.
	* @return - If read succeeded then it returns address of pointer otherwise returns GT_NULL.
	*/

	LPVOID GT_ReadPointer(LPVOID lp_address, SIZE_T lp_size);
	/**
	* @description - Read pointer's address from provided address with offset.
	* @param - Address in VOID* format and offset in DWORD format.
	* @return - If read succeeded then it returns address of pointer otherwise returns GT_NULL.
	*/

	LPVOID GT_ReadPointerOffset(LPVOID lp_address, DWORD dw_offset, SIZE_T lp_size);

	/**
	* @description - Read pointer's address from provided address with provided offsets.
	* @param - Address in VOID* format, offsets in DWORD* format and size of offsets.
	* @return - If read succeeded then it returns address of pointer otherwise returns GT_NULL.
	*/

	LPVOID GT_ReadPointerOffsets(LPVOID lp_address, DWORD* dw_offsets, SIZE_T sz_offsets, SIZE_T lp_size);

	/**
	* @description - Write value at pointer's address with offset.
	* @param - Address in VOID* format and offset in DWORD format,_pointer to value and size of value.
	* @return - If write succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_WritePointer(LPVOID lp_address, LPVOID value, SIZE_T lp_size);

	/**
	* @description - Write value at pointer's address with offset.
	* @param - Address in VOID* format and offset in DWORD format,_pointer to value.
	* @return - If write succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_WritePointerOffset(LPVOID lp_address, DWORD dw_offset, LPVOID value, SIZE_T lp_size);

	/**
	* @description - Write value at pointer's address with offsets.
	* @param - Address in VOID* format and offset in DWORD* format,_size in SIZE_T format
	* and Pointer to value to be written.
	* @return - If write succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_WritePointerOffsets(LPVOID lp_address, DWORD* dw_offsets, SIZE_T sz_offsets, LPVOID value, SIZE_T lp_size);

	/**
	* @description -  Get current game name from memory.
	* @return - If game found returns game name otherwise returns NULL
	*/

	LPCSTR GT_GetGameName();

	/**
	* @description -  Get process ID of game from memory.
	* @return - If game found returns process ID of current game otherwise returns NULL.
	*/

	DWORD GT_GetProcessID();

	/**
	* @description - Get game handle from HWND (handle to window).
	* @param - Handle to current window of game in HWND format.
	* @return - Handle to process on success otherwise returns GT_NULL.
	* NOTE : HANDLE is handle to Game's process and HWND is handle to Game's window.
	*/

	HANDLE GT_GetGameHandle4mHWND(HWND g_hwnd);

	/**
	* @description - Get process ID of game from HWND (handle to window).
	* @param - Handle to current window of game in HWND format.
	* @return - On success it returns game's process ID otherwise returns NULL.
	* NOTE : HANDLE is handle to Game's process and HWND is handle to Game's window.
	*/

	DWORD GT_GetProcessID4mHWND(HWND g_hwnd);

	/**
	* @description - Get base address of current game.
	* @param - Process ID in DWORD format.
	* @return - On success it returns base address of game otherwise returns NULL
	*/

	LPVOID GT_GetGameBaseAddress(DWORD gt_process_id);

	/**
	* @description - Get static address of any value.
	* @param - Static pointer of value, offsets in DWORD* format and size of offsets and last offset.
	* @return - On success it returns static address of value otherwise returns NULL
	* NOTE : Static pointer may look like this --> "GAME.exe"+0x16E210 : "0x16E210" is static pointer.
	*/

	LPVOID GT_GetStaticAddress(DWORD64 static_offset, DWORD* offsets, SIZE_T sz_offsets, DWORD end_offset, SIZE_T lp_size);

	/**
	* @description - Read single byte value from provided address.
	* @param - Address in VOID* format.
	* @return - If read succeeded then returns byte value otherwise returns NULL
	*/

	BYTE GT_ReadByte(LPVOID address);

	/**
	* @description - Write byte value to provided address.
	* @param - Address in VOID* format and value.
	* @return - If write is succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_WriteByte(LPVOID address, BYTE value);

	/**
	* @description - Read 2 bytes value from provided address.
	* @param - Address in VOID* format.
	* @return - If read succeeded then returns value otherwise returns NULL
	*/

	UINT16 GT_Read2Bytes(LPVOID address);

	/**
	* @description - Write 2 bytes value to provided address.
	* @param - Address in VOID* format and value.
	* @return - If write is succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_Write2Bytes(LPVOID address, UINT16 value);

	/**
	* @description - Read 4 integer(4Bytes) value from provided address.
	* @param - Address in VOID* format.
	* @return - If read succeeded then returns value otherwise returns NULL.
	*/

	UINT32 GT_Read4Bytes(LPVOID address);

	/**
	* @description - Write integer(4Bytes) value to provided address.
	* @param - Address in VOID* format and value.
	* @return - If write is succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_Write4Bytes(LPVOID address, UINT32 value);

	/**
	* @description - Read 8 integer(8Bytes) value from provided address.
	* @param - Address in VOID* format.
	* @return - If read succeeded then returns value otherwise returns NULL.
	*/

	UINT64 GT_Read8Bytes(LPVOID address);

	/**
	* @description - Write integer(8Bytes) value to provided address.
	* @param - Address in VOID* format and value.
	* @return - If write is succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_Write8Bytes(LPVOID address, UINT64 value);

	/**
	* @description - Read float value from provided address.
	* @param - Address in VOID* format.
	* @return - If read succeeded then returns value otherwise returns NULL.
	*/

	FLOAT GT_ReadFloat(LPVOID address);

	/**
	* @description - Write Float value to provided address.
	* @param - Address in VOID* format and value.
	* @return - If write is succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_WriteFloat(LPVOID address, FLOAT value);

	/**
	* @description - Read double value from provided address.
	* @param - Address in VOID* format.
	* @return - If read succeeded then returns value otherwise returns NULL.
	*/

	DOUBLE GT_ReadDouble(LPVOID address);

	/**
	* @description - Write Double value to provided address.
	* @param - Address in VOID* format and value.
	* @return - If write is succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_WriteDouble(LPVOID address, DOUBLE value);

	/**
	* @description - Read string value from provided address.
	* @param - Address in VOID* format and length of string.
	* @return - If read succeeded then returns value otherwise returns NULL.
	*/

	PCHAR GT_ReadString(LPVOID address, SIZE_T lp_size);

	/**
	* @description - Write string value to provided address.
	* @param - Address in VOID* format and value.
	* @return - If write is succeeded then it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_WriteString(LPVOID address, PCHAR value);

	/**
	* INFO : Get info module(s) of current process.
	* @description - Get (FullPath and Address) of single or list of modules of your current process(Game).
	* @param - Name of module to get single result or provide GT_NULL or empty string to get List of results.
	* NOTE : Process modules are modules attached to your process externally like "USER32.dll","Kernel32.dll","Ntdll.dll" etc.
	* To use it Enable GT_USE_PROC_MODULES Macro and compile it with '-lpsapi' or add the flag in compiler option.
	*/

#ifdef GT_USE_PROC_MODULES 
	LPSTR GT_GetProcessModule(LPSTR module_name);
#endif // GT_USE_PROC_MODULES 

	/**
	* @description - Suspend or resume any process.
	* @param - pid - Process id of process.
	* process_action - Action (Suspend or Resume) Use 'GT_PROC_ACTION' enum to provide values.
	*/

	void GT_SuspendResumeProcess(DWORD pid, GT_PROC_ACTION process_action);

	/**
	* @description  - Check whether Game running is 32bit or 64bit.
	* @param - p_handle - Handle of process.
	* @return - returns TRUE if game is 64bit otherwise returns FALSE.
	*/

	BOOL GT_Is64bitGame(HANDLE p_handle);

	/**
	* @description  - Set game focus to MainWindow - If game is in background.
	*/

	void GT_SetGameFocus();

	/**
	* INFO : Hot keys can be in combination of like GT_HotKeysDown(LCTRL,VK_F1) or GT_HotKeysDown(LSHIFT,'F')
	* @description - check for Hot keys combinations pressed or not.
	* @param - Combination of hot keys using virtual key_codes or characters A-Z,a-z.
	* @return - If keys pressed it will return TRUE otherwise returns FALSE.
	* PS : Don't use this method directly instead use 'GT_HotKeysPressed' MACRO.
	*/

	BOOL GT_HotKeysDown(INT key, ...);

	/**
	* @description - Check if provided key is pressed or not.
	* @param - virtual key_code in INT format. ex : (VK_SHIFT).
	* @return - If key is pressed it returns TRUE otherwise returns FALSE.
	* NOTE : This method must be in main game running loop or any continuous loop.
	*/

	BOOL GT_IsKeyPressed(CONST INT key);

	/**
	* @description - Check if provided key is toggled or not.
	* @param - virtual key_code in INT format. ex : (VK_SHIFT).
	* @return - If key is toggled it returns TRUE otherwise returns FALSE.
	* NOTE : This method must be in main game running loop or any continuous loop.
	*/

	BOOL GT_IsKeyToggled(CONST INT key);

	/****************************************************************************/
	/****************-SEMI-PRIVATE-METHODS-**************************************/
	/****************************************************************************/

	/**
	* INFO : Send Mouse input to current game.
	* @description - Send mouse input control to game.
	* @param - mouse code in INT format. ex : use these pre-defined macros FROM_LEFT_1ST_BUTTON_PRESSED or RIGHTMOST_BUTTON_PRESSED.
	* NOTE : This will be useful if you want to create some automated scripting for your game.
	*/

	VOID GT_DoMousePress(INT mouse_code);

	/**
	* INFO : Send Keyboard input to current game.
	* @description - Send keyboard input control to game.
	* @param - Virtual key_code in INT format. ex : VK_CONTROL,VK_SHIFT. (see winuser.h for full list of Key codes.)
	* NOTE : This will be useful if you want to create some automated scripting for your game.
	*/

	VOID GT_DoKeyPress(INT key_code);

	/**
	* @description - Apply provided cheat code to current game.
	* @param - cheat code in string format, (NULL terminated).
	*/
	VOID GT_SetCheatCode(LPCSTR cheat_code);

	/**
	* INFO : Search any value in current offset i.e. (base_address + offset) for finding new heath/ammos/weapons in game.
	* @description -  Search value in offset area.
	* @param - base address of Ammo/health/clips etc in VOID*,offset limit in size_t,offset size and value for searching.
	* @return - If value found it returns its address and offset in formatted string otherwise returns GT_NULL
	* NOTE : FREE this memory after using it to avoid memory leaks use HeapFree() Method.
	*/

	LPSTR GT_SearchOffsetArea(LPVOID offset_base_address, CONST size_t offset_limit, CONST size_t offset_size, DWORD search, SIZE_T lp_size);

	/**
	* INFO : Inject your custom opcode into the game.
	* @description - Inject single assembly/opcode directly into target process.
	* @param - Address where to inject, opcode to inject ,length of opcode in bytes.
	* @return - On success of injection it returns TRUE otherwise returns FALSE.

	* @WARNING : This is advanced stuff so be careful while injecting custom code it should be exact opcode
	* for target machine's architecture ex : (x86,x64,amd64) and length of opcode should be exact as defined by ISA
	* otherwise target process could result in SEGFAULT causing program to crash.
	*/

	BOOL GT_InjectOpcode(LPVOID lp_address, LPCVOID lp_opcode, SIZE_T sz_opcode_len);

	/**
	* INFO : Inject your multiple custom opccodes into the game.
	* @description - Inject multiple assembly/opcodes directly into target process.
	* @param - List of Address where to inject, List of opcodes to inject , List of length of opcode in bytes, Total no. of Addresses.
	* @return - On success of injection it returns TRUE otherwise returns FALSE.

	* @WARNING : This is advanced stuff so be careful while injecting custom code it should be exact opcode
	* for target machine's architecture ex : (x86,x64,amd64) and length of opcode should be exact as defined by ISA
	* otherwise target process could result in SEGFAULT causing program to crash.
	*/

	BOOL GT_InjectOpcodes(LPVOID lp_addresses[], LPBYTE lp_opcode[], SIZE_T sz_opcode_lens[], SIZE_T n_addresses);

	/**
	* @description - Inject DLL to remote process.
	* @param - dll_name - Name of DLL file to inject with '.dll' extenstion.
	* process_name - Name of process where to inject DLL.
	* @return - On success it returns TRUE otherwise returns FALSE.

	* @WARNING : Be careful while injecting DLL in remote process keep in mind that
	* 32bit-Process needs 32-bit DLL
	* Don't try to inject 32-bit DLL to 64-bit process or vice versa
	* otherwise remote process could signal SEGFAULT causing program to crash.
	*/

	BOOL GT_InjectDLL(LPCSTR dll_name, LPCSTR process_name);

	/**
	* INFO : This method only works with DLL Build so dont try to use it on normal build.
	* @description - Inject your method containing assembly directly to another process (Only works with DLL).
	* @param - Address where to inject,
	* asm_func - Fucntion pointer of your funtion which contains the assembly.
	* asm_len - length of assembly intstruction in bytes. (must be greater than 5).
	* inject_type - Inject type original or patched (custom) , Use Enum 'GT_ASM_TYPE' to provide values.

	* @return - On success it returns TRUE otherwise returns FALSE.
	*NOTE : Your assembly must contain jump back or return statement use GT_GetJmpBackAddress utility to get correct address and syntax.
	*/

#ifdef GT_BUILD_DLL 
	BOOL GT_InjectAsm(LPVOID address, LPVOID asm_func, INT asm_len, GT_ASM_TYPE inject_type);

	/**
	* INFO : This method only works with DLL Build so dont try to use it on normal build.
	* @description - Get return or jump back address for your assembly. (Only works with DLL).
	* @param - Address where to inject,
	* length - length of assembly intstruction in bytes. (must be greater than 5).
	* jmp_back_buf - Buffer to recieve assembly syntax of jump instruction- OPTIONAL.
	* @return - Returns jump back address on valid input otherwise returns NIL.
	*/

	DWORD GT_GetJmpBackAddress(LPVOID address, DWORD length, LPSTR jmp_back_buf);
#endif 

	/**
	* INFO : Write NOP (No-Operation) code into the game.
	* @description - Inject multiple assembly instruction NOP directly into target process.
	* @param - Address where to inject,size of opcode present at that address, in bytes.
	* @return - On success of writing NOP it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_WriteNOP(LPVOID lp_address, SIZE_T sz_opcode_len);

	/**
	* INFO : Write multiple NOP (No-Operation) code into the game.
	* @description - Inject multiple assembly instruction NOP directly into target process.
	* @param - List of Address where to inject,_list of size of opcode present at that address, in bytes. Total no. of address.
	* @return - On success of writing NOP it returns TRUE otherwise returns FALSE.

	* PS - size of opcode is needed because it will write NOP of N-byte length
	* where 'N' is size of current instruction present at that address.
	*/

	BOOL GT_WriteNOPs(LPVOID lp_addresses[], SIZE_T sz_opcode_lens[], SIZE_T n_addresses);

	/**
	* @description - Write the JMP or CALL assembly instruction at an address.

	* @param - source - Address where to set the jmp/call instruction,
	* destination - Address where you want to JMP or CALL.
	* opcode_type - Use ENUM opcode to provide opcode type values.
	* nops_amount - Amount of NOPs to fill after JMP or CALL.
	* @return - On success of writing it returns TRUE otherwise returns FALSE.
	*/

	BOOL GT_WriteJmpOrCall(LPVOID lp_source, LPVOID lp_destination, GT_OPCODE opcode_type, UINT nops_amount);

	/**
	* INFO : Inject custom or original shellcode into the game.
	* @description - Inject your custom/original shellcode directly into target process (Virtual Memory).
	* @param -
	lp_address - Address where to inject shellcode. [Required]
	lp_shell - Pointer to shellcode. [Required]
	sz_shell - Size of shellcode in bytes.  [Required]
	nops_amount - Amount of NOPs to fill after injecting shellcode. [Optional] (Required if GT_SHELL is of type Patched)
	shell_type - Shellcode type original or patched (custom) , Use Enum 'GT_SHELL' to provide values. [Required]
	opcode_type - Type of opcode to call your injected shellcode with, Use Enum 'GT_OPCODE' to provide values. [Optional] (Required if GT_SHELL is of type Patched)

	* @return -
	if GT_SHELL is of type Patched then on success of injection it returns address where shellcode was injected otherwise returns GT_NULL
	if GT_SHELL is of type Original it returns TRUE on success of code injection otherwise returns FALSE.

	* @WARNING : This is advanced stuff so be careful while injecting shellcode it should be exact opcode
	* for target machine's architecture ex : (x86,x64,amd64) and length of opcode should be exact as defined by ISA
	* otherwise target process could result in SEGFAULT causing program to crash.
	*
	* NOTE : Amount of NOPs to fill in necessary after instruction because CALL/JMP instruction is 5 bytes but current instruction could be lower
	* or higher , so in order to avoid crashing we will fill NOPs in empty locations.
	* Calculate using any disassembler the amount of NOPs needed after instruction depending upon current architecture.
	*/

	LPVOID GT_InjectShellCode(LPVOID lp_address, LPCVOID lp_shell, SIZE_T sz_shell, UINT nops_amount, GT_SHELL shell_type, GT_OPCODE opcode_type);

	/**
	* INFO : Whether library should maintain logs internally (enable this if you want this feature).
	* @description - Enable logs in library.
	* @return - Returns TRUE if enabled is success otherwise returns FALSE.
	*/

	BOOL GT_EnableLogs(VOID);

	/**
	* @description - Disable logs in library.
	* @return - Returns TRUE if disable is success otherwise returns FALSE.
	*/

	BOOL GT_DisableLogs(VOID);

	/**
	* INFO : Whether library should suppress all critical errors.
	* @description - Enable suppress errors in library.
	* @return - None.
	*/

	VOID GT_SuppressErrors(BOOL action);


	/**
	* INFO : Whether library should suppress all critical warnings.
	* @description - Enable suppress warnings in library.
	* @return - None.
	*/

	VOID GT_SuppressWarnings(BOOL action);

	/**
	* @description - Get Handle to current game's process.
	* @return - If game found it return Handle to current game's process otherwise returns GT_NULL.
	*/

	HANDLE GT_GetGameHandle();

	/**
	* @description - Get Handle to current game's window.
	* @return - If game found it return Handle to current game's windows otherwise returns GT_NULL.
	*/

	HWND GT_GetGameHWND();


	/**
	* @description - Show text information message on screen.
	* @return - None.
	*/
	VOID GT_ShowInfo(LPCSTR);

	/**
	* @description - Show text warning message on screen.
	* @return - None.
	*/
	VOID GT_ShowWarning(LPCSTR);

	/**
	* @description - Show text error message on screen.
	* @return - None.
	*/
	VOID GT_ShowError(LPCSTR);

	/**
	* @description - Check if file exists in current directory.
	* @param - file_name - Name of the file to check.
	* @return - Returns TRUE if file exists otherwise FALSE.
	*/

	BOOL GT_FileExist(LPCSTR file_name);

#ifdef __cplusplus 
}
#endif 
#endif /* _GTLIBC_H_ */