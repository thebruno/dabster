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
#include "file.h"
#include "drive.h"
#include "sfile.h"

#pragma pack(push, BMP_DABSTER_PACKING)

namespace bmp {
	using namespace bmptypes;

	const uint16 BMP_CURRENT_VERSION = 1; /*!< Aktualna wersja. */
	const uint32 BUFFOR_SIZE = 512; /*!< Wielkosc bufora, minimum 512 byteow. */
	const uint32 SYGNATURE = 0x08064b50; /*!< Sygnatura, zapisywana do pliku. */
	const uint32 DABSTER_NAME = 0x35424144; /*!< Nazwa = "DAB5". */
	const uint8 BitSet = 0xFF, BitNSet = 0x00; /*!< Bit ustawiony, bit nieustawiony. */
	
	const uint32 BMP_HEADER_BYTE_SIZE = 54; /*!< Wielkosc naglowka BMP. */
	const uint32 DAB_HEADER_BYTE_SIZE = 21; /*!< Wielkosc naglowka DAB. */
	const uint32 FILE_HEADER_BYTE_SIZE = 12; /*!< Wielkosc naglowka pliku. */
	const uint32 BUF_READING_SIZE = 180; /*!< Bufory moga doczytywac tyle bajtow: BUFFOR_SIZE lub BUF_READING_SIZE bufor. */

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
		uint32 biSize; /*!<  Wielkosc naglowka informacyjnego. Dlugosc stad do koñca naglówka - 40, czasem mniej. */
		uint32 biWidth; /*!<  Szerokosc obrazu w pixelach. */
		uint32 biHeight; /*!<  Wysokosc obrazu w pixelach. */
		uint16 biPlanes; /*!<  Liczba platów, zwykle 0. */
		uint16 biBitCount; /*!<  Liczba bitów na piksel. */
		uint32 biCompression; /*!<  Algorytm kompresji. */
		uint32 biSizeImage; /*!<  Rozmiar samego rysunku, z zerami dopelniajacymi. */
		uint32 biXPelsPerMeter; /*!<  Rozdzielczosc pozioma. */
		uint32 biYPelsPerMeter; /*!<  Rozdzielczosc pionowa. */
		uint32 biClrUsed; /*!<  Liczba kolorów w palecie. */
		uint8 biClrImportant; /*!<  Liczba wa¿nych kolorów w palecie. */
		uint8 biCrlRotation; /*!<  Flaga sygnalizujaca czy ma nastepowaæ rotacja palety. */
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
		uint16 Ver; /*!<  wersja modu³u bitmap. */
		uint8 Compr; /*!<  stopien kompresji, czyli ilosc najmlodszych bitow branych pod uwage. */
		uint16 FilesCount; /*!<  ilosc plikow i folderow. */
		uint32 FilesSize; /*!<  calkowita zajeta przestrzen. */
		uint32 FreeSpc; /*!<  szacowane wolne miejsce, które pozostalo (max wartosc). */
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
		const uint32 GetFileSize() const;
		const BUFFOR_STATE GetBufState() const;
		void BufReset(); 

		void OpenFile(string path, std::ios_base::open_mode DabFileMode, BUFFOR_MODE DabBufMode);

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

		uint32 & GetModReminder();
		uint32 & GetModWidth();

		const uint32 & GetModReminder() const;
		const uint32 & GetModWidth() const;

		void Fill();
		void Flush();

		void BufReset(); // zeruje wszystkie wskazniki bufora
		void SetCompr(uint8 Dabcompr);
		const uint8 GetCompr() const;
		
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

	int isBmp(sfile DabFile);

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
		BMP(sfile DabFile); // otwarcie bitmapy
	};
}
#pragma pack(pop)
#endif

