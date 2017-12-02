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
#ifndef _PLAYER_H
#define _PLAYER_H


class Player{

    int _ID; // pirmas ar antras ?

    int  _victoryCount;  //kiek kartu laimejo pvp
    long _score;         //taskai

    bool gameover;

public:
    bool nextpill; //ar generuosim sekancia piliule
    int level;     //lygis
    int lasticks;  //laikas skirtas pill valdymui kai ji ant pavirsiaus
    int speed;    //kritimo greitis
    bool falldown;
    int falltic;
    int craptim;
    bool rotatepressed;

    bool nextstage;
    unsigned char oldkey;
    long ticsplayed;
    int dudetim;
    bool lenkranka;


    int pillcounter;
    int basespeed;



    Player(int dID){

        nextpill = false;
        _score = 0;

        level = 1;
        lasticks = 0;
        falldown = false;
        falltic = 0;
        rotatepressed = false;
        gameover = false;
        nextstage = false;
        oldkey = 0;
        craptim = 0;
        speed = 15;
        ticsplayed = 0;
        dudetim = 0;
        lenkranka = false;
        _victoryCount = 0;
        _ID = dID;
        pillcounter = 0;
        basespeed = 15;

    }

    int  victoryCount(){return _victoryCount;}
    void winRound(){_victoryCount++;}
    void setVictoryCount(int count){_victoryCount = count;}

    void addScore(int points){ _score += points;}
    void setScore(long points){ _score = points;}
    long score(){return _score;}

    void loseMatch(){ gameover = true;}
    void resetMatch(){gameover = false;}
    bool isGameOver(){ return gameover;}

    int ID(){return _ID;}



};



#endif //_PLAYER_H
