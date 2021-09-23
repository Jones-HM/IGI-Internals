#pragma once
#define GT_BUILD_DLL
#define GT_BUILD_CLI
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1
#define _CRT_SECURE_NO_DEPRECATE
#define DLL_EXPORT __declspec( dllexport )

#include "Common.hpp"
#include "logger.hpp"
#include "Console.hpp"
#include "GTLibc.hpp"
#include "DllMain.hpp"
#include "AutoMsgBox.hpp"
#include "General.hpp"
#include "MinHook.hpp"
#include "NativeCaller.hpp"
#include "NativeHelper.hpp"
#include "Natives.hpp"
#include "PatternScanner.hpp"
#include "StackWalker.hpp"

using namespace Utility;
BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
void DllMainLoop();
