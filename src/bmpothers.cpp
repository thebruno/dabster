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

	bmpothers 
	bmpothers.cpp

*********************************************************************/
#include "bmp.h"
//********************namespace bmp********************//
namespace bmp {
//********************class BMP********************//
/*!
 * \brief
 * Odczyt naglowkow pliku bmp.
 */
void BMP::ReadBmpHeader() {
	uint32 i;
	uint8 * p = reinterpret_cast<uint8*> (&BmpHeader);
	for (i = 0; i < 54; ++i) {
		p[i] = BmpBuf.GetByte();
	}
	BmpHeader.biUsefulData = BmpHeader.biWidth * BmpHeader.biHeight * 3; // w bajtach
}
/*!
 * \brief
 * Konwersja Atrybutow na format binarny.
 */
uint16 BMP::AttribToBytes(smap Dabsmap) {
	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
	uint16 result = 0;

	if (Dabsmap["atrTemporary"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	if (Dabsmap["atrSystem"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	if (Dabsmap["atrSparseFile"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	if (Dabsmap["atrArchive"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	if (Dabsmap["atrCompressed"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	if (Dabsmap["atrEncrypted"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	if (Dabsmap["atrHidden"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	if (Dabsmap["atrNotContentIndexed"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	if (Dabsmap["atrOffline"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	if (Dabsmap["atrReadOnly"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	if (Dabsmap["atrDirectory"] == "true") FileBuf.PutBit(BitSet);
	else FileBuf.PutBit(BitNSet);

	for (uint8 i = 0; i < 5; ++i) {
		FileBuf.PutBit(BitNSet);
	}
	result = FileBuf.GetByte();
	result += FileBuf.GetByte() << 8;
	return result;
}

/*!
 * \brief
 * Konwersja Atrybutow na format slowny.
 */
smap BMP::BytesToAttrib(uint16 DabBytes) {
	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
	smap result;

	FileBuf.PutByte(uint8(0x000000FF & DabBytes));
	FileBuf.PutByte(uint8(0x000000FF & (DabBytes >> 8)));

	if (FileBuf.GetBit()) result["atrTemporary"] = "true";
	else result["atrTemporary"] = "false";

	if (FileBuf.GetBit()) result["atrSystem"] = "true";
	else result["atrSystem"] = "false";

	if (FileBuf.GetBit()) result["atrSparseFile"] = "true";
	else result["atrSparseFile"] = "false";
	
	if (FileBuf.GetBit()) result["atrArchive"] = "true";
	else result["atrArchive"] = "false";
	
	if (FileBuf.GetBit()) result["atrCompressed"] = "true";
	else result["atrCompressed"] = "false";
	
	if (FileBuf.GetBit()) result["atrEncrypted"] = "true";
	else result["atrEncrypted"] = "false";
	
	if (FileBuf.GetBit()) result["atrHidden"] = "true";
	else result["atrHidden"] = "false";
	
	if (FileBuf.GetBit()) result["atrNotContentIndexed"] = "true";
	else result["atrNotContentIndexed"] = "false";
	
	if (FileBuf.GetBit()) result["atrOffline"] = "true";
	else result["atrOffline"] = "false";
	
	if (FileBuf.GetBit()) result["atrReadOnly"] = "true";
	else result["atrReadOnly"] = "false";
	
	if (FileBuf.GetBit()) result["atrDirectory"] = "true";
	else result["atrDirectory"] = "false";

	return result;
}

/*!
 * \brief
 * Konwersja Daty na format binarny.
 */
uint32 BMP::UTCToBytes(string Dabs) {
	// dd.mm.rrrr hh:mm:ss
	uint16 dd, mm , hh, min, ss;
	uint16 rrrr;
	uint8 i, p;
	if (Dabs.size() == 19) {
		dd = Dabs[0] - '0'; dd *= 10; dd += Dabs[1] - '0';
		mm = Dabs[3] - '0'; mm *= 10; mm += Dabs[4] - '0';
		rrrr = Dabs[6] - '0'; rrrr *= 10; rrrr += Dabs[7] - '0'; rrrr *= 10;
		rrrr += Dabs[8] - '0'; rrrr *= 10; rrrr += Dabs[9] - '0';
		rrrr -= 1980;

		hh = Dabs[11] - '0'; hh *= 10; hh += Dabs[12] - '0';
		min = Dabs[14] - '0'; min *= 10; min += Dabs[15] - '0';
		ss = Dabs[17] - '0'; ss *= 10; ss += Dabs[18] - '0';
	}
	else 
		return 0;

	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
	p = uint8( 0x000000FF & rrrr);
	for (i = 9; i < 16; ++i) {
		if (p & BUFFOR::One[i % 8 ]) FileBuf.PutBit(BitSet);
		else FileBuf.PutBit(BitNSet);
	}
	for (i = 12; i < 16; ++i) {
		if (mm & BUFFOR::One[i % 8 ]) FileBuf.PutBit(BitSet);
		else FileBuf.PutBit(BitNSet);
	}
	for (i = 11; i < 16; ++i) {
		if (dd & BUFFOR::One[i % 8 ]) FileBuf.PutBit(BitSet);
		else FileBuf.PutBit(BitNSet);
	}
	for (i = 11; i < 16; ++i) {
		if (hh & BUFFOR::One[i % 8 ]) FileBuf.PutBit(BitSet);
		else FileBuf.PutBit(BitNSet);
	}
	for (i = 10; i < 16; ++i) {
		if (min & BUFFOR::One[i % 8 ]) FileBuf.PutBit(BitSet);
		else FileBuf.PutBit(BitNSet);
	}
	for (i = 11; i < 16; ++i) {
		if (ss & BUFFOR::One[i % 8 ]) FileBuf.PutBit(BitSet);
		else FileBuf.PutBit(BitNSet);
	}
	uint32 result = FileBuf.GetByte();
	result += FileBuf.GetByte() << 8;
	result += FileBuf.GetByte() << 16;
	result += FileBuf.GetByte() << 24;

	return result;

}
/*!
 * \brief
 * Konwersja Daty na format slowny.
 */
string BMP::BytesToUTC(uint32 DabBytes) {
	char temp [] = "0000";
	string result;
	uint16 dd = 0, mm = 0, hh = 0 , min = 0, ss = 0;
	uint16 rrrr = 0;
	uint8 i, p = 0;

	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
	FileBuf.PutByte(uint8(0x000000FF & DabBytes));
	FileBuf.PutByte(uint8(0x000000FF & (DabBytes >> 8)));
	FileBuf.PutByte(uint8(0x000000FF & (DabBytes >> 16)));
	FileBuf.PutByte(uint8(0x000000FF & (DabBytes >> 24)));

	for (i = 1; i < 8; ++i) {
		p = (p & BUFFOR::ExOne[i % 8]) | (FileBuf.GetBit() & BUFFOR::One[i % 8]);
	}
	rrrr = p;
	rrrr += 1980;
	for (i = 4, p = 0; i < 8; ++i) {
		p = (p & BUFFOR::ExOne[i % 8]) | (FileBuf.GetBit() & BUFFOR::One[i % 8]);
	}
	mm = p;
	for (i = 3, p = 0; i < 8; ++i) {
		p = (p & BUFFOR::ExOne[i % 8]) | (FileBuf.GetBit() & BUFFOR::One[i % 8]);
	}
	dd = p;
	for (i = 3, p = 0; i < 8; ++i) {
		p = (p & BUFFOR::ExOne[i % 8]) | (FileBuf.GetBit() & BUFFOR::One[i % 8]);
	}
	hh = p;
	for (i = 2, p = 0; i < 8; ++i) {
		p = (p & BUFFOR::ExOne[i % 8]) | (FileBuf.GetBit() & BUFFOR::One[i % 8]);
	}
	min = p;
	for (i = 3, p = 0; i < 8; ++i) {
		p = (p & BUFFOR::ExOne[i % 8]) | (FileBuf.GetBit() & BUFFOR::One[i % 8]);
	}
	ss = p;
	sstr s;
	s.clear(); dd < 10 ? s << "0" << dd : s << dd; s >> temp; result += temp; result +=  ".";
	s.clear(); mm < 10 ? s << "0" << mm : s << mm;  s >> temp; result += temp; result +=  ".";
	s.clear(); s << rrrr; s >> temp; result += temp; result +=  " ";

	s.clear(); hh < 10 ? s << "0" << hh : s << hh;  s >> temp; result += temp; result +=  ":";
	s.clear(); min < 10 ? s << "0" << min : s << min;  s >> temp; result += temp; result +=  ":";
	s.clear(); ss < 10 ? s << "0" << ss : s << ss;  s >> temp; result += temp;
	return result;

}
/*!
 * \brief
 * Sprawdzenie bitmapy
 * 
 * Sprawdzenie czy bitmapa jest plikiem obslugiwanym przez program. Wersja wewnetrzna.
 */
bool BMP::IsBmp() {
	// 19778 = "BM", 24 bit bitmapa, 14 + BmpHeader.biSize - wielkosc naglowka
	if (!(BmpHeader.bfType == 19778 && BmpHeader.bfSize == BmpBuf.GetFileSize() &&
		// naglowek BMP + kompresia + naglwek dab z kompresja == 7
		BmpBuf.GetFileSize() >= 54 + 3 + 24 &&
		BmpHeader.biBitCount == 24 && BmpHeader.biCompression == 0 && BmpHeader.biClrUsed == 0 &&
		BmpHeader.biSizeImage == BmpBuf.GetFileSize() - BmpHeader.biSize - 14))
		return false;
	else {
		BmpBuf.GetModReminder() = BmpHeader.biWidth % 4;
		BmpBuf.GetModWidth() = 3 * BmpHeader.biWidth + BmpBuf.GetModReminder();
		DabHeader.CompressionStart = 8 * BmpHeader.bfOffBits + 7;
		return true;
	}
}
/*!
 * \brief
 * Sprawdzenie naglowka.
 *
 * Sprawdzenie czy bitapa zawiera poprawny naglowek dabstera.
 */
bool BMP::IsDab() {
	// obligatoryjnie
	if (DabHeader.Sygn == SYGNATURE && DabHeader.DabName == DABSTER_NAME &&
		DabHeader.Ver == BMP_CURRENT_VERSION && DabHeader.Compr == BmpBuf.GetCompr() ) {
			PreferedCompr = DabHeader.Compr;
			DabHeader.FirstFileStart = DabHeader.DabHeaderStart +
			BmpBuf.FileSizeInBmp(8 * (DAB_HEADER_BYTE_SIZE), DabHeader.DabHeaderStart);
			return true;
	}
	else {
		return false;
	}
}
/*!
 * \brief
 * Odczytuje naglowek Dabstera.
 *	
 * \throws <exception class>
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 * Odczyt naglowka bez interpretacji jego zawartosci.
 */
void BMP::ReadDabHeader() {
	uint32 i;
	// ustawienie na pierwszy bit danych, Seek = 8 - Compr;
	// trzeba poprawnie wyznaczyc!!
	DabHeader.DabHeaderStart = 8 * (BmpHeader.bfOffBits + 4) - BmpBuf.GetCompr();
	// dla bitmapy o szerokosci 1 pixel trafimy w dopelniajace zero, 
	// wiec spr czy bitmapa ma 1 pixel szerokosci, dla wiekszych nie trzeba nic robic
	if (BmpHeader.biWidth == 1)
		DabHeader.DabHeaderStart += 8;

	BmpBuf.SetMode(BUFFOR::BUF_READONLY);
	BmpBuf.BitSeekg(DabHeader.DabHeaderStart);
	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
	for (i = 0; i <= DAB_HEADER_BYTE_SIZE ; ++i) {
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit()); 
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit()); 
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());	
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
	}
	DabHeader.Sygn = FileBuf.GetByte();
	DabHeader.Sygn += FileBuf.GetByte() << 8;
	DabHeader.Sygn += FileBuf.GetByte() << 16;
	DabHeader.Sygn += FileBuf.GetByte() << 24;

	DabHeader.DabName = FileBuf.GetByte();
	DabHeader.DabName += FileBuf.GetByte() << 8;
	DabHeader.DabName += FileBuf.GetByte() << 16;
	DabHeader.DabName += FileBuf.GetByte()<< 24;

	DabHeader.Ver = FileBuf.GetByte() ;
	DabHeader.Ver += FileBuf.GetByte() << 8;

	DabHeader.Compr = FileBuf.GetByte();

	DabHeader.FilesCount = FileBuf.GetByte();
	DabHeader.FilesCount += FileBuf.GetByte() << 8;

	DabHeader.FilesSize = FileBuf.GetByte();
	DabHeader.FilesSize += FileBuf.GetByte() << 8;
	DabHeader.FilesSize += FileBuf.GetByte() << 16;
	DabHeader.FilesSize += FileBuf.GetByte() << 24;

	DabHeader.FreeSpc = FileBuf.GetByte();
	DabHeader.FreeSpc += FileBuf.GetByte() << 8;
	DabHeader.FreeSpc += FileBuf.GetByte() << 16;
	DabHeader.FreeSpc += FileBuf.GetByte() << 24;
}

/*!
 * \brief
 * Zapisuje naglowek Dabstera.
 * 
 * \throws <exception class>
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 */
void BMP::WriteDabHeader() {
	uint32 i;
	BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
	BmpBuf.SetCompr(DabHeader.Compr);
	BmpBuf.BitSeekp(DabHeader.DabHeaderStart);

	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);

	FileBuf.PutByte(uint8(0x000000FF & DabHeader.Sygn));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.Sygn >> 8)));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.Sygn >> 16)));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.Sygn >> 24)));

	FileBuf.PutByte(uint8(0x000000FF & DabHeader.DabName));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.DabName >> 8)));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.DabName >> 16)));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.DabName >> 24)));

	FileBuf.PutByte(uint8(0x000000FF & DabHeader.Ver));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.Ver >> 8)));

	FileBuf.PutByte(uint8(0x000000FF & DabHeader.Compr));

	FileBuf.PutByte(uint8(0x000000FF & DabHeader.FilesCount));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.FilesCount >> 8)));

	FileBuf.PutByte(uint8(0x000000FF & DabHeader.FilesSize));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.FilesSize >> 8)));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.FilesSize >> 16)));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.FilesSize >> 24)));

	FileBuf.PutByte(uint8(0x000000FF & DabHeader.FreeSpc));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.FreeSpc >> 8)));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.FreeSpc >> 16)));
	FileBuf.PutByte(uint8(0x000000FF & (DabHeader.FreeSpc >> 24)));

	for (i = 0; i < DAB_HEADER_BYTE_SIZE ; ++i) {
		BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
		BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
		BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
		BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
	}
	BmpBuf.Flush();
}

/*!
 * \brief
 * Odczyt stopnia kompresji z dysku.
 * 
 * \returns
 * Wartosc odczytanej kompresji.
 * 
 * \throws <exception class>
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 * Nie interpretuje odczytanej wartosci.
 */
const uint8 BMP::ReadCompr() {
	// ustawienie na 1 bicie kompresji
	uint8 i;
	BmpBuf.SetMode(BUFFOR::BUF_READONLY);
	BmpBuf.SetCompr(1);
	BmpBuf.BitSeekg(DabHeader.CompressionStart);
	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
	for (i = 0; i < 5; ++i) {
		FileBuf.PutBit(BitNSet);
	}
	for (i = 0; i < 3; ++i) {
		FileBuf.PutBit(BmpBuf.GetBit());
	}
	return FileBuf.GetByte();
}
/*!
 * \brief
 * Zapis stopnia kompresji na dysku.
 *
 * \throws <exception class>
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 */
void BMP::WriteCompr() {
	uint8 i;
	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
	FileBuf.PutByte(PreferedCompr);
	BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
	BmpBuf.SetCompr(1);
	BmpBuf.BitSeekp(DabHeader.CompressionStart);
	for (i = 0; i < 5; ++i)
		FileBuf.GetBit();
	for (i = 0; i < 3; ++i)
		BmpBuf.PutBit(FileBuf.GetBit());
	BmpBuf.Flush();
}
/*!
 * \brief
 * Odczyt Wszystkich naglowkow z pliku.
 *
 * \throws <exception class>
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 * Nie interpretuje odczytanej wartosci.
 */
void BMP::ReadAllHeaders() {
	uint64 ReadingPos = DabHeader.FirstFileStart;
	FILE_HEADER* header = 0;
	uint32 i = DabHeader.FilesCount;
	BmpBuf.SetMode(BUFFOR::BUF_READONLY);
	BmpBuf.SetReadSize(BUF_READING_SIZE);
	while (i--) {
		BmpBuf.BitSeekg(ReadingPos);
		header = ReadFileHeader();
		FilesHeaders.push_back(header);
		ReadingPos = header->DataStart + BmpBuf.FileSizeInBmp(8 * header->DataSize, header->DataStart);
	}
}
/*!
 * \brief
 * Kasuje pamiec przydzielona pod naglowki plikow lub folderow.
 *
 */
void BMP::DeleteAllHeaders() {
	// kasowanie wektora wskaznikow na naglowki
	FILE_HEADER * h;
	while (FilesHeaders.size()) {
		h = FilesHeaders.back();
		FilesHeaders.pop_back();
		delete h;
	}
}
/*!
 * \brief
 * Odczyt pojedynczego naglowka.
 * 
 * \returns
 * Wskaznik na nowy naglowek.
 * 
 * \throws <exception class>
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 * Nie interpretuje odczytanej wartosci.
 */
FILE_HEADER* BMP::ReadFileHeader() {
	uint16 i;
	FILE_HEADER * header = new FILE_HEADER;
	header->HeaderStart = BmpBuf.BitTellg();

	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
	for (i = 0; i < FILE_HEADER_BYTE_SIZE; ++i) {
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
	}
	header->DataSize = FileBuf.GetByte();
	header->DataSize += FileBuf.GetByte() << 8;
	header->DataSize += FileBuf.GetByte() << 16;
	header->DataSize += FileBuf.GetByte() << 24;

	header->TimeDate = FileBuf.GetByte();
	header->TimeDate += FileBuf.GetByte() << 8;
	header->TimeDate += FileBuf.GetByte() << 16;
	header->TimeDate += FileBuf.GetByte() << 24;

	header->Attributes = FileBuf.GetByte();
	header->Attributes += FileBuf.GetByte() << 8;

	header->FileNameLen = FileBuf.GetByte();
	header->FileNameLen += FileBuf.GetByte() << 8;

	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
	// max dlugosc nazwy = 256
	for (i = 0; i < header->FileNameLen && i < 256; ++i) {
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
		FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
	}

	while(FileBuf.GetBufState() != BUFFOR::BUF_EMPTY) {
		header->FileName += FileBuf.GetByte();
	}
	header->DataStart = header->HeaderStart +
		BmpBuf.FileSizeInBmp(8 * (header->FileNameLen + FILE_HEADER_BYTE_SIZE), header->HeaderStart);
	header->PerformAction = false;
	return header;
}
/*!
 * \brief
 * Zapis pojedynczego naglowka.
 *
 * \param Dabheader
 * Wskaznik na naglowek do zapisu. 
 *
 * \throws <exception class>
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 */
void BMP::WriteFileHeader(FILE_HEADER* Dabheader) {
	uint32 i;
	BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
	BmpBuf.SetReadSize(BUF_READING_SIZE);
	BmpBuf.BitSeekp(Dabheader->HeaderStart);
	FileBuf.BufReset();
	FileBuf.SetMode(BUFFOR::BUF_TRANSFER);

	FileBuf.PutByte(uint8(0x000000FF & Dabheader->DataSize));
	FileBuf.PutByte(uint8(0x000000FF & (Dabheader->DataSize >> 8)));
	FileBuf.PutByte(uint8(0x000000FF & (Dabheader->DataSize >> 16)));
	FileBuf.PutByte(uint8(0x000000FF & (Dabheader->DataSize >> 24)));

	FileBuf.PutByte(uint8(0x000000FF & Dabheader->TimeDate));
	FileBuf.PutByte(uint8(0x000000FF & (Dabheader->TimeDate >> 8)));
	FileBuf.PutByte(uint8(0x000000FF & (Dabheader->TimeDate >> 16)));
	FileBuf.PutByte(uint8(0x000000FF & (Dabheader->TimeDate >> 24)));

	FileBuf.PutByte(uint8(0x000000FF & Dabheader->Attributes));
	FileBuf.PutByte(uint8(0x000000FF & (Dabheader->Attributes >> 8)));

	FileBuf.PutByte(uint8(0x000000FF & Dabheader->FileNameLen));
	FileBuf.PutByte(uint8(0x000000FF & (Dabheader->FileNameLen >> 8)));

	// max dlugosc nazwy = 256
	for (i = 0; Dabheader->FileNameLen - i > 0 && i < 256; ++i) {
		FileBuf.PutByte(Dabheader->FileName[i]);
	}

	for (i = 0; i < Dabheader->FileNameLen + FILE_HEADER_BYTE_SIZE; ++i) {
		BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
		BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
		BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
		BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
	}
	BmpBuf.Flush();
}

void BMP::edit() {
}
/*!
 * \brief
 * Tworzy nowy naglowek Dabstera.
 * 
 * \throws <exception class>
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 */
void BMP::CreateDabHeader() {
	DabHeader.CompressionStart = 8 * BmpHeader.bfOffBits + 7;
	DabHeader.Sygn = SYGNATURE;
	DabHeader.DabName = DABSTER_NAME;
	DabHeader.Ver = BMP_CURRENT_VERSION;
	DabHeader.Compr = PreferedCompr;
	DabHeader.FilesCount = 0;
	DabHeader.FilesSize = 0;
	// przeliczyc to jeszcze raz
	// kompr + dabheader + file header (w przyblizeniu)
	DabHeader.FreeSpc = PreferedCompr * (BmpHeader.biUsefulData - 3) / 8 - DAB_HEADER_BYTE_SIZE;
	DabHeader.DabHeaderStart = 8 * (BmpHeader.bfOffBits + 4) - PreferedCompr;
	// warunek dla 1 pixelowej bitmapy
	if (BmpHeader.biWidth == 1)
		DabHeader.DabHeaderStart += 8;

	// do obliczenia wielkosci pliku w bitmapie:
	BmpBuf.SetCompr(PreferedCompr);
	DabHeader.FirstFileStart = DabHeader.DabHeaderStart +
		BmpBuf.FileSizeInBmp(8 * (DAB_HEADER_BYTE_SIZE), DabHeader.DabHeaderStart); // nie ma plikow
}

//********************end of class BMP********************//
}//********************end of namespace bmp********************//
