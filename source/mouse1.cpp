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
Popis: Hlavni soubor
*/

#include "project.h"

///////////////
// Definice promenych
//////////////
int         m1Pos[2] = { 0, 0};
myBYTE      *m1Data[M1_DATA_SIZE], m1Pal[256][3];
int         m1End, m1LoopQuitCode;
m1ManStruct m1Man;
bool        m1InMenu = true;
vidRgbBuf_s m1RgbBuf;

/*
==================================================
Zpracovani vstupu z klavesnice
==================================================
*/
void P_KeyEvent (int key)
{
}

/*
==================================================
Reakce na aktivaci/deaktivaci
==================================================
*/
void P_ActiveEvent (bool active)
{
}

/*
==================================================
Smycka - vykresleni, zpracovani eventu
==================================================
*/
int P_DoLoop (void (*move)(void), void (*draw)(void))
{
    m1LoopQuitCode = 0;

    //CO_FpsReset ();
    while (!(g_app.flags & APP_FLAG_QUIT) && !m1LoopQuitCode)
    {
        CO_ProcessEvents ();
        CO_FpsSyncLoops (move, draw);
    }

    return m1LoopQuitCode;
}

/*
==================================================
Hlavni procedura
==================================================
*/
void P_Main (void)
{
    int     qc;

    while (!(g_app.flags & APP_FLAG_QUIT))
    {
        while (CO_InpGetKey (false))
            ;
        qc = P_DoLoop (menu_Menu, menu_Draw);

        if (qc == M1_QC_QUIT)
            break;
        if (qc == M1_QC_PLAY)
            menu_Play ();
    }
}

