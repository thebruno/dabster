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

	bmpbufs 
	bmpbufs.cpp

*********************************************************************/
#include "bmpbufs.h"
#include "err.h"
namespace bmp {
using namespace bmptypes;

const uint8 BUFFOR::One [8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
const uint8 BUFFOR::ExOne  [8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};

//********************class BUFFOR********************//
/*!
 * \brief
 * Konstruktor klasy BUFFOR.
 *
 * BUFFOR wykonuje inicjalizacjie (zerowanie) bufora.
 */
BUFFOR::BUFFOR() {
	FileSize = 0;
	BufSize = BUFFOR_SIZE;
	BufBitSize = 8 * BUFFOR_SIZE;
	DataCount = 0;
	BitDataCount = 0;
	ReadSize = BUFFOR_SIZE;
	BufBitPosg = 0;
	BufBitPosp = 0;
	BufBytePosg = 0;
	BufBytePosp = 0;
	BufState = BUF_BAD;
	BufMode = BUF_TRANSFER;
}
/*!
 * \brief
 * Destruktor ~BUFFOR.
 *
 * ~BUFFOR zamyka plik.
 */
BUFFOR::~BUFFOR() {
	File.close();
}

/*!
 * \brief
 * OpenFile otwiera plik i ustawia tryb pracy.
 * 
 * \param DabPath
 * Rzeczywista sciezka dostepu.
 * 
 * \param DabFileMode
 * Tryb otwarcia pliku.
 * 
 * \param DabBufMode
 * Tryb pracy bufora.
 * 
 * \throws err()
 * Gdy plik nie istnieje.
 * 
 * Otwiera plik, ustawia tryb pracy i oblicza jego wielkosc.
 */
void BUFFOR::OpenFile(std::string DabPath, std::ios_base::open_mode DabFileMode, BUFFOR_MODE DabBufMode) {
	File.open(DabPath.c_str(), DabFileMode | std::ios_base::binary);
	if (!File) {
		BufState = BUF_BAD;
		std::vector<string> params(1);
		params[0] = DabPath.c_str();
		throw err("!BMP0", params);
	}
	BufMode = DabBufMode;
	BufState = BUF_GOOD;

	File.seekg(0L,std::ios_base::end);
	FileSize = File.tellg();
	File.seekg(0L,std::ios_base::beg);
}

/*!
 * \brief
 * Pobranie jednego bita z bufora.
 * 
 * \returns
 * Zwraca 0xFF lub 0x00.
 * 
 * \throws err()
 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
 * 
 * Zwraca wartosc aktualnego bitu w buforze.
 */
uint8 BUFFOR::GetBit() {
	switch (BufMode) { 
		// tryb tylko odczyt
		case BUF_READONLY: 
			// w srodku bufora
			if (BufBitPosg + 1 < BitDataCount) {
				if (Buf[BufBytePosg] & One[BufBitPosg % 8 ]) {
					++BufBitPosg;
					if (!(BufBitPosg % 8)) {
						++BufBytePosg;
					}
					return BitSet;
				}
				else {
					++BufBitPosg;
					if (!(BufBitPosg % 8)) {
						++BufBytePosg;
					}
					return BitNSet;
				}
			}
			// na ostatniej pozycji danych w buforze
			else if (BufBitPosg + 1 == BitDataCount) {
				uint8 Result;
				if (Buf[BufBytePosg] & One[BufBitPosg % 8 ]) 
					Result = BitSet;
				else
					Result = BitNSet;
				//wczytanie
				File.clear();
				File.read((char *)Buf, ReadSize);
				DataCount = File.gcount();
				BitDataCount = 8 * DataCount;
				if (!DataCount) {
					if (File.eof())
						BufState = BUF_EOF;
					else {
						BufState = BUF_BAD;
						return Result;
					}
				}
				BufBytePosg = 0;
				BufBitPosg = 0;
				return Result;
			}
			// tu nie powinien buffor nigdy dojsc!
			{
				std::vector<string> params(1);
				throw err("!BMP2", params);
			}
		// tryb transfer
		case BUF_TRANSFER: 
			// w srodku bufora
			if (BufBitPosg + 1 < BitDataCount){
				if (Buf[BufBytePosg] & One[BufBitPosg % 8 ]) {
					++BufBitPosg;
					if (!(BufBitPosg % 8)) {
						++BufBytePosg;
					}
					return BitSet;
				}
				else {
					++BufBitPosg;
					if (!(BufBitPosg % 8)) {
						++BufBytePosg;
					}
					return BitNSet;
				}
			}
			// na ostatniej pozycji
			else if (BufBitPosg + 1 == BitDataCount) {
				uint8 Result;
				if (Buf[BufBytePosg] & One[BufBitPosg % 8 ]) 
					Result = BitSet;
				else
					Result = BitNSet;
				BufBytePosg = 0;
				BufBitPosg = 0;
				DataCount = 0;
				BitDataCount = 0;
				BufState = BUF_EMPTY;
				return Result;
			}
			else {
				// nie moze zabraknac danych w Buforze!!
				BufState = BUF_BAD;
				std::vector<string> params(1);
				throw err("!BMP2", params);
			}
		default: {
			// inny tryb niedozwolony
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}
	}
}

/*!
 * \brief
 * Zapis jednego bita do bufora.
 * 
 * \param DabBit
 * Wartosc zapisywanego bita 0xFF lub 0x00.
 * 
 * \throws err()
 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
 * 
 * Zapisuje wartosc bitu przeslanego w prametrze pod aktualna pozycje w buforze.
 */
void BUFFOR::PutBit(uint8 DabBit) {
	switch (BufMode) {
		case BUF_TRANSFER: 
			// w srodku bufora
			if (BufBitPosp + 1 < BufBitSize) {
				Buf[BufBytePosp] = (Buf[BufBytePosp] & ExOne[BufBitPosp % 8]) | (DabBit & One[BufBitPosp % 8]);
				++BufBitPosp;
				++BitDataCount;
				if (!(BufBitPosp % 8)) {
					++BufBytePosp;
					++DataCount;
				}
				return;
			}
			// na ostatniej pozycji
			else if(BufBitPosp + 1 == BufBitSize) {
				Buf[BufBytePosp] = (Buf[BufBytePosp] & ExOne[BufBitPosp % 8]) | (DabBit & One[BufBitPosp % 8]);
				BufBytePosp = 0;
				BufBitPosp = 0;
				DataCount = BufSize;
				BitDataCount = BufBitSize;
				BufState = BUF_FULL;
				return;
			}
			else {
				BufState = BUF_BAD;
				std::vector<string> params(1);
				throw err("!BMP2", params);
			}
		case BUF_WRITEONLY:	
			// w srodku bufora
			if (BufBitPosp + 1 < BufBitSize) {
				Buf[BufBytePosp] = (Buf[BufBytePosp] & ExOne[BufBitPosp % 8]) | (DabBit & One[BufBitPosp % 8]);
				++BufBitPosp;
				++BitDataCount;
				if (!(BufBitPosp % 8)) {
					++BufBytePosp;
					++DataCount;
				}
				return;
			}
			// na ostatniej pozycji
			else if(BufBitPosp + 1 == BufBitSize) {
				Buf[BufBytePosp] = (Buf[BufBytePosp] & ExOne[BufBitPosp % 8]) | (DabBit & One[BufBitPosp % 8]);
				// zpisujemy pelny bufor
				File.clear();
				File.write((char *)Buf,BufSize);
				File.flush();
				BufBytePosp = 0;
				BufBitPosp = 0;
				DataCount = 0;
				BitDataCount = 0;
				return ;
			}
			else {
				BufState = BUF_BAD;
				std::vector<string> params(1);
				throw err("!BMP2", params);
			}
		default: {
			// nie mozna wysylac w innym trybie na Bufor!!
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}
	}
}

/*!
 * \brief
 * Pobranie jednego bajta z bufora.
 * 
 * \returns
 * Zwraca 0xFF lub 0x00.
 * 
 * \throws err()
 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
 * 
 * Zwraca wartosc aktualnego bajtu w buforze.
 */
uint8 BUFFOR::GetByte() {
	switch (BufMode) {
		case BUF_READONLY: 
			// w srodku bufora
			if (BufBytePosg + 1 < DataCount) {
				BufBitPosg += 8;
				return Buf[BufBytePosg++];
			}
			// na ostatniej pozycji danych w buforze
			else if (BufBytePosg + 1 == DataCount) {
				uint8 Result = Buf[BufBytePosg];
				//wczytanie
				File.clear();
				File.read((char *)Buf,ReadSize);
				DataCount = File.gcount();
				BitDataCount = 8 * DataCount;
				if (!DataCount) {
					if (File.eof())
						BufState = BUF_EOF;
					else {
						BufState = BUF_BAD;
						return Result;
					}
				}
				BufBytePosg = 0;
				BufBitPosg = 0;
				return Result;
			}
			// tu nie powinien buffor nigdy dojsc!
			{
				std::vector<string> params(1);
				throw err("!BMP2", params);
			}

		
		case BUF_TRANSFER: 
			// w srodku bufora
			if (BufBytePosg + 1 < DataCount){
				BufBitPosg += 8;
				return Buf[BufBytePosg++];
			}
			// na ostatniej pozycji
			else if (BufBytePosg + 1 == DataCount) {
				uint8 Result = Buf[BufBytePosg];
				BufBytePosg = 0;
				BufBitPosg = 0;
				DataCount = 0;
				BitDataCount = 0;
				BufState = BUF_EMPTY;
				return Result;
			}
			else {
				// nie moze zabraknac danych w Buforze!!
				BufState = BUF_BAD;
				std::vector<string> params(1);
				throw err("!BMP2", params);
			}
		
		default: {
			// inny tryb niedozwolony
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}
		
	}
}

/*!
 * \brief
 * Zapis jednego bajtu do bufora.
 * 
 * \param DabByte
 * Wartosc zapisywanego bajtu 0xFF lub 0x00.
 * 
 * \throws err()
 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
 * 
 * Zapisuje wartosc bajtu przeslanego w prametrze pod aktualna pozycje w buforze.
 */
void BUFFOR::PutByte(uint8 DabByte) {
	switch (BufMode) {
		case BUF_TRANSFER: 
			// w srodku bufora
			if (BufBytePosp + 1 < BufSize) {
				Buf[BufBytePosp++] = DabByte;
				++DataCount;
				BitDataCount += 8;
				BufBitPosp += 8;
				return;
			}
			// na ostatniej pozycji
			else if(BufBytePosp + 1 == BufSize) {
				Buf[BufBytePosp] = DabByte;
				BufBytePosp = 0;
				BufBitPosp = 0;
				DataCount = BufSize;
				BitDataCount = BufBitSize;
				BufState = BUF_FULL;
				return;
			}
			else {
				BufState = BUF_BAD;
				std::vector<string> params(1);
				throw err("!BMP2", params);
			}
		case BUF_WRITEONLY:	
			// w srodku bufora
			if (BufBytePosp + 1 < BufSize) {
				Buf[BufBytePosp++] = DabByte;
				++DataCount;
				BitDataCount += 8;
				BufBitPosp += 8;
			}
			// na ostatniej pozycji
			else if(BufBitPosp + 1 == BufBitSize) {
				Buf[BufBytePosp] = DabByte;
				// zpisujemy pelny bufor
				File.clear();
				File.write((char *)Buf,BufSize);
				File.flush();
				BufBytePosp = 0;
				BufBitPosp = 0;
				DataCount = 0;
				BitDataCount = 0;
				return;
			}
			else {
				BufState = BUF_BAD;
				std::vector<string> params(1);
				throw err("!BMP2", params);
			}
		default: {
			// nie mozna wysylac w innym trybie na Bufor!!
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}
	}
}

/*!
 * \brief
 * Ustawia tryb bufora.
 * 
 * \param DabBufMode
 * Okresla nowy stan bufora.
 *
 * Zmienia stan bufora na ten przyslany w parametrze.
 */
void BUFFOR::SetMode(BUFFOR_MODE DabBufMode) {
	BufMode = DabBufMode;
	// nie wiadomo w jakim trybie zostaly umieszczone dane wiec trzeba wyzerowac bufor
	DataCount = 0;
	BitDataCount = 0;
	// ustawienie stanu, gdy transfer albo zapis bufor moze przyjac dane
	// w przeciwnym razie trzeba najpierw zrobic odczyt
	if (BufMode == BUF_TRANSFER || BufMode == BUF_WRITEONLY) 
		BufState = BUF_GOOD;
	else 
		BufState= BUF_BAD;
}

/*!
 * \brief
 * Zamkniecie pliku.
 *
 */
void BUFFOR::CloseFile() {
	BufReset();
	File.close();
}
/*!
 * \brief
 * Zerowanie bufora.
 * 
 * Resetuje bufor i ustawia stan poczatkowy.
 */
void BUFFOR::BufReset()	{
	File.clear();
	FileSize = 0;
	DataCount = 0;
	BitDataCount = 0;
	BufBitPosg = 0;
	BufBitPosp = 0;
	BufBytePosg = 0;
	BufBytePosp = 0;
	ReadSize = BUFFOR_SIZE;
	BufState = BUF_BAD;
	BufMode = BUF_TRANSFER;
}

//**************** "chodzenie po buforze"*******************//
/*!
 * \brief
 * Ustawienie pozycji w bajtach do odczytu.
 *
 * \throws err()
 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
 *
 */
void BUFFOR::BufByteSeekg(uint32 DabnewPos) {
	if (DabnewPos >= 0 && DabnewPos < DataCount) {
		BufBytePosg = DabnewPos;
		BufBitPosg = DabnewPos * 8;
	}
	else {
		std::vector<string> params(1);
		throw err("!BMP2", params);	
	}
}
/*!
 * \brief
 * Ustawienie pozycji w bajtach do zapisu.
 *
 * \throws err()
 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
 *
 */
void BUFFOR::BufByteSeekp(uint32 DabnewPos) {
	if (DabnewPos >= 0 && DabnewPos < DataCount) {
		BufBytePosp = DabnewPos;
		BufBitPosp = DabnewPos * 8;
	}
	else {
		std::vector<string> params(1);
		throw err("!BMP2", params);
	}
}
/*!
 * \brief
 * Ustawienie pozycji w bitach do odczytu.
 *
 * \throws err()
 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
 *
 */
void BUFFOR::BufBitSeekg(uint32 DabnewPos) {
	if (DabnewPos >= 0 && DabnewPos < BitDataCount) {
		BufBitPosg = DabnewPos;
		BufBytePosg = DabnewPos / 8;
	}
	else  {
		std::vector<string> params(1);
		throw err("!BMP2", params);
	}
}
/*!
 * \brief
 * Ustawienie pozycji w bitach do zapisu.
 *
 * \throws err()
 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
 *
 */
void BUFFOR::BufBitSeekp(uint32 DabnewPos) {
	if (DabnewPos >= 0 && DabnewPos < BitDataCount) {
		BufBitPosp = DabnewPos;
		BufBytePosp = DabnewPos / 8;			
	}
	else  {
		std::vector<string> params(1);
		throw err("!BMP2", params);
	}
}
/*!
 * \brief
 * Pobranie aktualnej pozycji do odczytu w buforze w bajtach.
 */
const uint32 BUFFOR::BufByteTellg() const {
	return BufBytePosg;
}
/*!
 * \brief
 * Pobranie aktualnej pozycji do zapisu w buforze w bajtach.
 */
const uint32 BUFFOR::BufByteTellp() const {
	return BufBytePosp;
}
/*!
 * \brief
 * Pobranie aktualnej pozycji do odczytu w buforze w bitach.
 */
const uint32 BUFFOR::BufBitTellg() const {
	return BufBitPosg;
}
/*!
 * \brief
 * Pobranie aktualnej pozycji do zapisu w buforze w bitach.
 */
const uint32 BUFFOR::BufBitTellp() const {
	return BufBitPosp;
}
/*!
 * \brief
 * Napelnienie bufora.
 *
 * \throws err()
 * Rzuca wyjatek przy bledach odczytu zapisu.
 * 
 * Odczyt danych z dysku do bufora. 
 */
void BUFFOR::Fill() {
	switch (BufMode) {
		case BUF_READONLY: 
			File.clear();
			File.read((char *)Buf, ReadSize);
			DataCount = File.gcount();
			BitDataCount = 8 * DataCount ;
			if (!DataCount) {
				if (File.eof())
					BufState = BUF_EOF;
				else
					BufState = BUF_BAD;
			}
			BufState = BUF_GOOD;
			BufBytePosg = 0;
			BufBitPosg = 0;
			break;
		default: {
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}
	}
}

/*!
 * \brief
 * Oproznienie bufora.
 * 
 * Zapis danych z bufora na dysk.
 * 
 */
void BUFFOR::Flush() {
	switch (BufMode) {
		case BUF_WRITEONLY: 
			File.clear();
			File.write((char *)Buf,DataCount);
			File.flush();
			BufState = BUF_EMPTY;
			BufBytePosg = BufBitPosg = 0;
			BitDataCount = DataCount = 0;
			break;
		default: {
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}
	}
}
//********************end of class BUFFOR********************//
//********************class BMP_BUFFOR********************//
/*!
 * \brief
 * Konstruktor klasy BMP_BUFFOR.
 *
 * BMP_BUFFOR wykonuje inicjalizacjie (zerowanie) bufora.
 */
BMP_BUFFOR::BMP_BUFFOR() {
	BmpHeader = 0;
	// domyslnie kompresja = 1
	Compr = 1;
	Seek = uint8 (8 - Compr);
	BytePosg = 0; 
	BytePosp = 0;
	BitPosg = 0; 
	BitPosp = 0;
	ModReminder = 0;
	ModWidth = 0;
}

/*!
 * \brief
 * Konstruktor klasy BMP_BUFFOR.
 * 
 * \param DabBmpHeader
 * Wskaznik na obiekt BMP_HEADER, przechowujacy informacjie o aktualnej bitmapie.
 *
 * BMP_BUFFOR wykonuje inicjalizacjie (zerowanie) bufora.
 */
BMP_BUFFOR::BMP_BUFFOR(BMP_HEADER* DabBmpHeader) {
	BmpHeader = DabBmpHeader;
	Compr = 1;
	Seek = uint8 (8 - Compr);
	BytePosg = 0;
	BytePosp = 0;
	BitPosg = 0;
	BitPosp = 0;
	ReadSize = BUFFOR_SIZE;
	ModReminder = 0;
	ModWidth = 0;
}
/*!
 * \brief
 * Pobranie jednego bita z bufora.
 * 
 * \returns
 * Zwraca 0xFF lub 0x00.
 * 
 * \throws err()
 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
 * 
 * Zwraca wartosc aktualnego bitu w buforze.
 */
uint8 BMP_BUFFOR::GetBit() {
	switch (BufMode) {
		case BUF_READONLY: {
// pojawi sie problem gdy ktos ustawi pozycje w bitach na czesc bajtu ktorej nie odczytujemy
// co wtedy, chyba w byte/bit seekach to poprawic
// trzeba zapewnic, ze przy seekowaniu ustawiony bêdzie tylko bit brany pod uwage podczas kompresji
// i ten bit nie bedzie sie znajdowac w dopelniajacych zerach!!!!!

			// w srodku bufora, gdzies przed ostatnim bitem, zatem odczyt uda sie na pewno,
			// jesli pobierany bit jest najmlodszym bitem z bajta -
			// - nastepuje przesuniecie glowicy o bajt, wtedy sprawdzamy czy nie wystepuja zera dopelniajace
			// i czy mozna ewentualnie przesunac
			if (BufBitPosg + 1 < BitDataCount) {
				uint8 Result;
				if (Buf[BufBytePosg] & One[BufBitPosg % 8 ]) 
					Result = BitSet;
				else
					Result = BitNSet;
				++BufBitPosg;
				// jesli spelniony to przesuniecie o Seek na pewno sie zmiesci
				if (!(BufBitPosg % 8)) {
					++BufBytePosg; 
					BufBitPosg += Seek; 
					// gdy zwiekszamy pozycje buf w bajtach to trzeba sprawdzic czy nie ma zer dopelniajacych
					uint32 ModSeek = (BytePosg + BufBytePosg - BmpHeader->bfOffBits) % ModWidth;
					if ((ModSeek + ModReminder) >= ModWidth) {
						// czy przesuniecie zmiesci sie w buforze?
						if (BufBitPosg + 8 * (ModWidth - ModSeek) < BitDataCount) {
							BufBytePosg += ModWidth - ModSeek;
							BufBitPosg = 8 * BufBytePosg + Seek;
						}
						// nie mozna przesucac trzeba wczytac ponownie!
						else {
							File.clear();
							BytePosg += DataCount;
							BitPosg = 8 * BytePosg ;
							File.seekg(BytePosg);
							File.read((char *)Buf, ReadSize);
							DataCount = File.gcount();
							BitDataCount = 8 * DataCount;
							if (!DataCount) {
								if (File.eof())
									BufState = BUF_EOF;
								else 
									BufState = BUF_BAD;
								return Result;
							}
							// wstepne ustawienie
							BufBytePosg = 0;
							BufBitPosg = Seek;
							// obliczenie czy akurat pierwszy bajte buforu to nie zera dopelniajace
							// BufBytePosg == 0
							ModSeek = (BytePosg - BmpHeader->bfOffBits) % ModWidth;
							// spr czy trzeba przesunac
							if ((ModSeek + ModReminder) >= ModWidth) {
								// czy przesuniecie zmiesci sie w buforze?
								if (BufBitPosg + 8 * (ModWidth - ModSeek) < BitDataCount) {
									BufBytePosg += ModWidth - ModSeek;
									BufBitPosg = 8 * BufBytePosg + Seek;
								}	
								// nie zmiescilo sie, ale przed chwila wczytywalismy, wiec koniec pliku:
								else {
									BufState = BUF_EOF;
								}
							}	
						}
					}
				}
				return Result;
			}
			// na ostatniej pozycji, po odczytaniu bitu trzeba wypelnic ponownie bufor
			else if (BufBitPosg + 1 == BitDataCount) {
				uint8 Result;
				if (Buf[BufBytePosg] & One[BufBitPosg % 8 ]) 
					Result = BitSet;
				else
					Result = BitNSet;

				File.clear();
				BytePosg += DataCount;
				BitPosg = 8 * BytePosg ;
				File.seekg(BytePosg);
				File.read((char *)Buf,ReadSize);
				DataCount = File.gcount();
				BitDataCount = 8 * DataCount;
				if (!DataCount) {
					if (File.eof())
						BufState = BUF_EOF;
					else 
						BufState = BUF_BAD;
					return Result;
				}
				// wstepne ustawienie
				BufBytePosg = 0;
				BufBitPosg = Seek;
				// obliczenie czy akurat pierwszy byte buforu to nie zera dopelniajace
				uint32 ModSeek = (BytePosg - BmpHeader->bfOffBits) % ModWidth;
				if ((ModSeek + ModReminder) >= ModWidth) {
					// spr czy mozna przesunac
					if (BufBitPosg + 8 * (ModWidth - ModSeek) < BitDataCount) {
						BufBytePosg += ModWidth - ModSeek;
						BufBitPosg = 8 * BufBytePosg + Seek;
					}	
					// przed chwila wczytywalismy wiec koniec pliku:
					else {
						BufState = BUF_EOF;
					}
				}
				return Result;
			} 
			std::vector<string> params(1);
			throw err("!BMP2", params);
	   }
		default: {
			// inny tryb niedozwolony
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}
	}
}

/*!
 * \brief
 * Zapis jednego bita do bufora.
 * 
 * \param DabBit
 * Wartosc zapisywanego bita 0xFF lub 0x00.
 * 
 * \throws err()
 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
 * 
 * Zapisuje wartosc bitu przeslanego w prametrze pod aktualna pozycje w buforze.
 */
void BMP_BUFFOR::PutBit(uint8 DabBit) {
	switch (BufMode) {
		// to bardziej przypomina read tylko ze z zapisujemy przed odczytem
		case BUF_READ_WRITE: {
			// w srodku bufora
			if ((BufBitPosp + 1 < BitDataCount)) {
				Buf[BufBytePosp] = (Buf[BufBytePosp] & ExOne[BufBitPosp % 8]) | (DabBit & One[BufBitPosp % 8]);
				++BufBitPosp;
				// nie zwiekszamy liczebnosci danych, bo dane bitmapy sa juz w buforze!!
				if (!(BufBitPosp % 8)) {
					++BufBytePosp;
					BufBitPosp += Seek;
					// gdy zwiekszamy pozycje buf w byteach to trzeba sprawdzic czy nie ma zer dopelniajacych
					uint32 ModSeek = (BufBytePosp + BytePosp - BmpHeader->bfOffBits) % ModWidth;
					if ((ModSeek + ModReminder) >= ModWidth) {
						if (BufBitPosp + 8 * (ModWidth - ModSeek) < BitDataCount) {
							BufBytePosp += ModWidth - ModSeek;
							BufBitPosp = 8 * BufBytePosp + Seek;
						}	
						// nie mozna przesucac trzeba zapisac i wczytac ponownie
						else {
							File.clear();
							File.seekp(BytePosp, std::ios_base::beg);
							File.write((char *)Buf, DataCount);
							File.flush();
							BytePosp += DataCount;
							BitPosp = 8 * BytePosp;
							// przygotowanie do odczytu z pozycji w ktora chcemy pisac
							File.seekg(BytePosp, std::ios_base::beg);
							File.read((char *)Buf, ReadSize);
							DataCount = File.gcount();
							BitDataCount = 8 * DataCount;
							if (!DataCount) {
								if (File.eof())
									BufState = BUF_EOF;
								else 
									// jakis blad
									BufState = BUF_BAD;
								std::vector<string> params(1);
								throw err("!BMP2", params);
							}
							// wstepne ustawienie
							BufBytePosp = 0;
							BufBitPosp = Seek;

							// obliczenie czy akurat pierwszy byte buforu to nie zera dopelniajace
							// wersja dla BytePosp
							ModSeek = (BytePosp - BmpHeader->bfOffBits) % ModWidth;
							// przesuniecie, musi sie zmiescic w buforze!!
							if ((ModSeek + ModReminder) >= ModWidth) {
								// spr czy trzeba przesunac
								if (BufBitPosp + 8 * (ModWidth - ModSeek) < BitDataCount) {
									BufBytePosp += ModWidth - ModSeek;
									BufBitPosp = 8 * BufBytePosp + Seek;
								}	
								// przed chwila wczytywalismy wiec koniec pliku:
								else {
									BufState = BUF_EOF;
								}
							}
						}
					}
				}
				return ;
			}
			// na ostatniej pozycji w buforze!
			else if (BufBitPosp + 1 == BitDataCount) {
				Buf[BufBytePosp] = (Buf[BufBytePosp] & ExOne[BufBitPosp % 8]) | (DabBit & One[BufBitPosp % 8]);

				File.clear();
				File.seekp(BytePosp, std::ios_base::beg);
				File.write((char *)Buf, DataCount);
				File.flush();
				BytePosp += DataCount;
				BitPosp = 8 * BytePosp;
				// przygotowanie do odczytu z pozycji w ktora chcemy pisac
				File.seekg(BytePosp, std::ios_base::beg);
				File.read((char *)Buf,ReadSize);
				DataCount = File.gcount();
				BitDataCount = 8 * DataCount;
				if (!DataCount) {
					if (File.eof())
						BufState = BUF_EOF;
					else 
						// jakis blad
						BufState = BUF_BAD;
				}
				BufBytePosp = 0;
				BufBitPosp = Seek;

				// obliczenie czy akurat pierwszy byte buforu to nie zera dopelniajace
				// BufBytePosp == 0
				uint32 ModSeek = (BytePosp - BmpHeader->bfOffBits) % ModWidth;
				// przesuniecie, musi sie zmiescic w buforze!!
				if ((ModSeek + ModReminder) >= ModWidth) {
					// spr czy trzeba przesunac
					if (BufBitPosp + 8 * (ModWidth - ModSeek) + 1 < BitDataCount) {
						BufBytePosp += ModWidth - ModSeek;
						BufBitPosp = 8 * BufBytePosp + Seek;
					}	
					// przed chwila wczytywalismy wiec koniec pliku:
					else {
						BufState = BUF_EOF;
					}
				}
				return;
			}
			// tu nie powinien sie nigdy znalezc!!
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}
		default: {
			// nie mozna wysylac w innym trybie na Bufor!!
			std::vector<string> params(1);
			throw err("!BMP2", params);

		}
	}
}

/*!
 * \brief
 * Zerowanie bufora.
 * 
 * Resetuje bufor i ustawia stan poczatkowy.
 */
void BMP_BUFFOR::BufReset() {
	BUFFOR::BufReset();
	BufMode = BUF_READONLY;
	BitPosg = 0;
	BitPosp = 0;
	BytePosg = 0;
	BytePosp = 0;
	Compr = 1;
	Seek = uint8 (8 - Compr);
	ModReminder = 0;
	ModWidth = 0;
}
/*!
 * \brief
 * Ustawienie noewej kompresji.
 * 
 * \param Dabcompr
 * Nowa kompresja, dozwolone wartosci od 1 do 7.
 *
 */
void BMP_BUFFOR::SetCompr(uint8 Dabcompr) {
	Compr = Dabcompr;
	Seek = uint8 (8 - Compr);
}

/*!
 * \brief
 * Ustawia wielkosc blokow doczytywanych z dysku.
 * 
 * \param DabNewSize
 * Ilosc danych w bajtach doczytywanych z dysku przez bufor .
 * 
 */
void BUFFOR::SetReadSize(uint32 DabNewSize) {
	if (DabNewSize <= BufSize)
		ReadSize = DabNewSize;
}
/*!
 * \brief
 * Ustawienie glowicy odczytujacej na okreslony bit.
 * 
 * \param DabnewPos
 * Przedstawia ilosc bitow.
 * 
 * \param DabSeekWay
 * Wartosci: BEGINING albo FORWARD.
 * 
 * \throws err()
 * Rzuca wyjatek przy bledach odczytu zapisu.
 * 
 * Gdy parametr == BEGINING, ustawiamy pozycje na bit przyslany w parametrze liczac od poczatku pliku. Gdy parametr == FORWARD, ustawiamy przesuwamy pozycje o DabnewPos w przod.
 * 
 */
void BMP_BUFFOR::BitSeekg(uint64 DabnewPos, SEEKWAY DabSeekWay) {
	switch (DabSeekWay) {
		case BEGINING: {
		// czy dane w buforze? BitDataCount oznacza ilosc wszystkich danych
		// nie tylko tych uzytecznych
			if (DabnewPos >= BitPosg && DabnewPos < BitDataCount + BitPosg ) {
				BufBytePosg = uint32(DabnewPos / 8) - BytePosg;
				BufBitPosg = uint32(DabnewPos - BitPosg);
			}
			else {
				BytePosg = uint32 (DabnewPos / 8);
				BitPosg = 8 * BytePosg;
				BufBitPosg = uint32 (DabnewPos % 8);
				BufBytePosg = 0;
				Fill();
			}
			break;
		}
		case FORWARD: {
			if (DabnewPos + BitPosg < BitDataCount) {
				BufBitPosg += (uint32) DabnewPos;
				BufBytePosg += uint32 (DabnewPos /8);
			}
			else {
				BytePosg += BufBytePosg + (uint32) (DabnewPos / 8);
				BitPosg += BufBitPosg + 8 * BytePosg;
				BufBitPosg = uint32 (DabnewPos % 8);
				BufBytePosg = 0;
				Fill();
			}
			break;
		}
		default: {
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}						  
	}
}


/*!
 * \brief
 * Ustawienie glowicy zapisujacej na okreslony bit.
 * 
 * \param DabnewPos
 * Przedstawia ilosc bitow.
 * 
 * \param DabSeekWay
 * Wartosci: BEGINING albo FORWARD.
 * 
 * \throws err()
 * Rzuca wyjatek przy bledach odczytu zapisu.
 * 
 * Gdy parametr == BEGINING, ustawiamy pozycje na bit przyslany w parametrze liczac od poczatku pliku. Gdy parametr == FORWARD, ustawiamy przesuwamy pozycje o DabnewPos w przod.
 * 
 */
void BMP_BUFFOR::BitSeekp(uint64 DabnewPos, SEEKWAY DabSeekWay) {
	switch (DabSeekWay) {
		case BEGINING: {
			// czy dane w buforze?
			if (DabnewPos >= BitPosp && DabnewPos < BitDataCount + BitPosp) {
				BufBytePosp = uint32(DabnewPos / 8 - BytePosp);
				BufBitPosp = uint32(DabnewPos - BitPosp);
			}
			else {
				// zapisanie bufora
				Flush();
				BytePosp = uint32(DabnewPos / 8);
				File.seekp(BytePosp, std::ios_base::beg);
				BitPosp = 8 * BytePosp;
				BufBytePosp = 0;
				BufBitPosp = uint32(DabnewPos % 8);
				// aby pisac do bufora musza byc tam dane bitmapy
				Fill();
			}
			break;
		}
		case FORWARD: {
			if (DabnewPos + BitPosp < BitDataCount) {
				BufBitPosp += (uint32) DabnewPos;
				BufBytePosp += (uint32) (DabnewPos / 8);
			}
			else {
				BytePosp += BufBytePosp + (uint32) (DabnewPos / 8);
				File.seekp(BytePosp, std::ios_base::beg);
				BitPosp = 8 * BytePosp;
				BufBytePosp = 0;
				BufBitPosp = (uint32) (DabnewPos % 8);
				Fill();
			}
			break;
		}
		default: {
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}
	}
}

/*!
 * \brief
 * Wielkosc pliku w bitmapie.
 * 
 * \param DabSize
 * Wielkosc rzeczywistego pliku w BITACH.
 * 
 * \param DabCurPos
 * Aktualna pozycja w buforze w BITACH.
 * 
 * \returns
 * Zwraca wielkosc przyslanego pliku w bitmapie w bitach.
 * 
 * Konwertuje wielkosc pliku w bitach na wielkosc tego pliku w bitach ALE W BITMAPIE. Bierze pod uwage kompresje oraz ilosc zer wyrownujacych.
 * 
 */
uint64 BMP_BUFFOR::FileSizeInBmp(uint64 DabSize,  uint64 DabCurPos) {
	if (!DabSize)
		return 0;
	// wielkosc przesunieca 
	// podstawowe przesuniecie - calkowity rozmiar pliku w bitmapie (nie uwzgledniajacy dopelniajacych zer):
	uint64 NewSize = (DabSize / Compr) * 8 + DabSize % Compr;
	if (DabCurPos % 8 + DabSize % Compr >= 8) {
		NewSize += Seek;
		NewSize += DabSize % Compr + DabCurPos % 8;
		NewSize -= 8;
	}
	// dodanie zer!!
	uint64 ModSeek = 8 * ModReminder * ( NewSize / (24 * BmpHeader->biWidth));
	// >= czy samo ?>
	if ((DabCurPos - 8 * BmpHeader->bfOffBits) % (8 * ModWidth) + NewSize % (24 * BmpHeader->biWidth) >= 24 * BmpHeader->biWidth ) {
		NewSize+= 8 * ModReminder; //ModWidth - NewSize % (24 * BmpHeader->biWidth);
	}
	return (NewSize + ModSeek);
}
/*!
 * \brief
 * Pobranie aktualnej pozycji do odczytu w buforze w bajtach.
 */
const uint32 BMP_BUFFOR::ByteTellg() const {
	return BytePosg + BufBytePosg;
}
/*!
 * \brief
 * Pobranie aktualnej pozycji do zapisu w buforze w bajtach.
 */
const uint32 BMP_BUFFOR::ByteTellp() const {
	return BytePosp + BufBytePosp;
}
/*!
 * \brief
 * Pobranie aktualnej pozycji do odczytu w buforze w bitach.
 */
const uint64 BMP_BUFFOR::BitTellg() const {
	return BitPosg + BufBitPosg;
}
/*!
 * \brief
 * Pobranie aktualnej pozycji do zapisu w buforze w bitach.
 */
const uint64 BMP_BUFFOR::BitTellp() const{
	return BitPosp + BufBitPosp;
}
/*!
 * \brief
 * Napelnienie bufora.
 *
 * \throws err()
 * Rzuca wyjatek przy bledach odczytu zapisu.
 * 
 * Odczyt danych z dysku do bufora. Wersja niemodyfikujaca pozycji glowic - np. gdy bedzie skok do jakiejs pozycji to nalezy zaczac czytac od tego ustawionego bita a nie od pierwszego w kompresjiUstawienia glowic zaleza od trybu.
 */
void BMP_BUFFOR::Fill() {
	switch (BufMode) {
		case BUF_READONLY: 
			// pierwszy odczyt - z dokladnie tego okreslonego miejsca
			File.clear();
			File.seekg(BytePosg,std::ios_base::beg);
			File.read((char *)Buf, ReadSize);
			DataCount = File.gcount();
			BitDataCount = 8 * DataCount ;
			if (!DataCount) {
				if (File.eof())
					BufState = BUF_EOF;
				else
					BufState = BUF_BAD;
				// wyjatek?
				return;
			}
			BufState = BUF_GOOD;
			break;
		// pierwszy odczyt, bez zapisu
		case BUF_READ_WRITE: 
			File.clear();
			// czytamy z pozycji gdzie bedziemy zapisuwac
			File.seekg(BytePosp, std::ios_base::beg);
			File.read((char *)Buf,ReadSize);
			DataCount = File.gcount();
			BitDataCount = 8 * DataCount ;
			if (!DataCount) {
				if (File.eof())
					BufState = BUF_EOF;
				else
					BufState = BUF_BAD;
				// wyjatek?
				return;
			}
			BufState = BUF_GOOD;
			break;
		default: {
			std::vector<string> params(1);
			throw err("!BMP2", params);
		}

	}

}

/*!
 * \brief
 * Oproznienie bufora.
 *
 * \throws err()
 * Rzuca wyjatek przy bledach odczytu zapisu.
 *  
 * Zapis danych z bufora na dysk.
 */
void BMP_BUFFOR::Flush() {
	switch (BufMode) {
		case BUF_READ_WRITE: 
			File.clear();
			File.seekp(BytePosp,std::ios_base::beg);
			File.write((char *)Buf,DataCount);
			DataCount = 0;
			BitDataCount = 0;
			File.flush();
			BufState = BUF_EMPTY;
			break;
		
		default: 
			std::vector<string> params(1);
			throw err("!BMP2", params);
	}
}
//********************end of class BMP_BUFFOR********************//
}//********************end of namespace bmp********************//
