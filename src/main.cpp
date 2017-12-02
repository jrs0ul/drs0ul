/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul                                          *
 *   jrs0ul@gmail.com                                                      *
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
#ifdef WIN32
    #ifdef _MSC_VER
        #pragma comment(lib,"SDL.lib")
        #pragma comment(lib,"SDLmain.lib")
        #pragma comment(lib,"OpenGl32.lib")
        #pragma comment(lib,"GLU32.lib")
        #pragma comment(lib,"openal32.lib")
        #ifdef _DEBUG
            #pragma comment(lib,"ogg_d.lib")
            #pragma comment(lib,"vorbis_d.lib")
            #pragma comment(lib,"vorbisfile_d.lib")
        #else
            #pragma comment(lib,"ogg.lib")
            #pragma comment(lib,"vorbis.lib")
            #pragma comment(lib,"vorbisfile.lib")
        #endif
    #endif
#endif
//--------------------------------------

#ifdef WIN32
    #ifdef _MSC_VER
        #include <SDL.h>
        #include <SDL_opengl.h>
        #include <alc.h>
    #else
        #include <SDL/SDL.h>
        #include <AL/alc.h>
        #include <SDL/SDL_opengl.h>
    #endif
#else
	#include <SDL/SDL.h>
	#include <AL/alc.h>
	#include <SDL/SDL_opengl.h>
#endif


#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "Image.h"
#include "TextureLoader.h"
#include "Utils.h"
#include "SoundSystem.h"
#include "OggStream.h"
#include "CIniFile.h"
#include "Colors.h"
#include "Pill.h"
#include "Player.h"
#include "EvilVirus.h"
#include "Bottle.h"
#include "SelectMenu.h"

#ifdef WIN32
	#ifdef _DEBUG
	#include <crtdbg.h>//ieskoti memory leaku
	#endif
    #define WIN32_LEAN_AND_MEAN
#endif


//============================



struct iPoint{
  int x;
  int y;
  iPoint(int dx, int dy){
      x = dx; y = dy;
  }
};

struct nextCPill{
  unsigned char col1;
  unsigned char col2;
  void generate(){
	col1=rand()%3*6;
	col2=rand()%3*6;
  }
};


//===================================GLOBALS


bool LIMIT_FPS=false;
bool PAUSE_GAME = false;
int SCREENW = 640;
int SCREENH = 480;
bool FULLSCREEN = false;
bool USE_SOUND = 1;
bool USE_CUSTOM_SOUND_DEVICE = false;


Uint32 tick = 0;

PicsContainer pics;


SoundSystem SS;
char SoundDeviceName[128] = {0};

OggStream Stream;


bool IsWindowed = true;
bool joypresent = false;
bool inactive = false;
bool musicon = true;
bool sfxon = true;



int keys[6] = {0};
int keys2[6] = {0};



Bottle Bottle1, Bottle2;


Player player1(1),
       player2(2);


int virustim = 0;
int virusframe = 0;


bool gameover=false;
bool gameon=false;
int menustate=0;
bool shutdown=false;


bool playeronemenu=false;
bool playertwomenu=false;

int p1menustate=0;
int p2menustate=0;

bool configkeys=false;
bool configkeys2=false;
int actionkey=0;


Pill Pill1;
Pill Pill2;

iPoint P1NextPill(0,0);
iPoint P2NextPill(0,0);

bool showdebugtext=false;

bool PVPend=false;

int highscore=10000;


int p1keycodes[5]={0};
int p2keycodes[5]={0};

CIniFile INI;

nextCPill nextpills[50];

bool isMultiplayer = false; // ar du zaidejai

bool _QuitApp = false;

SDL_Joystick *Joy;


int GlobalKey = 0;

SelectMenu MMenu;
SelectMenu OMenu;

EvilVirus Vir;

//=======================================

void QuitApp(){
    _QuitApp = true;
}


//----------------------------------
int FPS (void){
	static int time = 0, FPS = 0, frames = 0, frames0 = 0;
	if ((int)SDL_GetTicks() >= time) {
		FPS = frames-frames0;
		time = SDL_GetTicks()+1000;
		frames0 = frames;
	}
	frames = frames+1;
	return FPS;
}
//----------------------------
void InitOpenAL(){
	bool res = false;

    if (USE_CUSTOM_SOUND_DEVICE)
        res = SS.init( SoundDeviceName );
    else
        res = SS.init(NULL);

    if (res){
        Vector3D p(0,0,0);
        SS.setupListener(p,p);
        SS.loadFiles("audio/list.txt");
    }
    else
        USE_SOUND = false;

}
//-----------------------------------------
void QuitOpenAL(){
	SS.freeData();
	SS.exit();

}

//----------------------------------------

bool InitInput(){

    FILE* f;
	f=fopen("keys.cfg","rt");
	if (f){
	 for (int i=0;i<5;i++){
      fscanf(f,"%d",&p1keycodes[i]);
     }
	 for (int i=0;i<5;i++){
      fscanf(f,"%d",&p2keycodes[i]);
     }
	 fclose(f);
	} else
	{
	  f= fopen("keys.cfg","wt+");


	  p1keycodes[0]=273; p1keycodes[3]=275;
	  p1keycodes[1]=274; p1keycodes[4]=256;
	  p1keycodes[2]=276;

	  fprintf(f,"%d %d %d %d %d\n",
		  p1keycodes[0], p1keycodes[1], p1keycodes[2],
		  p1keycodes[3], p1keycodes[4]);

	  p2keycodes[0]=119; p2keycodes[3]=100;
	  p2keycodes[1]=115; p2keycodes[4]=9;
	  p2keycodes[2]=97;

	  fprintf(f,"%d %d %d %d %d\n",
		  p2keycodes[0], p2keycodes[1], p2keycodes[2],
		  p2keycodes[3], p2keycodes[4]);

	  fclose(f);
	}

	return true;
}

//------------------------------------

void CheckKeys(){

    Uint8 * dkeys;
    dkeys = SDL_GetKeyState ( NULL );

    memset(keys, 0, sizeof(int) * 6);
    memset(keys2, 0, sizeof(int) * 6);

    if (gameon){
        if (dkeys[p1keycodes[0]] == SDL_PRESSED)
            keys[0]=1;
        if (dkeys[p1keycodes[1]] == SDL_PRESSED)
            keys[1]=1;
        if (dkeys[p1keycodes[2]] == SDL_PRESSED)
            keys[2]=1;
        if (dkeys[p1keycodes[3]] == SDL_PRESSED)
            keys[3]=1;
        if (dkeys[p1keycodes[4]] == SDL_PRESSED)
            keys[4]=1;
    }
    else{
         if (dkeys[SDLK_UP] == SDL_PRESSED)
            keys[0] = 1;
         if (dkeys[SDLK_DOWN] == SDL_PRESSED)
            keys[1] = 1;
        if (dkeys[SDLK_LEFT] == SDL_PRESSED)
            keys[2] = 1;
        if (dkeys[SDLK_RIGHT] == SDL_PRESSED)
            keys[3] = 1;
        if (dkeys[SDLK_RETURN] == SDL_PRESSED)
            keys[4] = 1;

        if (configkeys)
            if (dkeys[p1keycodes[4]] == SDL_PRESSED)
                keys[4]=1;
    }


   if (dkeys[p2keycodes[0]] == SDL_PRESSED)  keys2[0]=1;
   if (dkeys[p2keycodes[1]] == SDL_PRESSED)  keys2[1]=1;
   if (dkeys[p2keycodes[2]] == SDL_PRESSED)  keys2[2]=1;
   if (dkeys[p2keycodes[3]] == SDL_PRESSED)  keys2[3]=1;
   if (dkeys[p2keycodes[4]] == SDL_PRESSED)  keys2[4]=1;


    if (SDL_NumJoysticks()>0) {
        SDL_JoystickUpdate ();

        if (Joy){
            if (SDL_JoystickGetAxis(Joy, 0)>1)
                keys[3]=1;
            if (SDL_JoystickGetAxis(Joy, 0)<-1)
                keys[2]=1;
            if (SDL_JoystickGetAxis(Joy, 1)<-1)
                keys[0]=1;
            if (SDL_JoystickGetAxis(Joy, 1)>1)
                keys[1]=1;

            if ( SDL_JoystickGetButton (Joy, 0))
                keys[4]=1;
            if ( SDL_JoystickGetButton (Joy, 1))
                keys[5]=1;
        }

    }



       GlobalKey = 0;
	if ( keys[0] )
		GlobalKey = 38;
	if ( keys[1] )
		GlobalKey = 40;
	if ( keys[4] )
		GlobalKey = 13;
	if ( keys[5] )
		GlobalKey = 27;


}




//-------------------

void DrawSomeText(){

	//biski texto

	char buf[80];

	sprintf(buf,"FPS : %d",FPS());
	DrawText(12,30,buf,pics,0);
	if (gameon)
        DrawText(12,80,"gameon",pics,0);
    else
        DrawText(12,80,"gameoff",pics,0);
	sprintf(buf,"r%d + y%d + b%d = %d",Bottle1.redVirusCount(),Bottle1.yellowVirusCount(),Bottle1.blueVirusCount(),Bottle1.viruscount());
	DrawText(12,50,buf,pics,0);

}
//--------------------
void generateBottle2(){
    if (player1.level==player2.level){
        Bottle2 = Bottle1;
    }
    else
        Bottle2.generate(player2.level);
}


//------------------
void DrawBottle(int x, int y, Bottle& bottle, Player& player){

    pics.draw(3,x,y,0,false);
    pics.draw(3,x,y+89,1,false);
    pics.draw(3,x,y+178,1,false);
    pics.draw(3,x,y+267,2,false);


    if ((!player.isGameOver())&&(!player.nextstage)){

        for(int a = 0;a<16;a++)
            for(int i=0;i<8;i++)

                if (bottle.getTile(i,a)){

                    if(bottle.getTile(i,a)<4)
                        pics.draw(2,x+16+i*16,y+80+a*16,(bottle.getTile(i,a)-1)*3+virusframe);
                    else
                    if(bottle.getTile(i,a)>=4)
                        pics.draw(1,x+16+i*16,y+80+a*16,bottle.getTile(i,a)-4);


                }

    }
    else{
        if (player.isGameOver())
            pics.draw(8,x+20,y+150);
        else
            if (player.nextstage){
                if (player.level==20)
                    pics.draw(16,x+20,y+150);
                else
                    pics.draw(9,x+20,y+150);
            }
    }

}


//----------------
void animateVirus(){

 virustim++;
 if (virustim==10){
  virusframe++;
  virustim=0;
 }
 if (virusframe==3){
	 virusframe=0;
 }
}



//---------------
//greicio counteris
void countTics(Player& player){
 player.ticsplayed++;
 if (player.ticsplayed%(800+player.level*50)==0){
  if (player.speed>0)
   player.speed--;
 }
}

//------------------

void WinerCheck(Player& player, Bottle& bottle, Player& oponent, bool multi){

    if (bottle.viruscount() == 0){

        player.nextstage = true;

        if (multi){

            player.winRound();
            oponent.loseMatch();

            if (player.victoryCount() == 3){
                PVPend = true;
            }

        }
    }

}



//---------------------------------------------------------------------------

void Game( Bottle& bottle, //butelis
           Player& player, //playeris
           Player& oponent,
           Pill& pill, //piliule
           int* keys, //mygtukai
           bool isMulti = false
         ){



    if ( (!player.isGameOver()) && (!player.nextstage) ){

        if (player.nextpill){

            pill.make();

            if (isMulti){

                pill.col1 = nextpills[player.pillcounter].col1 + 4;
                pill.col2 = nextpills[player.pillcounter].col2 + 7;
                player.pillcounter++;

                if (player.pillcounter > 50){

                    for (int i = 0; i < 50; i++)
                        nextpills[i].generate();

                    player.pillcounter = 0;
                }

                pill.nextcol1 = nextpills[player.pillcounter].col1;
                pill.nextcol2 = nextpills[player.pillcounter].col2;
            }

            if (bottle.getTile(3, 0)){

                player.loseMatch();

                if (isMulti){
                    if (player.ID() == 1){

                        player2.nextstage = true;
                        player2.winRound();
                        if (player2.victoryCount() == 3)
                            PVPend = true;

                    }
                    else{
                        player1.nextstage = true;
                        player1.winRound();
                        if (player1.victoryCount() == 3)
                            PVPend = true;
                    }
                }
            }

            player.nextpill=false;
        }




        if (!player.falldown){

            if (player.dudetim == 19)
                player.nextpill = true;

            if (!player.lenkranka){

                countTics(player);


                if ((pill.y + pill.party + 1 < 16)
                    &&(bottle.getTile(pill.x + pill.partx, pill.y + pill.party + 1) == 0)
                    &&(bottle.getTile(pill.x, pill.y + pill.party + 1) == 0) || (player.lasticks < 25)){


                    if(!((pill.y + pill.party + 1 < 16)
                        &&(bottle.getTile(pill.x + pill.partx, pill.y + pill.party +1) == 0)
                        &&(bottle.getTile(pill.x, pill.y + pill.party + 1) == 0)))

                        player.lasticks ++ ;

                    bottle.setTile(pill.x, pill.y, 0);            //isvalom senaja busena
                    bottle.setTile(pill.x + pill.partx, pill.y + pill.party, 0);


                    //piliule krenta

                    pill.tim ++ ;
                    if (pill.tim > player.speed){
                        if ((pill.y + pill.party + 1 < 16)
                            &&(bottle.getTile(pill.x + pill.partx, pill.y + pill.party + 1) == 0)
                            &&(bottle.getTile(pill.x, pill.y + pill.party + 1) == 0))

                            pill.y++;

                        pill.tim = 0;
                    }

                    //pasukam piliule --

                    if (!keys[4])
                        player.rotatepressed = false;

                    if ( (keys[4]) && (!player.rotatepressed) ){
                        if (USE_SOUND){
                            if (sfxon)
                                SS.playsound(1);
                        }
                        pill.rotate(bottle);

                        player.rotatepressed = true;
                    }
                    //-----

                    //stumiam i desine
                    if ((keys[2]) && ( pill.x > 0)
                        &&(!bottle.getTile(pill.x - 1, pill.y))
                        &&(!bottle.getTile(pill.x + pill.partx - 1,pill.y + pill.party))){

                        pill.timx ++;
                        if (pill.timx == 5){
                            pill.x--;
                            pill.timx = 0;
                        }
                    }
                    //i kaire ?
                    if ((keys[3]) && (pill.x + abs(pill.partx) < 7)
                        &&(!bottle.getTile(pill.x + 1, pill.y))
                        &&(!bottle.getTile(pill.x + pill.partx + 1, pill.y + pill.party))){
                        pill.timx++;
                        if (pill.timx == 5){
                            pill.x++;
                            pill.timx = 0;
                        }
                    }

                    //zemyn
                    if (keys[1])
                        pill.tim += 10;


                    //padedam nauja
                    bottle.setTile(pill.x, pill.y, pill.col1);
                    bottle.setTile(pill.x + pill.partx, pill.y + pill.party, pill.col2);

                    WinerCheck(player, bottle, oponent, isMulti);

                } //ar krenta

                else { //nukrito
                    player.lasticks = 0;
                    player.falldown = bottle.eliminate(player, SS, 0, sfxon);

                    if (bottle.redVirusCount() == 0)
                        Vir.kill(2);
                    if (bottle.yellowVirusCount() == 0)
                        Vir.kill(1);
                    if (bottle.blueVirusCount() == 0)
                        Vir.kill(0);
                    if ((!player.falldown) && (bottle.viruscount())){

                        player.lenkranka = true;

                    }
                }


            } //nelenkia rankos


        } //nekrenta atliekos

        else{
            if (player.craptim > 4){

                player.craptim = 0;
                bottle.clearcrap();
                bottle.pillsFall();
                player.falltic++;

                if (player.falltic == 15){
                    player.falldown = false;
                    player.falltic = 0;

                    player.falldown = bottle.eliminate(player, SS, 0, sfxon);

                    if (bottle.redVirusCount() == 0)
                        Vir.kill(2);
                    if (bottle.yellowVirusCount() == 0)
                        Vir.kill(1);
                    if (bottle.blueVirusCount() == 0)
                        Vir.kill(0);

                    if ((!player.falldown) && (bottle.viruscount())){

                        player.lenkranka = true;

                    }

                }

            }
            else
                player.craptim++;

        }//krito sudai

    } //ne geimoveris ir ne kitas lygis

    else{

        if ((player.oldkey) && (!keys[4]) && (player.isGameOver()) && (!PVPend)){ //geimoveris ?

            if (!isMulti){

                gameover = true;

                if (player.score() > highscore)
                    highscore = player.score();
            }

            else{

                for (int i = 0; i < 50; i++)
                    nextpills[i].generate();

                player1.pillcounter = 0;
                player2.pillcounter = 0;

                bottle.generate(player.level);



                if (player.ID() == 1){
                    if (player2.nextstage){
                        player2.nextstage = false;
                        player2.lenkranka = true;
                        generateBottle2();
                    }
                }
                else{
                    if (player1.nextstage){
                        player1.nextstage = false;
                        player1.lenkranka = true;
                        Bottle1.generate(player1.level);
                        generateBottle2();
                    }
                }
            }


            player.resetMatch();
            player.ticsplayed = 0;
            player.speed = player.basespeed;

        }//geimoveris ?
        else
        if ((player.oldkey) && (!keys[4]) && (player.nextstage)){ //kitas lygis

            player.nextstage = false;

            if (!isMulti){

                player.level++;

                Vir.init();

                if (player.level > 20) {

                    gameover = true;
                    player.level = 0;

                }
            }

            bottle.generate(player.level);

            player.lenkranka = true;

            if (isMulti){

                for (int i=0; i<50; i++)
                    nextpills[i].generate();

                player1.pillcounter=0;
                player2.pillcounter=0;

                if (player.ID() == 1){
                    if (player2.isGameOver()){
                        player2.resetMatch();
                        generateBottle2();
                        player2.lenkranka = true;
                    }
                }
                else{

                    if (player1.isGameOver()){

                        player1.resetMatch();
                        Bottle1.generate(player1.level);
                        generateBottle2();
                        player1.lenkranka=true;
                    }
                }
            }




	player.ticsplayed=0;
	player.speed=player.basespeed;
   }
 }


}

//------------------

void SetKeys(int* keycodes, bool& configkeys){

    Uint8 * dkeys;
    int num=0;
    dkeys = SDL_GetKeyState (&num);
    int i=1;
    while (!(dkeys[i] == SDL_PRESSED)&&(i<num))
        i++;


 if (i<num){

  int a=0;
  while ((keycodes[a]!=i)&&(a<actionkey))
   a++;
  if ((a<=actionkey)&&(keycodes[a]!=i)&&(actionkey<5)&&(i!=300)){
   keycodes[actionkey]=i;
   actionkey++;
  }
 }


 if ((actionkey==5)&&(((player1.oldkey)&&(!keys[4]))||((player2.oldkey)&&(!keys2[4]))))
	 configkeys=false;

 FILE* f;

 f=fopen("keys.cfg","wt+");
 if (!f)
  return;

 for (i=0;i<5;i++){
  fprintf(f,"%d ",p1keycodes[i]);
 }
 fputc('\n',f);

 for (i=0;i<5;i++){
  fprintf(f,"%d ",p2keycodes[i]);
 }

 fclose(f);


}



//------------------
void OptionsMenu(){

    if (!OMenu.selected)
        OMenu.getInput(GlobalKey);
    else {
        switch(OMenu.state){
            case 0: {
                if (musicon){
                    Stream.stop();
                    musicon=false;
                    strcpy(OMenu.selection.opt[0] , "Music Off");

                }
                else{
                    Stream.playback();
                    strcpy(OMenu.selection.opt[0], "Music On");
                    musicon=true;

                }
                OMenu.selected = false;
            } break;
            case 1: {

                if (sfxon){
				  sfxon=false;
				  strcpy(OMenu.selection.opt[1], "Sound fx Off");
                }
                else{
				  sfxon=true;
				  strcpy(OMenu.selection.opt[1], "Sound fx On");
                }

              OMenu.selected = false;
            } break;
            case 2: {
                configkeys = true;
                actionkey = 0;
                memset(p1keycodes,0,sizeof(int)*5);
                OMenu.selected = false;
            } break;

            case 3: {
                configkeys2 = true;
                actionkey = 0;
                memset(p2keycodes,0,sizeof(int)*5);
                OMenu.selected = false;
            } break;
            case 4: {
                OMenu.reset();
                OMenu.deactivate();
                MMenu.activate();
            } break;
        }
    }



}


//-------------------

void PlayerOneMenu(){

 static int keytim=0;

 if (keys[3]){
	  keytim++;
      if (keytim>2){
	   keytim=0;
	   if (p1menustate==0){
	    if (player1.level<20)
	     player1.level++;
	   }
	   else{
		  if (player1.speed>0){
	       player1.speed--;
		   player1.basespeed--;
		  }
	   }
	  }
	}
  if (keys[2]){
	  keytim++;
	  if (keytim>2){
	   keytim=0;
	   if (p1menustate==0){
	    if (player1.level>1)
	     player1.level--;
	   }
	   else{
		 if (player1.speed<30){
	      player1.speed++;
		  player1.basespeed++;
		 }
	   }
	  }
  }
  if (keys[0]){
	  keytim++;
	  if (keytim>2){
	   keytim=0;
	   if (p1menustate>0)
	    p1menustate--;
	  }
  }
  if (keys[1]){
	  keytim++;
	  if (keytim>2){
	   keytim=0;
	   if (p1menustate<1)
	    p1menustate++;
	  }
  }


  if ((player1.oldkey)&&(!keys[4])){


	   player1.setScore(0);
	   player1.lenkranka=true;
	   gameon=true;
	   playeronemenu=false;
	   Bottle1.generate(player1.level);
	   Vir.init();

  }




}

//------------------
void PlayerTwoMenu(){
 static int keytim=0;
 static int keytim2=0;

 if (keys[3]){
	  keytim++;
      if (keytim>2){
	   keytim=0;
	   if (p1menustate==0){
	    if (player1.level<20)
	     player1.level++;
	   }
	   else{
		   if (player1.speed>0){
	        player1.speed--;
			player1.basespeed--;
		   }
	   }
	  }
	}
 if (keys2[3]){
	  keytim2++;
      if (keytim2>2){
	   keytim2=0;
	   if (p2menustate==0){
	    if (player2.level<20)
	     player2.level++;
	   }
	   else{
		   if (player2.speed>0){
	        player2.speed--;
			player2.basespeed--;
		   }
	   }
	  }
	}
  if (keys[2]){
	  keytim++;
	  if (keytim>2){
	   keytim=0;
	   if (p1menustate==0){
	    if (player1.level>1)
	     player1.level--;
	   }
	   else{
		   if (player1.speed<30){
	        player1.speed++;
			player1.basespeed++;
		   }
	   }
	  }
  }
  if (keys2[2]){
	  keytim2++;
	  if (keytim2>2){
	   keytim2=0;
	   if (p2menustate==0){
	    if (player2.level>1)
	     player2.level--;
	   }
	   else{
		  if (player2.speed<30){
	       player2.speed++;
		   player2.basespeed++;
		  }
	   }
	  }
  }
  if (keys[0]){
	  keytim++;
	  if (keytim>2){
	   keytim=0;
	   if (p1menustate>0)
	    p1menustate--;
	  }
  }
  if (keys2[0]){
	  keytim2++;
	  if (keytim2>2){
	   keytim2=0;
	   if (p2menustate>0)
	    p2menustate--;
	  }
  }
  if (keys[1]){
	  keytim++;
	  if (keytim>2){
	   keytim=0;
	   if (p1menustate<1)
	    p1menustate++;
	  }
  }
  if (keys2[1]){
	  keytim2++;
	  if (keytim2>2){
	   keytim2=0;
	   if (p2menustate<1)
	    p2menustate++;
	  }
  }

  if ((player1.oldkey)&&(!keys[4])){

   isMultiplayer = true;
   player1.setScore(0);
   player2.setScore(0);
   player2.lenkranka=true;
   player1.lenkranka=true;
   gameon=true;
   Bottle1.generate(player1.level);
   generateBottle2();
   playertwomenu=false;

   for (int i=0; i<50; i++){
	 nextpills[i].generate();
   }
   player1.pillcounter=0;
   player2.pillcounter=0;

  }
}

//-------------------------
void MainMenu(){
      if (!MMenu.selected)
            MMenu.getInput(GlobalKey);
        else{
            switch(MMenu.state){
                case 0:{
                    playeronemenu=true;
                    isMultiplayer = false;
                }
                break;
                case 1: playertwomenu=true;
                break;
                case 2: OMenu.activate(); break;
                case 3:QuitApp();
            }
            MMenu.deactivate();
            MMenu.reset();
        }
}

//--------------------
void OnTimer(){

    if (!gameon){

        if (MMenu.active()){
            MainMenu();
        }
        else{
            if (playeronemenu)
                PlayerOneMenu();
            else
            if (playertwomenu){
                PlayerTwoMenu();
            }
            else
            if (OMenu.active()){
                if ((!configkeys)&&(!configkeys2))
                    OptionsMenu();
                else {
                    if (configkeys)
                        SetKeys(p1keycodes,configkeys);
                    if (configkeys2)
                        SetKeys(p2keycodes,configkeys2);
                }
            }
        }
    }
    else{

        Vir.move();

        if (player1.lenkranka){

            player1.dudetim++;

            if (!isMultiplayer){

                if (player1.dudetim < 10)
                    P1NextPill.y -= 5;
                if (P1NextPill.x > -100)
                    P1NextPill.x -= 10;
                else
                    P1NextPill.y += 10;

                if (player1.dudetim == 20){
                    player1.dudetim = 0;
                    P1NextPill.x = 0;
                    P1NextPill.y = 0;
                    player1.lenkranka = false;
                }
            }
            else{
                P1NextPill.y += 2;
                if (player1.dudetim == 20){
                    player1.dudetim = 0;
                    P1NextPill.x = 0;
                    P1NextPill.y = 0;
                    player1.lenkranka = false;
                }
            }


        }

        if (isMultiplayer){
            if (player2.lenkranka){
                player2.dudetim++;
                if (P2NextPill.x > -100)
                    P2NextPill.x -= 10;
                else
                    P2NextPill.y += 10;
                if (player2.dudetim == 20){
                    player2.dudetim=0;
                    P2NextPill.x=0;
                    P2NextPill.y=0;
                    player2.lenkranka=false;
                }
            }
        }

    animateVirus();

    Game(Bottle1,player1, player2, Pill1, keys, isMultiplayer);

    if (isMultiplayer)
        Game(Bottle2, player2, player1, Pill2, keys2, isMultiplayer);

  if (PVPend){
	  player1.loseMatch();
	  player2.loseMatch();

	  if (((player1.oldkey)&&(!keys[4]))||((player2.oldkey)&&(!keys2[4])))
	  {
		  player1.resetMatch();
		  player2.resetMatch();
		  player1.nextstage=false;
		  player2.nextstage=false;
		  PVPend=false;
		  gameover=true;
		  player1.setVictoryCount(0);
		  player2.setVictoryCount(0);
	  }

  }

 }
 if (gameover) {
	 gameon=false;
	 gameover=false;
	 MMenu.activate();
 }
}

//--------------
void scroller(int x, int y, int spot,int step,int n){
    for (int i=0;i<n;i++)
        pics.draw(14,x+16*i,y,1);
    pics.draw(14, x-8+step*spot,y,0);
}


//-----------------

void drawpstats(Player& player, int x, int y, int state){

 pics.draw(2,x,y+50*state,3);


 scroller(x+40,y+30,player.level,4,5);
 scroller(x+40,y+80,30-player.speed,4,8);


 char buf[50];
 sprintf(buf,"Level: %d",player.level);
 DrawText(x+22,y+2,buf,pics,0,1.0f,COLOR(0,0,0),COLOR(0,0,0));
 DrawText(x+20,y,buf,pics,0, 1.0f, COLOR(1,1,0),COLOR(0,1,1));

 sprintf(buf,"Speed: %d",30-player.speed);
 DrawText(x+22,y+52,buf,pics,0, 1.0f, COLOR(0,0,0),COLOR(0,0,0));
 DrawText(x+20,y+50,buf,pics,0, 1.0f, COLOR(1,1,0),COLOR(0,1,1));

}

//-----------------
void drawp1menu(){

    pics.draw(15,280,200);

    DrawText(SCREENW/2-(32*12)/2+2,452,"ACTION to confirm, ESC to cancel",pics,0,1.0f, COLOR(0,0,0), COLOR(0,0,0));
    DrawText(SCREENW/2-(32*12)/2,450,"ACTION to confirm, ESC to cancel",pics,0, 1.0f, COLOR(1,1,0), COLOR(0,1,1));


    drawpstats(player1,230,280,p1menustate);

}

//--------------
void drawp2menu(){

 drawpstats(player1,70,280,p1menustate);
 drawpstats(player2,450,280,p2menustate);
 pics.draw(12,260,200,1);
 pics.draw(15,70,245);
 pics.draw(15,450,245,1);

 DrawText(SCREENW/2-(32*12)/2+2,452,"ACTION to confirm, ESC to cancel",pics,0,1.0f, COLOR(0,0,0), COLOR(0,0,0));
 DrawText(SCREENW/2-(32*12)/2,450,"ACTION to confirm, ESC to cancel",pics,0, 1.0f, COLOR(1,1,0), COLOR(0,1,1));
}


//-----------------
void drawoptmenu(){

 char tmp[5][7]={"Up","Down","Left","Right","Action"};

 OMenu.draw(pics, 2, 0, 0);


 if ((configkeys)||(configkeys2)){
  char buf[50];
  if (actionkey<5){
   sprintf(buf,"Press key for %s",tmp[actionkey]);
   DrawText(SCREENW/2-(strlen(buf)*12)/2+2,342,buf,pics,0, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
   DrawText(SCREENW/2-(strlen(buf)*12)/2,340,buf,pics,0, 1.0f, COLOR(1,1,0), COLOR(0,1,1));
  }
 }


}


//--------------------------
void drawNum(int x, int y, int num, int maxlen){
 int i=maxlen;
 unsigned char masyvas[10];

 memset(masyvas,0,maxlen);
 while (num>0){
   masyvas[i-1]=num%10;
   num=num/10;
   i--;

 }

 for (int a=0; a<maxlen;a++)
   pics.draw(5,x+a*16,y,masyvas[a]);


}

//-----------------------

void drawstatus(int x, int y, Player& player, Bottle& bot){

 pics.draw(6,x,y);

 drawNum(x+80,y+25,player.level,2);
 drawNum(x+80,y+55, bot.viruscount(), 2);
 drawNum(x+80,y+90,30-player.speed,2);
}

//------------------------------
void drawscore(int x, int y, Player& player){
    pics.draw(7,x,y);
    drawNum(x+15,y+40,highscore,7);
    drawNum(x+15,y+80,player.score(), 7);
}
//--------------------------
void drawFrags(int x, int y){
 pics.draw(12,x,y);

 for (int a=0; a < player1.victoryCount(); a++)
  pics.draw(2,x+17,y+27+18*a,0+virusframe);

 for (int a=0; a<player2.victoryCount(); a++)
  pics.draw(2,x+38,y+27+18*a,6+virusframe);
}


//---------------------------
void RenderScreen(){


    glClearColor (0, 0, 0, 1.0);
	glClear( GL_COLOR_BUFFER_BIT  );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if ((isMultiplayer)&&(gameon)){
        PicData* p;
        p=pics.getInfo(10);
        pics.draw(10,SCREENW/2.0f,SCREENH/2.0f,0,true,SCREENW/(p->width*1.0f), SCREENH/(p->height*1.0f));
    }


    if (gameon){

        if (!isMultiplayer){
            PicData* p;
            p=pics.getInfo(17);
            pics.draw(17,SCREENW/2.0f,SCREENH/2.0f,0,true,SCREENW/(p->width*1.0f), SCREENH/(p->height*1.0f));

            DrawBottle(250,100,Bottle1,player1);

            int frm=0;

            if (player1.lenkranka)
                frm=1;

            if (player1.isGameOver())
                frm = 2;

            pics.draw(11,420,30,frm);
            if (!player1.isGameOver())
                Pill1.drawnext(pics,1, 415 + P1NextPill.x, 30 + P1NextPill.y);

            drawscore(10,100,player1);
            drawstatus(480,200,player1, Bottle1);

            int vfrm = 0;
            if (virusframe < 2)
                vfrm = virusframe;

            Vir.draw(pics, 18, vfrm );

        }
        else{
            pics.draw(15, 10, 10, 0);
            drawstatus(190, 5, player1, Bottle1);
            DrawBottle(80, 100, Bottle1, player1);

            if (player1.lenkranka)
                Pill1.drawnext(pics, 1, 146, 50 + P1NextPill.y);
            else
                Pill1.drawnext(pics, 1, 146, 50);

            pics.draw(15,550,10,1);
            drawstatus(320,5,player2,Bottle2);
            DrawBottle(400,100,Bottle2,player2);
            if (player2.lenkranka)
                Pill2.drawnext(pics, 1, 466, 50 + P2NextPill.y);
            else
                Pill2.drawnext(pics, 1, 466, 50);


	  drawFrags(286,170);

	  if (PVPend){
		  if (player1.victoryCount() > player2.victoryCount()){
			pics.draw(15,250,400,0);
			pics.draw(13,340,400,0);
		  }
		  else{
			pics.draw(15,250,400,1);
			pics.draw(13,340,400,0);
		  }
	  }


	 }
    }
	else{

	 PicData* p;
	 p=pics.getInfo(4);

     pics.draw(4,SCREENW/2.0f,SCREENH/2.0f,0,true,SCREENW/(p->width*1.0f), SCREENH/(p->height*1.0f));

     if (MMenu.active())
        MMenu.draw(pics, 2, 0, 0);


	  if (playeronemenu)
	   drawp1menu();
	  else
	  if (playertwomenu)
	   drawp2menu();
	  if (OMenu.active())
	   drawoptmenu();
	}

    if (PAUSE_GAME){
        DrawText(SCREENW/2-(3*12*5+17)+3, SCREENH/2+3, "Game Paused", pics, 0, 3, COLOR(0,0,0),COLOR(0,0,0));
        DrawText(SCREENW/2-(3*12*5+17), SCREENH/2, "Game Paused", pics, 0, 3, COLOR(1,0,1));

    }

    if (showdebugtext)
     DrawSomeText();

	glDisable(GL_BLEND);

	glFlush();

	//````````````

	SDL_GL_SwapBuffers( );

}




//---------------------------------
static void process_events( void ){

	SDL_Event event;

	while( SDL_PollEvent( &event ) ) {

		switch( event.type ) {
		case SDL_KEYUP:
			switch( event.key.keysym.sym ) {
				case SDLK_ESCAPE:{
					if ((gameon)||(playeronemenu)||(OMenu.active())||(playertwomenu)){
                        if (gameon){
                            gameon=false;
                            MMenu.activate();
                        }
                        if (playeronemenu){
                            playeronemenu=false;
                            MMenu.activate();
                        }
                        if (playertwomenu){
                            playertwomenu=false;
                            MMenu.activate();
                        }
                        if (OMenu.active()){
                            OMenu.deactivate();
                            OMenu.reset();
                            MMenu.activate();
                        }
                    }

                    else
                        QuitApp();
								 }
				break;
				case SDLK_F1:{
					showdebugtext=!showdebugtext;
								 }
				 break;

				 case SDLK_F4:{
				     int flags=0;
				     if (FULLSCREEN){
                        flags = SDL_OPENGL;
                        FULLSCREEN = false;
				     }
                    else{
                        flags =  SDL_OPENGL | SDL_FULLSCREEN;
                        FULLSCREEN = true;
                    }
				     SDL_SetVideoMode( SCREENW, SCREENH, 32, flags );
				 } break;

				case SDLK_p:
					PAUSE_GAME=!PAUSE_GAME;
                break;
                default:break;

			}
			break;
		case SDL_QUIT:

			QuitApp();
			break;
		}

	}

}
//----------------------------------

void ReadIniFile(){
	INI.read("drsoul.ini");
	wchar_t buf[255];
	char abuf[255];
	/*INI.get ( L"width",buf );
	wcstombs(abuf,buf,255);
	SCREENW = atoi ( abuf );
	if ( !SCREENW )
		SCREENW = 640;
	INI.get ( L"height",buf );
	wcstombs(abuf,buf,255);
	SCREENH = atoi ( abuf );
	if ( !SCREENH )
		SCREENH = 480;*/
	INI.get ( L"fullscreen",buf );
	wcstombs(abuf,buf,255);
	 if (!atoi ( abuf ))
		FULLSCREEN=false;
	else
		FULLSCREEN=true;
	INI.get ( L"limitfps",buf );
	wcstombs(abuf,buf,255);
	if (!atoi ( abuf ))
		LIMIT_FPS=false;
	else
		LIMIT_FPS=true;

     INI.get ( L"usesound",buf );
	wcstombs(abuf,buf,255);
	if (!atoi ( abuf ))
		USE_SOUND = false;
	else
		USE_SOUND = true;

    INI.get ( L"sdevice",buf );
	wcstombs(abuf,buf,255);
	if ( strcmp( abuf, "0" ) == 0 )
        USE_CUSTOM_SOUND_DEVICE = false;
	else{
		USE_CUSTOM_SOUND_DEVICE = true;
		strcpy( SoundDeviceName, abuf );
	}


}
//----------------------------------

static void SetupOpengl( int width, int height ){


	glEnable(GL_TEXTURE_2D);


	glDisable( GL_DEPTH_TEST );
	glDisable(GL_LIGHTING);
	glDisable( GL_DITHER );
	glDisable( GL_BLEND );



	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	gluOrtho2D (0.0, (GLdouble) SCREENW,  (GLdouble) SCREENH, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.375, 0.375, 0.);

	glPushAttrib(GL_DEPTH_BUFFER_BIT);


}

//-------------------------------------------
int main( int argc, char* argv[] ){


	srand(time(0));

	ReadIniFile();

    Smenu m;
    strcpy(m.opt[0], "1 player game");
    strcpy(m.opt[1], "2 player game");
    strcpy(m.opt[2], "Options");
    strcpy(m.opt[3], "Exit");
    m.count=4;

    MMenu.init(SCREENW-250 , SCREENH/2-50,
			 "", m, 0, 20);

    if ( musicon )
        strcpy(m.opt[0], "Music On");
    else
        strcpy(m.opt[0], "Music Off");
    if (sfxon)
        strcpy(m.opt[1], "Sound fx On");
    else
        strcpy(m.opt[1], "Sound fx Off");
    strcpy(m.opt[2], "Change P1 keys");
    strcpy(m.opt[3], "Change P2 keys");
    strcpy(m.opt[4], "Back");
    m.count = 5;
    OMenu.init(SCREENW-250 , SCREENH/2-50,
			 "", m, 0, 20);

    MMenu.activate();




	const SDL_VideoInfo* info = NULL;


	int bpp = 0;

	// Flags we will pass into SDL_SetVideoMode.
	int flags = 0;

	// First, initialize SDL's video subsystem.
	puts("Trying SDL_INIT...");
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {

		fprintf( stderr, "Video initialization failed: %s\n",
			SDL_GetError( ) );
		QuitApp();
	}
	puts("SDL_INIT succsess!");

	// Let's get some video information.
	info = SDL_GetVideoInfo( );

	if( !info ) {
		// This should probably never happen.
		fprintf( stderr, "Video query failed: %s\n",
			SDL_GetError( ) );
		QuitApp();
	}


	bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );


	if (FULLSCREEN)
		flags = SDL_OPENGL | SDL_FULLSCREEN;
	else
		flags =  SDL_OPENGL;

	if( SDL_SetVideoMode( SCREENW, SCREENH, bpp, flags ) == 0 ) {

		fprintf( stderr, "Video mode set failed: %s\n",
			SDL_GetError( ) );
		QuitApp();
	}

    puts("init joystick...");
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    if (USE_SOUND){
        puts("Initializing OpenAL...");
        InitOpenAL();
        puts("done!");
    }

	SDL_ShowCursor(0);
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	SetupOpengl( SCREENW, SCREENH );

    if (USE_SOUND){
        Stream.open("audio/music.ogg");
        Stream.playback();
    }

	pics.load("pics/list.txt");


    InitInput();

    SDL_WM_SetCaption ( "Drs0ul", "Drs0ul" );
    SDL_WM_SetIcon(SDL_LoadBMP("drs0ul.bmp"),NULL);



    if(SDL_NumJoysticks()>0){
        Joy = SDL_JoystickOpen(0);
    }


	while(!_QuitApp) {



		if (SDL_GetTicks() > tick){

            if (USE_SOUND)
                Stream.update();
			 //Stream.playback();


            player1.oldkey=keys[4];
            player2.oldkey=keys2[4];

            if (!PAUSE_GAME){

                CheckKeys();

                OnTimer();
            }


			if (LIMIT_FPS)
				RenderScreen();

			tick = SDL_GetTicks()+20;

		}


		else
			if (LIMIT_FPS)
				SDL_Delay(1);

		if (!LIMIT_FPS)
			RenderScreen();

		process_events();
	}

    if (USE_SOUND)
        Stream.release();
	pics.destroy();
	INI.destroy();

    if(SDL_JoystickOpened(0))
        SDL_JoystickClose(Joy);
    if (USE_SOUND)
        QuitOpenAL();
	SDL_Quit();

#ifdef WIN32

	#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
	#endif

#endif


	return 0;
}

