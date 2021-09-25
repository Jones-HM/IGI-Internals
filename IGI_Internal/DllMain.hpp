#pragma once
#define GT_BUILD_DLL
#define GT_BUILD_CLI
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1
#define _CRT_SECURE_NO_DEPRECATE
#define DLL_EXPORT __declspec( dllexport )

#include "Common.hpp"
#include "Logger.hpp"
#include "Console.hpp"
#include "GTLibc.hpp"
#include "AutoMsgBox.hpp"
#include "Utility.hpp"
#include "MinHook.hpp"
#include "NativeCaller.hpp"
#include "NativeHelper.hpp"
#include "Natives.hpp"
#include "PatternScanner.hpp"

#ifdef _DEBUG
#include "DbgHelper.hpp"
#endif

using namespace Utility;
using namespace IGI;

BOOL __stdcall DllMain(HINSTANCE, DWORD, LPVOID);
void DllMainLoop();
