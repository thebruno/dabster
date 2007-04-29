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

	const uint16 BMP_CURRENT_VERSION = 1;
	const uint32 BUFFOR_SIZE = 200; //4 * 1024; // minimum 512 byteow
	const uint32 SYGNATURE = 0x08064b50;
	const uint32 DABSTER_NAME = 0x35424144;
	// przeniesc do buffora
	const uint8 BitSet = 0xFF, BitNSet = 0x00;
	
	const uint32 BMP_HEADER_BYTE_SIZE = 54;/**< wielkosc naglowka w bytach*/
	const uint32 DAB_HEADER_BYTE_SIZE = 21;
	const uint32 FILE_HEADER_BYTE_SIZE = 12; //+ nazwa 
	const uint32 BUF_READING_SIZE = 180;

	struct BMP_HEADER {
	public:
		uint16 bfType; // Sygnatura pliku, zawsze przyjmuje wartosc BM
		uint32 bfSize; // Dlugosc calego pliku w bajtach
		uint16 bfReserved1; // Pole zarezerwowane, zwykle ma wartosc 0
		uint16 bfReserved2; // Pole zarezerwowane, zwykle ma wartosc 0
		uint32 bfOffBits; // Pozycja danych obrazowych w pliku
		uint32 biSize; // Wielkosc naglowka informacyjnego. Dlugosc stad do koñca naglówka - 40, czasem mniej
		uint32 biWidth; // Szerokosc obrazu w pixelach
		uint32 biHeight; // Wysokosc obrazu w pixelach
		uint16 biPlanes; // Liczba platów, zwykle 0
		uint16 biBitCount; // Liczba bitów na piksel
		uint32 biCompression; // Algorytm kompresji
		uint32 biSizeImage; // Rozmiar samego rysunku, z zerami dopelniajacymi
		uint32 biXPelsPerMeter; // Rozdzielczosc pozioma
		uint32 biYPelsPerMeter; // Rozdzielczosc pionowa
		uint32 biClrUsed; // Liczba kolorów w palecie
		uint8 biClrImportant; // Liczba wa¿nych kolorów w palecie 
		uint8 biCrlRotation; // Flaga sygnalizujaca czy ma nastepowaæ rotacja palety
		uint16 biReserved; // 
	public:
		uint32 biUsefulData; // = biWidth * biHeight * 3 B
		BMP_HEADER();
	};

	class DAB_HEADER { // 4+2+4+4+2+4 = 20bajtow
	private:
		uint32 Sygn; // sygnatura ustawiana przez program
		uint32 DabName; // "DAB5" - na dysku 4 bajty
		uint16 Ver; // wersja modu³u bitmap
		uint8 Compr; // stopien kompresji, czyli ilosc najmlodszych bitow branych pod uwage
		uint16 FilesCount; // ilosc plikow i folderow
		uint32 FilesSize; // calkowita zajeta przestrzen
		uint32 FreeSpc; // szacowane wolne miejsce, które pozostalo (max wartosc)
	private: //dodatkowe pola uzywane juz tylko przez klase bmp
		uint64 DabHeaderStart; // poczatek naglowka dabstera	
		uint64 FirstFileStart; // poczatek naglowka pierwszetgo pliku/folderu
		uint64 CompressionStart; // pierszy bit kompresji
	public:
		DAB_HEADER();
		~DAB_HEADER();
		friend class BMP;
		/*
		uint32 FilesCountStart; // pozycja w bmp gdzie jest zapisana ilosc plikow w bitmapie
		uint32 ComprStart; // przechowuje pozycje 3 bajtow, na ktorych jest przechowywana kompresja

		*/
	};

	class FILE_HEADER { // 4 + 4 + 2 + x = 10 B + x B
	private:
		uint32 DataSize;
		uint32 TimeDate; // data i czas spakowania
		uint16 Attributes; // atrybuty pliku
		uint16 FileNameLen;
		string FileName;  // x - zmienna wielkosc max 255 znakow!!
	private: //dodatkowe pola uzywane juz tylko przez klase bmp
		uint64 HeaderStart; // poczatek naglowka pliku
		uint64 DataStart; // pierwszy bit danych
		bool PerformAction;

		//pozycje plikow w bitmapie
	public:
		FILE_HEADER();
		~FILE_HEADER();
		friend class BMP;
	};

	class BUFFOR {
	public:
		enum BUFFOR_STATE {BUF_GOOD=0, BUF_FAIL=1, BUF_BAD=2, BUF_EOF=4, BUF_EMPTY =8, BUF_FULL = 16};
		enum BUFFOR_MODE {BUF_READONLY=0, BUF_WRITEONLY=1, BUF_READ_WRITE=2, BUF_TRANSFER = 4};

		static const uint8 One [8]; // ustawiony jeden bit
		static const uint8 ExOne [8]; // ustawione wszystkie POZA jednym bitem
	protected:
		fstream File;
		uint8 Buf[BUFFOR_SIZE]; // unsigned char buffor

		uint32 BufSize; // wielkosc bufora
		uint64 BufBitSize ; // wielkosc bufora w bitach
		uint32 FileSize; // wielkosc otwartego pliku typu std::fstream

		uint32 DataCount; // ilosc danych
		uint64 BitDataCount;

		uint32 BufBytePosp; // pozycja wskaznika w buforze put
		uint32 BufBitPosp; // pozycja bitu w bajcie wskazywanym przez bufBytePos put
		uint32 BufBytePosg; // pozycja wskaznika w buforze get
		uint32 BufBitPosg; // pozycja bitu w bajcie wskazywanym przez bufBytePos get

		BUFFOR_MODE BufMode;
		BUFFOR_STATE BufState;
		uint32 ReadSize; // tyle bajtow wczytujemy do bufora w operajach odczytu
	public:
		const uint32 GetFileSize() const;
		const BUFFOR_STATE GetBufState() const;
		void BufReset(); //zeruje wszystkie wskazniki bufora

		void OpenFile(string path, std::ios_base::open_mode DabFileMode, BUFFOR_MODE DabBufMode);

		// do przegladniecia
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
		//void Flush(uint32 bufStart, uint32 count); // zapisuje count znakow do pliku od pozybji bufStart
		void SetReadSize(uint32 DabNewSize = BUFFOR_SIZE);


		//tu cos zmienic
		void SetMode(BUFFOR_MODE DabbufMode);
		void CloseFile();

		BUFFOR();
		BUFFOR(string path);
		~BUFFOR();
		//friend class BMP;
	};

	class BMP_BUFFOR: public BUFFOR {
	public:
		enum SEEKWAY {BEGINING, FORWARD, BACKWARD};
	private:
		BMP_HEADER* BmpHeader;
		uint32 BytePosg; // pozycja w pliku glowicy odczytujacej (w bajtach)
		uint32 BytePosp; // pozycja w pliku glowicy zapisujacej (w bajtach)
		uint64 BitPosg; // pozycja w pliku glowicy odczytujacej (w bitach)
		uint64 BitPosp; // pozycja w pliku glowicy zapisujacej (w bitach)

		uint32 ModReminder; // = BmpWidth % 4 = tylma zerami jest dopelniany wiersz aby dzielil sie przez 4
		uint32 ModWidth; // = 3 * bmpwidth + reminder = dlugosc wiersza, liczac dopelniajace zera

		uint8 Compr; // kompresja
		uint8 Seek; // = 8 - compr
	private:
		void BufByteSeekg(uint32 DabNewPos){}
		void BufByteSeekp(uint32 DabNewPos){}
		void BufBitSeekg(uint32 DabNewPos){}
		void BufBitSeekp(uint32 DabNewPos){}
	public:
		uint64 FileSizeInBmp(uint64 DabSize,  uint64 DabCurPos, bool DabDirection = true); // konwertuje wielkosc pliku w bitach na wielkosc tego pliku w bitach w bitmapie
		// bierze pod uwage kompresje, oraz ilosc zer wyrownujacych

		// czy to odzwierciedla pozycje po przeczytaniu czy nie??:/
		// co gdyby zakomentowac naglowek funkcji??
		// jak to sie ma do dziedziczenia?

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
		~BMP_BUFFOR();
		//friend class BMP;
	};

	class FILE_BUFFOR: public BUFFOR {
	public:
		FILE_BUFFOR();
		~FILE_BUFFOR();
	};

	int isBmp(sfile DabFile);

	class BMP: public drive, public file {
	private: //data
		BMP_HEADER BmpHeader;
		DAB_HEADER DabHeader;
		std::vector<FILE_HEADER *> FilesHeaders;
		uint8 PreferedCompr;
		BMP_BUFFOR BmpBuf;
		FILE_BUFFOR FileBuf;
	private: //functions
		uint16 AttribToBytes(smap Dabsmap);
		smap BytesToAttrib(uint16 DabBytes);

		uint32 UTCToBytes(string Dabs);
		string BytesToUTC(uint32 DabBytes);

		bool IsBmp();
		bool IsDab();

		void ReadBmpHeader();
		/** \brief tworz naglowek
		* funkcja tworzaca naglowek dabstera
		*/
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
		//void format(uint8 DabCompr, bool DabFirstTime = false); // kasuje wszystko
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

