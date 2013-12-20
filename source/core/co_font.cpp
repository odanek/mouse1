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
Projekt: Sablona aplikace
Popis: Prace s fonty, psani na obrazovku
*/

#include "co_core.h"

static  myBYTE      *fontData = NULL;
static  vidRgbBuf_s *fontBuf = NULL;
static  int         fontSZX, fontSZY;
static  GLubyte     fontCol[3];
static  char        fontStr[500];
static  bool        font_x_mul = false;         // Nasobit x sirkou fontu?
static  bool        font_y_mul = false;         // Nasobit y sirkou fontu?
static  bool        font_y_rev = false;         // Otocit osu y?

/*
==================================================
Nahrani fontu ze souboru
==================================================
*/
void CO_FontLoad (const char *fontFile)
{
    MY_Free (fontData);

    if (MY_FSize (fontFile) < 50)
        MY_Err (MY_ErrDump(MY_L("COSTR0001|Nepodarilo se nahrat soubor s fontem %s"), fontFile));

    fontData = (myBYTE *) MY_Alloc (MY_FSize (fontFile) - 50);
    MY_FLoadBlock (fontFile, 50, -1, (void *) fontData);
    fontSZX = (int) fontData[0];
    fontSZY = (int) fontData[1];
}

/*
==================================================
Uvolneni fontu z pameti
==================================================
*/
void CO_FontFree (void)
{
    MY_Free (fontData);
}

/*
==================================================
Nastaveni barvy jakou se bude psat
==================================================
*/
void CO_FontColor (GLubyte red, GLubyte green, GLubyte blue)
{
    fontCol[0] = red;
    fontCol[1] = green;
    fontCol[2] = blue;
}

/*
==================================================
Vypsani retezce primo do OpenGL
==================================================
*/
void CO_FontPrintToGl (int x, int y, const char *str)
{
    int     i, l = (int) strlen (str);

    if (font_x_mul) x *= fontSZX;
    if (font_y_mul) y *= fontSZY;
    if (font_y_rev) y = g_vid.cl_height - y - fontSZY;

    y += fontSZY - 1;

    glColor3ubv (fontCol);
    glBegin (GL_POINTS);

    for (i = 0; i < l; i++, x += 8)
    {
        if (str[i] > 0 && str[i] != ' ')
        {
            myBYTE  *frm, c = (myBYTE)str[i];
            myWORD  ii, jj, b;

            frm = &fontData[2 + c * fontSZY];

            for (ii = 0; ii < fontSZY; ii++, frm++)
            {
                b = *frm;

                for (jj = 0; jj < 8; jj++)
                {
                    if (b & (1 << (7 - jj)))
                    {
                        glVertex2i (x + jj, y - ii);
                    }
                }
            }
        }
    }

    glEnd ();
}

/*
==================================================
Vypsani retezce primo do RGB bufferu
==================================================
*/
void CO_FontPrintToBuffer (int x, int y, const char *str)
{
    int     i, l = (int) strlen (str);

    if (font_x_mul) x *= fontSZX;
    if (font_y_mul) y *= fontSZY;
    if (font_y_rev) y = fontBuf->height - y - fontSZY;

    //y += fontSZY - 1;

    for (i = 0; i < l; i++, x += 8)
    {
        if (str[i] > 0 && str[i] != ' ')
        {
            myBYTE  *frm, c = (myBYTE)str[i];
            myWORD  ii, jj, b;

            frm = &fontData[2 + c * fontSZY];

            for (ii = 0; ii < fontSZY; ii++, frm++)
            {
                b = *frm;
                myBYTE *ptr = &fontBuf->data[3 * (x + (y + ii) * fontBuf->width)];

                for (jj = 0; jj < 8; jj++)
                {
                    if (b & (1 << (7 - jj)))
                    {
                        *ptr++ = fontCol[0];
                        *ptr++ = fontCol[1];
                        *ptr++ = fontCol[2];
                    }
                    else
                    {
                        ptr += 3;
                    }
                }
            }
        }
    }
}

/*
==================================================
Vypsani retezce
==================================================
*/
void CO_FontPrint (int x, int y, const char *str)
{
    if (fontBuf == NULL)
    {
        CO_FontPrintToGl (x, y, str);
    }
    else
    {
        CO_FontPrintToBuffer (x, y, str);
    }
}

/*
==================================================
Vypsani retezce
==================================================
*/
void CO_FontPrintf (int x, int y, const char *str, ...)
{
    va_list argptr;

    va_start (argptr, str);
    vsprintf (fontStr, str, argptr);
    va_end (argptr);

    CO_FontPrint (x, y, fontStr);
}

/*
==================================================
Vraci ukazatel na nahrany font
==================================================
*/
const myBYTE *CO_FontGet (void)
{
    return fontData;
}

/*
==================================================
Nastaveni vykreslovaciho bufferu
==================================================
*/
void CO_FontUseBuffer (vidRgbBuf_s *buf)
{
    fontBuf = buf;
}

/*
==================================================
Nastaveni modu - zda se x/y pozice nasobi sirkou/vyskou
a zda je osa y obracena
==================================================
*/
void CO_FontSetMode (bool x_mul, bool y_mul, bool y_rev)
{
    font_x_mul = x_mul;
    font_y_mul = y_mul;
    font_y_rev = y_rev;
}
