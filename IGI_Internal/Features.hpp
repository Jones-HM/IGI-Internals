#pragma once
#include "DllMain.hpp"

void ScriptCompile();
void StatusMsgShow();
string InternalDataRead();
bool InternalDataWrite(string data);
void DllMainLoop();
void StartLevelMain(int = 1, bool = true, bool = true, int = 1);
void QuitLevelMain();
inline bool g_DbgEnabled = true, g_PlayerEnabled = true;
const LPVOID viewport_x = (LPVOID)0x00BCAB08, viewport_y = (LPVOID)0x00BCAB10, viewport_z = (LPVOID)0x00BCAB18;
const int delay_ms = 2500;

void ThreadCallerDelay(int delay);
template<typename RT = Void, typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void>
RT ThreadCallerExec(void* func_ptr, T1 param1 = nullptr, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr);
