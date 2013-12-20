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
Popis: Menu
*/

#include "project.h"

const char *menuFl[5] =
    {
        "levels/lev1.kr3",
        "levels/lev2.kr3",
        "levels/lev3.kr3",
        "levels/lev4.kr3",
        "levels/lev5.kr3"
    };

void menu_RedrawLives (int ziv)
{
    int i, j;

    // Vycisteni
    memset (m1RgbBuf.data, 0, 8 * m1RgbBuf.width * 3);

    // Logo
    CO_FontColorPal (m1Pal, 32);
    CO_FontPrintf (16, 0, "The");
    CO_FontColorPal (m1Pal, 46);
    CO_FontPrintf (20, 0, "Mouse");

    // Zivoty
    for (i = 0; i < ziv; i++)
    {
        myBYTE *ptr = &m1RgbBuf.data[3 * (5 * (i + 1) + m1RgbBuf.width)];

        for (j = 0; j < 6; j++)
        {
            ptr[0] = m1Pal[13][0];
            ptr[1] = m1Pal[13][1];
            ptr[2] = m1Pal[13][2];

            ptr += 3 * m1RgbBuf.width;
        }
    }
}

void menu_UberZiv (int ziv)
{
    while (!g_inp.key[SDLK_RETURN] && !(g_app.flags & APP_FLAG_QUIT))
    {
        draw_ClearBuffer ();
        menu_RedrawLives (ziv);
        CO_FontColorPal (m1Pal, 46);
        if (ziv)
            CO_FontPrintf (12, 12, "Ztratil jsi zivot");
        else
        {
            CO_FontPrintf (16, 12, "Zemrel jsi");
            m1End = M1_EXIT;
        }
        CO_FontColorPal (m1Pal, 6);
        CO_FontPrintf (14, 24, "STISKNI ENTER");
        CO_ProcessEvents ();
        draw_BlitBuffer ();
    }
}

void menu_Blahopreji (void)
{
    while (!g_inp.key[SDLK_RETURN] && !(g_app.flags & APP_FLAG_QUIT))
    {
        draw_ClearBuffer ();
        menu_RedrawLives (0);

        CO_FontColorPal (m1Pal, 46);
        CO_FontPrintf (7, 12, "Blahopreji dokoncil jsi hru!");
        CO_FontColorPal (m1Pal, 6);
        CO_FontPrintf (14, 24, "STISKNI ENTER");
        CO_ProcessEvents ();
        draw_BlitBuffer ();
    }
}

void menu_Uvod (int lev)
{
    myFile_s    *f;
    char        name[21];
    int         i, h = 10;

    name[20] = 0;
    f = MY_FOpen (M1_FILE_NAMES, 0, "rb", true);
    MY_FSeek (f, lev * 20 - 20, SEEK_SET);
    MY_FRead (name, 1, 20, f);
    MY_FClose (&f);
    for (i = 0; i < 20; i++)
        if ((name[i] -= 30) > 32)
            h = i / 2;

    while (!g_inp.key[SDLK_RETURN] && !(g_app.flags & APP_FLAG_QUIT))
    {
        draw_ClearBuffer ();
        menu_RedrawLives (0);

        CO_FontColorPal (m1Pal, 40);
        CO_FontPrintf (17, 10, "Level %d", lev);
        CO_FontColorPal (m1Pal, 6);
        CO_FontPrintf (14, 24, "STISKNI ENTER");
        CO_FontColorPal (m1Pal, 44);
        CO_FontPrintf (20 - h, 12, name);
        CO_ProcessEvents ();
        draw_BlitBuffer ();
    }
}

void menu_Play (void)
{
    int     lev = 1, ziv = 5;

    m1End = M1_NONE;
    while (lev < 6 && m1End != M1_EXIT)
    {
        if (m1End != M1_DEAD)
            menu_Uvod (lev);
        m1End = M1_NONE;
        load_Data (menuFl[lev - 1], m1Data[M1_DAT_FORE], m1Data[M1_DAT_BACK]);
        load_Prepare (&m1Man);
        m1Man.Live = ziv;
        hlavni_PlayLoop ();
        switch (m1End)
        {
            case M1_NEXT : lev++; break;
            case M1_DEAD : ziv--; menu_UberZiv (ziv); break;
        }
    }

    if (m1End == M1_NEXT)
        menu_Blahopreji ();
}

void menu_Draw (void)
{
    draw_ClearBuffer ();
    menu_RedrawLives (0);

    // Menu
    CO_FontColorPal (m1Pal, 6);
    CO_FontPrintf (18, 11, "Nova hra");
    CO_FontPrintf (18, 12, "Konec");
    CO_FontColorPal (m1Pal, 10);
    CO_FontPrintf (15, 11, "1.");
    CO_FontPrintf (15, 12, "2. ");
    CO_FontColorPal (m1Pal, 64);
    CO_FontPrintf (1, 24, "Napsal O.Danek v roce 2008 v C++");
    draw_BlitBuffer ();
}

void menu_Menu (void)
{
    int c = CO_InpGetKey (true);

    if (c == '2') 
    {
        m1LoopQuitCode = M1_QC_QUIT;
    }
    else if (c == '1')
    {
        m1LoopQuitCode = M1_QC_PLAY;
    }
}
