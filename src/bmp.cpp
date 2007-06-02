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
#include "err.h"
//********************namespace bmp********************//
namespace bmp {
//********************class BMP********************//
/*!
 * \brief
 * Konstruktor klasy BMP.
 * 
 * \param DabFile
 * Rzeczywisty plik.
 * 
 * \throws err()
 * Rzuca wyjatek w przypadku bledu odczytu zapisu lub b³êdnego pliku bmp.
 * 
 * Otwarcie bitmapy i odczyt naglowka.
 */
/*BMP::BMP(file *DabFile):BmpBuf(&BmpHeader) {
	PreferedCompr = 1;
	BmpBuf.BufReset();
	BmpBuf.OpenFile(DabFile->getRealPath().c_str(), std::ios_base::in | std::ios_base::out, BUFFOR::BUF_READONLY);
	BmpBuf.SetReadSize(BUF_READING_SIZE);
	BmpBuf.Fill();
	ReadBmpHeader();
	if (!IsBmp()) {
		std::vector<string> params(1);
		params[0] = DabFile->getRealPath().c_str();
		throw err("!BMP1", params);
	}
}*/
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
 * Kasuje pliki w bmp.
 * 
 * \param Dabpath
 * Przechowuje nazwy plikow do skasowania.
 * 
 * \throws err()
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 * Kasowanie mozna przerwac tylko do momentu zaznaczenia plikow do skasowania. Nie mozna cofnac kasowania juz zamazanego pliku, ani zostawic niespojnych danych w bmp.
 */
void BMP::del(vmstring Dabpath) {
	pProgress.show(0);
	int PrgrsTask = 0, PrgrsFin = 0;
	// odczyt naglowka bmp
	ReadBmpHeader();
	// czy odczytano naglowki?
	BmpBuf.SetCompr(ReadCompr());
	ReadDabHeader();

	if (!IsDab()) {
		std::vector<string> params(1);
		throw err("!BMP4", params);
	}
	DeleteAllHeaders();
	ReadAllHeaders();
	vmstring::iterator i;
	std::vector<FILE_HEADER *>::iterator j, k, l;
	uint32 FilesToDel = 0, FilesToMove = 0;
	int64 WritingPos, ReadingPos, z;
	int32 count;

	DAB_HEADER DabHeaderCopy = DabHeader;
	pProgress.show(2);
	// zaznaczenie plikow do usuniecia
	for (i = Dabpath.begin(); i != Dabpath.end(); ++i) {
		for (j = FilesHeaders.begin(); j != FilesHeaders.end() ; ++j) {
			// plik do usuniecia:
			if ((*i)["relativePath"] == (*j)->FileName) {
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
	if (FilesToDel != Dabpath.size()){
		std::vector<string> params(1);
		throw err("!BMP0", params);
	}

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
 * \throws err()
 * Rzuca wyjatek w przypadku bledu odczytu zapisu oraz gdy zabraklo miejsca.
 * 
 * Przerwanie moze nastapic co jeden spakowany plik.
 * 
 */
void BMP::store(vmstring Dabsrc, vmstring Dabdest) {
	pProgress.show(0);
	int PrgrsFin = 0, PrgrsTask = (int)Dabsrc.size() + 1;
	// odczyt naglowka bmp
	ReadBmpHeader();
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
	for (i = Dabsrc.begin(), j = Dabdest.begin(); i != Dabsrc.end() && j != Dabdest.end() && !bCanceled; ++i, ++j) {

		// pasek postepu
		pProgress.show((100 * PrgrsFin++) / PrgrsTask);

		header = new FILE_HEADER;
		header->HeaderStart = WritingPos;
		// src jest w rzeczywistosci, dest w BMP
		header->FileName = (*j)["relativePath"];
		header->FileNameLen = uint16(header->FileName.size());
		// sprawdzenie czy sie zmiesci naglowek + nazwa:
		if (DabHeader.FreeSpc < header->FileNameLen + FILE_HEADER_BYTE_SIZE) {
			delete header;

			std::vector<string> params(1);
			throw err("!BMP3", params);
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
		if ((*j)["atrDirectory"] == "true") {
			// folder nie ma danych!
			header->DataSize = 0;
		}
		// pakujemy plik
		else {
			FileBuf.BufReset();
			FileBuf.SetReadSize(BUFFOR_SIZE);
			FileBuf.OpenFile((*i)["realPath"].c_str(), std::ios_base::in, BUFFOR::BUF_READONLY);
			FileBuf.Fill();
			header->DataSize = FileBuf.GetFileSize();
			// czy plik z danymi sie zmiesci?
			if (DabHeader.FreeSpc < header->FileNameLen + FILE_HEADER_BYTE_SIZE + header->DataSize) {
				delete header;

				std::vector<string> params(1);
				throw err("!BMP3", params);
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
 * \throws err()
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 * Przerwanie moze nastapic co jeden wypakowany plik.
 * 
 */
void BMP::extract(vmstring Dabsrc, vmstring Dabdest) {
	pProgress.show(0);
	int PrgrsFin = 0, PrgrsTask = (int) Dabsrc.size() + 1;
	// odczyt naglowka bmp
	ReadBmpHeader();
	BmpBuf.SetCompr(ReadCompr());
	ReadDabHeader();
	if (!IsDab()) {
		std::vector<string> params(1);
		throw err("!BMP4", params);
	}
	DeleteAllHeaders();
	ReadAllHeaders();
	uint32 iter;
	vmstring::iterator i, j;
	std::vector<FILE_HEADER *>::iterator k;
	BmpBuf.SetMode(BUFFOR::BUF_READONLY);
	BmpBuf.SetReadSize(BUFFOR_SIZE);

	for (i = Dabsrc.begin(), j = Dabdest.begin();	i != Dabsrc.end() && j != Dabdest.end()&& !bCanceled; ++i, ++j) {
		pProgress.show((100 * PrgrsFin++) / PrgrsTask);
		for (k = FilesHeaders.begin(); k != FilesHeaders.end() ; ++k) {
		// src jest w BMP, dest w rzeczywistosci
			if ((*k)->FileName == (*i)["relativePath"]) {
				FileBuf.BufReset();
				FileBuf.SetReadSize(BUFFOR_SIZE);
				FileBuf.OpenFile((*j)["realPath"].c_str(),std::ios_base::out, BUFFOR::BUF_WRITEONLY);
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
 * \throws err()
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 * Przerwanie moze nastapic co jeden skopiowany plik.
 * 
 */
void BMP::copyInside(vmstring DabSrc, vmstring DabDest) {
	pProgress.show(0);
	int PrgrsFin = 0, PrgrsTask = (int) DabSrc.size() + 1;
	// odczyt naglowka bmp
	ReadBmpHeader();
	// czy odczytano naglowki?
	BmpBuf.SetCompr(ReadCompr());
	ReadDabHeader();
	if (!IsDab()) {
		std::vector<string> params(1);
		throw err("!BMP4", params);
	}
	DeleteAllHeaders();
	ReadAllHeaders();
	// nie ma zadnego naglowka lub nieprawidlowy parametr
	if (!FilesHeaders.size() && DabSrc.size() ) {
		std::vector<string> params(1);
		throw err("!BMP0", params);
	}

	FILE_HEADER * h;
	uint64 WritingPos = 0, ReadingPos = 0, temp;
	uint32 number;
	std::vector<FILE_HEADER *>::iterator i, j;
	vmstring::iterator k, m;

	for (k = DabSrc.begin(), m = DabDest.begin();
		k != DabSrc.end() && m != DabDest.end()&& !bCanceled; ++k, ++m) {
		pProgress.show((100 * PrgrsFin++) / PrgrsTask);
		for (i = FilesHeaders.begin(), number = 0 ; i != FilesHeaders.end(); ++i, ++number) {
			if ((*k)["relativePath"] == (*i)->FileName) {
				h = new FILE_HEADER;
				h->Attributes = (*i)->Attributes;
				h->DataSize = (*i)->DataSize;
				h->TimeDate = (*i)->TimeDate;
				h->FileName  = (*m)["relativePath"];
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
					std::vector<string> params(1);
					throw err("!BMP3", params);
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
 * \throws err()
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 * Mozna przerwac tylko do momentu ustalenia nowego polozenia plikow, potem wszystko trzeba juz poprzesuwac najpierw tworzymy nowe naglowki z nowymi nazwami, potem obliczamy polozenie wszystkich naglowkow i wykonujemy operacje.
 * 
 */
void BMP::modify(vmstring DabOldPath, vmstring DabNewPath) {
	pProgress.show(0);
	int PrgrsFin = 0, PrgrsTask = (int) DabOldPath.size() + 2;
	// odczyt naglowka bmp
	ReadBmpHeader();
	// czy odczytano naglowki?
	BmpBuf.SetCompr(ReadCompr());
	ReadDabHeader();
	if (!IsDab()) {
		std::vector<string> params(1);
		throw err("!BMP4", params);
	}
	DeleteAllHeaders();
	ReadAllHeaders();
	// nie ma zadnego naglowka 
	if (!FilesHeaders.size() && DabOldPath.size() ) {
		std::vector<string> params(1);
		throw err("!BMP0", params);
	}
	// struktura pomocnicza przy przenoszeniu
	struct R_W_Pos {
		uint64 ReadingPos;
		uint64 WritingPos;
		R_W_Pos(): ReadingPos(0), WritingPos(0) {}
	} Pos;
	
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
			// modyfukujemy nazwe pliku
			if ((*k)["relativePath"] == (*i)->FileName) {
				h->FileName  = (*m)["relativePath"];
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
		std::vector<string> params(1);
		throw err("!BMP4", params);
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
 * \throws err()
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 */
vmstring BMP::getContent(string Dabpath) {
	// odczytaj dane
	ReadBmpHeader();
	BmpBuf.SetCompr(ReadCompr());
	ReadDabHeader();
	if (!IsDab())
		return vmstring();
	
	size_t k;
	// odswiez naglowki
	DeleteAllHeaders();
	ReadAllHeaders();
	// nazwa do testowania
	string TestFileName;
	vmstring result;
	smap temp;
	std::vector<FILE_HEADER*>::iterator i;
	std::stringstream s;
	pProgress.show(0);
	int PrgrsFin = 0, PrgrsTask = (int) FilesHeaders.size() + 1;
	// sprawdzaj po kolei wszystkie naglowki
	if (Dabpath.size() && Dabpath[Dabpath.size() - 1] != '\\') 
		Dabpath.push_back('\\');
	for (i = FilesHeaders.begin(); i!= FilesHeaders.end(); ++i) {
		pProgress.show((100 * PrgrsFin++) / PrgrsTask);

		// sprawdzenie czy pocz¹tki œcie¿ek siê zgadzaj¹?
		// jestli tak to rozpatruj napis skrocony z przodu o czesc wspolna
		// jesli poczatki nie sa zgodne idz dalej

		// sprawdzic czy "" == "" ?
		if ((*i)->FileName.substr(0, Dabpath.size()) == Dabpath) {
			// kopuje podstring za znakiem '\\' do konca
			TestFileName = (*i)->FileName.substr(Dabpath.size()) ;
		
			// dodaj nazwe do wektora wynikowego jesli:
			// rozpatrywana nazwa nie zawiera znaku '\\' (pliki)
			// albo zawiera jeden znak '\\' na koncu nazwy (foldery)
			if (TestFileName != "" && ((k = TestFileName.find_first_of('\\')) == std::string::npos || 
				k == TestFileName.size() - 1)) {

				temp.clear();
				temp = BytesToAttrib((*i)->Attributes);
				if (TestFileName.size() > 1 &&k == TestFileName.size() - 1)
					temp["name"] = TestFileName.substr(0,TestFileName.size() - 1);
				else
					temp["name"] = TestFileName;
				temp["realPath"] = "";
				temp["relativePath"] = TestFileName;
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
 * \throws err()
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
 * \throws err()
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 */
void BMP::setCompression(uint8 DabnewCompr) {
	PreferedCompr = DabnewCompr;
	// odczyt naglowka bmp
	ReadBmpHeader();
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
 * \throws err()
 * Rzuca wyjatek w przypadku bledu odczytu zapisu.
 * 
 */
const uint8 BMP::getCompression()  {
	// odczyt naglowka bmp
	ReadBmpHeader();
	BmpBuf.SetCompr(ReadCompr());	
	ReadDabHeader();
	if (IsDab()) 
		return DabHeader.Compr;
	else
		return 0;
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