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
#include <stdio.h>
#include "TextureLoader.h"


void PicsContainer::load(const char *list){


	FILE* failas=fopen(list,"rt");

	if (failas) {
		while (!feof(failas)){
			PicData data;
			data.name[0]=0;
			fscanf(failas,"%s\n",data.name);
			fscanf(failas,"%d %d %d\n",&data.theight,&data.twidth, &data.filter);
			PicInfo.add(data);
		}

		fclose(failas);



		TexNames = new GLuint[PicInfo.count()];



		glGenTextures(PicInfo.count(), TexNames);

		for (unsigned long i=0;i<PicInfo.count();i++){

			Image naujas;

			unsigned short imageBits=0;
			if (!naujas.LoadTga(PicInfo[i].name,imageBits))
                printf("%s not found or corrupted by M$\n",PicInfo[i].name);
			PicInfo[i].width = naujas.width;
			PicInfo[i].height = naujas.height;


			PicInfo[i].htilew =PicInfo[i].twidth/2.0f;
			PicInfo[i].htileh =PicInfo[i].theight/2.0f;
			PicInfo[i].vframes=PicInfo[i].height/PicInfo[i].theight;
			PicInfo[i].hframes=PicInfo[i].width/PicInfo[i].twidth;

			int filtras = GL_NEAREST;
			if (PicInfo[i].filter)
				filtras = GL_LINEAR;


			glBindTexture(GL_TEXTURE_2D, TexNames[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtras );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtras );

			if (imageBits>24)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, naujas.width, naujas.height,
					 0, GL_RGBA, GL_UNSIGNED_BYTE,naujas.data);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, naujas.width, naujas.height,
					 0, GL_RGB, GL_UNSIGNED_BYTE,naujas.data);

			naujas.deleteImage();

		}



	}
	else
		printf("file not found: %s",list);


}

//---------------------------------------------------
void PicsContainer::draw(unsigned long index, float x, float y, int frame, bool center, float scalex, float scaley, float angle, COLOR c, COLOR c2 ){

	PicData * p = &PicInfo[index];


	glBindTexture(GL_TEXTURE_2D, TexNames[index]);



	int hf=frame/p->hframes;

	int startx =  (frame-p->hframes*hf)*p->twidth;
	int starty = hf*p->theight;

	float u1 = (startx*1.0f)/(p->width*1.0f);
	float u2 = ((startx+p->twidth)*1.0f)/(p->width*1.0f);
	float v1 = (( p->height - starty) * 1.0f ) / ( p->height * 1.0f )- 0.0001f;
	float v2 = (( p->height - starty - p->theight ) * 1.0f) / (p->height * 1.0f);

	glLoadIdentity();
	glTranslatef(x,y,0);
	if ((scalex!=1.0000f)||(scaley!=1.0000f))
		glScalef(scalex,scaley,1.0f);
	if (angle!=0.0000f)
		glRotatef(angle,0,0,1);




    if (center){
        glBegin(GL_QUADS);

        glColor4f(c.r, c.g, c.b, c.a);
        glTexCoord2f ( u1, v1 );
        glVertex2f ( 0 - p->htilew, 0 - p->htileh );


        glColor4f(c.r, c.g, c.b, c.a);
        glTexCoord2f ( u2, v1 );
        glVertex2f ( p->htilew,0 - p->htileh );


        glColor4f(c2.r, c2.g, c2.b, c2.a);
        glTexCoord2f ( u2, v2 );
        glVertex2f ( p->htilew, p->htileh );

        glColor4f(c2.r, c2.g, c2.b, c2.a);
        glTexCoord2f ( u1,v2 );
        glVertex2f ( 0 - p->htilew, p->htileh );


        glEnd();
    }
    else{
        glBegin(GL_QUADS);

        glColor4f(c.r, c.g, c.b, c.a);
        glTexCoord2f ( u1, v1 );
        glVertex2f ( 0 , 0);

        glColor4f(c.r, c.g, c.b, c.a);
        glTexCoord2f ( u2, v1 );
        glVertex2f ( p->twidth,0  );

        glColor4f(c2.r, c2.g, c2.b, c2.a);
        glTexCoord2f ( u2, v2 );
        glVertex2f ( p->twidth, p->theight );

        glColor4f(c2.r, c2.g, c2.b, c2.a);
        glTexCoord2f ( u1,v2 );
        glVertex2f ( 0 , p->theight );

        glEnd();
    }


}


//----------------------------------
void PicsContainer::destroy(){

	if (TexNames){
		delete []TexNames;
		TexNames = 0;
	}
	PicInfo.destroy();
}



