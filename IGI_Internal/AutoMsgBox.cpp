#include "AutoMsgBox.hpp"

static class AutoMsgBox {

private:

	static void MessageBoxHook(string caption, uint32_t uTimeout)
	{
		MessageBoxInit();
		if (hHook != NULL)
			throw new std::exception("multiple calls are not supported");

		hookTimeout = uTimeout;
		hookCaption = !caption.empty() ? caption : "";
		hHook = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)hookProc, 0, GetCurrentThreadId());
	}

	static intptr_t MessageBoxHookProc(int nCode, intptr_t wParam, intptr_t lParam)
	{
		if (nCode < 0)
			return CallNextHookEx(hHook, nCode, wParam, lParam);

		auto msg = reinterpret_cast<CWPRETSTRUCT*>(lParam);
		auto hook = hHook;

		//Hook Messagebox on Initialization.
		if (!hookCaption.empty() && msg->message == WM_INITDIALOG)
		{
			int nLength = GetWindowTextLength(msg->hwnd);
			char* text = new char[captionLen + 1];

			GetWindowText(msg->hwnd, text, captionLen + 1);

			//If Capttion window found Unhook it.
			if (hookCaption == text)
			{
				hookCaption = string("");
				SetTimer(msg->hwnd, (uintptr_t)timerID, hookTimeout, (TIMERPROC)hookTimer);
				UnhookWindowsHookEx(hHook);
				hHook = 0;
			}
		}

		return CallNextHookEx(hook, nCode, wParam, lParam);
	}

	static void MessageBoxTimerProc(HWND hWnd, uint32_t uMsg, uintptr_t nIDEvent, uint32_t dwTime)
	{
		if (nIDEvent == (uintptr_t)timerID)
		{
			short dw = (short)SendMessage(hWnd, DM_GETDEFID, 0, 0);
			EndDialog(hWnd, (intptr_t)dw);
		}
	}

public:
	static void MessageBoxInit() {
		hookProc = HookProc(MessageBoxHookProc);
		hookTimer = TimerProc(MessageBoxTimerProc);
		hookTimeout = 0;
		hookCaption = string("");
		hHook = 0;
		timerID = 42;
	}

	//Different overloaded methods for Showing message.
	static int Show() {
		string message = "";
		string caption = " ";
		captionLen = caption.length();
		MessageBoxHook(caption, 1000);
		return MessageBox((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
	}

	static int Show(string message) {
		string caption = " ";
		captionLen = caption.length();
		MessageBoxHook(caption, 1000);
		return MessageBox((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
	}

	static int Show(string message, uint32_t uTimeout) {
		string caption = " ";
		captionLen = caption.length();
		MessageBoxHook(caption, uTimeout);
		return MessageBox((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
	}

	static int Show(string message, string caption, uint32_t uTimeout) {
		captionLen = caption.length();
		MessageBoxHook(caption, uTimeout);
		return MessageBox((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
	}

	static int Show(string message, string caption, uint32_t uTimeout, uint32_t iconType) {
		captionLen = caption.length();
		MessageBoxHook(caption, uTimeout);
		caption = !caption.empty() ? caption : "";
		return MessageBox((HWND)0, message.c_str(), caption.c_str(), iconType);
	}

};