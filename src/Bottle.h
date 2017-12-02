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
#ifndef _BOTTLE_H
#define _BOTTLE_H

#include "Player.h"
#include "SoundSystem.h"


class Bottle{
    unsigned char bottle[16][8];
    int _viruscount;
    int rviruscount;
    int yviruscount;
    int bviruscount;

    public:
        Bottle(){
            _viruscount=0;
            rviruscount=0;
            yviruscount=0;
            bviruscount=0;
        }

        Bottle(Bottle const & copy);

        int     viruscount(){return _viruscount;}
        void    generate(int level);
        void    checkvirus(int x,int y, int maxcount);
        int     getTile(int x, int y){return bottle[y][x];}
        void    setTile(int x, int y, unsigned char val){bottle[y][x]=val;}
        int     getPillColor(int y, int x);
        int     getPillFrame(int x, int y);
        bool    eliminate(Player& player, SoundSystem& SS, int soundindex, bool sfxon);
        void    pillsFall();
        void    clearcrap();
        int     redVirusCount(){ return rviruscount;}
        int     yellowVirusCount(){return yviruscount;}
        int     blueVirusCount(){return bviruscount;}



};


#endif //_BOTTLE_H
