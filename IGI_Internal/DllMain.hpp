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
#include "Hook.hpp" 
#endif 

#ifdef USE_GTLIBC_LIB 
#include "GTLibc.hpp" 
#endif 

#if defined(USE_STACKTRACE_LIB) && defined(RLS_x86)
#error Stacktrace only works for optimised code. Change your build settings to Debug.
#elif defined(USE_STACKTRACE_LIB) && defined(DBG_x86)
#include "DbgHelper.hpp" 
#endif 

using namespace Utility;
using namespace IGI;

BOOL __stdcall DllMain(HINSTANCE, DWORD, LPVOID);
void DllMainLoop();