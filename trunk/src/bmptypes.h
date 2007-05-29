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

namespace bmptypes {

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
const uint16 BMP_CURRENT_VERSION = 1; /*!< Aktualna wersja. */
const uint32 BUFFOR_SIZE = 512; /*!< Wielkosc bufora, minimum 512 byteow. */
const uint32 SYGNATURE = 0x08064b50; /*!< Sygnatura, zapisywana do pliku. */
const uint32 DABSTER_NAME = 0x35424144; /*!< Nazwa = "DAB5". */
const uint8 BitSet = 0xFF; /*!< Bit ustawiony. */
const uint8  BitNSet = 0x00; /*!< Bit nieustawiony. */

const uint32 BMP_HEADER_BYTE_SIZE = 54; /*!< Wielkosc naglowka BMP. */
const uint32 DAB_HEADER_BYTE_SIZE = 21; /*!< Wielkosc naglowka DAB. */
const uint32 FILE_HEADER_BYTE_SIZE = 12; /*!< Wielkosc naglowka pliku. */
const uint32 BUF_READING_SIZE = 180; /*!< Bufory moga doczytywac tyle bajtow: BUFFOR_SIZE lub BUF_READING_SIZE bufor. */

	// nie znalesiono pliku przy otwieraniu
class NO_SUCH_FILE: public std::exception {
};
}
#endif
