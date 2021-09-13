#pragma once
#include "Common.h"
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
using namespace Utility;
BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
void DllMainLoop();
