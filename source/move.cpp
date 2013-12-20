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
Popis: Kontroly pohybu mysi
*/

#include "project.h"

bool move_Can (int X, int Y)
{
    int i, j, pos;

    pos = Y * 3200 + X;
    for (j = 0; j < 16; j++, pos += 3190)
        for (i = 0; i < 10; i++, pos++)
            if ((myBYTE) m1Data[M1_DAT_FORE][pos] < 16)
                return false;
    return true;
}

bool move_CanFall (int X, int Y)
{
    int i, pos;

    if (Y + 16 > 191)
        return false;

    pos = (Y + 16) * 3200 + X;
    for (i = 0; i < 10; i++, pos++)
        if ((myBYTE) m1Data[M1_DAT_FORE][pos] < 16)
            return false;
    return true;
}

void move_Faz (m1ManStruct *m)
{
    if (++m->SF >= M1_FAZ_MAX * M1_FAZ_SPEED)
        m->SF = 0;
}

void move_Doprava (m1ManStruct *m)
{
    m->O = M1_ORI_RIGHT;

    if (!move_Can (m->X + 1, m->Y) || m->X + 1 > 3180)
        return;

    m->X++;
    move_Faz (m);
}

void move_Doleva (m1ManStruct *m)
{
    m->O = M1_ORI_LEFT;

    if (!move_Can (m->X - 1, m->Y) || m->X - 1 < 1)
        return;

    m->X--;
    move_Faz (m);
}

void move_Nahoru (m1ManStruct *m)
{
    if (!move_Can (m->X, m->Y - 1) || m->Y - 1 < 1 || m->J >= M1_JUMP_MAX)
    {
        m->S = 2;
        return;
    }

    m->Y--;
    m->J++;
}

void move_Dolu (m1ManStruct *m)
{
    if (!move_Can (m->X, m->Y + 1) || m->Y + 1 > 176)
    {
        m->S = 0;
        return;
    }

    m->Y++;
}

void move_Jump (m1ManStruct *m)
{
    if (!move_Can (m->X, m->Y - 1) || m->S)
        return;

    m->S = 1;
    m->J = 0;
}
