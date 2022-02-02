//
// Created by Frityet on 2022-01-25.
//

#ifndef HANGMANC_ANSI_
#define HANGMANC_ANSI_

#include <stdio.h>

#define ESC                 \x1B
#define _STRMACRO(_mac)     #_mac
#define STRMACRO(_mac)      _STRMACRO(_mac)
#define PRINT_MACRO(_seq)   printf(STRMACRO(_seq))
#define ESC_SEQ(_seq)       PRINT_MACRO(ESC[_seq)

#define PRINT_MSG_MIDDLE(_x, _msg, ...) setcursorposition((_x), 50 - strlen(_msg) / 2); printf(_msg, __VA_ARGS__)

static inline void clearscreen(void)
{
    ESC_SEQ(2J);
}

static inline void clearline(void)
{
    ESC_SEQ(2K);
}

static inline void setwindowsize(int x, int y)
{
    printf("\x1B[4;%d;%dt", x, y);
}

static inline void setcursorposition(int x, int y)
{
    printf("\x1B[%d;%dH", x, y);
}

static inline void linedown(void)
{
    ESC_SEQ(1B);
}

#endif //HANGMANC_ANSI_
