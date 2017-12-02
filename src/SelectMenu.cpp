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

#include "SelectMenu.h"
#include "Utils.h"





void SelectMenu::init(unsigned int dx, unsigned int dy, const char* dt, Smenu& dsel, unsigned char dst, unsigned int elemheight, unsigned int dheight){

 elementheight = elemheight;
 x=dx;
 y=dy;
 strcpy(title,dt);
 for (int i=0;i<dsel.count;i++)
  strcpy(selection.opt[i],dsel.opt[i]);
 selection.count=dsel.count;

 if ((strlen(title)==0)
  ||(strlen(selection.opt[0])>strlen(title)))
   width=(int)strlen(selection.opt[0])*11+70;
  else
   width=(int)strlen(title)*11+70;

 memcpy(selection.pics,dsel.pics,20);

 if (dheight==0)
  height=selection.count*20+50;
 else
  height=dheight;

 defstate=dst;
 state=defstate;
 selected=false; //dar nieko neisirinkom
 canceled=false;
 deactivate();
 pressedkey=0;
}
//--------------------------------------------
void SelectMenu::reset(){
 state=defstate;
 selected=false;
 canceled=false;
}
//--------------------------------------------
void SelectMenu::getInput(unsigned char key){
 if (key!=pressedkey){
  //cia tam kad parinktu kai mygtuka atleidi
  if (pressedkey==13) {
   if (selection.count)
    selected=true;      //enter
  }

  if (pressedkey==27) {
   canceled=true;      //esc
  }

  if (selection.count){
   switch(key){
    case 38:if (state>0) //up
        state--;
       else
        state=selection.count-1;
	   break;

    case 40:if (state<selection.count-1)  //down
        state++;
       else
        state=0;
	    break;

    default: pressedkey=0;
   }
  }


  pressedkey=key;
 }

}
//--------------------------------------------
void SelectMenu::draw( PicsContainer& pics,
		unsigned int pointerindex,
		unsigned int fontindex,
		unsigned int iconindex,
		bool drawblock,
		float r,float b,float g){


	COLOR black = COLOR(0.0f, 0.0f, 0.0f, 1.0f);

	unsigned newcount=0;
	unsigned start=0;
	int half=(((height-28)/20)/2);
	unsigned tmpheight=height;
	if (selection.count*20+28>tmpheight){
		if (((tmpheight-28)/20)+state/half>selection.count)
			newcount=selection.count;
		else
			newcount=((height-28)/20)+state/half;
		start=state/half;
	}
	else newcount=selection.count;

  /*if ((icons)&&(((newcount-start)*icons->info.Height)+28>tmpheight))
	  height=(newcount-start)*icons->info.Height+40;*/

	if (drawblock)
			DrawBlock(x,y,width,height, COLOR(0.0f , 0.0f, 1.0f, 0.5f));
	DrawText(x+34, y+16,title,pics,0, elementheight/16.0f, black);
	DrawText(x+32, y+16,title,pics,0, elementheight/16.0f,COLOR(1.0f,1.0f,1.0f));

	for (int i=start;i< newcount;i++){

		DrawText(x+34, y+36+((i-start)*elementheight), selection.opt[i],pics, 0, elementheight/16.0f, black, black);
		DrawText(x+32,y+38+((i-start)*elementheight), selection.opt[i],pics,0,elementheight/16.0f,COLOR(1.0f,1.0f,0.0f), COLOR(0.0f,1.0f,1.0f));


	  }


 	if ((rand()%100)%10==0)
		pics.draw( pointerindex,x+13,y+38+((state-start)*elementheight),0);
 	else
		pics.draw( pointerindex,x+15,y+38+((state-start)*elementheight),0);


}

