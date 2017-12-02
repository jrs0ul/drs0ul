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
#ifndef VECTORS_H
#define VECTORS_H


class Vector3D{
	float _x;
	float _y;
	float _z;
public:
	Vector3D(float nx,float ny,float nz){_x=nx; _y=ny; _z=nz;}
	Vector3D(){_x=0; _y=0; _z=0;}
	float x(){return _x;}
	float y(){return _y;}
	float z(){return _z;}
	void set(float nx,float ny,float nz){_x=nx; _y=ny; _z=nz;}
	void transform(float matrix[4][4]);
	void normalize();
	float length();
	
	friend const Vector3D operator + (const Vector3D& left, const Vector3D& right );

	friend const Vector3D operator - (const Vector3D& left, const Vector3D& right );

	friend const Vector3D operator ^ (const Vector3D& left, //cross
								const Vector3D& right);

	friend const float operator * (const Vector3D& left, //dot
								const Vector3D& right);

};


#endif //VECTORS_H


