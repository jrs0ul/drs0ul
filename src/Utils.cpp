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
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <vorbis/vorbisfile.h>
#include "Utils.h"




void DrawText(int x, int y,const char* text, PicsContainer& pic, int imgindex,
			float scale, COLOR color, COLOR color2){
	for (int i=0;i<strlen(text);i++){

        pic.draw( imgindex, x+i*(12*scale), y, text[i]-32, false, scale, scale, 0.0f, color, color2);

	}
}
//------------------------------------------------------------------
void DrawBlock(int x, int y,int width,int height, COLOR c){

	glLoadIdentity();
	glTranslatef(x,y,0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);


	glColor4f(c.r, c.g, c.b, c.a);
	glVertex2i(0, 0 );

	glColor4f(c.r, c.g, c.b, c.a);
	glVertex2i(width,0);

	glColor4f(0.0f, 0.0f, 0.0f, c.a);
	glVertex2i(width, height);

	glColor4f(0.0f, 0.0f, 0.0f, c.a);
	glVertex2i(0, height);

	glEnd();

}
//----------------------------------------------------------------
int _round(double x){
	return int(x > 0.0 ? x + 0.5 : x - 0.5);
}
//--------------------------------------------------------------


bool CirclesColide(float x1,float y1,float radius1, float x2, float y2, float radius2){

	 float difx = (float) fabs (x1 - x2);
 	 float  dify = (float) fabs (y1 - y2);
 	 float   distance = (float) sqrt (difx * difx + dify * dify);

 	 if   (distance < (radius1 + radius2))
 		return   true;

 	 return   false;

}



//-------------------------------------------------------------------
char* LoadOGG(char *fileName,  ALsizei & size, ALenum &format, ALsizei &freq){


	char * buffer =0;
	const int BUFFER_SIZE = 16384 ;
    int endian = 0;
    int bitStream;
    long bytes;
    char data[BUFFER_SIZE];
    FILE *f = 0;

    f = fopen(fileName, "rb");

    if (!f){
        printf("Cannot open %s\n",fileName);
        return 0;
    }

    vorbis_info *pInfo;
    OggVorbis_File oggFile;

    if (ov_open(f, &oggFile, NULL, 0) != 0){
		printf("Error while opening %s\n",fileName);
        return 0;
    }

    pInfo = ov_info(&oggFile, -1);

    if (pInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;

    freq = pInfo->rate;

	bytes = 1;
	buffer = 0;
	size = 0;
	int siz =0;
	int btmp = 1;
    while (bytes > 0){

        //uzpildome buferi
		siz = 0;
		while ((btmp) && (siz<BUFFER_SIZE)){

			btmp = ov_read(&oggFile, data+siz, BUFFER_SIZE-siz, endian, 2, 1, 0);
			siz+=btmp;
		}

		bytes = siz;
        if (bytes< 0){
            ov_clear(&oggFile);
            printf("Error while decoding %s\n",fileName);
            return 0;
        }
		printf("#");


		char * tmp = 0;
		if (size){
			tmp = new char[size+1];
			memcpy(tmp,buffer,size);
			delete []buffer;
		}
		size+=bytes;
		buffer=new char[size];
		memcpy(buffer,tmp,size-bytes);
		memcpy(&buffer[size-bytes],data,bytes);
		if (tmp){
			delete []tmp;
		}


	}



    ov_clear(&oggFile);
	puts("__");
	return buffer;
    }
