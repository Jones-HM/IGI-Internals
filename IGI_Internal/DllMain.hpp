#pragma once 
#define GT_BUILD_DLL 
#define GT_BUILD_CLI 
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1 
#define _CRT_SECURE_NO_DEPRECATE 
#define DLL_EXPORT __declspec( dllexport )

#include "Common.hpp" 
#include "Logger.hpp" 
#include "Console.hpp" 
#include "AutoMsgBox.hpp" 
#include "Utility.hpp" 
#include "NativeCaller.hpp" 
#include "NativeHelper.hpp" 
#include "Natives.hpp" 
#include "Memory.hpp"

#ifdef USE_MINHOOK_LIB 
#include "MinHook.hpp" 
#endif 

#ifdef USE_GTLIBC_LIB 
#include "GTLibc.hpp" 
#endif 

#ifdef USE_STACKTRACE_LIB 
#include "DbgHelper.hpp" 
#endif 

using namespace Utility;
using namespace IGI;

BOOL __stdcall DllMain(HINSTANCE, DWORD, LPVOID);
void DllMainLoop();