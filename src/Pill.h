/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul   *
 *   jrs0ul@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef PILL_H
#define PILL_H

#include "TextureLoader.h"
#include "Bottle.h"

class Bottle;

class Pill{
public:
 int x,y;
 int tim;
 int timx;
 int partx,party;
 int col1,col2;
 int nextcol1,nextcol2;
 Pill(int dx=0,int dy=0):x(dx), y(dy){ tim=0; timx=0; partx=0; party=1;
 col1=5; col2=6;}

 void rotate(Bottle& bot);
 void make();
 void drawnext(PicsContainer& pics, int index, int x, int y);
};








#endif //PILL_H
