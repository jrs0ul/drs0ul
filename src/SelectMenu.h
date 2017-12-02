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
#ifndef	_SELECTMENIU_H_
#define _SELECTMENIU_H_

#include <cstring>
#include <cstdlib>
#include "ActiveControl.h"
#include "TextureLoader.h"

struct Smenu{
  char opt[20][30];
  unsigned char pics[20];
  unsigned char ids[20];
  unsigned char count;
  Smenu(){memset(pics,0,20); memset(ids,0,20);}
 };

//--------------------------------------





 class SelectMenu:public ActiveControl{ //selecto objectas
 public:
  char title[30];  //antraste
  Smenu selection;   //meniu
  unsigned int width;        //plotis
  unsigned int height;       //aukstis
  unsigned char defstate;     //defaultine opcija
  unsigned char state;        //parinkta tuo metu opcija
  unsigned int elementheight;
  
  bool selected;  //jau viskas pasirinkta, enter paspaustas
  bool canceled;  //ar menu atshauktas
  unsigned char pressedkey;
   

  //priskiriam data dx,dy:lango metrikos dst:defstate
  void init(unsigned int dx, unsigned int dy,
			 const char* dt, Smenu& dsel,
			 unsigned char dst,
			 unsigned int elemheight=38,
			 unsigned int dheight=0);
  //numusa userio atlikta pasirinkima
  void reset();
  //jei keyus i virsu apcia tai keiciasi ir state
  void getInput(unsigned char key);
  //nupaisom
  void draw( PicsContainer& pics,
		unsigned int pointerindex,
		unsigned int fontindex,
		unsigned int iconindex,
		bool drawblock=false,
		float r=1.0f,float b=1.0f,float g=1.0f);
 
 };

#endif

