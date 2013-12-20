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
Popis: Nastaveni programu
*/

#include "project.h"

void set_Pointers (void)
{
    m1Data[M1_DAT_FORE] = (myBYTE *) MY_Alloc (640000);
    m1Data[M1_DAT_BACK] = (myBYTE *) MY_Alloc (64000);
    m1Data[M1_DAT_IMG] = (myBYTE *) MY_Alloc (MY_FSize (M1_FILE_IMAGE));
    m1Data[M1_DAT_SCR_GL] = (myBYTE *) MY_Alloc (3 * 64000);
}

void set_Load (void)
{
    MY_FLoad (M1_FILE_IMAGE, (void *) m1Data[M1_DAT_IMG]);
    MY_FLoad (M1_FILE_PAL, (void *) m1Pal);
}

void set_Video (void)
{
    // Zjisteni aktualniho rozliseni
    SDL_DisplayMode currentDisplayMode;
    SDL_GetCurrentDisplayMode(0, &currentDisplayMode);

    // Nastaveni grafickeho modu    
    VID_SetMode(currentDisplayMode.w, currentDisplayMode.h, M1_CL_BPP, M1_CL_AA, true);

    // Spocitame zoom, abychom zabrali co nejvetsi cast obrazovky pri zachovani aspect ratia
    m1RgbBuf.data = m1Data[M1_DAT_SCR_GL];
    m1RgbBuf.width = 320;
    m1RgbBuf.height = 200;
    if (g_vid.cl_width / 320.0f > g_vid.cl_height / 200.0f)
    {
        m1RgbBuf.zoom = g_vid.cl_height / 200.0f;
    }
    else
    {
        m1RgbBuf.zoom = g_vid.cl_width / 320.0f;
    }

    glViewport (g_vid.cl_width / 2 - int(160 * m1RgbBuf.zoom), g_vid.cl_height / 2 - int(100 * m1RgbBuf.zoom), int(320 * m1RgbBuf.zoom), int(200 * m1RgbBuf.zoom));

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, 320, 0, 200, 0, 1);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    // Clear both buffers
    glDrawBuffer (GL_FRONT_AND_BACK);
    glClear (GL_COLOR_BUFFER_BIT);
    glDrawBuffer (GL_BACK);

    // Nastaveni fontu
    CO_FontSetMode (true, true, false);
    CO_FontUseBuffer (&m1RgbBuf);
}

void P_DeInit (void)
{
    int i;

    for (i = 0; i < M1_DATA_SIZE; i++)
        MY_Free (m1Data[i]);
}

void P_Init (void)
{
    set_Pointers ();
    set_Load ();
    set_Video ();
}
