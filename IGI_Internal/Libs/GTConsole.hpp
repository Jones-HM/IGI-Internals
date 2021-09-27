#ifndef _GTCONSOLE_H_ 
#define _GTCONSOLE_H_ 
#ifdef __cplusplus 
extern "C" {
#endif 

	/*
	BRIEF : GTConsole is library to make intuitive and clean UI for (Console based) Game trainer.
	It contains all useful methods needed to make UI for CLI based Game trainer,
	it uses WIN-32 API internally to provide layer over it.

	AIM : The aim of this library is only to provide the most efficient way of creating intuitive and clean UI for (Console based) game trainer
	and to provide a layer on top of WIN-32 API cumbersome console methods and to make console methods easier and convenient.
	*/
	/*===========================*/
	/* ==== Main components.==== */
	/*===========================*/

	/*
	1)Console methods : Changing/clearing console text or background --> use GT_SetConsoleTextColor(), GT_SetConsoleBGColor(), GT_ClearConsoleText() etc methods.
	2)Window methods : Change/Move/resize window --> use GT_SetWindowTitle() , GT_SetWindowStyle(), GT_ResizeWindow(), GT_DisableWindowStyle() etc methods.
	3)Cursor methods : Show/hide/Move/reset cursor  --> use GT_ShowConsoleCursor(),GT_WriteConsoleXY(), GT_SetConsoleCursorXY(), GT_ResetConsole() methods.
	4)Font methods : Set/Get/Print/ font  --> use GT_GetFontSize(),GT_SetFontSize(), GT_WriteConsoleFont().
	*/

	/*=================================*/
	/* ==== Additional components.==== */
	/*=================================*/

	/*
	5)Loading bar : Show loading bar animation on console  --> use GT_ShowLoadingBar() method.
	6)Console/Window Handle : Get Console Standard handle or Console Window handle --> use GT_GetConsoleStdHandle(), GT_GetConsoleWindowHandle() methods.
	7)Flush/clear input buffer : Flush or clear input buffer --> use GT_FlushInputBuffer(), GT_FlushConsoleInputBuffer() methods.
	8)Formatted Text :  Read/Write/Blink formatted text --> use GT_BlinkConsoleText(), GT_ReadConsole(), GT_WriteConsole() methods.
	9)Trainer specific :  Create Trainer menu or play/stop sound --> Use GT_CreateMenu(), GT_PlaySound(), GT_StopSound methods.

	NOTE : This is standalone library and this doesn't need GTLibc to work, it can be included alone in project ,
	or if you want to use with GTLibc then define GT_BUILD_CLI and include GTLibc only.

	PS :  To use GT_PlaySound() method use private macro GT_USE_SOUND to enable
	and include this flag '-lwinmm' (without quotes) in compiler options


	DOCUMENTATION INFO :
	All Public and Semi-Private methods are well documented.
	but private methods are not documented as it was not necessary to do so.

	VERSION INFO :
	GTConsole Version : V 1.2.1

	Written by Ha5eeB Mir (haseebmir.hm@gmail.com)
	*/

	/*Undefine UNICODE if already defined*/
#ifdef UNICODE 
#undef UNICODE 
#endif 

/*Defining WIN32 Constants*/
#define WINVER 0x0500 
#define _WIN32_WINNT 0x0501 

/*Including WIN32 library*/
#include <windows.h> 

/*Defining NULL constants*/
#define NIL 0x0  /*Integer NULL*/ 
#define NUL '\0'  /*Character NULL*/ 

/*Re-Defining standard constants*/
#if !defined(FILE_NAME) && !defined(LINE_NO) && !defined(FUNC_NAME)
#define FILE_NAME __FILE__ 
#define LINE_NO __LINE__ 
#define FUNC_NAME __func__ 
#endif 

/*Enum to store Foreground colors*/
	typedef enum FG_COLORS {
		FG_BLACK = 0, FG_BLUE = 1, FG_GREEN = 2, FG_CYAN = 3, FG_RED = 4,
		FG_MAGENTA = 5, FG_BROWN = 6, FG_LIGHTGRAY = 7, FG_GRAY = 8,
		FG_LIGHTBLUE = 9, FG_LIGHTGREEN = 10, FG_LIGHTCYAN = 11, FG_LIGHTRED = 12,
		FG_LIGHTMAGENTA = 13, FG_YELLOW = 14, FG_WHITE = 15
	} FG_COLORS;

	/*Enum to store Background colors*/
	typedef enum BG_COLORS {
		BG_NAVYBLUE = 16, BG_GREEN = 32, BG_TEAL = 48,
		BG_MAROON = 64, BG_PURPLE = 80, BG_OLIVE = 96,
		BG_SILVER = 112, BG_GRAY = 128, BG_BLUE = 144,
		BG_LIME = 160, BG_CYAN = 176, BG_RED = 192,
		BG_MAGENTA = 208, BG_YELLOW = 224, BG_WHITE = 240
	} BG_COLORS;

	/*Enum to store window parameter*/
	enum WINDOW_PARAM {
		WP_WINDOW_SIZE = 1, WP_WINDOW_CURSOR_POS = 2,
		WP_WINDOW_ATTRIBS = 4, WP_WINDOW_RECT = 8, WP_WINDOW_MAX_SIZE = 16
	};

	/*Enum to store console data-type for reading*/
	enum CONSOLE_READ_TYPE {
		CONSOLE_READ_CHAR, CONSOLE_READ_INT, CONSOLE_READ_STR
	};

	/*===========================*/
	/* ==== PUBLIC METHODS.===== */
	/*==========================*/

	/**
	* @description - Change console text Foreground and Background color.
	* @param - w_color [in] The character Foreground or Background color in WORD format.
	* Use Enum FG_COLORS to provide Foreground colors for text.
	* Or Use Enum BG_COLORS to provide Background colors for text.
	*Foreground and Background could be used like this : (FG_BLUE | BG_GRAY)
	* @return - If the function succeeds, the return value is nonzero. If the function fails, the return value is zero.
	*/

	BOOL GT_SetConsoleTextColor(WORD w_color);

	/**
	* @description - change background color of console.
	* @param - bg_color [in] - Background color in WORD format.
	*Use Enum BG_COLORS to provide Background colors for console.
	* @return -If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
	*/

	BOOL GT_SetConsoleBGColor(WORD bg_color);


	/**
	* @description - Clear specific text on console.
	* @param -
	* text_x [in] X-axis of text.
	* text_y [in] Y-axis of text.
	* text_len [in] Length of text.
	* @return -If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
	*/

	BOOL GT_ClearConsoleText(SHORT text_x, SHORT text_y, SIZE_T text_len);


	/**
	* @description - Clear whole screen of current console.
	* @param - None
	*/

	VOID GT_ClearConsole();


	/**
	* @description - Sets the title for the current console window.
	* @param - window_name [in] Type: LPCTSTR - The string to be displayed on the title bar of the console window. The total size must be less than 64K.
	* @return - If the function succeeds, the return value is nonzero. If the function fails, the return value is zero.
	* NOTE : When the process terminates, the system restores the original console title.
	*/

	BOOL GT_SetWindowTitle(LPCSTR window_name);

	/**
	* @description - Changes an attribute of the specified window..
	* @param - n_index [in] Type: int
	* The zero-based offset to the value to be set. Valid values are in the range zero through the number of bytes of extra window memory, minus the size of an integer.
	*to specify other values use Macros GWL_EXSTYLE,GWL_HINSTANCE etc.

	*new_style [in] Type: LONG_PTR
	new window style ex : WS_MAXIMIZEBOX,WS_OVERLAPPEDWINDOW etc.

	* @return - If the function succeeds, the return value is the previous value of the specified offset.
	* If the function fails, the return value is zero
	*/

	LONG_PTR GT_SetWindowStyle(INT n_index, LONG_PTR new_style);

	/**
	* @description - Resize current console window to new size.
	* @param -
	* win_xPos [in] - The new position of the left side of the window.
	* win_y_pos [in] -  The new position of the top of the window.
	* win_width [in] -  The new width of the window.
	* win_height [in] -  The new height of the window.
	* @return - If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
	*/

	BOOL GT_ResizeWindow(LONG win_xPos, LONG win_y_pos, LONG win_width, LONG win_height);

	/**
	* @description - Changes the size, position, and Z order of window.
	* @param -
	*win_xPos [in] - The new position of the left side of the window, in client coordinates.
	*win_yPos [in] - The new position of the top of the window, in client coordinates.
	*win_width [in] -The new width of the window, in pixels.
	*win_height [in] - The new height of the window, in pixels.
	* @return - If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
	*/

	BOOL GT_MoveWindow(INT win_xPos, INT win_y_pos, INT win_width, INT win_height);

	/**
	* @description - Disable single/multiple window style of current console window.
	* @param - n_index [in] Type: int
	* The zero-based offset to the value to be set. Valid values are in the range zero through the number of bytes of extra window memory, minus the size of an integer.
	*to specify other values use Macros GWL_EXSTYLE,GWL_HINSTANCE etc.

	*disable_style [in] Type: LONG_PTR
	*new window style you wish to disable ex : WS_MAXIMIZEBOX,WS_VSCROLL etc. (NOTE : Mutltiple styles like (WS_MAXIMIZEBOX | WS_SIZEBOX | WS_MINIMIZEBOX) are valid)

	* @return - If the function succeeds, the return value is the previous value of the specified offset.
	* If the function fails, the return value is zero
	*/

	BOOL GT_DisableWindowStyle(INT n_index, LONG_PTR disable_style);

	/**
	* @description - Disable vertical or horizontal scrollbar of current console.
	* @param - scrollbar_type [in] - Scrollbar type ex : WS_VSCROLL , WS_HSCROLL,
	* to disable both scroll bars use bitwise OR and provide input like (WS_VSCROLL | WS_HSCROLL)
	* @return - If the function succeeds, the return value is nonzero.
	* If the function fails, the return value is zero
	*/

	BOOL GT_DisableWindowScrollbar(LONG_PTR scrollbar_type);


	/**
	* @description - Get screen information of current window.
	* @param -
	* w_param [in] - Window parameter type, use Enum WINDOW_PARAM to provide single or multiple values.
	* Multiple values ex : WP_WINDOW_SIZE | WP_WINDOW_RECT.
	* p_wsize [out] - Pointer to COORD that will receive window current size co-ordinates.
	* p_cursor [out] - Pointer to COORD that will receive cursor co-ordinates.
	* p_attribs [out] - Pointer to WORD that will receive window attributes.
	* psrct_window [out] - Pointer to SMALL_RECT that will receive window rect co-ordinates.
	* pMaxWindow [out] - Pointer to COORD that will receive window max size co-ordinates.

	* @return - If the function succeeds, the return value is nonzero.
	* If the function fails, the return value is zero
	*/

	BOOL GT_GetWindowScreenInfo(INT w_param, PCOORD p_wsize, PCOORD p_cursor, PWORD p_attribs, PSMALL_RECT psrct_window, PCOORD pMaxWindow);

	/**
	* @description - Show/hide console cursor.
	* @param - show_flag [in] - Show flag use TRUE to show cursor or FALSE to hide cursor.
	* @return - If the function succeeds, the return value is nonzero.
	* If the function fails, the return value is zero
	*/

	BOOL GT_ShowConsoleCursor(BOOL show_flag);

	/**
	* @description - Print string/text at specified position on console.
	* @param - lp_string [in] - String to print, string_x [in] - X-axis of string, string_y [in] - Y-axis of string
	* @return -  If the function succeeds, the return value is number of characters written on console.
	* If the function fails, the return value is zero
	*/

	DWORD GT_WriteConsoleXY(INT string_x, INT string_y, LPCSTR format, ...);

	/**
	* @description - Get the current font size of console.
	* @param - handle - A handle to the console screen buffer. The handle must have the GENERIC_WRITE.
	*max_window - If this parameter is TRUE, font information is set for the maximum window size. If this parameter is FALSE, font information is set for the current window size.
	* coord_size[Out] - Buffer to receive current size of fonts. Must be of type COORD.
	* @return -If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
	*/

	BOOL GT_GetFontSize(HANDLE handle, BOOL max_window, PCOORD coord_size);

	/**
	* @description - Set the new font size of console.
	* @param
	* size_percent - Size of fonts in percent must be a float value.
	*max_window - If this parameter is TRUE, font information is set for the maximum window size. If this parameter is FALSE, font information is set for the current window size.
	* @return -If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
	*/

	BOOL GT_SetFontSize(FLOAT size_percent, BOOL max_window);

	/**
	* @description - Write the text with font size.
	* @param
	* size_percent - Size of fonts in percent must be a float value.
	*max_window - If this parameter is TRUE, font information is set for the maximum window size. If this parameter is FALSE, font information is set for the current window size.
	* format - Text to print on console with or without arguments.
	*/

	void GT_WriteConsoleFont(FLOAT size_percent, BOOL max_window, LPCSTR format, ...);

	/**
	* @description - Flush the input buffer in case of character.
	* @param
	* ch - Character designated with input buffer.
	*/

	VOID GT_FlushInputBuffer(CHAR ch);


	/**
	* @description - Flushes the console input buffer. All input records currently in the input buffer are discarded.
	* @param
	* A handle to the console input buffer. The handle must have the GENERIC_WRITE access right.
	* @return -If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
	*/

	BOOL GT_FlushConsoleInputBuffer(HANDLE console_handle);

	/**
	* @description - Shows loading bar on current console.
	* @param - bar_breadth [in] - Breadth of loading bar.
	* bar_speed [in] - Speed of loading bar.
	* bar_color [in] - Color of loading bar.
	* bar_x [in] - X position of loading bar.
	* bar_y [in] - Y position of loading bar.
	* border_color [in] - Color of outer border of loading bar.
	*/

	VOID GT_ShowLoadingBar(UINT bar_breadth, UINT bar_speed, UINT bar_color, UINT bar_x, UINT bar_y, UINT border_color);

	/**
	* @description - Print Menu with options on console.
	* @param -
	* menu_option [in] - Menu option to print.
	* menu_hotkey [in] - Menu option hotkey to print.
	* menu_seperator [in] - Separator between menu_option and menu_hotkey . ex : Tab '\t' , Space ' ', Hyphen '-'.
	* op_color [in] - Foreground or Background color of menu_option.
	* hkey_color [in] - Foreground or Background color of menu_hotkey.
	* menu_op_x [in - optional] - X-position of menu_option. Provide -1 to disable this.
	* menu_op_y [in- optional] - Y-position of menu_option. Provide -1 to disable this.

	* @return -  If the function succeeds, the return value is number of characters written on console.
	* If the function fails, the return value is zero
	*/

	DWORD GT_PrintMenu(LPCSTR menu_option, LPCSTR menu_hotkey, LPCSTR menu_separator, WORD op_color, WORD hkey_color, SHORT menu_op_x, SHORT menu_op_y);

	/**
	* @description - Create Menu with options on console.
	* @param -
	* menu_options [in] - Menu options to print.
	* menu_hotkeys [in] - Menu options hotkeys to print.
	* menu_seperators [in] - Separators between menu_option and menu_hotkey . ex : Tab '\t' , Space ' ', Hyphen '-'.
	* sz_menu [in] - Size of Menu.
	* ops_color [in] - Foreground or Background colors of menu_options.
	* hkeys_color [in] - Foreground or Background colors of menu_hotkeys.
	* menu_ops_x [in - optional] - X-positions of menu_options. Provide -1 to disable this.
	* menu_ops_y [in- optional] - Y-positions of menu_options. Provide -1 to disable this.

	* @return -  If the function succeeds, the return value is number of characters written on console.
	* If the function fails, the return value is zero
	*/


	DWORD GT_CreateMenu(LPCSTR menu_options[], LPCSTR menu_hotkeys[], LPCSTR menu_separators[], SIZE_T sz_menu, PWORD ops_colors, PWORD hkeys_color, PSHORT menu_ops_x, PSHORT menu_ops_y);


	/**
	* @description - Play a sound specified by the given file name, resource, or system event.
	* @param - sound_path [in] - A string that specifies the sound to play. The maximum length, including the null terminator, is 256 characters. If this parameter is NULL, any currently playing waveform sound is stopped.
	* To stop a non-waveform sound, specify SND_PURGE in the sound_flag parameter.
	* sound_flag [in] - Flags for playing the sound. The following values are defined. Ex : SND_ASYNC | SND_LOOP etc.
	* @return -  Returns TRUE if successful or FALSE otherwise.

	*NOTE : Use GT_USE_SOUND Macro to enable sound methods.
	If you get error undefined reference to PlaySound then include this flag '-lwinmm' (without quotes) in compiler options.
	*/

#ifdef GT_USE_SOUND 
	BOOL GT_PlaySound(LPCTSTR sound_path, DWORD sound_flag);

	/**
	* @description - Stop playback of a sound that is playing asynchronously.
	* @param - None.
	* @return -  Returns TRUE if successful or FALSE otherwise.
	*NOTE : If you get error undefined reference to PlaySound then include this flag '-lwinmm' (without quotes) in compiler options.
	*/

	BOOL GT_StopSound();

#endif /*End of GT_USE_SOUND*/ 

	/*================================*/
	/* ====SEMI-PRIVATE-METHODS.===== */
	/*===============================*/


	/**
	* @description - Retrieves a handle to the specified standard device (standard input, output, or error).
	* @param - dw_std_device [in]
	* The standard device. STD_INPUT_HANDLE ,or STD_OUTPUT_HANDLE or STD_ERROR_HANDLE
	* @return - If the function succeeds, the return value is a handle to the specified device,
	* If the function fails, the return value is INVALID_HANDLE_VALUE.

	* NOTE : If an application does not have associated standard handles, such as a service running on an interactive desktop,
	* and has not redirected them, the return value is NULL.
	*/

	HANDLE GT_GetConsoleStdHandle(DWORD dw_std_device);

	/**
	* @description - Retrieves the window handle used by the console associated with the calling process.
	* @return - Handle to the window used by the console associated with the calling process
	* or NULL if there is no such associated console.
	*/

	HWND GT_GetConsoleWindowHandle();


	/**
	* @description - Set console cursor to specified postion on console.
	* @param - cursor_x [in] - X-axis of console, cursor_y [in] - y-axis of console
	* @return - If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
	*/

	BOOL GT_SetConsoleCursorXY(SHORT cursor_x, SHORT cursor_y);

	/**
	* @description - Blink console text.
	* @param - lp_text [in] - Text to blink,
	* repeat [in] - Number of times to print text on console
	* speed [in] - Speed of blinking text.
	*/

	VOID GT_BlinkConsoleText(LPCSTR lp_text, UINT repeat, UINT speed);

	/**
	* @description - Write formatted text on console .
	* @param - format [in] - Text to print.
	* @return -  If the function succeeds, the return value is number of characters written on console.
	* If the function fails, the return value is zero.
	*/

	DWORD GT_WriteConsole(LPCSTR format, ...);

	/**
	* @description - Write buffer text on console .
	* @param - format_buf [in] - Buffer containing text.
	* @return -  If the function succeeds, the return value is number of characters written on console.
	* If the function fails, the return value is zero.
	*/

	DWORD GT_WriteConsoleBuf(LPCSTR format_buf);


	/**
	* @description - Reads input from the console.
	* @param - lp_buf [out] - A pointer to a buffer that receives the data read from the console.
	* dw_n_chars [in] - The number of characters to be read.
	* The size of the buffer pointed to by the lp_buf parameter should be at least dw_n_chars * sizeof(TCHAR) bytes
	* dw_d_type [in] - Data-type to read from console, use Enum CONSOLE_READ_TYPE to provide values.
	* @return -  If the function succeeds, the return value is number of characters read from console.
	* If the function fails, the return value is zero.

	* NOTE - Only Integer,character and string data-types are supported.
	*/

	DWORD GT_ReadConsole(LPVOID lp_buf, DWORD dw_n_chars, DWORD dw_d_type);

	/**
	* @description - Reset text color attributes / console cursor position.
	* @param - txt_color [in - optional] - Text foreground or background color to reset.
	* console_color [in - optional] - Console background color to reset.
	* pcoord [in - optional] - Pointer to Cursor co-ordinates to reset.
	* @return -  If the function succeeds, the return value is nonzero.
	* If the function fails, the return value is zero.
	*/

	BOOL GT_ResetConsole(WORD txt_color, WORD console_color, PCOORD pcoord);

	/**
	* @description - Init the trainer window with name and size.
	* @param
	* window_name - Name of the trainer window.
	*x,y - Positions of trainer x and y coords.
	*width,height - Width and Height of trainer.
	*/

	VOID GT_InitTrainerWindow(LPCSTR window_name, int x, int y, int width, int height);

	/**
	* @description - Init the trainer window with name and size with provided theme.
	* @param
	* window_name - Name of the trainer window.
	*x,y - Positions of trainer x and y coords.
	*width,height - Width and Height of trainer.
	*TRAINER_BG,TRAINER_FG - Background and foreground colors of trainer to apply theme.
	*/

	VOID GT_InitTrainerWindowEx(LPCSTR window_name, int x, int y, int width, int height, BG_COLORS TRAINER_BG, FG_COLORS TRAINER_FG);

#ifdef __cplusplus 
}
#endif 
#endif