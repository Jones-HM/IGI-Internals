/* AutoCloseMessageBox - C++ Implementation of original code of C# from CodeProject https://www.codeproject.com/Articles/7968/MessageBox-with-a-timeout-for-NET
Written by HaseeB Mir.
*/
#pragma once
#ifndef AUTO_MSG_BOX
#define AUTO_MSG_BOX
#undef UNICODE
#include <windows.h>
#include <iostream>
using std::string;

//Pointer to Hook functions.
typedef intptr_t(*HookProc)(int nCode, intptr_t wParam, intptr_t lParam);
typedef void(*TimerProc)(intptr_t hWnd, uint32_t uMsg, uintptr_t nIDEvent, uint32_t dwTime);

//Global vars.
extern int timerID;
extern HookProc hookProc;
extern TimerProc hookTimer;
extern uint32_t hookTimeout;
extern string hookCaption;
extern HHOOK hHook;
extern int captionLen;

#endif // AUTO_MSG_BOX
