
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
#ifndef _DARRAY_H_
#define	_DARRAY_H_


template <class T>
class DArray{
	unsigned long _count;
	T* mas;
public:
	
	
	DArray(){_count=0; mas=0;}
    //----------------------------------
    DArray(DArray const & copy){
		this->destroy();
		for (unsigned long i=0;i<copy.count();i++){
			this->add(copy[i]);
		}
	}   

	//---------------------------
	void add(T& newitem){
		T* tmp=0;
		if (_count){
			tmp=new T[_count];
			for (unsigned long i=0;i<_count;i++){
				tmp[i]=mas[i];
			}
			delete []mas;
		}

		_count++;
		mas=new T[_count];

		if (_count>1){
			for (unsigned long i=0;i<_count-1;i++){
				mas[i]=tmp[i];
			}
			delete []tmp;
		}
		
		mas[_count-1]=newitem;
		
	}
	//---------------------------
	bool remove(unsigned long index){
		if ((index<0)||(index>_count))
			return false;

		if (_count>1){
			T* tmp=new T[_count-1];
			for (unsigned long i=0;i<index;i++){
				tmp[i]=mas[i];
			}
			for (unsigned long i=index+1;i<_count;i++){
				tmp[i-1]=mas[i];
			}

			delete []mas;

			_count--;

			mas=new T[_count];

			for (unsigned long i=0;i<_count;i++){
				mas[i]=tmp[i];
			}
			delete []tmp;
		}

		else 
			destroy();
		return true;
	}
	//---------------------------
	void destroy(){
		if (mas){
			delete []mas;
			mas=0;
			_count=0;
		}
	}
	//---------------------------
	unsigned long count(){return _count;}

	//---------------------------
	T& operator[](unsigned long index) { 

		return mas[index]; 
	}

	//----------------------------
	//~DArray(){
	//	destroy();
	//}


};





#endif //_DARRAY_H_


