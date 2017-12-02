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
#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#ifdef WIN32
    #ifdef MSVC_VER
        #include <SDL_opengl.h>
    #else
        #include <SDL/SDL_opengl.h>
    #endif
#else
	#include <SDL/SDL_opengl.h>
#endif


#include "Image.h"
#include "DArray.h"
#include "Colors.h"


struct PicData{
   char name[255];
   int twidth;
   int theight;
   int width;
	int height;
	int filter;
   //pagalbiniai duomenys, kad grei�iau pai�yt�
   float htilew;
   float htileh;
   int vframes;
   int hframes;
   float dalisx;
   float dalisy;
};

class PicsContainer{
	GLuint * TexNames;
	DArray<PicData> PicInfo;

public:
	void load(const char* list);
	void destroy();
	void draw(unsigned long index, float x, float y,
				int frame = 0, bool center=false, float scalex = 1.0f, float scaley = 1.0f, float angle = 0.0f,
				COLOR c = COLOR(1.0f, 1.0f, 1.0f, 1.0f), COLOR c2 = COLOR(1.0f, 1.0f, 1.0f, 1.0f));
	GLuint getname(unsigned long index){return TexNames[index];}
	PicData* getInfo(unsigned long index){return &PicInfo[index];}


};


#endif //TEXTURE_LOADER_H



