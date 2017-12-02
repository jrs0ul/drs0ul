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
#include "Bottle.h"
#include <cstdlib>

//----------------------------------------------------------
Bottle::Bottle(Bottle const & copy){
		for (int i=0; i<16; i++)
            for (int a=0;a<8;a++)
                this->bottle[i][a] = copy.bottle[i][a];
        this->_viruscount = copy._viruscount;
   }


//-------------------------------------------
//grazina piliules segmento spalva
int Bottle::getPillColor(int y, int x){
    int col=0;
    unsigned char * t;
    if ((y<16)&&(x<8)&&(y>-1)&&(x>-1)){
        t = &bottle[y][x];

        if (*t < 4)
            col = *t;
        if ( ( *t >= 4 ) && ( *t < 10 ) )
            col = 1;
        if ( ( *t >= 10) && ( *t < 16 ) )
            col = 2;
        if ( ( *t >= 16) && ( *t < 22 ) )
            col = 3;
   }
  return col;

}

//------------------------------------------------
//grazina piliules kadro nr
int Bottle::getPillFrame(int x, int y){

  int pilnum=0;
  if (bottle[y][x]>3){
   pilnum=bottle[y][x]-3;
   while (pilnum>6)
    pilnum-=6;
  }

  return pilnum;

}
//------------------------------------------------------------
bool Bottle::eliminate(Player& player, SoundSystem& SS, int soundindex, bool sfxon){

    int col = 0;
	int oldcol=0;
    int count=0;
    bool eliminated=false;
    int num;

    //eliminate vertical

 for (int i=0; i<8;i++){
  count=1;
  oldcol=0;
  for (int a=0;a<16;a++){
   oldcol=col;

   col=getPillColor(a,i);
   num=getPillFrame(i,a);

   if ((col==oldcol)&&(col!=0)&&(num!=6)&&(a>0))
	   count++;



   if (((col!=oldcol)||(a==15))&&(count>3)){
	 if (sfxon)
	  SS.playsound(soundindex);
	 int startn=1;
	 if (col==oldcol)
		 startn=0;

	 for (int b=startn;b<count+startn;b++){
		if ((bottle[a-b][i])&&(bottle[a-b][i]<4)){
	     _viruscount--;
	     switch(bottle[a-b][i]){
	        case 1: bviruscount--; break;
	        case 2: yviruscount--; break;
	        case 3: rviruscount--; break;
	     }
		 player.addScore(200);
		}
        //vercia piliules i tabletes
		int tmp;
        int numas=getPillFrame(i, a-b);
		if ((numas)&&(numas!=5)){
			switch(numas){
			case 1:{
				tmp=getPillFrame(i+1,a-b);
				if ((i<7)&&(tmp==4))
					bottle[a-b][i+1]++;
				   } break;
			case 2:{
				tmp=getPillFrame(i,a-b+1);
				if ((a-b<15)&&(tmp==3))
					bottle[a-b+1][i]+=2;

				   } break;
			case 3:{
                 tmp=getPillFrame(i,a-b-1);
				 if ((a-b>0)&&(tmp==2))
					bottle[a-b-1][i]+=3;
				   } break;
			case 4:{
                 tmp=getPillFrame(i-1,a-b);
				 if ((i>0)&&(tmp==1))
					bottle[a-b][i-1]+=4;
				   }
			}
		}

		int newval=0;
		for (int g=0;g<oldcol;g++)
         newval+=6;

		bottle[a-b][i]=newval+3;
	 }

	 if (!eliminated)
      eliminated=true;
	 count=1;
   }

    if (col!=oldcol)
	   count=1;


  }

 }

 //eliminate horizontal

 for (int i=0; i<16;i++){
  count=1;
  oldcol=0;
  for (int a=0;a<8;a++){
   oldcol=col;

   col=getPillColor(i,a);
   if ((col==oldcol)&&(col!=0)&&(a>0))
	   count++;



   if (((col!=oldcol)||(a==7))&&(count>3)){
    if (sfxon)
	  SS.playsound(soundindex);
     int startn=1;
	 if (col==oldcol)
		 startn=0;

	 for (int b=startn;b<count+startn;b++){
		 if ((bottle[i][a-b])&&(bottle[i][a-b]<4)){
	      _viruscount--;
	      switch(bottle[i][a-b]){
            case 1: bviruscount--; break;
            case 2: yviruscount--; break;
            case 3: rviruscount--; break;
	      }
		  player.addScore(200);
		 }

         //verciam piliules i tabletes

         int tmp;
        int numas=getPillFrame(a-b,i);
		if ((numas)&&(numas!=5)){
			switch(numas){
			case 1:{
				tmp=getPillFrame(a-b+1,i);
				if ((a-b<7)&&(tmp==4))
					bottle[i][a-b+1]++;
				   } break;
			case 2:{
				tmp=getPillFrame(a-b,i+1);
				if ((a-b<15)&&(tmp==3))
					bottle[i+1][a-b]+=2;

				   } break;
			case 3:{
                 tmp=getPillFrame(a-b,i-1);
				 if ((i>0)&&(tmp==2))
					bottle[i-1][a-b]+=3;
				   } break;
			case 4:{
                 tmp=getPillFrame(a-b-1,i);
				 if ((a-b>0)&&(tmp==1))
					bottle[i][a-b-1]+=4;
				   }
			}
		}

		int newval=0;
		for (int g=0;g<oldcol;g++)
         newval+=6;

		bottle[i][a-b]=newval+3;
	 }

	 if (!eliminated)
      eliminated=true;
	 count=1;
   }

  if (col!=oldcol)
	   count=1;
  }
 }

 return eliminated;

}
//----------------------------------------------------

void Bottle::pillsFall(){

 int pilnum=0;
 int nextp=0;
 int prevp=0;

 unsigned char tmp[16][8];

 for (int z=0;z<16;z++)
  for(int y=0;y<8;y++)
	  tmp[z][y]=bottle[z][y];


 for (int i=0;i<16;i++)
	 for( int a=0;a<8;a++){
		 if (bottle[i][a]>3){ //ar ne virusas


           pilnum=getPillFrame(a, i);

		  if (a<7)
		   if(bottle[i][a+1]>3)
		    nextp=getPillFrame(a+1, i);

		  if (a>0)
		   if (bottle[i][a-1]>3)
			 prevp=getPillFrame(a-1,i);






		   if (i<15){

		   if ((pilnum==1)&&(nextp==4)){
			   if ((bottle[i+1][a]==0)&&(bottle[i+1][a+1]==0)){
                 tmp[i+1][a]=bottle[i][a];
                 tmp[i+1][a+1]=bottle[i][a+1];

                 tmp[i][a]=0;
				 tmp[i][a+1]=0;
			   }

		   }

		   else
			if ((pilnum==4)&&(prevp==1)){
             if ((bottle[i+1][a]==0)&&(bottle[i+1][a-1]==0)){
                 tmp[i+1][a]=bottle[i][a];
                 tmp[i+1][a-1]=bottle[i][a-1];

                 tmp[i][a]=0;
				 tmp[i][a-1]=0;
			   }

			}


		   else{





				 if(bottle[i+1][a]==0){ //ar yra kas nors apacioj?
                   if (pilnum==1)
                    tmp[i+1][a]=bottle[i][a]+4;
				   else
                   if (pilnum==4)
                    tmp[i+1][a]=bottle[i][a]+1;
				   else
                   tmp[i+1][a]=bottle[i][a];
				   if (pilnum==3){			//jei krenta staciai
				    tmp[i][a]=bottle[i-1][a];
					tmp[i-1][a]=0;
				   }
				   else
				    tmp[i][a]=0;
				 }
			 }

		 }//
		 }

	 }


 for (int z=0;z<16;z++)
  for(int y=0;y<8;y++)
	  bottle[z][y]=tmp[z][y];



}



//--------------------
//pasalina eliminavusiu piliuliu liekanas
void Bottle::clearcrap(){

 int num=0;

 for (int i=0;i<16;i++)
   for(int a=0;a<8;a++){
    num=getPillFrame(a,i);
	if (num==6)
	 bottle[i][a]=0;
   }

}

//-------------------------------------------------------------
void Bottle::checkvirus(int x,int y, int maxcount){

  int vircol=bottle[y][x];
  //sukuriam ne viruso spalva
  int nonvircol=rand()%3+1;
  while (nonvircol==vircol)
   nonvircol=rand()%3+1;

  int count=1;
  int vx=x;
  int vy=y;
  //einam i virsu
  while ((vy>0)&&(bottle[vy][x]==vircol)){
   vy--;
   if (bottle[vy][x]==vircol)
    count++;
  }

  if (count>=maxcount)
	bottle[y][x]=nonvircol;
  else{
   //einam i apacia
   vy=y;

   while ((vy<16)&&(bottle[vy][x]==vircol)){
    vy++;
    if (bottle[vy][x]==vircol)
     count++;
   }


   if (count>=maxcount)
	bottle[y][x]=nonvircol;

   else{
	//einam i kaire
    count=1;
    while ((vx>0)&&(bottle[y][vx]==vircol)){
      vx--;
      if (bottle[y][vx]==vircol)
       count++;
    }

    if (count>=maxcount)
		bottle[y][x]=nonvircol;
    else{
     vx=x;
     while ((vx<8)&&(bottle[y][vx]==vircol)){
      vx++;
      if (bottle[y][vx]==vircol)
       count++;
     }

	 if (count>=maxcount)
		bottle[y][x]=nonvircol;

	}


   }

  }


}


//--------------------------------------
void Bottle::generate(int level){

    int x,y;

    for (int z=0;z<16;z++)//isvalom butelka
        for (int y=0;y<8;y++)
            bottle[z][y]=0;

    int maxpart = (level*4)/3;

    rviruscount = maxpart-maxpart/5;
    if (maxpart/5){
        rviruscount += rand()%maxpart/5;
    }

    maxpart = (level*4 - rviruscount)/2;

    yviruscount = maxpart - maxpart/5;
    if (maxpart/5){
        yviruscount += rand()%maxpart/5;
    }

    bviruscount = level*4 - rviruscount - yviruscount;

//------------------------
    int vcount[3];
    vcount[0] = bviruscount ;
    vcount[1] = yviruscount ;
    vcount[2] = rviruscount ;

    for (int a = 0; a<3; a++){
        for (int i=0;i<vcount[a];i++){
            x=rand()%8;
            y=rand()%11+5;

            while (bottle[y][x]){
                x=rand()%8;
                y=rand()%11+5;
            }


            bottle[y][x]=a+1;

            checkvirus(x,y,3);
            checkvirus(x,y,4);

        }
    }

    _viruscount=level*4;


}
