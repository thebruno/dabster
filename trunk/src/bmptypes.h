/*********************************************************************

	Sigma Dabster 5
	Copyright (C) The Dabster Team 2007.
	All rights reserved.

	http://www.dabster.prv.pl/
	http://code.google.com/p/dabster/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 dated June, 1991. 

	This program is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	General Public License for more details. 

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
	02110-1301 USA. 

	BMP Types header
	bmptypes.h

*********************************************************************/

#ifndef BMPTYPESHEADERFILE
#define BMPTYPESHEADERFILE

#include <exception>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

namespace bmptypes
{
	#define BMP_DABSTER_PACKING 1
	typedef unsigned long long uint64;
	typedef signed long long int64;
	typedef unsigned long int uint32;

	typedef signed long int int32;
	typedef unsigned short int uint16;
	typedef unsigned char uint8;

	typedef char int8;

	typedef std::string string;
	typedef std::map<string, string> smap;
	typedef std::vector<smap> vmstring;
	typedef std::fstream fstream;
	typedef std::stringstream sstr;

	class DAB_EXCEPTION: public std::exception {
	};
	// nie znalesiono pliku przy otwieraniu
	class NO_SUCH_FILE: public DAB_EXCEPTION {
	};
	// bledy podczas odczytow z buforow 
	class BMP_INTERNAL_ERROR: public DAB_EXCEPTION {
	};
	// zla nazwa - plik to folder, za maly plik, nie bmp
	class BMP_WRONG_FILE: public DAB_EXCEPTION {
	};
	class BMP_NOT_ENOUGH_SPACE: public DAB_EXCEPTION{
	};
}

#endif
