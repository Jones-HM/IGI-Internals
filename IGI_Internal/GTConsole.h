#ifndef _GTCONSOLE_H_
#define _GTCONSOLE_H_

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
GTConsole Version : V 1.2

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
#define NIL 0x0 	/*Integer NULL*/
#define NUL '\0' 	/*Character NULL*/

/*Re-Defining standard constants*/
#if !defined(FILE_NAME) && !defined(LINE_NO) && !defined(FUNC_NAME)
#define FILE_NAME __FILE__
#define LINE_NO __LINE__
#define FUNC_NAME __func__
#endif

/*Defining exception handling constants*/
#if !defined(gt_try) && !defined(gt_catch) && !defined(gt_throw)
#define gt_try BOOL GT_HadError=FALSE;
#define gt_catch(x) GT_ExitJump:if(GT_HadError)
#define gt_throw(x) GT_HadError=TRUE;goto GT_ExitJump;
#endif

/*Enum to store Foreground colors*/
typedef enum FG_COLORS {
    FG_BLACK = 0,FG_BLUE = 1,FG_GREEN = 2,FG_CYAN = 3,FG_RED = 4,
    FG_MAGENTA = 5,FG_BROWN = 6,FG_LIGHTGRAY = 7,FG_GRAY = 8,
    FG_LIGHTBLUE = 9,FG_LIGHTGREEN = 10,FG_LIGHTCYAN = 11,FG_LIGHTRED = 12,
    FG_LIGHTMAGENTA = 13,FG_YELLOW = 14,FG_WHITE = 15
} FG_COLORS;

/*Enum to store Background colors*/
typedef enum BG_COLORS {
    BG_NAVYBLUE = 16,BG_GREEN = 32,BG_TEAL = 48,
    BG_MAROON = 64,BG_PURPLE = 80,BG_OLIVE = 96,
    BG_SILVER = 112,BG_GRAY = 128,BG_BLUE = 144,
    BG_LIME = 160,BG_CYAN = 176,BG_RED = 192,
    BG_MAGENTA = 208,BG_YELLOW = 224,BG_WHITE = 240
} BG_COLORS;

/*Enum to store window parameter*/
enum WINDOW_PARAM {
    WP_WINDOW_SIZE = 1,WP_WINDOW_CURSOR_POS = 2,
    WP_WINDOW_ATTRIBS = 4,WP_WINDOW_RECT = 8,WP_WINDOW_MAX_SIZE = 16
};

/*Enum to store console data-type for reading*/
enum CONSOLE_READ_TYPE {
    CONSOLE_READ_CHAR,CONSOLE_READ_INT,CONSOLE_READ_STR
};

/*===========================*/
/* ==== PUBLIC METHODS.===== */
/*==========================*/

/*Public console methods.*/
BOOL GT_SetConsoleTextColor(WORD);
BOOL GT_SetConsoleBGColor(WORD);
BOOL GT_ClearConsoleText(SHORT,SHORT,SIZE_T);
VOID GT_ClearConsole(VOID);

/*Public Window methods.*/
BOOL GT_SetWindowTitle(LPCSTR);
LONG_PTR GT_SetWindowStyle(INT,LONG_PTR);
BOOL GT_ResizeWindow(LONG,LONG,LONG,LONG);
BOOL GT_MoveWindow(INT,INT,INT,INT);
BOOL GT_DisableWindowStyle(INT,LONG_PTR);
BOOL GT_DisableWindowScrollbar(LONG_PTR);
BOOL GT_GetWindowScreenInfo(INT,PCOORD,PCOORD,PWORD,PSMALL_RECT,PCOORD);
VOID GT_InitTrainerWindow(LPCSTR,INT,INT,INT,INT);
VOID GT_InitTrainerWindowEx(LPCSTR,INT,INT,INT,INT,BG_COLORS,FG_COLORS);


/*Public console fonts methods.*/
BOOL GT_GetFontSize(HANDLE,BOOL,PCOORD);
BOOL GT_SetFontSize(FLOAT,BOOL);
void GT_WriteConsoleFont(FLOAT,BOOL,LPCSTR, ...);

/*Public console cursor methods.*/
BOOL GT_ShowConsoleCursor(BOOL);
DWORD GT_WriteConsoleXY(INT,INT,LPCSTR, ...);

/*Public flush buffer methods.*/
VOID GT_FlushInputBuffer(CHAR);
BOOL GT_FlushConsoleInputBuffer(HANDLE);

/*Public loading bar method.*/
VOID GT_ShowLoadingBar(UINT,UINT,UINT,UINT,UINT,UINT);

/*Public Game Trainer specific methods to Create/Show menu*/
DWORD GT_CreateMenu(LPCSTR[],LPCSTR[],LPCSTR[],SIZE_T,PWORD,PWORD,PSHORT,PSHORT);
DWORD GT_PrintMenu(LPCSTR,LPCSTR,LPCSTR,WORD,WORD,SHORT,SHORT);

/*Public Sound methods to play/stop sound
NOTE : Use GT_USE_SOUND Macro to enable sound methods*/

#ifdef GT_USE_SOUND
BOOL GT_PlaySound(LPCTSTR,DWORD);
BOOL GT_StopSound(VOID);
#endif

/*================================*/
/* ====SEMI-PRIVATE-METHODS.===== */
/*===============================*/

/*Semi-Private methods for getting handle*/
HANDLE GT_GetConsoleStdHandle(DWORD);
HWND GT_GetConsoleWindowHandle(VOID);

/*Semi-Private methods for setting cursor position*/
BOOL GT_SetConsoleCursorXY(SHORT,SHORT);

/*Semi-Private methods for Writing and reading text.*/
DWORD GT_WriteConsole(LPCSTR, ...);
DWORD GT_ReadConsole(LPVOID,DWORD,DWORD);

/*Semi-Private methods for Reset console attributes and Blink text*/
VOID GT_BlinkConsoleText(LPCSTR,UINT,UINT);
BOOL GT_ResetConsole(WORD,WORD,PCOORD);

/*=============================*/
/* ==== PRIVATE-METHODS.===== */
/*===========================*/

/*Private methods for showing error/info/warning*/
static VOID GT_ShowCError(DWORD, LPCSTR, DWORD);
static VOID GT_ShowCInfo(LPCSTR);
static VOID GT_ShowCWarning(LPCSTR);
static DWORD GT_GetCError(VOID);

/*Private miscellaneous methods*/
static VOID GT_CreateBorder(UINT,UINT,UINT,UINT,UINT);
static BOOL GT_IsPrivateCMethod(BOOL, LPCSTR, INT);


/*Global variable for storing console handles*/
HANDLE gt_std_handle = (HANDLE)NULL;
HWND gt_console_window_handle = (HWND)NULL;

/*Global variable for storing error code*/
extern DWORD gt_cerror_code;

/*Global variable for Setting private methods inaccessible*/
extern BOOL gt_private_cmethod;

#endif
