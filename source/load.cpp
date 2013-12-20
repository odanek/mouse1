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
Popis: Nahravani dat
*/

#include "project.h"

void load_Prepare (m1ManStruct *m)
{
    m->X = 3180;
    m->Y = 50;
    m->O = M1_ORI_LEFT;
    m->J = 0;
    m->S = 2;
    m->F = 0;
    m->SF = 0;
}

void load_Ob5 (myFile_s *f, myBYTE *t)
{
    myBYTE  cl;
    int i = 0, dl = 0;

    while (i < 61440)
    {
        MY_FRead (&dl, 1, 1, f);
        MY_FRead (&cl, 1, 1, f);
        memset (&t[i], cl, dl);
        i += dl;
    }
}

void load_PutIn (myBYTE *co, myBYTE *kam, int pos)
{
    unsigned int    i;

    for (i = 0; i < 192; i++)
        memcpy (&kam[3200 * i + pos], &co[320 * i], 320);
}

void load_Data (const char *fl, myBYTE *pt1, myBYTE *pt2)
{
    int         j;
    myFile_s    *f;

    f = MY_FOpen (fl, 0, "rb", true);
    for (j = 0; j < 10; j++)
    {
        load_Ob5 (f, pt2);
        load_PutIn (pt2, pt1, 2880 - j * 320);
    }
    load_Ob5 (f, pt2);

    MY_FClose (&f);
}
