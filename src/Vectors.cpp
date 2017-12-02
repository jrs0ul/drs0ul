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
#include <math.h>
#include "Vectors.h"

void Vector3D::transform(float matrix[4][4]){
	float newx,newy,newz;
	newx=_x*matrix[0][0]+_y*matrix[1][0]+_z*matrix[2][0]+1*matrix[3][0];
	newy=_x*matrix[0][1]+_y*matrix[1][1]+_z*matrix[2][1]+1*matrix[3][1];
	newz=_x*matrix[0][2]+_y*matrix[1][2]+_z*matrix[2][2]+1*matrix[3][2];
	_x=newx; _y=newy; _z=newz;
}
//----------------------------
float Vector3D::length(){
	return sqrt(_x * _x + _y * _y + _z * _z);
}
//----------------------------
void Vector3D::normalize(){
	float l = length();
	_x = _x / l;
	_y = _y / l;
	_z = _z / l;
}
//----------------------
const Vector3D operator ^ (const Vector3D& left,
								const Vector3D& right){ //cross
		return Vector3D(left._y*right._z-left._z*right._y,
						left._z*right._x-left._x*right._z,
						left._x*right._y-left._y*right._x );
}
//----------------------
const float operator * (const Vector3D& left,
								const Vector3D& right){ //dot
		return left._x*right._x+left._y*right._y+left._z*right._z;
}

//----------------------------------
const Vector3D operator - (const Vector3D& left, const Vector3D& right ){
	return Vector3D(left._x-right._x,left._y-right._y,left._z-right._z);
}
//--------------------------------------
const Vector3D operator + (const Vector3D& left, const Vector3D& right ){
	return Vector3D(left._x+right._x,left._y+right._y,left._z+right._z);
}


