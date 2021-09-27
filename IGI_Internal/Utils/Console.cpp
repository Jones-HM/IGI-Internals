#include "Console.hpp" 

namespace Utility {

	static Console g__console;

	void Console::Allocate() {

		if (is_allocated) {
			return;
		}

		AllocConsole();
		SetConsoleTitle("IGI Internals");

		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);

		output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		const int width = 60;
		const int height = 50;

		// Add some more scrolling 
		COORD size;
		size.X = width;
		size.Y = height * 10;
		SetConsoleScreenBufferSize(output_handle, size);

		// Resize our console window 
		SMALL_RECT rect;
		rect.Left = rect.Top = 0;
		rect.Right = width - 1;
		rect.Bottom = height - 1;
		SetConsoleWindowInfo(output_handle, TRUE, &rect);

		is_allocated = true;
	}

	void Console::DeAllocate() {

		if (!is_allocated) {
			return;
		}

		FreeConsole();
	}

	void Console::SetTitle(const string& title) {

		SetConsoleTitle(title.c_str());
	}

	const string Console::GetTitle() {

		TCHAR title[MAX_PATH];
		GetConsoleTitle(title, MAX_PATH);

		return title;
	}

	void Console::Clear() {

		system("cls");
	}

	void Console::SetTextColor(const int color) {
		if (!is_allocated) {
			return;

		}
		CONSOLE_SCREEN_BUFFER_INFO screen_buffer;
		GetConsoleScreenBufferInfo(output_handle, &screen_buffer);

		WORD attributes = screen_buffer.wAttributes & ~FOREGROUND_RED & ~FOREGROUND_GREEN & ~FOREGROUND_BLUE & ~FOREGROUND_INTENSITY;
		attributes |= color;

		SetConsoleTextAttribute(output_handle, attributes);
	}

	void Console::SetBackgroundColor(const int color) {
		if (!is_allocated) {
			return;

		}

		CONSOLE_SCREEN_BUFFER_INFO screen_buffer;
		GetConsoleScreenBufferInfo(output_handle, &screen_buffer);

		WORD attributes = screen_buffer.wAttributes & ~BACKGROUND_RED & ~BACKGROUND_GREEN & ~BACKGROUND_BLUE & ~BACKGROUND_INTENSITY;
		attributes |= color;

		SetConsoleTextAttribute(output_handle, attributes);
	}

	Console* GetConsole() {

		return &g__console;
	}
}