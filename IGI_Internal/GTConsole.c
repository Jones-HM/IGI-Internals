/*
GTConsole - Library to make intuitive and clean UI for (Console based) Game trainer.
*/

#include "GTConsole.h"

/*Global variable for storing error code*/
DWORD gt_cerror_code = NO_ERROR;

/*Global variable for Setting private methods inaccessible*/
BOOL gt_private_cmethod = FALSE;


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

BOOL GT_SetConsoleTextColor(WORD w_color)
{
    HANDLE console_handle = GT_GetConsoleStdHandle(STD_OUTPUT_HANDLE);
    BOOL color_status = FALSE;

    gt_try {
        if (!(color_status = SetConsoleTextAttribute(console_handle,w_color)))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
    return color_status;
}


/**
 * @description - change background color of console.
 * @param - bg_color [in] - Background color in WORD format.
  *Use Enum BG_COLORS to provide Background colors for console.
 * @return -If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
*/

BOOL GT_SetConsoleBGColor(WORD bg_color)
{

    HANDLE console_handle = GT_GetConsoleStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    DWORD dw_con_size = NIL, ch_out = NIL;
    COORD coord_screen = { 0, 0 };
    BOOL color_status = FALSE;

    gt_try {
        if (GetConsoleScreenBufferInfo(console_handle, &csbi))
        {
            dw_con_size = csbi.dwSize.X * csbi.dwSize.Y;
        }

        else
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

        // Change the console background color by filling whole console.
        if (!(color_status = FillConsoleOutputAttribute(console_handle,bg_color,dw_con_size,coord_screen,&ch_out)))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
    }

    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }

    gt_private_cmethod = FALSE;
    return color_status;
}

/**
 * @description - Clear specific text on console.
 * @param -
 * text_x [in] X-axis of text.
 * text_y [in] Y-axis of text.
 * text_len [in] Length of text.
 * @return -If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
*/

BOOL GT_ClearConsoleText(SHORT text_x, SHORT text_y, SIZE_T text_len)
{
    UINT index = NIL;
    BOOL clear_status = FALSE;
    LPCSTR clear_ch = " ";

    gt_try {
        //move cursor to text position.
        GT_SetConsoleCursorXY(text_x,text_y);

        for (index = 0; index < text_len; index++)
        {
            if ((clear_status = GT_WriteConsole("%s",clear_ch)) != lstrlen(clear_ch)) {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }
        }
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }

    gt_private_cmethod = FALSE;
    return clear_status != NIL;
}

/**
 * @description - Clear whole screen of current console.
 * @param - None
 */


VOID GT_ClearConsole()
{
    COORD coord_screen = { 0,0 };
    DWORD chars_written;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dw_con_size;
    HANDLE console_handle = GT_GetConsoleStdHandle(STD_OUTPUT_HANDLE);

    gt_try {
        //Get the number of character cells in the current buffer.
        if (!GetConsoleScreenBufferInfo(console_handle, &csbi))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

        dw_con_size = csbi.dwSize.X * csbi.dwSize.Y;

        // Fill the entire screen with blanks.
        if (!FillConsoleOutputCharacter(console_handle,(TCHAR)' ',dw_con_size,coord_screen,&chars_written))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

        // Get the current text attribute.
        if (!GetConsoleScreenBufferInfo(console_handle,&csbi))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

        // Set the buffer's attributes accordingly.
        if (!FillConsoleOutputAttribute(console_handle,csbi.wAttributes,dw_con_size,coord_screen,&chars_written))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

        // Put the cursor at its home coordinates.
        GT_SetConsoleCursorXY(coord_screen.X,coord_screen.Y);
    }

    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
}


/**
 * @description - Sets the title for the current console window.
 * @param - window_name [in] Type: LPCTSTR - The string to be displayed on the title bar of the console window. The total size must be less than 64K.
 * @return - If the function succeeds, the return value is nonzero. If the function fails, the return value is zero.
 * NOTE : When the process terminates, the system restores the original console title.
 */

BOOL GT_SetWindowTitle(LPCSTR window_name)
{
    BOOL title_status = NIL;

    gt_try {
        if (!(title_status = SetConsoleTitle(window_name)))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
    return title_status;
}


/**
 * @description - Changes an attribute of the specified window..
 * @param - n_index [in] Type: int
 * The zero-based offset to the value to be set. Valid values are in the range zero through the number of bytes of extra window memory, minus the size of an integer.
 *to specify other values use Macros GWL_EXSTYLE,GWL_HINSTANCE etc.

 *new_style [in] Type: LONG_PTR
 new window style ex : WS_MAXIMIZEBOX,WS_OVERLAPPEDWINDOW etc.

 * @return - If the function succeeds, the return value is the previous value of the specified offset.
 *	If the function fails, the return value is zero
 */

LONG_PTR GT_SetWindowStyle(INT n_index, LONG_PTR new_style)
{
    /*The function does not clear the last error information. if last value was zero.*/
    SetLastError(NO_ERROR);
    HWND console_hwnd = NULL;
    LONG_PTR style_ptr = NIL;

    gt_try {
        console_hwnd = GT_GetConsoleWindowHandle();
        style_ptr = SetWindowLongPtr(console_hwnd,n_index,new_style);

        if (style_ptr)
        {
            if (SetWindowPos(console_hwnd,NIL,NIL,NIL,NIL,NIL,SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME)) {
                //show window after updating style.
                ShowWindow(console_hwnd,SW_SHOW);
            } else {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }
        } else
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
    return style_ptr;
}

/**
 * @description - Resize current console window to new size.
 * @param -
 * win_xPos [in] - The new position of the left side of the window.
 * win_yPos [in] -  The new position of the top of the window.
 * win_width [in] -  The new width of the window.
 * win_height [in] -  The new height of the window.
 * @return - If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
*/

BOOL GT_ResizeWindow(LONG win_xPos, LONG win_yPos, LONG win_width, LONG win_height)
{
    //set console window size.
    HWND console_hwnd = GT_GetConsoleWindowHandle();
    RECT rect = { win_xPos,win_yPos,win_height,win_width};
    return MoveWindow(console_hwnd, rect.top, rect.left, rect.bottom - rect.top, rect.right - rect.left, TRUE);
}

/**
 * @description - Changes the size, position, and Z order of window.
 * @param -
 *win_xPos [in] - The new position of the left side of the window, in client coordinates.
 *win_yPos [in] - The new position of the top of the window, in client coordinates.
 *win_width [in] -The new width of the window, in pixels.
 *win_height [in] - The new height of the window, in pixels.
 * @return - If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
*/

BOOL GT_MoveWindow(INT win_xPos, INT win_yPos, INT win_width, INT win_height)
{
    HWND console_hwnd = GT_GetConsoleWindowHandle();
    BOOL pos_status = SetWindowPos(console_hwnd, NIL, win_xPos, win_yPos, win_width, win_height, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);

    //show window after updating
    ShowWindow(console_hwnd, SW_SHOW);

    return pos_status;
}

/**
 * @description - Disable single/multiple window style of current console window.
  * @param - n_index [in] Type: int
 * The zero-based offset to the value to be set. Valid values are in the range zero through the number of bytes of extra window memory, minus the size of an integer.
 *to specify other values use Macros GWL_EXSTYLE,GWL_HINSTANCE etc.

 *disable_style [in] Type: LONG_PTR
 *new window style you wish to disable ex : WS_MAXIMIZEBOX,WS_VSCROLL etc. (NOTE : Mutltiple styles like (WS_MAXIMIZEBOX | WS_SIZEBOX | WS_MINIMIZEBOX) are valid)

 * @return - If the function succeeds, the return value is the previous value of the specified offset.
 *	If the function fails, the return value is zero
 */

BOOL GT_DisableWindowStyle(INT n_index, LONG_PTR disable_style)
{
    /*The function does not clear the last error information. if last value was zero.*/
    SetLastError(NO_ERROR);

    HWND console_hwnd = GT_GetConsoleWindowHandle();
    DWORD w_style = GetWindowLongPtr(console_hwnd, n_index)&~disable_style;
    BOOL disable_status = FALSE;

    gt_try {
        if (!w_style)
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
        disable_status = SetWindowLong(console_hwnd,n_index,w_style) != NIL;
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
    return disable_status;
}

/**
 * @description - Disable vertical or horizontal scrollbar of current console.
 * @param - scrollbar_type [in] - Scrollbar type ex : WS_VSCROLL , WS_HSCROLL,
 * to disable both scroll bars use bitwise OR and provide input like (WS_VSCROLL | WS_HSCROLL)
 * @return - If the function succeeds, the return value is nonzero.
 *	If the function fails, the return value is zero
*/

BOOL GT_DisableWindowScrollbar(LONG_PTR scrollbar_type)
{
    HWND console_hwnd = GT_GetConsoleWindowHandle();
    scrollbar_type = (scrollbar_type == WS_VSCROLL) ? (WS_OVERLAPPEDWINDOW | WS_HSCROLL) :
                     (scrollbar_type == WS_HSCROLL) ? (WS_OVERLAPPEDWINDOW | WS_VSCROLL) :
                     ((scrollbar_type == (WS_VSCROLL | WS_HSCROLL))) ? (WS_OVERLAPPEDWINDOW) : WS_OVERLAPPEDWINDOW | WS_VSCROLL;

    BOOL disable_status = GT_SetWindowStyle(GWL_STYLE, GetWindowLong(console_hwnd, GWL_STYLE) & scrollbar_type) != NIL;
    return disable_status;
}


/**
 * @description - Get screen information of current window.
 * @param -
 * wParam [in] - Window parameter type, use Enum WINDOW_PARAM to provide single or multiple values.
 * Multiple values ex : WP_WINDOW_SIZE | WP_WINDOW_RECT.
 * pWsize [out] - Pointer to COORD that will receive window current size co-ordinates.
 * pCursor [out] - Pointer to COORD that will receive cursor co-ordinates.
 * pAttribs [out] - Pointer to WORD that will receive window attributes.
 * psrctWindow [out] - Pointer to SMALL_RECT that will receive window rect co-ordinates.
 * pMaxWindow [out] - Pointer to COORD that will receive window max size co-ordinates.

 * @return - If the function succeeds, the return value is nonzero.
 *	If the function fails, the return value is zero
*/

BOOL GT_GetWindowScreenInfo(INT wParam, PCOORD pWsize, PCOORD pCursor, PWORD pAttribs, PSMALL_RECT psrctWindow, PCOORD pMaxWindow)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    BOOL csbi_status = FALSE;

    gt_try {
        csbi_status = GetConsoleScreenBufferInfo(GT_GetConsoleStdHandle(STD_OUTPUT_HANDLE),&csbi);
        if (csbi_status)
        {
            if (wParam & WP_WINDOW_SIZE) {
                *(PSHORT)((INT)pWsize + NIL) = csbi.dwSize.X;
                *(PSHORT)((INT)pWsize + sizeof(SHORT)) = csbi.dwSize.Y;
            }

            if (wParam & WP_WINDOW_CURSOR_POS) {
                *(PSHORT)((INT)pCursor + NIL) = csbi.dwCursorPosition.X;
                *(PSHORT)((INT)pCursor + sizeof(SHORT)) = csbi.dwCursorPosition.Y;
            }

            if (wParam &  WP_WINDOW_ATTRIBS) {
                *(PWORD)(pAttribs) = csbi.wAttributes;
            }

            if (wParam &  WP_WINDOW_RECT) {
                *(PSHORT)((INT)psrctWindow + NIL) = csbi.srWindow.Left;
                *(PSHORT)((INT)psrctWindow + sizeof(SHORT) * 1) = csbi.srWindow.Top;
                *(PSHORT)((INT)psrctWindow + sizeof(SHORT) * 2) = csbi.srWindow.Right;
                *(PSHORT)((INT)psrctWindow + sizeof(SHORT) * 3) = csbi.srWindow.Bottom;
            }

            if (wParam &  WP_WINDOW_MAX_SIZE) {
                *(PSHORT)((INT)pMaxWindow + NIL) = csbi.dwMaximumWindowSize.X;
                *(PSHORT)((INT)pMaxWindow + sizeof(SHORT)) = csbi.dwMaximumWindowSize.Y;
            }
        }

        else
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

    }

    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
    return csbi_status;
}

/**
 * @description - Show/hide console cursor.
 * @param - show_flag [in] - Show flag use TRUE to show cursor or FALSE to hide cursor.
 * @return - If the function succeeds, the return value is nonzero.
 * If the function fails, the return value is zero
*/

BOOL GT_ShowConsoleCursor(BOOL show_flag)
{
    HANDLE console_handle = GT_GetConsoleStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO   cursor_info;
    BOOL cursor_result = FALSE;

    gt_try {
        if (GetConsoleCursorInfo(console_handle, &cursor_info))
        {
            cursor_info.bVisible = show_flag;
        }

        else
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
        cursor_result = SetConsoleCursorInfo(console_handle,&cursor_info);
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }

    gt_private_cmethod = FALSE;
    return cursor_result;
}

/**
 * @description - Print string/text at specified position on console.
 * @param - lp_string [in] - String to print, string_x [in] - X-axis of string, string_y [in] - Y-axis of string
 * @return -  If the function succeeds, the return value is number of characters written on console.
 * If the function fails, the return value is zero
*/

DWORD GT_WriteConsoleXY(INT string_x, INT string_y, LPCSTR format, ...)
{
    DWORD dw_nChars = NIL;
    HANDLE console_handle = GT_GetConsoleStdHandle(STD_OUTPUT_HANDLE);
    CHAR format_buf[0x100] = { NUL };

    va_list va_alist;
    va_start(va_alist, format);
    wvsprintf(format_buf, format, va_alist);
    va_end(va_alist);

    gt_try {

        GT_SetConsoleCursorXY(string_x,string_y);

        if (!WriteConsole(console_handle,format_buf, lstrlen(format_buf),&dw_nChars, NULL))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
    return dw_nChars;
}

/**
 * @description - Get the current font size of console.
 * @param - handle - A handle to the console screen buffer. The handle must have the GENERIC_WRITE.
  *max_window - If this parameter is TRUE, font information is set for the maximum window size. If this parameter is FALSE, font information is set for the current window size.
  * coord_size[Out] - Buffer to receive current size of fonts. Must be of type COORD.
 * @return -If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
*/

BOOL GT_GetFontSize(HANDLE handle,BOOL max_window,PCOORD coord_size)
{
    BOOL font_status = FALSE;
    CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };

    if (!GetCurrentConsoleFontEx(handle,max_window,&font))
    {
        font_status =  FALSE;
    }
    else
    {
        *coord_size = font.dwFontSize;
        font_status = TRUE;
    }

    return font_status;
}

/**
 * @description - Set the new font size of console.
 * @param
 * size_percent - Size of fonts in percent must be a float value.
  *max_window - If this parameter is TRUE, font information is set for the maximum window size. If this parameter is FALSE, font information is set for the current window size.
 * @return -If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
*/

BOOL GT_SetFontSize(FLOAT size_percent,BOOL max_window)
{
    BOOL font_status = FALSE;
    CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };
    HANDLE handle = GT_GetConsoleStdHandle(STD_OUTPUT_HANDLE);
    COORD coord_size;

    if (!GetCurrentConsoleFontEx(handle,max_window, &font))
    {
        font_status = FALSE;
    }

    else if (GT_GetFontSize(handle,max_window,&coord_size))
    {
        /* Grow by coord_size percent% */
        coord_size.X += (SHORT)(coord_size.X * size_percent);
        coord_size.Y += (SHORT)(coord_size.Y * size_percent);
        font.dwFontSize = coord_size;
        font_status = TRUE;
    }

    if (!SetCurrentConsoleFontEx(handle,max_window, &font))
    {
        font_status = FALSE;
    }

    return font_status;
}

/**
 * @description - Write the text with font size.
 * @param
 * size_percent - Size of fonts in percent must be a float value.
  *max_window - If this parameter is TRUE, font information is set for the maximum window size. If this parameter is FALSE, font information is set for the current window size.
 * format - Text to print on console with or without arguments.
*/

void GT_WriteConsoleFont(FLOAT size_percent,BOOL max_window,LPCSTR format, ...)
{
    GT_SetFontSize(size_percent,max_window);
    GT_WriteConsole(format);
}

/**
 * @description - Flush the input buffer in case of character.
 * @param
 * ch - Character designated with input buffer.
*/

VOID GT_FlushInputBuffer(CHAR ch)
{
    while(ch == '\n' || ch == '\r')
        GT_ReadConsole(&ch,1,CONSOLE_READ_CHAR);
}

/**
 * @description - Flushes the console input buffer. All input records currently in the input buffer are discarded.
 * @param
 * A handle to the console input buffer. The handle must have the GENERIC_WRITE access right.
* @return -If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
*/

BOOL GT_FlushConsoleInputBuffer(HANDLE console_handle)
{
    BOOL flush_status = FALSE;
    if(console_handle == INVALID_HANDLE_VALUE)
    {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
        flush_status = FALSE;
    }
    else
    {
        FlushConsoleInputBuffer(console_handle);
        flush_status = TRUE;
    }
    return  flush_status;
}


/**
 * @description - Shows loading bar on current console.
 * @param - bar_breadth [in] - Breadth of loading bar.
 * bar_speed [in] - Speed of loading bar.
 * bar_color [in] - Color of loading bar.
 * bar_x [in] - X position of loading bar.
 * bar_y [in] - Y position of loading bar.
 * border_color [in] - Color of outer border of loading bar.
*/

VOID GT_ShowLoadingBar(UINT bar_breadth, UINT bar_speed, UINT bar_color, UINT bar_x, UINT bar_y, UINT border_color)
{

    /*==========================*/
    /* Setting up Loading Bar   */
    /*==========================*/

    INT height_index = NIL, breadth_index = -1, _bar_breadth = (INT)bar_breadth;
    UINT bar_height = 2;
    UCHAR loading_bar = 219; //EASCII code 219 ( Block, graphic character )
    gt_private_cmethod = TRUE;

    gt_try {
        GT_CreateBorder(bar_height,_bar_breadth,bar_x,bar_y,border_color); //Create an Empty border for Loading Bar

        GT_SetConsoleCursorXY(bar_x + 1,bar_y + 1); // Move the Axis Inside the border
        GT_SetConsoleTextColor(bar_color); //bar_color of Loading Bar


        while (height_index++ < _bar_breadth)
        {
            if (GT_WriteConsole("%c",loading_bar) == 1) {
                Sleep(bar_speed);
            }

            else {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }
        }


        /*===========================*/
        /* Deleting of Loading Bar. */
        /*==========================*/
        for (height_index = bar_height; height_index >= 0;)
        {
            // Deleting line by line by changing only bar_y-Axis. Bottom to top deleting scheme..
            GT_SetConsoleCursorXY((bar_x + bar_height) + _bar_breadth, bar_y + height_index--);

            //Go bar_breadth times Back .. i.e starting position of lower left corner of border
            while (breadth_index++ <= _bar_breadth) {
                if (GT_WriteConsole("\b") != 1) {
                    gt_private_cmethod = TRUE;
                    gt_throw(GT_GetCError());
                }
            }

            //Now Write there empty char forcing it to delete.
            while ((breadth_index--) != -1) {
                if (GT_WriteConsole("%c",' ') != 1) {
                    gt_private_cmethod = TRUE;
                    gt_throw(GT_GetCError());
                }
            }
        }

        //Reset co-ordinates back to normal.
        COORD original_coord = {0,bar_y};
        if (!GT_ResetConsole(NIL,NIL,&original_coord))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
}

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

DWORD GT_PrintMenu(LPCSTR menu_option, LPCSTR menu_hotkey, LPCSTR menu_separator, WORD op_color, WORD hkey_color, SHORT menu_op_x, SHORT menu_op_y)
{
    DWORD dw_nChars = NIL;
    SIZE_T menu_len = NIL;
    WORD wAttrib = NIL;
    COORD coord = { 0,0 };
    PCOORD pcoord = NULL;
    DWORD dw_wpFlags = NIL;
    UINT index = NIL;

    gt_try {
        //calculate menu length.
        menu_len = (lstrlen(menu_option) + lstrlen(menu_separator));

        if ((menu_op_x != -1 || menu_op_y != -1))
        {

            //disable Window cursor position flags if co-ordinates not provided.
            dw_wpFlags = (WP_WINDOW_ATTRIBS | WP_WINDOW_CURSOR_POS);
            pcoord = &coord;
        }

        else
        {
            dw_wpFlags = (WP_WINDOW_ATTRIBS);
            pcoord = NULL;
        }

        //store color and cursor properties to reset later.
        if (!GT_GetWindowScreenInfo(dw_wpFlags,NULL,pcoord,&wAttrib,NULL,NULL))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }


        //set menu option text color.
        if (!GT_SetConsoleTextColor(op_color))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }


        //check for x,y positions provided and print Menu option accordingly.
        dw_nChars = (menu_op_x != -1 || menu_op_y != -1) ?
        //Print menu option and hotkey at provided x,y position.
        GT_WriteConsoleXY(menu_op_x,menu_op_y,"%s%s",menu_option,menu_separator)
        :
        //Print menu option at default x,y position.
        GT_WriteConsole("%s%s",menu_option,menu_separator);


        if (dw_nChars == menu_len)
        {
            //add spaces moved by Tab to no. of chars written.
            for (index = 0; index < lstrlen(menu_separator); index++) {
                if (menu_separator[index] == '\t')
                    dw_nChars += (0x4 - 1);
            }

            //set hotkey option text color.
            if (!GT_SetConsoleTextColor(hkey_color)) {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }


            //check for x,y positions provided and print Hotkey option accordingly.
            dw_nChars += (menu_op_x != -1 || menu_op_y != -1) ?
                         //Print menu option and hotkey at provided x,y position.
                         GT_WriteConsoleXY(menu_op_x + dw_nChars,menu_op_y,"%s",menu_hotkey)
                         :
                         //Print hotkey at default x,y position.
                         GT_WriteConsole("%s",menu_hotkey);

            if (!(dw_nChars > 0)) {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }
        }

        else
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

//reset back color attributes and cursor position.
        if (!GT_ResetConsole(wAttrib,NIL,pcoord))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
    return dw_nChars;
}

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


DWORD GT_CreateMenu(LPCSTR menu_options[], LPCSTR menu_hotkeys[], LPCSTR menu_separators[], SIZE_T sz_menu, PWORD ops_colors, PWORD hkeys_color, PSHORT menu_ops_x, PSHORT menu_ops_y)
{
    UINT menu_index = NIL;
    DWORD dw_nChars = NIL;

    /*Print menu items one by one,
    and don't check for error because its already handled in GT_PrintMenu*/

    for (menu_index = 0; menu_index < sz_menu; menu_index++) {
        dw_nChars += GT_PrintMenu(menu_options[menu_index], menu_hotkeys[menu_index], menu_separators[menu_index], ops_colors[menu_index], hkeys_color[menu_index], menu_ops_x[menu_index], menu_ops_y[menu_index]);
    }

    return dw_nChars;
}


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
BOOL GT_PlaySound(LPCTSTR sound_path, DWORD sound_flag)
{
    BOOL play_status = FALSE;

    gt_try {
        if (!(play_status = PlaySound(sound_path,NULL,sound_flag)))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
    return play_status;
}

/**
 * @description - Stop playback of a sound that is playing asynchronously.
 * @param - None.
 * @return -  Returns TRUE if successful or FALSE otherwise.
 *NOTE : If you get error undefined reference to PlaySound then include this flag '-lwinmm' (without quotes) in compiler options.
*/

BOOL GT_StopSound()
{
    BOOL stop_status = FALSE;
    gt_try {
        if (!(stop_status = PlaySound(NULL, 0, 0)))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }

    gt_private_cmethod = FALSE;
    return stop_status;
}
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

HANDLE GT_GetConsoleStdHandle(DWORD dw_std_device)
{
    gt_std_handle = GetStdHandle(dw_std_device);

    gt_try {
        if (gt_std_handle == INVALID_HANDLE_VALUE)
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }

    gt_private_cmethod = FALSE;
    return gt_std_handle;
}

/**
 * @description - Retrieves the window handle used by the console associated with the calling process.
 * @return - Handle to the window used by the console associated with the calling process
 *	or NULL if there is no such associated console.
 */

HWND GT_GetConsoleWindowHandle()
{
    gt_console_window_handle = GetConsoleWindow();

    gt_try {
        if (gt_console_window_handle == NULL)
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }

    gt_private_cmethod = FALSE;
    return gt_console_window_handle;
}


/**
 * @description - Set console cursor to specified postion on console.
 * @param - cursor_x [in] - X-axis of console, cursor_y [in] - y-axis of console
 * @return - If the function succeeds, the return value is nonzero. If the function fails, the return value is zero
*/

BOOL GT_SetConsoleCursorXY(SHORT cursor_x, SHORT cursor_y)
{
    BOOL cursor_staus = FALSE;
    HANDLE console_handle = GT_GetConsoleStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { coord.X = cursor_x,coord.Y = cursor_y };

    gt_try {
        if (!(cursor_staus = SetConsoleCursorPosition(console_handle,coord)))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError())
        }
    }
    gt_catch(gt_cerror_code) {
        gt_private_cmethod = TRUE;
        GT_ShowCError(GT_GetCError(), FUNC_NAME, LINE_NO);
    }

    gt_private_cmethod = FALSE;
    return cursor_staus;
}


/**
 * @description - Blink console text.
 * @param - lp_text [in] - Text to blink,
 * repeat [in] - Number of times to print text on console
 * speed [in] - Speed of blinking text.
*/

VOID GT_BlinkConsoleText(LPCSTR lp_text, UINT repeat, UINT speed)
{
    UINT repeat_index = NIL, index = NIL, sz_text = lstrlen(lp_text);

    gt_try {

        if (speed == 0)
            GT_ShowCWarning("Speed is negligible \n");

        for (repeat_index = 1; repeat_index <= repeat; repeat_index++)
        {
            if (GT_WriteConsole("%s",lp_text) == lstrlen(lp_text)) {
                if (GT_WriteConsole("\r") == 1) { //Move Cursor Back to Starting Position
                    Sleep(speed);
                }
            }

            else {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }

            for (index = 0; index < sz_text; index++)
                if (GT_WriteConsole("%c",' ') != 1) { //print space to replace text to blink.
                    gt_private_cmethod = TRUE;
                    gt_throw(GT_GetCError());
                }

            Sleep(speed);

            if (GT_WriteConsole("\r") != 1);//Move Cursor Back to Starting Position
            {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }
        }

        if (GT_WriteConsole("%s",lp_text) != lstrlen(lp_text)); // Print String in the End After Blinking
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
}

/**
 * @description - Write formatted text on console .
 * @param - format [in] - Text to print.
 * @return -  If the function succeeds, the return value is number of characters written on console.
 * If the function fails, the return value is zero.
*/

DWORD GT_WriteConsole(LPCSTR format, ...)
{
    DWORD dw_nChars = NIL;
    HANDLE console_handle = GT_GetConsoleStdHandle(STD_OUTPUT_HANDLE);
    CHAR format_buf[0x100] = { NUL };

    va_list va_alist;
    va_start(va_alist, format);
    wvsprintf(format_buf, format, va_alist);
    va_end(va_alist);

    gt_try {
        if (!WriteConsole(console_handle,format_buf,lstrlen(format_buf),&dw_nChars, NULL))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
    return dw_nChars;
}

/**
 * @description - Reads input from the console.
 * @param - lp_buf [out] - A pointer to a buffer that receives the data read from the console.
 * dw_nChars [in] - The number of characters to be read.
 * The size of the buffer pointed to by the lp_buf parameter should be at least dw_nChars * sizeof(TCHAR) bytes
 * dw_dType [in] - Data-type to read from console, use Enum CONSOLE_READ_TYPE to provide values.
 * @return -  If the function succeeds, the return value is number of characters read from console.
 * If the function fails, the return value is zero.

 * NOTE - Only Integer,character and string data-types are supported.
*/

DWORD GT_ReadConsole(LPVOID lp_buf, DWORD dw_nChars, DWORD dw_dType)
{
    HANDLE console_handle = GT_GetConsoleStdHandle(STD_INPUT_HANDLE);
    DWORD dw_rChars = NIL;
    UINT index = NIL, places = 0x1, n_places = NIL, uNum = NIL;

    gt_try {
        if (!ReadConsole(console_handle,lp_buf,dw_nChars,&dw_rChars,NULL))
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

        else
        {

            if (dw_dType == CONSOLE_READ_CHAR) {
                ;//No further processing required for character.
            }

            //Hack to read Integer from console.
            else if (dw_dType == CONSOLE_READ_INT) {
                //get places of number.
                for (n_places = 0; n_places < (dw_nChars - 1); n_places++)
                    places *= 0xA;

                //Convert character ASCII num to Integer and add places.
                for (index = 0; index < dw_nChars; index++) {
                    uNum += (*((LPSTR)(lp_buf)+index) - (0x30)) * places;
                    places /= 0xA;
                }

                //copy converted number back to buffer.
                *(PUINT)lp_buf = uNum;
            }

            else if (dw_dType == CONSOLE_READ_STR) {
                //remove Carriage return 'CR' from buffer.
                (*((LPSTR)(lp_buf)+(dw_rChars - 2))) = NUL;

                //remove Linefeed 'LF' from buffer.
                (*((LPSTR)(lp_buf)+(dw_rChars - 1))) = NUL;
            }

        }

    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }

    gt_private_cmethod = FALSE;
    return dw_rChars;
}


/**
 * @description - Reset text color attributes / console cursor position.
 * @param - txt_color [in - optional] - Text foreground or background color to reset.
 * console_color [in - optional] - Console background color to reset.
 * pcoord [in - optional] - Pointer to Cursor co-ordinates to reset.
 * @return -  If the function succeeds, the return value is nonzero.
 * If the function fails, the return value is zero.
*/

BOOL GT_ResetConsole(WORD txt_color, WORD console_color, PCOORD pcoord)
{
    BOOL reset_status = FALSE;

    gt_try {
        //reset back original text color attribute.
        if (txt_color != NIL)
        {
            if (!(reset_status = GT_SetConsoleTextColor(txt_color))) {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }
        }

        if (console_color != NIL)
        {
            //reset back original console background color attribute.
            if (!(reset_status = GT_SetConsoleBGColor(console_color))) {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }
        }


        if (pcoord != NULL)
        {
            //reset back original co-ordinates.
            COORD coord;
            coord.X = *(PSHORT)((INT)pcoord + NIL);
            coord.Y = *(PSHORT)((INT)pcoord + sizeof(SHORT));

            if (!(reset_status = GT_SetConsoleCursorXY(coord.X,coord.Y))) {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }
        }
    }
    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }

    gt_private_cmethod = FALSE;
    return reset_status;
}

/**
 * @description - Init the trainer window with name and size.
 * @param
 * window_name - Name of the trainer window.
  *x,y - Positions of trainer x and y coords.
  *width,height - Width and Height of trainer.
*/

VOID GT_InitTrainerWindow(LPCSTR window_name, int x, int y, int width, int height)
{
    /*Defining trainer bg and fg colors*/
    const BG_COLORS TRAINER_FG = FG_LIGHTCYAN;
    const BG_COLORS TRAINER_BG = FG_BLACK;

    GT_InitTrainerWindowEx(window_name, x, y, width, height, TRAINER_BG, TRAINER_FG);
}

/**
 * @description - Init the trainer window with name and size with provided theme.
 * @param
 * window_name - Name of the trainer window.
  *x,y - Positions of trainer x and y coords.
  *width,height - Width and Height of trainer.
  *TRAINER_BG,TRAINER_FG - Background and foreground colors of trainer to apply theme.
*/
VOID GT_InitTrainerWindowEx(LPCSTR window_name, int x, int y, int width, int height, BG_COLORS TRAINER_BG, FG_COLORS TRAINER_FG)
{
    //set console background color.
    GT_SetConsoleBGColor(TRAINER_BG);

    //Set console windows text.
    GT_SetWindowTitle(window_name);
    GT_SetConsoleTextColor(TRAINER_FG | TRAINER_BG);

    //set console window size.
    GT_ResizeWindow(x, y, width, height);

    //set console window style.
    GT_DisableWindowStyle(GWL_STYLE, WS_SIZEBOX | WS_MAXIMIZEBOX);

    //remove vertical scroll bar.
    GT_DisableWindowScrollbar(WS_VSCROLL);

    //remove blinking cursor.
    GT_ShowConsoleCursor(FALSE);
}


/*=============================*/
/* ==== PRIVATE-METHODS.===== */
/*===========================*/

static VOID GT_ShowCError(DWORD gt_cerror_code, LPCSTR func_name, DWORD line_no)
{
    if (GT_IsPrivateCMethod(gt_private_cmethod, FUNC_NAME, LINE_NO)) {
        CHAR err_msg_buf[0xFF] = { '\0' };
        CHAR sys_err_buf[0xFF] = { '\0' };
        LPSTR ptr = NUL;

        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL, gt_cerror_code,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                      sys_err_buf, sizeof(sys_err_buf), (va_list *)NULL);

        // Trim the end of the line and terminate it with a null
        ptr = sys_err_buf;
        while ((*ptr > 31) || (*ptr == 9))
            ++ptr;

        do {
            *ptr-- = '\0';
        } while ((ptr >= sys_err_buf) && ((*ptr == '.') || (*ptr < 33)));

        //copy error from getLastError() to error buffer.
        wsprintf(err_msg_buf, "\nINFO : %s method failed!\nREASON : (%s)\nLINE. : occurred at line no. %d\n", func_name, sys_err_buf, line_no);

        //Show error from error buffer.
        MessageBox((HWND)NULL, err_msg_buf, "ERROR!", MB_ICONERROR);
    }
}

static VOID GT_ShowCInfo(LPCSTR info_message)
{
    if (GT_IsPrivateCMethod(gt_private_cmethod, FUNC_NAME, LINE_NO)) {
        MessageBox((HWND)NULL, info_message, "INFO!", MB_ICONINFORMATION);
    }
}

static VOID GT_ShowCWarning(LPCSTR warning_message)
{
    if (GT_IsPrivateCMethod(gt_private_cmethod, FUNC_NAME, LINE_NO)) {
        MessageBox((HWND)NULL, warning_message, "WARNING!", MB_ICONWARNING);
    }
}

static BOOL GT_IsPrivateCMethod(BOOL gt_private_cmethod, LPCSTR proc_name, INT line_no)
{
    if (gt_private_cmethod)
        return TRUE;

    else {
        CHAR err_buf[0x100] = { NUL };
        wsprintf(err_buf, "ERROR : %s method failed!\nREASON : Access to private method! (ERROR_INVALID_FUNCTION) \nLINE : occurred at line no. %d", proc_name, line_no);
        MessageBox((HWND)NULL, err_buf, "ERROR!", MB_ICONERROR);

        DWORD exit_code = NIL;
        GetExitCodeProcess(GetCurrentProcess(), &exit_code);
        ExitProcess(exit_code);
    }
}

static VOID GT_CreateBorder(UINT border_height, UINT border_breadth, UINT border_x, UINT border_y, UINT border_color)
{
    //Border char's in EASCII values.
    UCHAR upper_left_corner = 201, upper_right_corner = 187, border_breadth_char = 205, border_height_char = 186,
          bottom_left_corner = 200, bottom_right_corner = 188;
    INT index = 1;

    border_breadth += ((border_height + 1) - 2); //Makes Perfect Square....

    if (border_height == 0 || border_breadth == 0) {
        gt_private_cmethod = TRUE;
        GT_ShowCWarning("border height or border breadth can't be zero\n");
    }

    gt_try {

        //set border color and position.
        GT_SetConsoleTextColor(border_color);
        GT_SetConsoleCursorXY(border_x,border_y);


        // Upper Left Corner
        if (GT_WriteConsole("%c",upper_left_corner) != 1)
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

        while (index++ <= border_breadth)
        {
            if (GT_WriteConsole("%c",border_breadth_char) != 1) { // border breadth
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }
        }


        if (GT_WriteConsole("%c\n",upper_right_corner) != 2) //Upper Right Corner
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

        index = 1; // Reset Back
        while (index <= border_height)
        {
            GT_SetConsoleCursorXY(border_x,border_y + index); //Goto Left side of border

            if (GT_WriteConsole("%c",border_height_char) != 1) { //border height
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }


            GT_SetConsoleCursorXY(border_breadth + (border_x + 1),border_y + (index++)); //Goto Right Side of border

            if (GT_WriteConsole("%c",border_height_char) != 1) { //border height
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }


            if (GT_WriteConsole("\n") != 1) {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }

        }

        index = 1;

        GT_SetConsoleCursorXY(border_x,border_y + border_height);

        if (GT_WriteConsole("%c",bottom_left_corner) != 1)  //Bottom left corner
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }

        while (index++ <= border_breadth)
            if (GT_WriteConsole("%c",border_breadth_char) != 1) //border_breadth
            {
                gt_private_cmethod = TRUE;
                gt_throw(GT_GetCError());
            }

        if (GT_WriteConsole("%c\n",bottom_right_corner) != 2)  //Bottom Right Corner
        {
            gt_private_cmethod = TRUE;
            gt_throw(GT_GetCError());
        }
    }

    gt_catch(gt_cerror_code) {
        GT_ShowCError(gt_cerror_code, FUNC_NAME, LINE_NO);
    }
    gt_private_cmethod = FALSE;
}

static DWORD GT_GetCError()
{
    if (GT_IsPrivateCMethod(gt_private_cmethod, FUNC_NAME, LINE_NO)) {
        if ((GetLastError() != ERROR_SUCCESS || GetLastError() == ERROR_ALREADY_EXISTS)) {
            gt_cerror_code = GetLastError();
        }
    }
    return gt_cerror_code;
}
