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

	bmp header
	bmp.h

*********************************************************************/

#ifndef BMPHEADERFILE
#define BMPHEADERFILE

#include "bmptypes.h"
#include "bmpbufs.h"
#include "bmpfhdrs.h"

#include "file.h"
#include "drive.h"
#include "sfile.h"

#pragma pack(push, BMP_DABSTER_PACKING)

namespace bmp {
using namespace bmptypes;

int isBmp(file *DabFile);
/*!
 * \brief
 * Klasa BMP.
 *
 * Wykonuje wszyskie operacje na bitmapach i plikach.
 */
class BMP: public drive, public file {
private: //data
	BMP_HEADER BmpHeader; /*!< Reprezentuje naglowek otwartego pliku bmp. */
	DAB_HEADER DabHeader; /*!< Reprezentuje naglowek dabstera. */
	std::vector<FILE_HEADER *> FilesHeaders; /*!< Naglowki plikow ukrytych w bmp. */
	uint8 PreferedCompr; /*!< Kompresja ustawiona przez uzytkownika. */
	BMP_BUFFOR BmpBuf; /*!< Bufor dla pliku bmp. */
	FILE_BUFFOR FileBuf; /*!< Bufor dla innych plikow. */
private: //functions
	uint16 AttribToBytes(smap Dabsmap);
	smap BytesToAttrib(uint16 DabBytes);

	uint32 UTCToBytes(string Dabs);
	string BytesToUTC(uint32 DabBytes);

	bool IsBmp();
	bool IsDab();

	void ReadBmpHeader();
	void CreateDabHeader();
	void ReadDabHeader();
	void WriteDabHeader();

	void ReadAllHeaders();
	void DeleteAllHeaders();

	FILE_HEADER* ReadFileHeader();
	void WriteFileHeader(FILE_HEADER *DabHeader);

	void ReorganizeAfterDelete();
	const uint8 ReadCompr();
	void WriteCompr();
public: //functions
	void edit();
	void del(vmstring path);
	void store(vmstring src, vmstring dest);
	void extract(vmstring src, vmstring dest);
	void copyInside(vmstring src, vmstring dest);
	void modify(vmstring oldPath, vmstring newPath);
	vmstring getContent(string path);	
	uint64 getCapacity(void);
	void  setCompression(uint8 DabDegree);
	const uint8 getCompression() ;

	BMP();
	~BMP();
	BMP(file *DabFile); // otwarcie bitmapy
};
}
#pragma pack(pop)
#endif

