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
#include "Pill.h"
#include <cstdlib>


void Pill::rotate(Bottle& bot){

 int eilnum1=col1-3;
 while (eilnum1>6)
	 eilnum1-=6;
 int eilnum2=col2-3;
 while (eilnum2>6)
	 eilnum2-=6;

 if ((partx==0)&&(party==1)){ // jei stacia
  if ((!bot.getTile(x+1,y))&&(x<7)){
   partx=1;
   party=0;

   if (eilnum1==2)
    col1--;
   if (eilnum2==3)
    col2++;
  }
  else
  if ((!bot.getTile(x-1, y))&&(x>0)){
   partx=-1;
   party=0;

   col1+=2;
   col2-=2;
  }

 }
 else{
  if ((!bot.getTile(x, y+1))&&(y<15)){

   int tmp=(col2-4)/6;
   int tmp2=(col1-4)/6; //spalvos

   col1=0;
   for (int p=0;p<tmp;p++)
    col1+=6;
   col1+=eilnum1+3;

   col2=0;
   for (int p=0;p<tmp2;p++)
    col2+=6;
   col2+=eilnum2+3;



   if (eilnum1==4)
    col1-=2;
   else
   if (eilnum1==1)
    col1++;

   if (eilnum2==1)
    col2+=2;
   else
   if (eilnum2==4)
    col2--;






   partx=0;
   party=1;




  }
 }
}

//----------------
void Pill::make(){
 y=0;
 x=3;
 col1=nextcol1+4;
 col2=nextcol2+4+3;

 tim=0;
 partx=1;
 party=0;
 nextcol1=rand()%3*6;
 nextcol2=rand()%3*6;

}
//--------------------

void Pill::drawnext(PicsContainer& pics, int index, int x, int y){
 pics.draw(index,x,y,nextcol1);
 pics.draw(index,x+16,y,nextcol2+3);
}

