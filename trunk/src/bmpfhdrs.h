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
	bmpfhdrs.h

*********************************************************************/
#include "bmptypes.h"
#ifndef BMPHDRHEADERFILE
#define BMPHDRHEADERFILE

#pragma pack(push, BMP_DABSTER_PACKING)
namespace bmp {
using namespace bmptypes;

/*!
 * \brief
 * Naglowek bitmapy.
 * 
 * Zawiera naglowek standardowej bitmapy w wersji 5.
 */
struct BMP_HEADER {
public:
	uint16 bfType; /*!< Sygnatura pliku, zawsze przyjmuje wartosc BM. */
	uint32 bfSize; /*!<  Dlugosc calego pliku w bajtach. */
	uint16 bfReserved1; /*!<  Pole zarezerwowane, zwykle ma wartosc 0. */
	uint16 bfReserved2; /*!<  Pole zarezerwowane, zwykle ma wartosc 0. */
	uint32 bfOffBits; /*!<  Pozycja danych obrazowych w pliku. */
	uint32 biSize; /*!<  Wielkosc naglowka informacyjnego. Dlugosc stad do konca naglowka - 40, czasem mniej. */
	uint32 biWidth; /*!<  Szerokosc obrazu w pixelach. */
	uint32 biHeight; /*!<  Wysokosc obrazu w pixelach. */
	uint16 biPlanes; /*!<  Liczba platow, zwykle 0. */
	uint16 biBitCount; /*!<  Liczba bitow na piksel. */
	uint32 biCompression; /*!<  Algorytm kompresji. */
	uint32 biSizeImage; /*!<  Rozmiar samego rysunku, z zerami dopelniajacymi. */
	uint32 biXPelsPerMeter; /*!<  Rozdzielczosc pozioma. */
	uint32 biYPelsPerMeter; /*!<  Rozdzielczosc pionowa. */
	uint32 biClrUsed; /*!<  Liczba kolorow w palecie. */
	uint8 biClrImportant; /*!<  Liczba waznych kolorow w palecie. */
	uint8 biCrlRotation; /*!<  Flaga sygnalizujaca czy ma nastepowac rotacja palety. */
	uint16 biReserved; /*!< Nie uzywane. */
public:
	uint32 biUsefulData; /*!< = biWidth * biHeight * 3 B. */
	BMP_HEADER();
};

/*!
 * \brief
 * Naglowek Dabstera.
 * 
 * Naglowek Dabstera zapisywany do plikow bmp. Wielkosc 21 bajtow
 */
class DAB_HEADER { //  4+4+2+1+2+4+4 = 21 bajtow
private:
	uint32 Sygn; /*!<  sygnatura ustawiana przez program. */
	uint32 DabName; /*!<  "DAB5" - na dysku 4 bajty. */
	uint16 Ver; /*!<  wersja modulu bitmap. */
	uint8 Compr; /*!<  stopien kompresji, czyli ilosc najmlodszych bitow branych pod uwage. */
	uint16 FilesCount; /*!<  ilosc plikow i folderow. */
	uint32 FilesSize; /*!<  calkowita zajeta przestrzen. */
	uint32 FreeSpc; /*!<  szacowane wolne miejsce, ktore pozostalo (max wartosc). */
private: /*!< dodatkowe pola uzywane juz tylko przez klase bmp. */
	uint64 DabHeaderStart; /*!<  poczatek naglowka dabstera	. */
	uint64 FirstFileStart; /*!<  poczatek naglowka pierwszetgo pliku/folderu. */
	uint64 CompressionStart; /*!<  pierszy bit kompresji. */
public:
	DAB_HEADER();
	friend class BMP;
};

/*!
 * \brief
 * Naglowek kazdego pliku lub folderu w bitmapie.
 * 
 * Naglowek kazdego pliku lub folderu w bitmapie. Zapisywany do plikow bmp. Wielkosc 12B + dlugosc nazwy
 */
class FILE_HEADER { // 4 + 4 + 2 + 2 + x = 12 B + x B
private:
	uint32 DataSize; /*!< Ilosc danych (w bajtach) zapisana w pliku. */
	uint32 TimeDate; /*!<  Data i czas spakowania. */
	uint16 Attributes; /*!<  aDtrybuty pliku. */
	uint16 FileNameLen;
	string FileName;  /*!<  Nazwa pliku o zmiennej dlugosci - max 255 znakow!!. */
private: //dodatkowe pola uzywane juz tylko przez klase bmp
	uint64 HeaderStart; /*!<  poczatek naglowka pliku. */
	uint64 DataStart; /*!<  pierwszy bit danych. */
	bool PerformAction; /*!< Uzywane glownie przy usuwaniu pliku z bitmapy. */

	//pozycje plikow w bitmapie
public:
	FILE_HEADER();
	friend class BMP;
};
}
#pragma pack(pop)
#endif 