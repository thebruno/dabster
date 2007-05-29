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

	bmpfileheaders 
	bmpfhdrs.cpp

*********************************************************************/
#include "bmpfhdrs.h"
namespace bmp {
//********************class DabHeader********************//
/*!
 * \brief
 * Konstruktor naglowka bitmapy.
 * 
 * BMP_HEADER wykonuje inicjalizacjie (zerowanie) struktury przechowujacej informacjie o aktualnym pliku bmp.
 */
BMP_HEADER::BMP_HEADER() {
	bfType = 0; 
	bfSize = 0; 
	bfReserved1 = 0; 
	bfReserved2 = 0; 
	bfOffBits = 0; 
	biSize = 0; 
	biWidth = 0;
	biHeight = 0;
	biPlanes = 0;
	biBitCount = 0;
	biCompression = 0;
	biSizeImage = 0;
	biXPelsPerMeter = 0;
	biYPelsPerMeter = 0;
	biClrUsed = 0; 
	biClrImportant = 0; 
	biCrlRotation = 0; 
	biReserved = 0; 
	biUsefulData = 0; 
}
//********************end of class DabHeader********************//
//********************class DabHeader********************//
/*!
 * \brief
 * Konstruktor naglowka Dabstera.
 * 
 * DAB_HEADER wykonuje inicjalizacjie (zerowanie) struktury przechowujacej naglowek Dbastera w plikach bmp.
 */
DAB_HEADER::DAB_HEADER() {
	Sygn = 0;
	DabName = 0;
	Ver = 0;
	Compr = 0;
	FilesCount = 0;
	FilesSize = 0;
	FreeSpc = 0;
	DabHeaderStart = 0;
	FirstFileStart = 0;
	CompressionStart = 0;
}
//********************end of class DabHeader********************//
//********************class FILE_HEADER********************//
/*!
 * \brief
 * Konstruktor naglowka pliku lub folderu.
 * 
 * FILE_HEADER wykonuje inicjalizacjie (zerowanie) struktury przechowujacej naglowek kazdego pliku lub folderu zapisanego w pliku bmp.
 */
FILE_HEADER::FILE_HEADER(){
	DataSize = 0;
	TimeDate = 0;
	Attributes = 0;
	FileNameLen = 0;
	PerformAction = false;
	// dla FileName konstruktor domyslny
	HeaderStart = 0;
	DataStart = 0;
}
//********************end of class FILE_HEADER********************//
}
