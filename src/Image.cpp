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
#include <cstdio>
#include "Image.h"


bool Image::LoadTga(const char *name, unsigned short& imageBits ){
	FILE* TGAfile;
	TGAfile=fopen(name,"rb");
	if (TGAfile){
		unsigned char fieldDescSize;
		fread(&fieldDescSize,sizeof(unsigned char),1,TGAfile);
		fseek(TGAfile,1,SEEK_CUR);
		unsigned char imageCode;
		fread(&imageCode,sizeof(unsigned char),1,TGAfile);

		if ((imageCode != 2)&&(imageCode != 10)){
			fclose(TGAfile);
			return false;
		}
		fseek(TGAfile,2,SEEK_CUR);
		unsigned short mapLength;
		fread(&mapLength,sizeof(unsigned short), 1,TGAfile);
		fseek(TGAfile,1,SEEK_CUR);

		unsigned short xStart, yStart;
		fread(&xStart,sizeof(unsigned short),1,TGAfile);
		fread(&yStart,sizeof(unsigned short),1,TGAfile);
		fread(&width,sizeof(unsigned short),1,TGAfile);
		fread(&height,sizeof(unsigned short),1,TGAfile);

		//unsigned char imageBits;
		fread(&imageBits,sizeof(unsigned char), 1,TGAfile);
		if (imageBits<24){
			fclose(TGAfile);
			return false;
		}
		unsigned char imageDesc;
		fread(&imageDesc,sizeof(unsigned char), 1,TGAfile);
		fseek(TGAfile,fieldDescSize,SEEK_CUR);
		fseek(TGAfile,mapLength*(imageBits/8),SEEK_CUR);



        data = new unsigned char[width*height*(imageBits/8)];



		if ( imageCode == 2){ //uncompressed TGA
		    unsigned char* tmp_data= new unsigned char[width*height*(imageBits/8)];
            fread(tmp_data,sizeof(unsigned char),width*height*(imageBits/8),TGAfile);

            for (int i = 0; i<height*width*(imageBits/8); i+=(imageBits/8)){

                data [i] = tmp_data[i+2]; //R
                data [i+1] = tmp_data[i+1]; //G
                data [i+2] = tmp_data[i]; //B
                if (imageBits>24)
                    data [i+3] = tmp_data[i+3]; //A

            }


            if (tmp_data){
                    delete []tmp_data;
            }

		}

        else{ //RLE compressed

            int n = 0;
            int j = 0;
            unsigned char p[5];

             while (n < width * height) {
                fread(p,1,imageBits/8+1,TGAfile);
                j = p[0] & 0x7f;

                data [n*(imageBits/8)] = p[3]; //R
                data [n*(imageBits/8)+1] = p[2]; //G
                data [n*(imageBits/8)+2] = p[1]; //B
                if (imageBits>24)
                    data [n*(imageBits/8)+3] = p[4]; //A

                n++;
                if (p[0] & 0x80) {
                    for (int i=0;i<j;i++) {
                        data [n*(imageBits/8)] = p[3]; //R
                        data [n*(imageBits/8)+1] = p[2]; //G
                        data [n*(imageBits/8)+2] = p[1]; //B
                        if (imageBits>24)
                            data [n*(imageBits/8)+3] = p[4]; //A

                        n++;
                    }
                }
                else{
                    for (int i=0;i<j;i++) {
                        fread(p,1,imageBits/8,TGAfile);

                        data [n*(imageBits/8)] = p[2]; //R
                        data [n*(imageBits/8)+1] = p[1]; //G
                        data [n*(imageBits/8)+2] = p[0]; //B
                        if (imageBits>24)
                            data [n*(imageBits/8)+3] = p[3]; //A

                        n++;
                    }
                }



             }
        }
        //--------------------------------------------------


     fclose(TGAfile);
   }
   else return false;

  return true;

}


//--------------------------------
void Image::deleteImage(){
	if (data){
		delete []data;
		data=0;
	}
	width=0;
	height=0;
}


