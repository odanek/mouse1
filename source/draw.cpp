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
Popis: Vykreslovani
*/

#include "project.h"

/*
==================================================
Nastavi offset pozadi a popredi
==================================================
*/
static void draw_SetView (int X)
{
    if (m1Pos[M1_FORE] + 120 > X)
        m1Pos[M1_FORE] = MY_Max (0, X - 120);
    if (m1Pos[M1_FORE] + 200 < X)
        m1Pos[M1_FORE] = MY_Min (2880, X - 200);

    m1Pos[M1_BACK] = (m1Pos[M1_FORE] >> 1) % 320;
}

/*
==================================================
Nastavi fazi animace postavicky
==================================================
*/
static void draw_SetFaz (m1ManStruct *m)
{
    if (m->S)
        m->F = 8;
    else
        m->F = int(m->SF) / M1_FAZ_SPEED;

    m->F += 9 * m->O;
}

/*
==================================================
Vykresli sprite do bufferu
==================================================
*/
static void draw_Image (int X, int Y, int F, myBYTE *from)
{
    int     x, y;

    from += F * 160;
    for (y = 0; y < 16; y++)
    {
        myBYTE *ptr = &m1RgbBuf.data[3 * (X + (Y + y) * m1RgbBuf.width)];

        for (x = 0; x < 10; x++, from++, ptr += 3)
        {
            if (*from != 0)
            {
                ptr[0] = m1Pal[*from][0];
                ptr[1] = m1Pal[*from][1];
                ptr[2] = m1Pal[*from][2];
            }
        }
    }
}

/*
==================================================
Vykresleni pozadi a popredi do bufferu
==================================================
*/
static void draw_Layers (myBYTE *from)
{
    myBYTE  *back = m1Data[M1_DAT_BACK], *col;
    int     x, y, bx;

    from += m1Pos[M1_FORE];

    for (y = 0; y < 192; y++, from += 3200 - 320)
    {
        myBYTE *ptr = &m1RgbBuf.data[3 * (8 + y) * m1RgbBuf.width];

        for (x = 0, bx = m1Pos[M1_BACK]; x < 320; x++, from++, bx++)
        {
            if (*from != 255)
            {
                col = m1Pal[*from];
            }
            else
            {
                if (bx >= 320)
                {
                    bx -= 320;
                }
                col = m1Pal[back[(y << 8) + (y << 6) + bx]];
            }

            *ptr++ = *col++;
            *ptr++ = *col++;
            *ptr++ = *col++;
        }
    }
}

/*
==================================================
Vymazani RGB bufferu
==================================================
*/
void draw_ClearBuffer (void)
{
    memset (m1RgbBuf.data, 0, m1RgbBuf.height * m1RgbBuf.width * 3);
}

/*
==================================================
Vykresleni RGB bufferu do OpenGL
==================================================
*/
void draw_BlitBuffer (void)
{
    glRasterPos2f (0, 200);
    glPixelZoom (m1RgbBuf.zoom, -m1RgbBuf.zoom);
    glDrawPixels (320, 200, GL_RGB, GL_UNSIGNED_BYTE, m1RgbBuf.data);
    VID_SwapBuffers ();
}

/*
==================================================
Hlavni vykreslovaci procedura
==================================================
*/
void draw_Draw ()
{
    m1ManStruct *m = &m1Man;

    menu_RedrawLives (m->Live);
    draw_SetView (m->X);
    draw_SetFaz (m);
    draw_Layers (m1Data[M1_DAT_FORE]);
    draw_Image (int(m->X) - m1Pos[M1_FORE], 8 + int(m->Y), m->F, m1Data[M1_DAT_IMG]);
    draw_BlitBuffer ();
}
