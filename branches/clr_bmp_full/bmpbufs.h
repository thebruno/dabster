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
	bmpbufs.h

*********************************************************************/
#include "bmpfhdrs.h"
#ifndef BMPBUFSHEADERFILE
#define BMPBUFSHEADERFILE
namespace bmp {
using namespace bmptypes;
using namespace bmp;
/*!
 * \brief
 * Klasa bufor zarzadzajaca operacjami na plikach.
 * 
 * Klasa bufor ktora dostarczajaca operacji na plikach, wraz z ich buforowaniem. Umozliwia odczyt, zapis po jednym bicie do pliku oraz wygodne poruszanie sie po nim.
 */
class BUFFOR {
public:
	enum BUFFOR_STATE {BUF_GOOD=0, BUF_FAIL=1, BUF_BAD=2, BUF_EOF=4, BUF_EMPTY =8, BUF_FULL = 16}; /*!< Deklaracja dozwolonych stanow, w ktorych moze znajdowac sie bufor. */
	enum BUFFOR_MODE {BUF_READONLY=0, BUF_WRITEONLY=1, BUF_READ_WRITE=2, BUF_TRANSFER = 4}; /*!< Deklaracja dozwolonych trybow, w ktorych moze pracowac bufor. */

	static const uint8 One [8]; /*!<  Ustawiony jeden bit. */
	static const uint8 ExOne [8]; /*!<  Ustawione wszystkie POZA jednym bitem. */
protected:
	fstream File; /*!< Plik. */
	uint8 Buf[BUFFOR_SIZE]; /*!< Unsigned char buffor. */

	uint32 BufSize; /*!< Wielkosc bufora. */
	uint64 BufBitSize ; /*!< Wielkosc bufora w bitach. */
	uint32 FileSize; /*!< Wielkosc otwartego pliku typu std::fstream. */

	uint32 DataCount; /*!< Ilosc danych w bajtach. */
	uint64 BitDataCount; /*!< Ilosc danych w bitach. */

	uint32 BufBytePosp; /*!< Pozycja wskaznika w buforze put. */
	uint32 BufBitPosp; /*!< Pozycja bitu w bajcie wskazywanym przez bufBytePos put. */
	uint32 BufBytePosg; /*!< Pozycja wskaznika w buforze get. */
	uint32 BufBitPosg; /*!< Pozycja bitu w bajcie wskazywanym przez bufBytePos get. */

	BUFFOR_MODE BufMode; /*!< Tryb pracy buforwa. */
	BUFFOR_STATE BufState; /*!< Aktualny stan. */
	uint32 ReadSize; /*!<  tyle bajtow wczytujemy do bufora w operajach odczytu. */
public:
/*!
 * \brief
 * Pobiera wielkosc pliku.
 * 
 * \returns
 * Zwraca wielkosc pliku w bajtach.
 * 
 */
	const uint32 GetFileSize() const {
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
	const BUFFOR_STATE GetBufState() const {
		return BufState;
	}

	void BufReset(); 
	void OpenFile(string Dabpath, std::ios_base::open_mode DabFileMode, BUFFOR_MODE DabBufMode);

	uint8 GetBit();
	void PutBit(uint8 DabBit);
	uint8 GetByte();
	void PutByte(uint8 DabByte);

	void BufByteSeekg(uint32 DabNewPos);
	void BufByteSeekp(uint32 DabNewPos);
	void BufBitSeekg(uint32 DabNewPos);		
	void BufBitSeekp(uint32 DabNewPos);

	const uint32 BufByteTellg() const;
	const uint32 BufByteTellp() const;
	const uint32 BufBitTellg() const;
	const uint32 BufBitTellp() const;

	void Fill();
	void Flush();

	void SetReadSize(uint32 DabNewSize = BUFFOR_SIZE);
	void SetMode(BUFFOR_MODE DabbufMode);
	void CloseFile();

	BUFFOR();
	BUFFOR(string path);
	~BUFFOR();
};

/*!
 * \brief
 * Klasa dziedziczaca po klasie BUFFOR.
 * 
 * Rozszerza swoja klase podstawowa BUFFOR o specyficzne operacje wykonywane na bitmapach.
 */
class BMP_BUFFOR: public BUFFOR {
public:
	enum SEEKWAY {BEGINING, FORWARD, BACKWARD};
private:
	BMP_HEADER* BmpHeader;
	uint32 BytePosg; /*!< Pozycja w pliku glowicy odczytujacej (w bajtach). */
	uint32 BytePosp; /*!< Pozycja w pliku glowicy zapisujacej (w bajtach). */
	uint64 BitPosg; /*!< Pozycja w pliku glowicy odczytujacej (w bitach). */
	uint64 BitPosp; /*!< Pozycja w pliku glowicy zapisujacej (w bitach). */

	uint32 ModReminder; /*!<  = BmpWidth % 4 = tylma zerami jest dopelniany wiersz aby dzielil sie przez 4. */
	uint32 ModWidth; /*!<  = 3 * bmpwidth + reminder = dlugosc wiersza, liczac dopelniajace zera. */

	uint8 Compr; /*!<  kompresja. */
	uint8 Seek; /*!<  = 8 - compr. */
private:
	void BufByteSeekg(uint32 DabNewPos){}
	void BufByteSeekp(uint32 DabNewPos){}
	void BufBitSeekg(uint32 DabNewPos){}
	void BufBitSeekp(uint32 DabNewPos){}
public:
	uint64 FileSizeInBmp(uint64 DabSize,  uint64 DabCurPos);
	
	void BitSeekg(uint64 DabnewPos, SEEKWAY DabSeekWay = BEGINING);		
	void BitSeekp(uint64 DabnewPos, SEEKWAY DabSeekWay = BEGINING);

	const uint32 ByteTellg() const;
	const uint32 ByteTellp() const;
	const uint64 BitTellg() const;
	const uint64 BitTellp() const;

/*!
 * \brief
 * Pobiera wartosc aktualnej reszty modulo.
 * 
 * \returns
 * Wartosc aktualnej reszty modulo.
 *
 * Reszta modulo = BmpWidth % 4 - ilosc zer dopelniajacych w bitmapie. Wartosci od 0 do 3. Udostepnia na zawnatrz pole ModReminder.
 */
	uint32 & GetModReminder(){
		return ModReminder;
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
	const uint32 & GetModReminder() const {
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
	uint32 & GetModWidth(){
		return ModWidth;
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
	const uint32 & GetModWidth() const {
		return ModWidth;
	}
	void Fill();
	void Flush();

	void BufReset(); // zeruje wszystkie wskazniki bufora
	void SetCompr(uint8 Dabcompr);
/*!
 * \brief
 * Pobiera wartosc aktualnej kompresji.
 * 
 * \returns
 * Wartosc aktualnej komrpesji.
 * 
 */
	const uint8 GetCompr() const {
		return Compr;
	}
	uint8 GetBit();
	void PutBit(uint8 DabBit);

	BMP_BUFFOR();
	BMP_BUFFOR(BMP_HEADER* DabBmpHeader);
};

/*!
 * \brief
 * Klasa dziedziczaca po klasie BUFFOR.
 * 
 * Uzytecznosc taka jak dla klasy podstawowej.
 */
class FILE_BUFFOR: public BUFFOR {
};

}
#endif 