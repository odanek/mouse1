/*
* Copyright (c) 2006, Ondrej Danek (www.ondrej-danek.net)
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Ondrej Danek nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
Projekt: The Mouse 1
Popis: Hlavni herni smycka
*/

#include "project.h"

void hlavni_Check (int X, int Y)
{
    int pos, c;

    pos = (Y + 15) * 3200 + X + 5;
    c = (myBYTE) m1Data[M1_DAT_FORE][pos];
    if (c == 53) m1End = M1_DEAD;
    if (c == 43) m1End = M1_NEXT;
}

void hlavni_Move (void)
{
    if (!m1Man.S)
        if (move_CanFall (m1Man.X, m1Man.Y))
            m1Man.S = 2;

    if (g_inp.key[SDLK_ESCAPE]) m1End = M1_EXIT;
    if (g_inp.key[SDLK_UP]) move_Jump (&m1Man);
    if (g_inp.key[SDLK_DOWN] && m1Man.S == 1) m1Man.S = 2;
    if (g_inp.key[SDLK_LEFT]) move_Doleva (&m1Man);
    if (g_inp.key[SDLK_RIGHT]) move_Doprava (&m1Man);
    if (g_inp.key[SDLK_m] && g_inp.key[SDLK_y] && g_inp.key[SDLK_s]) m1End = M1_NEXT;
    if (m1Man.S == 1) move_Nahoru (&m1Man);
    if (m1Man.S == 2) move_Dolu (&m1Man);
    if (!m1Man.S)
        hlavni_Check (m1Man.X, m1Man.Y);

    m1LoopQuitCode = m1End;
}

void hlavni_PlayLoop (void)
{
    if (!move_CanFall (m1Man.X, m1Man.Y))
        m1Man.F = 0;

    //CO_FpsReset ();
    m1End = P_DoLoop (hlavni_Move, draw_Draw);
}
