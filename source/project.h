/*
Copyright (C) 2005 Ondrej Danek

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef __PROJECT_H
#define __PROJECT_H

#include "core/co_core.h"

#define M1_FAZ_SPEED    7
#define M1_FAZ_MAX      8
#define M1_JUMP_MAX     40
#define M1_ORI_LEFT     0
#define M1_ORI_RIGHT    1

#define M1_FILE_IMAGE   "data/mouse1.art"
#define M1_FILE_NAMES   "data/mouse1.dat"
#define M1_FILE_PAL     "data/vga.pal"

#define M1_CL_BPP       32
#define M1_CL_AA        0

enum
{
    M1_DAT_FORE,
    M1_DAT_BACK,
    M1_DAT_IMG,
    M1_DAT_SCR_GL,

    M1_DATA_SIZE
};

// Loop quit kody
enum
{
    M1_QC_LOOP,
    M1_QC_QUIT,
    M1_QC_PLAY
};

enum
{
    M1_FORE,
    M1_BACK
};

enum
{
    M1_NONE,
    M1_NEXT,
    M1_DEAD,
    M1_EXIT
};

typedef struct
{
    int X;
    int Y;
    int F;
    int S;
    int J;
    int O;
    int SF;
    int Live;
} m1ManStruct;

extern myBYTE       *m1Data[M1_DATA_SIZE], m1Pal[256][3];
extern int          m1Pos[2], m1End, m1LoopQuitCode;
extern m1ManStruct  m1Man;
extern bool         m1InMenu;
extern vidRgbBuf_s  m1RgbBuf;

///////////////////////////////// Mouse1 /////////////////////////////////
int     P_DoLoop            (void (*move)(void), void (*draw)(void));

///////////////////////////////// Load ///////////////////////////////////
void    load_Prepare        (m1ManStruct *m);
void    load_Data           (const char *fl, myBYTE *pt1, myBYTE *pt2);

///////////////////////////////// Draw ///////////////////////////////////
void    draw_ClearBuffer    (void);
void    draw_BlitBuffer     (void);
void    draw_Draw           (void);

///////////////////////////////// Hlavni /////////////////////////////////
void    hlavni_PlayLoop     (void);

///////////////////////////////// Menu /////////////////////////////////
void    menu_Menu           (void);
void    menu_Draw           (void);
void    menu_RedrawLives    (int ziv);
void    menu_Play           (void);

///////////////////////////////// Move /////////////////////////////////
bool    move_Can            (int X, int Y);
bool    move_CanFall        (int X, int Y);
void    move_Doleva         (m1ManStruct *m);
void    move_Doprava        (m1ManStruct *m);
void    move_Nahoru         (m1ManStruct *m);
void    move_Dolu           (m1ManStruct *m);
void    move_Jump           (m1ManStruct *m);

#endif
