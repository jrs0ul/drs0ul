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
#ifndef _EVIL_VIRUS_H
#define _EVIL_VIRUS_H

#include "Matrix.h"
#include "Vectors.h"
#include "TextureLoader.h"


class EvilVirus{
    Vector3D viruspos[3];
    Vector3D virusdir[3];
    bool isAlive[3];
public:

    void kill(int index){
        if ((index<3)&&(index>-1))
            if (isAlive[index])
                isAlive[index] = false;
    }

    void draw(PicsContainer& pics,int index, int frame=0){
        if (isAlive[0])
            pics.draw(index, viruspos[0].x(),viruspos[0].z(),0 + frame,true,0.8f,0.8f);
        if (isAlive[1])
            pics.draw(index, viruspos[1].x(),viruspos[1].z(),2 + frame,true,0.8f,0.8f);
        if (isAlive[2])
            pics.draw(index, viruspos[2].x(),viruspos[2].z(),4 + frame,true,0.8f,0.8f);
    }
//------------------------------
    void move(){
        viruspos[0] = Vector3D(viruspos[0].x()+virusdir[0].x(),0,viruspos[0].z()+virusdir[0].z());
        viruspos[1] = Vector3D(viruspos[1].x()+virusdir[1].x(),0,viruspos[1].z()+virusdir[1].z());
        viruspos[2] = Vector3D(viruspos[2].x()+virusdir[2].x(),0,viruspos[2].z()+virusdir[2].z());

        Matrix mat;
        RotationY(0.015,mat);
        virusdir[0].transform(mat);
        virusdir[1].transform(mat);
        virusdir[2].transform(mat);
    }

//--------------------------------------------
    void init(){

        viruspos[0]=Vector3D(110,0,390);
        virusdir[0]=Vector3D(0.5,0,0);
        viruspos[1]=Vector3D(110,0,390);
        virusdir[1]=Vector3D(0.5,0,0);
        viruspos[2]=Vector3D(110,0,390);
        virusdir[2]=Vector3D(0.5,0,0);
        isAlive[0]=true;
        isAlive[1]=true;
        isAlive[2]=true;

        for (int i=0;i<150;i++){
            viruspos[1] = Vector3D(viruspos[1].x()+virusdir[1].x(),0,viruspos[1].z()+virusdir[1].z());
            Matrix mat;
            RotationY(0.015,mat);
            virusdir[1].transform(mat);
        }
        for (int i=0;i<300;i++){
            viruspos[2] = Vector3D(viruspos[2].x()+virusdir[2].x(),0,viruspos[2].z()+virusdir[2].z());
            Matrix mat;
            RotationY(0.015,mat);
            virusdir[2].transform(mat);
        }

    }
};

#endif //_EVIL_VIRUS_H
