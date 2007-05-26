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

	bmp 
	bmp.cpp

*********************************************************************/

#include "bmp.h"

//********************namespace bmp********************//
namespace bmp {
	const uint8 BUFFOR::One [8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
	const uint8 BUFFOR::ExOne  [8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
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
	 * \throws <exception class>
	 * Gdy plik nie istnieje.
	 * 
	 * Otwiera plik, ustawia tryb pracy i oblicza jego wielkosc.
	 */
	void BUFFOR::OpenFile(std::string DabPath, std::ios_base::open_mode DabFileMode, BUFFOR_MODE DabBufMode) {
		File.open(DabPath.c_str(), DabFileMode | std::ios_base::binary);
		if (!File) {
			BufState = BUF_BAD;
			throw NO_SUCH_FILE();
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
	 * \throws <exception class>
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
			throw BMP_INTERNAL_ERROR();
			
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
					throw BMP_INTERNAL_ERROR();
				}
			default: 
				// inny tryb niedozwolony
				throw BMP_INTERNAL_ERROR();
		}
	}

	/*!
	 * \brief
	 * Zapis jednego bita do bufora.
	 * 
	 * \param DabBit
	 * Wartosc zapisywanego bita 0xFF lub 0x00.
	 * 
	 * \throws <exception class>
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
					throw BMP_INTERNAL_ERROR();
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
					throw BMP_INTERNAL_ERROR();
				}
			default: 
				// nie mozna wysylac w innym trybie na Bufor!!
				throw BMP_INTERNAL_ERROR();
		}
	}

	/*!
	 * \brief
	 * Pobranie jednego bajta z bufora.
	 * 
	 * \returns
	 * Zwraca 0xFF lub 0x00.
	 * 
	 * \throws <exception class>
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
				throw BMP_INTERNAL_ERROR();
			
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
					throw BMP_INTERNAL_ERROR();
				}
			
			default: 
				// inny tryb niedozwolony
				throw BMP_INTERNAL_ERROR();
			
		}
	}

	/*!
	 * \brief
	 * Zapis jednego bajtu do bufora.
	 * 
	 * \param DabByte
	 * Wartosc zapisywanego bajtu 0xFF lub 0x00.
	 * 
	 * \throws <exception class>
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
					throw BMP_INTERNAL_ERROR();
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
					throw BMP_INTERNAL_ERROR();
				}
			default: 
				// nie mozna wysylac w innym trybie na Bufor!!
				throw BMP_INTERNAL_ERROR();
		}
	}

	/*!
	 * \brief
	 * Pobiera wielkosc pliku.
	 * 
	 * \returns
	 * Zwraca wielkosc pliku w bajtach.
	 * 
	 */
	inline const uint32 BUFFOR::GetFileSize() const {
		return FileSize;
	}

	/*!
	 * \brief
	 * Pobiera stan bufora.
	 * 
	 * \returns
	 * Zwraca aktualny stan bufora.
	 * 
	 */
	inline const BUFFOR::BUFFOR_STATE BUFFOR::GetBufState() const {
		return BufState;
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
	 * \throws <exception class>
	 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
	 *
	 */
	void BUFFOR::BufByteSeekg(uint32 DabnewPos) {
		if (DabnewPos >= 0 && DabnewPos < DataCount) {
			BufBytePosg = DabnewPos;
			BufBitPosg = DabnewPos * 8;
		}
		else 
			throw BMP_INTERNAL_ERROR();
	}
	/*!
	 * \brief
	 * Ustawienie pozycji w bajtach do zapisu.
	 *
	 * \throws <exception class>
	 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
	 *
	 */
	void BUFFOR::BufByteSeekp(uint32 DabnewPos) {
		if (DabnewPos >= 0 && DabnewPos < DataCount) {
			BufBytePosp = DabnewPos;
			BufBitPosp = DabnewPos * 8;
		}
		else 
			throw BMP_INTERNAL_ERROR();
	}
	/*!
	 * \brief
	 * Ustawienie pozycji w bitach do odczytu.
	 *
	 * \throws <exception class>
	 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
	 *
	 */
	void BUFFOR::BufBitSeekg(uint32 DabnewPos) {
		if (DabnewPos >= 0 && DabnewPos < BitDataCount) {
			BufBitPosg = DabnewPos;
			BufBytePosg = DabnewPos / 8;
		}
		else 
			throw BMP_INTERNAL_ERROR();
	}
	/*!
	 * \brief
	 * Ustawienie pozycji w bitach do zapisu.
	 *
	 * \throws <exception class>
	 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
	 *
	 */
	void BUFFOR::BufBitSeekp(uint32 DabnewPos) {
		if (DabnewPos >= 0 && DabnewPos < BitDataCount) {
			BufBitPosp = DabnewPos;
			BufBytePosp = DabnewPos / 8;			
		}
		else 
			throw BMP_INTERNAL_ERROR();
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
	 * \throws <exception class>
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
			default: 
				throw BMP_INTERNAL_ERROR();
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
			default: 
				throw BMP_INTERNAL_ERROR();
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
	 * \throws <exception class>
	 * Rzuca wyjatek, gdy nastepuje proba wykonania niedozwolonej operacji na buforze.
	 * 
	 * Zwraca wartosc aktualnego bitu w buforze.
	 */
	uint8 BMP_BUFFOR::GetBit() {
		switch (BufMode) {
			case BUF_READONLY: {
// pojawi sie problem gdy ktos ustawi pozycje w bitach na czesc bajtu ktorej nie odczytujemy
// co wtedy, chyba w byte/bit seekach to poprawic
// trzeba zapewnic, ze przy seekowaniu ustawiony b�dzie tylko bit brany pod uwage podczas kompresji
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
				throw BMP_INTERNAL_ERROR();
		   }
			default: {
				// inny tryb niedozwolony
				throw BMP_INTERNAL_ERROR();
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
	 * \throws <exception class>
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
									throw BMP_INTERNAL_ERROR();
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
				throw BMP_INTERNAL_ERROR();
			}
			default: {
				// nie mozna wysylac w innym trybie na Bufor!!
				throw BMP_INTERNAL_ERROR();
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
	 * Pobiera wartosc aktualnej kompresji.
	 * 
	 * \returns
	 * Wartosc aktualnej komrpesji.
	 * 
	 */
	inline const uint8 BMP_BUFFOR::GetCompr() const {
		return Compr;
	}
	/*!
	 * \brief
	 * Pobiera wartosc aktualnej reszty modulo.
	 * 
	 * \returns
	 * Wartosc aktualnej reszty modulo.
	 *
	 * Reszta modulo = BmpWidth % 4 - ilosc zer dopelniajacych w bitmapie. Wartosci od 0 do 3. Udostepnia na zawnatrz pole ModReminder.
	 */
	inline uint32 & BMP_BUFFOR::GetModReminder(){
		return ModReminder;
	}
	/*!
	 * \brief
	 * Pobiera wartosc aktualnej szerokosci modulo bitmapy.
	 * 
	 * \returns
	 * Wartosc aktualnej szerokosci modulo bitmapy.
	 *
	 * Szerokosc modulo = 3 * bmpwidth + ModReminder - Dlugosc wiersza bitmapy. Udostepnia na zawnatrz pole ModWidth.
	 */
	inline uint32 & BMP_BUFFOR::GetModWidth(){
		return ModWidth;
	}
	/*!
	 * \brief
	 * Pobiera wartosc aktualnej reszty modulo.
	 * 
	 * \returns
	 * Wartosc aktualnej reszty modulo.
	 *
	 * Reszta modulo = BmpWidth % 4 - ilosc zer dopelniajacych w bitmapie. Wartosci od 0 do 3. Udostepnia na zawnatrz pole ModReminder.
	 */
	inline const uint32 & BMP_BUFFOR::GetModReminder() const {
		return ModReminder;
	}
	/*!
	 * \brief
	 * Pobiera wartosc aktualnej szerokosci modulo bitmapy.
	 * 
	 * \returns
	 * Wartosc aktualnej szerokosci modulo bitmapy.
	 *
	 * Szerokosc modulo = 3 * bmpwidth + ModReminder - Dlugosc wiersza bitmapy. Udostepnia na zawnatrz pole ModWidth.
	 */
	inline const uint32 & BMP_BUFFOR::GetModWidth() const {
		return ModWidth;
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
	 * \throws <exception class>
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
				throw BMP_INTERNAL_ERROR();
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
	 * \throws <exception class>
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
				throw BMP_INTERNAL_ERROR();
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
	 * \throws <exception class>
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
			default: 
				throw BMP_INTERNAL_ERROR();
		}

	}

	/*!
	 * \brief
	 * Oproznienie bufora.
	 *
	 * \throws <exception class>
	 * Rzuca wyjatek przy bledach odczytu zapisu.
	 *  
	 * Zapis danych z bufora na dysk.
	 */
	void BMP_BUFFOR::Flush() {
		switch (BufMode) {
			case BUF_READ_WRITE: {
				File.clear();
				File.seekp(BytePosp,std::ios_base::beg);
				File.write((char *)Buf,DataCount);
				DataCount = 0;
				BitDataCount = 0;
				File.flush();
				BufState = BUF_EMPTY;
				break;
			}
			default: {
				throw BMP_INTERNAL_ERROR();
			}
		}
	}
//********************end of class BMP_BUFFOR********************//
//********************class BMP********************//
	/*!
	 * \brief
	 * Konstruktor klasy BMP.
	 * 
	 * \param DabFile
	 * Rzeczywisty plik.
	 * 
	 * \throws <exception class>
	 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
	 * 
	 * Otwarcie bitmapy i odczyt naglowka.
	 */
	BMP::BMP(file *DabFile):BmpBuf(&BmpHeader) {
		PreferedCompr = 1;
		BmpBuf.BufReset();
		BmpBuf.OpenFile(DabFile->getRealPath().c_str(), std::ios_base::in | std::ios_base::out, BUFFOR::BUF_READONLY);
		BmpBuf.SetReadSize(BUF_READING_SIZE);
		BmpBuf.Fill();
		ReadBmpHeader();
		if (!IsBmp()) 
			throw BMP_WRONG_FILE();
	}
	/*!
	 * \brief
	 * Konstruktor klasy BMP
	 */
	BMP::BMP():BmpBuf(&BmpHeader) {
		PreferedCompr = 1; // najmniejsza kompresja
	}
	/*!
	 * \brief
	 * Destruktor klasy BMP.
	 *
	 * Kasuje naglowki.
	 */
	BMP::~BMP() {
		DeleteAllHeaders();
	}
	/*!
	 * \brief
	 * Odczyt naglowko pliku bmp.
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
	 * Sprawdzenie czy bitmapa jest plikiem obslugiwanym przez program.
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
	 * Kasuje pliki w bmp.
	 * 
	 * \param Dabpath
	 * Przechowuje nazwy plikow do skasowania.
	 * 
	 * \throws <exception class>
	 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
	 * 
	 * Kasowanie mozna przerwac tylko do momentu zaznaczenia plikow do skasowania. Nie mozna cofnac kasowania juz zamazanego pliku, ani zostawic niespojnych danych w bmp.
	 */
	void BMP::del(vmstring Dabpath) {
		pProgress.show(0);
		int PrgrsTask = 0, PrgrsFin = 0;
		// czy odczytano naglowki?
		BmpBuf.SetCompr(ReadCompr());
		ReadDabHeader();
		if (!IsDab()) {
			throw DAB_EXCEPTION();
		}
		DeleteAllHeaders();
		ReadAllHeaders();
		vmstring::iterator i;
		std::vector<FILE_HEADER *>::iterator j, k, l;
		uint32 FilesToDel = 0, FilesToMove = 0;
		int64 WritingPos, ReadingPos, z;
		int32 count;
		string path;
		DAB_HEADER DabHeaderCopy = DabHeader;
		pProgress.show(2);
		// zaznaczenie plikow do usuniecia
		for (i = Dabpath.begin(); i != Dabpath.end(); ++i) {
			path = "";
			if ((*i)["relativePath"].size())
				path = (*i)["relativePath"] + '\\';
			path += (*i)["name"];
			for (j = FilesHeaders.begin(); j != FilesHeaders.end() ; ++j) {
				// plik do usuniecia:
				if (path == (*j)->FileName) {
					(*j)->PerformAction = true;
					--DabHeader.FilesCount;
					DabHeader.FilesSize -= (*j)->DataSize;
					DabHeader.FreeSpc += FILE_HEADER_BYTE_SIZE + (*j)->FileNameLen + (*j)->DataSize;
					++FilesToDel;
				}
			}
		}
		// 1 - na operacje dodatkowe
		PrgrsTask = FilesToDel + 1;
		PrgrsFin = 1; 
		pProgress.show((100 * PrgrsFin) / PrgrsTask);
		// przerwano kasowanie:
		if (bCanceled) {
			DabHeader = DabHeaderCopy;
			for (j = FilesHeaders.begin(); j != FilesHeaders.end() ; ++j) {
				(*j)->PerformAction = false;
			}
			return;
		}

		// ktoregos pliku nie znaleziono, niepotrzebne sprawdzanie, do usuniecia w wersji finalnej
		if (FilesToDel != Dabpath.size())
			throw NO_SUCH_FILE();

		// wszystkie naglowki zaznaczone do usuniecia:
		if (FilesToDel == FilesHeaders.size()) {
			WriteDabHeader();
			return;
		}

		j = FilesHeaders.begin();
		// pierwszy do usuniecia, pomijamy te na ktorych nie trzeba nic robic
		while (j!= FilesHeaders.end() && !(*j)->PerformAction)
			++j;
		WritingPos = (*j)->HeaderStart;
		k = j;
		++k;
		// pierwszy do przeniesienia, pomijamy te co maja flage do usuniecia
		while (k!= FilesHeaders.end() && (*k)->PerformAction)
			++k;
		l = k;
		while (l!= FilesHeaders.end()) {
			// liczymy pliki ktore trzeba przeniesc:
			if (!(*l)->PerformAction)
				++FilesToMove;
			++l;
		}
		// nie trzeba nic przenosic, np. kasownie 2 ostatnich plikow
		if (!FilesToMove) {
			WriteDabHeader();
			pProgress.show(100);
			return;
		}
		ReadingPos = (*k)->HeaderStart;

		while (FilesToMove--) {
			// pasek postepu
			pProgress.show((100 * PrgrsFin++) / PrgrsTask);
			BmpBuf.SetMode(BUFFOR::BUF_READONLY);
			BmpBuf.SetReadSize(BUFFOR_SIZE);
			BmpBuf.BitSeekg(ReadingPos);
			// zmiana danych przenoszonego pliku
			(*k)->HeaderStart = WritingPos;
			// wielkosc pliku w bitach!!
			(*k)->DataStart = WritingPos + BmpBuf.FileSizeInBmp(8*(FILE_HEADER_BYTE_SIZE +
				(*k)->FileNameLen), WritingPos);
			FileBuf.BufReset();
			FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
			//przenies plik o naglowku *k w miejsce wskazywane przez WritingPos
			for (z = 0; z < 8 * ((*k)->DataSize + (*k)->FileNameLen + FILE_HEADER_BYTE_SIZE); ++z) {
				FileBuf.PutBit(BmpBuf.GetBit());
				if (FileBuf.GetBufState() == BUFFOR::BUF_FULL ||
					// po skopiowaniu ostatniego bitu
					z + 1 == 8 * ((*k)->DataSize + (*k)->FileNameLen + FILE_HEADER_BYTE_SIZE)) {
					ReadingPos = BmpBuf.BitTellg();
					BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
					BmpBuf.BitSeekp(WritingPos);
					// przenies fragment pliku
					while (FileBuf.GetBufState() != BUFFOR::BUF_EMPTY &&
						FileBuf.GetBufState() != BUFFOR::BUF_BAD) {
						BmpBuf.PutBit(FileBuf.GetBit());
					}
					WritingPos = BmpBuf.BitTellp();
					BmpBuf.Flush();
					FileBuf.BufReset();
					FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
					BmpBuf.SetMode(BUFFOR::BUF_READONLY);
					BmpBuf.BitSeekg(ReadingPos);
				}
			}

			// nowa pozycja - za wlasnie przesunientym plikiem (o uaktualnionych polach)
			WritingPos = (*k)->DataStart + BmpBuf.FileSizeInBmp(8 * (*k)->DataSize,(*k)->DataStart);
			++k;
			// pomijamy pliki z ustawiana flaga do usuniecia
			while (k!= FilesHeaders.end() && (*k)->PerformAction)
				++k;
			// czy sa jeszcze jakies naglowki do przeniesienia? (k != ostatni element)
			if (k!= FilesHeaders.end())
				ReadingPos = (*k)->HeaderStart;
		}

		FILE_HEADER * h;
		j = FilesHeaders.begin();
		count = 0;
		while (j != FilesHeaders.end()) {
			if ((*j)->PerformAction) {
				count = int32(j - FilesHeaders.begin());
				h = *j;
				FilesHeaders.erase(j);
				delete h;
				j = FilesHeaders.begin() + count;
			}
			else 
				++j;
		}
		// zastosuj zmiany
		WriteDabHeader();
		pProgress.show(100);
	}

	/*!
	 * \brief
	 * Ukrywa pliki w bmp.
	 * 
	 * \param Dabsrc
	 * Nazwy plikow do spakowania.
	 * 
	 * \param Dabdest
	 * Nazwy pod jakimi beda one spakowane w bmp.
	 * 
	 * \throws <exception class>
	 * Rzuca wyjatek w przypadku bledu odczytu zapisu oraz gdy zabraklo miejsca.
	 * 
	 * Przerwanie moze nastapic co jeden spakowany plik.
	 * 
	 */
	void BMP::store(vmstring Dabsrc, vmstring Dabdest) {
		pProgress.show(0);
		int PrgrsFin = 0, PrgrsTask = (int)Dabsrc.size() + 1;
		// czy odczytano naglowki?
		BmpBuf.SetCompr(ReadCompr());
		ReadDabHeader();
		if (!IsDab()) {
        	setCompression(PreferedCompr);
		}
		DeleteAllHeaders();
		ReadAllHeaders();

		BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
		BmpBuf.SetReadSize(BUF_READING_SIZE);
		FILE_HEADER * header = 0;
		uint64 WritingPos;
		// brak naglowkow
		if (!FilesHeaders.size())
			WritingPos = DabHeader.FirstFileStart;
		else
			WritingPos = FilesHeaders.back()->DataStart +
				BmpBuf.FileSizeInBmp(8 * FilesHeaders.back()->DataSize, FilesHeaders.back()->DataStart);

		vmstring::iterator i, j;
		string path;

		for (i = Dabsrc.begin(), j = Dabdest.begin(); i != Dabsrc.end() && j != Dabdest.end() && !bCanceled; ++i, ++j) {

			// pasek postepu
			pProgress.show((100 * PrgrsFin++) / PrgrsTask);

			header = new FILE_HEADER;
			// FileName ::= <relativePath> + '\\' + <name> | <name>
			header->HeaderStart = WritingPos;
			// src jest w rzeczywistosci, dest w BMP
			if ((*j)["relativePath"].size())
				header->FileName = (*j)["relativePath"] + '\\';
			header->FileName += (*j)["name"];
			header->FileNameLen = uint16(header->FileName.size());
			// sprawdzenie czy sie zmiesci naglowek + nazwa:
			if (DabHeader.FreeSpc < header->FileNameLen + FILE_HEADER_BYTE_SIZE) {
				delete header;
				throw BMP_NOT_ENOUGH_SPACE();
			}

			header->PerformAction = false;
			// atrybuty i date odczytujemy z parametru docelowego
			header->TimeDate = UTCToBytes((*j)["creationTimeUtc"]);
			header->Attributes = AttribToBytes(*j);
			WritingPos += BmpBuf.FileSizeInBmp(8 * (header->FileNameLen + FILE_HEADER_BYTE_SIZE), header->HeaderStart);
			header->DataStart = WritingPos;
			++DabHeader.FilesCount;
			DabHeader.FreeSpc -= FILE_HEADER_BYTE_SIZE + header->FileNameLen;
			
			// pakujemy folder
			if ((*i)["atrDirectory"] == "true") {
				// folder nie ma danych!
				header->DataSize = 0;
			}
			// pakujemy plik
			else {
				FileBuf.BufReset();
				FileBuf.SetReadSize(BUFFOR_SIZE);
				path = "";
				if ((*i)["realPath"].size())
					path = (*i)["realPath"] + '\\';
				path += (*i)["name"];
				FileBuf.OpenFile(path.c_str(), std::ios_base::in, BUFFOR::BUF_READONLY);
				FileBuf.Fill();
				header->DataSize = FileBuf.GetFileSize();
				// czy plik z danymi sie zmiesci?
				if (DabHeader.FreeSpc < header->FileNameLen + FILE_HEADER_BYTE_SIZE + header->DataSize) {
					delete header;
					throw BMP_NOT_ENOUGH_SPACE();
				}
				BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
				BmpBuf.SetReadSize(BUFFOR_SIZE);
				BmpBuf.BitSeekp(header->DataStart);
				// zapis pliku 
				while (FileBuf.GetBufState() != BUFFOR::BUF_EOF) {
					BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
					BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
					BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
					BmpBuf.PutBit(FileBuf.GetBit()); BmpBuf.PutBit(FileBuf.GetBit());
				}
				BmpBuf.Flush();
				FileBuf.CloseFile();
				WritingPos += BmpBuf.FileSizeInBmp(8 * header->DataSize, header->DataStart);
				DabHeader.FilesSize += header->DataSize;
				// miejsce na naglowek juz odjelismy, odejmujemy tylko miejsce na dane
				DabHeader.FreeSpc -= header->DataSize;
			}
			// zapisujemy na biezaco
			FilesHeaders.push_back(header);
			WriteFileHeader(header);
			WriteDabHeader();
		}
		pProgress.show(100);
	}

	/*!
	 * \brief
	 * Wypakowuje pliki z bmp.
	 * 
	 * \param Dabsrc
	 * Nazwy plikow w bmp.
	 * 
	 * \param Dabdest
	 * Nazwy pod jakimi zostana one wypakowane.
	 * 
	 * \throws <exception class>
	 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
	 * 
	 * Przerwanie moze nastapic co jeden wypakowany plik.
	 * 
	 */
	void BMP::extract(vmstring Dabsrc, vmstring Dabdest) {
		pProgress.show(0);
		int PrgrsFin = 0, PrgrsTask = (int) Dabsrc.size() + 1;
		BmpBuf.SetCompr(ReadCompr());
		ReadDabHeader();
		if (!IsDab()) {
			throw DAB_EXCEPTION();
		}
		DeleteAllHeaders();
		ReadAllHeaders();
		uint32 iter;
		string path;
		vmstring::iterator i, j;
		std::vector<FILE_HEADER *>::iterator k;
		BmpBuf.SetMode(BUFFOR::BUF_READONLY);
		BmpBuf.SetReadSize(BUFFOR_SIZE);

		for (i = Dabsrc.begin(), j = Dabdest.begin();	i != Dabsrc.end() && j != Dabdest.end()&& !bCanceled; ++i, ++j) {
			pProgress.show((100 * PrgrsFin++) / PrgrsTask);
			for (k = FilesHeaders.begin(); k != FilesHeaders.end() ; ++k) {
			// src jest w BMP, dest w rzeczywistosci
				path = "";
				if ((*i)["relativePath"].size()) 
					path = (*i)["relativePath"] + '\\';
				path += (*i)["name"];
				if ((*k)->FileName == path) {
					FileBuf.BufReset();
					FileBuf.SetReadSize(BUFFOR_SIZE);
					path = "";
					if ((*j)["realPath"].size()) 
						path = (*j)["realPath"] + '\\';
					path += (*j)["name"];

					FileBuf.OpenFile(path.c_str(),std::ios_base::out, BUFFOR::BUF_WRITEONLY);
					// reszta ustawien byla powyzej  
					BmpBuf.BitSeekg((*k)->DataStart);

					for (iter = 0; iter < (*k)->DataSize ; ++iter) {
						FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
						FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
						FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
						FileBuf.PutBit(BmpBuf.GetBit()); FileBuf.PutBit(BmpBuf.GetBit());
					}
					FileBuf.Flush();
					FileBuf.CloseFile();
				}
			}
		}
		pProgress.show(100);
	}

	/*!
	 * \brief
	 * Kopiuje pliki lub foldery wewnatrz bmp.
	 * 
	 * \param DabSrc
	 * Nazwy plikow w bmp - zrodlowe.
	 * 
	 * \param DabDest
	 * Nazwy plikow w bmp - docelowe.
	 * 
	 * \throws <exception class>
	 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
	 * 
	 * Przerwanie moze nastapic co jeden skopiowany plik.
	 * 
	 */
	void BMP::copyInside(vmstring DabSrc, vmstring DabDest) {
		pProgress.show(0);
		int PrgrsFin = 0, PrgrsTask = (int) DabSrc.size() + 1;
		// czy odczytano naglowki?
		BmpBuf.SetCompr(ReadCompr());
		ReadDabHeader();
		if (!IsDab()) {
			throw DAB_EXCEPTION();
		}
		DeleteAllHeaders();
		ReadAllHeaders();
		// nie ma zadnego naglowka lub nieprawidlowy parametr
		if (!FilesHeaders.size() && DabSrc.size() )
			throw NO_SUCH_FILE();

		FILE_HEADER * h;
		uint64 WritingPos = 0, ReadingPos = 0, temp;
		uint32 number;
		std::vector<FILE_HEADER *>::iterator i, j;
		vmstring::iterator k, m;
		string path;

		for (k = DabSrc.begin(), m = DabDest.begin();
			k != DabSrc.end() && m != DabDest.end()&& !bCanceled; ++k, ++m) {
			pProgress.show((100 * PrgrsFin++) / PrgrsTask);
			path = "";
			if ((*k)["relativePath"].size())
				path = (*k)["relativePath"] + '\\';
			path += (*k)["name"];
			for (i = FilesHeaders.begin(), number = 0 ; i != FilesHeaders.end(); ++i, ++number) {
				if (path == (*i)->FileName) {
					h = new FILE_HEADER;
					h->Attributes = (*i)->Attributes;
					h->DataSize = (*i)->DataSize;
					h->TimeDate = (*i)->TimeDate;
					h->FileName = "";
					if ((*m)["relativePath"].size())
						h->FileName = (*m)["relativePath"] + '\\';
					h->FileName  += (*m)["name"];
					h->FileNameLen = (uint16) h->FileName.size();
					
					if (DabHeader.FreeSpc >= (*i)->DataSize + FILE_HEADER_BYTE_SIZE + (*i)->FileNameLen) {
						DabHeader.FreeSpc -= (*i)->DataSize + FILE_HEADER_BYTE_SIZE + (*i)->FileNameLen;
						++DabHeader.FilesCount;
						DabHeader.FilesSize += (*i)->DataSize;
						h->HeaderStart = FilesHeaders.back()->DataStart +
							BmpBuf.FileSizeInBmp(8 * FilesHeaders.back()->DataSize, 
							FilesHeaders.back()->DataStart);
						h->DataStart = h->HeaderStart +
							BmpBuf.FileSizeInBmp(8 * (h->FileNameLen + FILE_HEADER_BYTE_SIZE), 
							h->HeaderStart);
					}
					else { 
						delete h;
						// zastosuj juz wprowadzone zmiany
						WriteDabHeader();
						// braklo miejsca na skopiowanie:
						throw BMP_NOT_ENOUGH_SPACE();
					}
					FilesHeaders.push_back(h);
					// dokonaj kopii pliku o naglowku *i w miejsce naglowka *h
					// co najmniej 2 elemnty na pewno sa!
					i = FilesHeaders.begin() + number;
					ReadingPos = (*i)->DataStart;
					WritingPos = h->DataStart;

					BmpBuf.SetMode(BUFFOR::BUF_READONLY);
					BmpBuf.SetReadSize(BUFFOR_SIZE);
					FileBuf.BufReset();
					FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
					BmpBuf.BitSeekg(ReadingPos);

					for (temp = 0; temp < 8 * (*i)->DataSize ; ++temp) {
						FileBuf.PutBit(BmpBuf.GetBit());
						if (FileBuf.GetBufState() == BUFFOR::BUF_FULL ||
							// po skopiowaniu ostatniego bitu
							temp + 1 == 8 * (*i)->DataSize) {
							ReadingPos = BmpBuf.BitTellg();
							BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
							BmpBuf.BitSeekp(WritingPos);
							while (FileBuf.GetBufState() != BUFFOR::BUF_EMPTY &&
								FileBuf.GetBufState() != BUFFOR::BUF_BAD) {
								BmpBuf.PutBit(FileBuf.GetBit());
							}
							WritingPos = BmpBuf.BitTellp();
							BmpBuf.Flush();
							FileBuf.BufReset();
							FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
							BmpBuf.SetMode(BUFFOR::BUF_READONLY);
							BmpBuf.BitSeekg(ReadingPos);
						}
					}	
					WriteFileHeader(h);
				}
			}
		}
		WriteDabHeader();
		pProgress.show(100);
	}

	/*!
	 * \brief
	 * Modyfikuje pliki w bmp.
	 * 
	 * \param DabOldPath
	 * Nazwy plikow w bmp - zrodlowe.
	 * 
	 * \param DabNewPath
	 * Nazwy plikow w bmp - docelowe.
	 * 
	 * \throws <exception class>
	 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
	 * 
	 * Mozna przerwac tylko do momentu ustalenia nowego polozenia plikow, potem wszystko trzeba juz poprzesuwac najpierw tworzymy nowe naglowki z nowymi nazwami, potem obliczamy polozenie wszystkich naglowkow i wykonujemy operacje.
	 * 
	 */
	void BMP::modify(vmstring DabOldPath, vmstring DabNewPath) {
		pProgress.show(0);
		int PrgrsFin = 0, PrgrsTask = (int) DabOldPath.size() + 2;
		// czy odczytano naglowki?
		BmpBuf.SetCompr(ReadCompr());
		ReadDabHeader();
		if (!IsDab()) {
			throw DAB_EXCEPTION();
		}
		DeleteAllHeaders();
		ReadAllHeaders();
		// nie ma zadnego naglowka 
		if (!FilesHeaders.size() && DabOldPath.size() )
			throw NO_SUCH_FILE();
		// struktura pomocnicza przy przenoszeniu
		struct R_W_Pos {
			uint64 ReadingPos;
			uint64 WritingPos;
			R_W_Pos(): ReadingPos(0), WritingPos(0) {}
		} Pos;
		
		string path;
		std::vector<FILE_HEADER *> NewFilesHeaders;
		std::vector<FILE_HEADER *>::iterator i, j;
		std::vector<FILE_HEADER *>::reverse_iterator ri,rj;

		FILE_HEADER * h;
		uint64 temp;
		int32 FileNamesChanges = 0;
		size_t FilesToChange = 0;

		std::vector<R_W_Pos> changes;
		std::vector<R_W_Pos>::iterator r;
		std::vector<R_W_Pos>::reverse_iterator rr;
		vmstring::iterator k, m;

		pProgress.show((100 * ++PrgrsFin) / PrgrsTask);

		// kopiowanie naglowkow i pierwszy etap zmian - nowe nazwy
		for (i = FilesHeaders.begin(); i != FilesHeaders.end(); ++i) {
			h = new FILE_HEADER;
			// czesc informacji kopiujemy
			h->Attributes = (*i)->Attributes;
			h->DataSize = (*i)->DataSize;
			h->TimeDate = (*i)->TimeDate;
			for (k = DabOldPath.begin(), m = DabNewPath.begin();
				k != DabOldPath.end() && m != DabNewPath.end(); ++k, ++m) {
				path = "";
				if ((*k)["relativePath"].size())
					path = (*k)["relativePath"] + '\\';
				path += (*k)["name"];
				// modyfukujemy nazwe pliku
				if (path == (*i)->FileName) {
					h->FileName = "";
					if ((*m)["relativePath"].size())
						h->FileName = (*m)["relativePath"] + '\\';
					h->FileName  += (*m)["name"];
					// dodac ograniczeni nazwy do 256 znakow
					h->FileNameLen = (uint16) h->FileName.size();
					FileNamesChanges += h->FileNameLen - (*i)->FileNameLen;
					++FilesToChange;
					// znaleziono i zmieniono nazwe - przejscie do kolejnego naglowka
					break;
				}
				// plik pozostaje bez zmian
				else {
					h->FileName = (*i)->FileName;
					h->FileNameLen = (*i)->FileNameLen;
				}

			}
			NewFilesHeaders.push_back(h);
		}
		// nie znaleziono ktoregos pliku, lub nowe nazwy sie nie zmieszcza (beda za dlugie)
		if (FilesToChange != DabOldPath.size() || 
			FileNamesChanges < 0 ? false : (uint16)FileNamesChanges > DabHeader.FreeSpc) {
			// skasuj stworzone naglowki
			while (NewFilesHeaders.size()) {
				h = NewFilesHeaders.back();
				FilesHeaders.pop_back();
				delete h;
			}
			throw NO_SUCH_FILE();
		}
		// poczatek naglowka pierwszego pliku pozostaje bez zmian
		NewFilesHeaders.front()->HeaderStart = DabHeader.FirstFileStart;
		NewFilesHeaders.front()->DataStart = DabHeader.FirstFileStart + 
			BmpBuf.FileSizeInBmp(8 * (FILE_HEADER_BYTE_SIZE + NewFilesHeaders.front()->FileNameLen), 
			DabHeader.FirstFileStart); 

		// poczatek nastepnego naglowka
		temp = NewFilesHeaders.front()->DataStart +
			BmpBuf.FileSizeInBmp(8 * NewFilesHeaders.front()->DataSize,
			NewFilesHeaders.front()->DataStart);
		// obliczanie pozycji wszystkich naglowkow w pliku:
		// wczesniej sprawdzono czy jest co najmniej jeden naglowek
		for (i = (NewFilesHeaders.begin() + 1) ; i != NewFilesHeaders.end(); ++i) {
			(*i)->HeaderStart = temp;
			(*i)->DataStart = temp + BmpBuf.FileSizeInBmp(
				8 * (FILE_HEADER_BYTE_SIZE + (*i)->FileNameLen), temp); 
			// poczatek nastepnego naglowka
			temp = (*i)->DataStart + BmpBuf.FileSizeInBmp(8 * (*i)->DataSize, (*i)->DataStart);
		}

		// tu jeszcze mozna przerwac operacje
		if(bCanceled) {
			while (NewFilesHeaders.size()) {
				h = NewFilesHeaders.back();
				FilesHeaders.pop_back();
				delete h;
			}
			pProgress.show(100);
			return;
		}
		pProgress.show((100 * ++PrgrsFin) / PrgrsTask);
		// przesuwanie danych w lewo poczawszy od lewej strony
		// i - nowe naglowki, j - stare
		for (i = NewFilesHeaders.begin(), j = FilesHeaders.begin();
			i != NewFilesHeaders.end() && j != FilesHeaders.end(); ++i, ++j ) {
			// dokonaj zamiany - przesuwanie pliku w lewo
			if ((*i)->DataStart < (*j)->DataStart) {
				pProgress.show((100 * PrgrsFin++) / PrgrsTask);
				BmpBuf.SetMode(BUFFOR::BUF_READONLY);
				BmpBuf.SetReadSize(BUFFOR_SIZE);
				FileBuf.BufReset();
				FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
				Pos.ReadingPos = (*j)->DataStart;
				Pos.WritingPos = (*i)->DataStart;
				BmpBuf.BitSeekg(Pos.ReadingPos);
		
				//przenies plik o naglowku *j w miejsce okreslone w naglowku *i
				for (temp = 0; temp < 8 * (*i)->DataSize ; ++temp) {
					FileBuf.PutBit(BmpBuf.GetBit());
					if (FileBuf.GetBufState() == BUFFOR::BUF_FULL ||
						// po skopiowaniu ostatniego bitu
						temp + 1 == 8 * (*i)->DataSize) {
						Pos.ReadingPos = BmpBuf.BitTellg();
						BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
						BmpBuf.BitSeekp(Pos.WritingPos);
						while (FileBuf.GetBufState() != BUFFOR::BUF_EMPTY &&
							FileBuf.GetBufState() != BUFFOR::BUF_BAD) {
							BmpBuf.PutBit(FileBuf.GetBit());
						}
						Pos.WritingPos = BmpBuf.BitTellp();
						BmpBuf.Flush();
						FileBuf.BufReset();
						FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
						BmpBuf.SetMode(BUFFOR::BUF_READONLY);
						BmpBuf.BitSeekg(Pos.ReadingPos);
					}
				}
			}
		}

		// przesuwanie danych w prawo poczawszy od prawej strony
		// ri - nowe naglowki, rj - stare, iteracja odwrotna
		for (ri = NewFilesHeaders.rbegin(), rj = FilesHeaders.rbegin();
			ri != NewFilesHeaders.rend() && rj != FilesHeaders.rend(); ++ri, ++rj ) {
			if ((*ri)->DataStart > (*rj)->DataStart) {
				pProgress.show((100 * PrgrsFin++) / PrgrsTask);
				// caly plik mozna przesunac wczytujac go do buforu FileBuf
				if ((*ri)->DataSize <= BUFFOR_SIZE) {
					BmpBuf.SetMode(BUFFOR::BUF_READONLY);
					BmpBuf.SetReadSize(BUFFOR_SIZE);
					FileBuf.BufReset();
					FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
					BmpBuf.BitSeekg((*rj)->DataStart);
					for (temp = 0; temp < 8 * (*ri)->DataSize ; ++temp) {
						FileBuf.PutBit(BmpBuf.GetBit());
					}
					BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
					BmpBuf.BitSeekp((*ri)->DataStart);
					while (FileBuf.GetBufState() != BUFFOR::BUF_EMPTY &&
						FileBuf.GetBufState() != BUFFOR::BUF_BAD) {
						BmpBuf.PutBit(FileBuf.GetBit());
					}
					BmpBuf.Flush();
				}
				else {
					// jeden raz przenosimy niepelny bufor, wymiar w bitach
					temp = 8 * ((*ri)->DataSize % BUFFOR_SIZE);
					// ile przeniesien wykonanych pelnymi buforami, co najmniej jeden raz
					changes.resize((*ri)->DataSize / BUFFOR_SIZE);
					r = changes.begin();
					r->ReadingPos = Pos.ReadingPos = (*rj)->DataStart;
					r->WritingPos = Pos.WritingPos = (*ri)->DataStart;
					for (++r; r != changes.end(); ++r) {
						r->ReadingPos = Pos.ReadingPos = Pos.ReadingPos + 
							BmpBuf.FileSizeInBmp(8 * BUFFOR_SIZE, Pos.ReadingPos);
						r->WritingPos = Pos.WritingPos = Pos.WritingPos + 
							BmpBuf.FileSizeInBmp(8 * BUFFOR_SIZE, Pos.WritingPos);
					}
					// ustalenie pozycji ostatniego fragmentu pliku:
					Pos.ReadingPos = Pos.ReadingPos + BmpBuf.FileSizeInBmp(8 * BUFFOR_SIZE, Pos.ReadingPos);
					Pos.WritingPos = Pos.WritingPos + BmpBuf.FileSizeInBmp(8 * BUFFOR_SIZE, Pos.WritingPos);
					// skopiowanie ostatniej czesci pliku
					BmpBuf.SetMode(BUFFOR::BUF_READONLY);
					BmpBuf.SetReadSize(BUFFOR_SIZE);
					FileBuf.BufReset();
					FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
					// ReadingPos i WritingPos wskazuja na ostatni fagment pliku do przeniesienia
					BmpBuf.BitSeekg(Pos.ReadingPos);
					while (temp--) {
						FileBuf.PutBit(BmpBuf.GetBit());
					}
					BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
					BmpBuf.BitSeekp(Pos.WritingPos);
					while (FileBuf.GetBufState() != BUFFOR::BUF_EMPTY &&
						FileBuf.GetBufState() != BUFFOR::BUF_BAD) {
						BmpBuf.PutBit(FileBuf.GetBit());
					}
					BmpBuf.Flush();
						// przenoszenie w prawo blokow o wielkosci bufora
					for (rr = changes.rbegin(); rr != changes.rend(); ++rr) {
						BmpBuf.SetMode(BUFFOR::BUF_READONLY);
						BmpBuf.SetReadSize(BUFFOR_SIZE);
						FileBuf.BufReset();
						FileBuf.SetMode(BUFFOR::BUF_TRANSFER);
						BmpBuf.BitSeekg(rr->ReadingPos);
						while (FileBuf.GetBufState() != BUFFOR::BUF_FULL) {
							FileBuf.PutBit(BmpBuf.GetBit());
						}
						BmpBuf.SetMode(BUFFOR::BUF_READ_WRITE);
						BmpBuf.BitSeekp(rr->WritingPos);
						while (FileBuf.GetBufState() != BUFFOR::BUF_EMPTY &&
							FileBuf.GetBufState() != BUFFOR::BUF_BAD) {
							BmpBuf.PutBit(FileBuf.GetBit());
						}
						BmpBuf.Flush();
					}
				}
			}
		}
		// usuniecie starych naglowkow i zastapienie ich nowymi
		DeleteAllHeaders();
		// skopiuj tylko wskazniki
		FilesHeaders = NewFilesHeaders;

		DabHeader.FilesSize = 0;
		DabHeader.FreeSpc = DabHeader.Compr * (BmpHeader.biUsefulData - 3) / 8 - DAB_HEADER_BYTE_SIZE;
		//zapis naglowkow na dysk i modyfikacja naglowka dabstera
		for (i = FilesHeaders.begin(); i != FilesHeaders.end(); ++i) {
			DabHeader.FilesSize += (*i)->DataSize;
			DabHeader.FreeSpc -= (*i)->FileNameLen + FILE_HEADER_BYTE_SIZE + (*i)->DataSize;
			WriteFileHeader(*i);
		}
		WriteDabHeader();
		pProgress.show(100);
	}

	/*!
	 * \brief
	 * Pobiera zawartosc bitmapy.
	 * 
	 * \param Dabpath
	 * Sciezka do folderu w bmp. Pusty parametr == glowny folder w bmp
	 * 
	 * \returns
	 * Zawartosc podanego folderu.
	 * 
	 * \throws <exception class>
	 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
	 * 
	 */
	vmstring BMP::getContent(string Dabpath) {
		BmpBuf.SetCompr(ReadCompr());
		ReadDabHeader();
		if (!IsDab())
			return vmstring();
		
		uint32 k;
		DeleteAllHeaders();
		ReadAllHeaders();
		vmstring result;
		smap temp;
		std::vector<FILE_HEADER*>::iterator i;
		std::stringstream s;
		pProgress.show(0);
		int PrgrsFin = 0, PrgrsTask = (int) FilesHeaders.size() + 1;
		for (i = FilesHeaders.begin(); i!= FilesHeaders.end(); ++i) {
		pProgress.show((100 * PrgrsFin++) / PrgrsTask);
			if (!Dabpath.size() || (*i)->FileName.substr(0, Dabpath.size()) == Dabpath) {
				temp.clear();
				temp = BytesToAttrib((*i)->Attributes);
				// wyznaczenie gdzie zaczyna sie nazwa obiektu (i gdzie konczy sie sciezka)
				if (k = (uint32)(*i)->FileName.size()) {
					--k;
					while (k && (*i)->FileName[k] != '\\' )
					--k;
				}
					if (k)
						// pominiece znaku '\\'
						temp["name"] = (*i)->FileName.substr(k + 1, (*i)->FileName.size());
					else 
						temp["name"] = (*i)->FileName.substr(0, (*i)->FileName.size());
					temp["realPath"] = "";
					temp["relativePath"] = (*i)->FileName.substr(0,k);
					temp["volumeLabel"] = "none";
					temp["driveFormat"] = "DFS";
					temp["driveType"] = "DFS";
				 
					s.clear();
					s << DabHeader.FreeSpc;
					s >> temp["availableFreeSpace"];
					s.clear();
					s << DabHeader.FilesSize;
					s >> temp["totalSize"];
					s.clear();
					//s << DabHeader.Compr * (BmpHeader.biUsefulData - 3 - DAB_HEADER_BYTE_SIZE) / 8;
					s << DabHeader.Compr * (BmpHeader.biUsefulData - 3) / 8 - DAB_HEADER_BYTE_SIZE;
					s >> temp["totalFreeSpace"];
					temp["isReady"] = "true";
					// dd.mm.rrrr hh:mm:ss
					temp["creationTimeUtc"] = BytesToUTC((*i)->TimeDate);
					temp["lastWriteTimeUtc"] = temp["creationTimeUtc"] ;
					temp["lastAccessTimeUtc"] = temp["creationTimeUtc"] ;
			
					s.clear();
					s << (*i)->DataSize;
					s >> temp["length"];
					result.push_back(temp);
				}
			}
		pProgress.show(100);
		return result;
	}

	/*!
	 * \brief
	 * Zwraca pojemnosc bitmapy przy ustalonej kompresji.
	 * 
	 * \returns
	 * Pojemnosc bitmapy w bajtach.
	 * 
	 * \throws <exception class>
	 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
	 * 
	 */
	uint64 BMP::getCapacity(void) {
		BmpBuf.SetCompr(ReadCompr());
		ReadDabHeader();
		if (IsDab())
			return DabHeader.FreeSpc;
		else 
			return 0;
	}

	/*!
	 * \brief
	 * Ustawia kompresje i zapisuje ja w pliku.
	 * 
	 * \param DabnewCompr
	 * Stopien kompresji.
	 * 
	 * \throws <exception class>
	 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
	 */
	void BMP::setCompression(uint8 DabnewCompr) {
		PreferedCompr = DabnewCompr;
		CreateDabHeader();
		WriteCompr();
		WriteDabHeader();
	}

	/*!
	 * \brief
	 * Pobiera kompresje z pliku.
	 * 
	 * \returns
	 * Wartosc stopnia kompresji.
	 * 
	 * \throws <exception class>
	 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
	 * 
	 */
	const uint8 BMP::getCompression()  {
		BmpBuf.SetCompr(ReadCompr());	
		ReadDabHeader();
		if (IsDab()) 
			return DabHeader.Compr;
		else
			return 0;
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
	/*!
	 * \brief
	 * Sprawdza czy plik jest bitmapa.
	 * 
	 * \param DabFile
	 * Plik do sprawdzenia.
	 * 
	 */
	int isBmp(file *DabFile)  {
		BMP_HEADER BmpHeader;
		FILE_BUFFOR file;
		try {
			file.OpenFile(DabFile->getRealPath().c_str(), std::ios_base::in, BUFFOR::BUF_READONLY);
			if (file.GetBufState() == BUFFOR::BUF_GOOD && file.GetFileSize() >= 54) {
				uint32 i;
				uint8 * p = reinterpret_cast<uint8*> (&BmpHeader);
				// zeby nie doczytywal
				file.SetReadSize(55);
				file.Fill();
				for (i = 0; i < 54; ++i) {
					p[i] = file.GetByte();
				}
				BmpHeader.biUsefulData = BmpHeader.biWidth * BmpHeader.biHeight * 3; // w bajtach
				if (BmpHeader.bfType == 19778 && BmpHeader.bfSize == file.GetFileSize() &&
					BmpHeader.biBitCount == 24 && BmpHeader.biCompression == 0 && BmpHeader.biClrUsed == 0 &&
					// 14 - pozycja biSize w Bmp
					BmpHeader.biSizeImage == file.GetFileSize() - BmpHeader.biSize - 14)
					return 1;
			}
		}
		catch (...) {
		}
		return 0;
	}
}//********************end of namespace bmp********************//