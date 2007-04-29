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

	Simple Folder
	sfolder.cpp

*********************************************************************/

#include <string>
#include <vector>
#include <map>
#include "sfolder.h"
#include "err.h"
#include "str.h"
#include "stdMcrs.h"

static const int iDIR_CREATING_TIME = 10;	//Przyjmujemy czas tworzenia folderu jak czas kopiowania 10B pliku

sfolder::~sfolder(void) {
}

/* Kopiuje do folderu obiekty podane w parametrze */
void sfolder::store(std::vector< std::map< std::string, std::string > > src, 
					std::vector< std::map< std::string, std::string > > dest) {
	long len = 0, made = 0;

	/* TODO: Zapisywanie atrybutow plikow/folderow */

	/* Szacowanie calkowitego czasu kopiowania */
	try {
		pProgress.show(0);
		for (unsigned int i = 0; i < src.size(); i++) {
			if (src[i][dabKeyAtrDirectory] == dabFalse) {
				//dla pliku
				len += str::stringToLong(src[i][dabKeyLength]);
			} else {
				//dla folderu
				len += iDIR_CREATING_TIME;
			}
		}
	}
	catch (...) {}
	if (!len) len = 1;

	/* Kopiowanie */
	for (unsigned int i = 0; i < src.size(); i++) {
		if (bCanceled) return;

		if (src[i][dabKeyAtrDirectory] == dabFalse) {
			/* Zapisywanie pliku */
			made += str::stringToLong(src[i][dabKeyLength]);

			std::string sPath1 = src[i][dabKeyRealPath];
			if (sPath1[sPath1.size() - 1] != '\\') sPath1.push_back('\\');
			sPath1 += src[i][dabKeyName];
			sPath1 = str::fixDelims(sPath1);

			std::string sPath2 = sRealPath;
			if (sPath2[sPath2.size() - 1] != '\\') sPath2.push_back('\\');
			sPath2 += dest[i][dabKeyRelativePath];
			if (sPath2[sPath2.size() - 1] != '\\') sPath2.push_back('\\');
			sPath2 += dest[i][dabKeyName];
			sPath2 = str::fixDelims(sPath2);

			try {
				dabFile::Copy(dabToSysStr(sPath1.c_str()), dabToSysStr(sPath2.c_str()), true);
			}
			catch (...) {
				std::vector<std::string> params(1);
				params[0] = sPath1;
				throw err("!SFL2", params);
			}
		} else {
			/* Tworzenie folderu */
			made += iDIR_CREATING_TIME;

			std::string sPath = sRealPath;
			if (sPath[sPath.size() - 1] != '\\') sPath.push_back('\\');
			sPath += dest[i][dabKeyRelativePath];
			if (sPath[sPath.size() - 1] != '\\') sPath.push_back('\\');
			sPath += dest[i][dabKeyName];
			sPath = str::fixDelims(sPath);

			try {
				if (!dabDir::Exists(dabToSysStr(sPath.c_str()))) {
					dabDir::CreateDirectory(dabToSysStr(sPath.c_str()));
				}
			}
			catch (...) {
				std::vector<std::string> params(1);
				params[0] = sPath;
				throw err("!SFL3", params);
			}
		}

		pProgress.show(made * 100 / len);
	}

	pProgress.show(100);
}

/* Kopiuje obiekty podane w parametrze poza FB */
void sfolder::extract(std::vector< std::map< std::string, std::string > > src, 
					  std::vector< std::map< std::string, std::string > > dest) {
	long len = 0, made = 0;
	dabFileInfo^ fInfo;

	/* TODO: Zapisywanie atrybutow plikow/folderow */

	/* Szacowanie calkowitego czasu kopiowania */
	try {
		pProgress.show(0);
		for (unsigned int i = 0; i < src.size(); i++) {
			if (src[i][dabKeyAtrDirectory] == dabFalse) {
				//dla pliku
				std::string sPath = sRealPath;
				if (sPath[sPath.size() - 1] != '\\') sPath.push_back('\\');
				sPath += src[i][dabKeyRelativePath];
				if (sPath[sPath.size() - 1] != '\\') sPath.push_back('\\');
				sPath += src[i][dabKeyName];
				sPath = str::fixDelims(sPath);

				fInfo = gcnew dabFileInfo(dabToSysStr(sPath.c_str()));
				len += str::stringToLong(str::sysStrToCppStr(fInfo->Length.ToString()));
				delete fInfo;
			} else {
				//dla foderu
				len += iDIR_CREATING_TIME;
			}
		}
	}
	catch (...) {}
	if (!len) len = 1;

	/* Kopiowanie */
	for (unsigned int i = 0; i < src.size(); i++) {
		if (bCanceled) return;

		if (src[i][dabKeyAtrDirectory] == dabFalse) {
			/* Zapisywanie pliku */
			std::string sPath1 = sRealPath;
			if (sPath1[sPath1.size() - 1] != '\\') sPath1.push_back('\\');
			sPath1 += src[i][dabKeyRelativePath];
			if (sPath1[sPath1.size() - 1] != '\\') sPath1.push_back('\\');
			sPath1 += src[i][dabKeyName];
			sPath1 = str::fixDelims(sPath1);

			std::string sPath2 = dest[i][dabKeyRealPath];
			if (sPath2[sPath2.size() - 1] != '\\') sPath2.push_back('\\');
			sPath2 += dest[i][dabKeyName];
			sPath2 = str::fixDelims(sPath2);

			fInfo = gcnew dabFileInfo(dabToSysStr(sPath1.c_str()));
			made += str::stringToLong(str::sysStrToCppStr(fInfo->Length.ToString()));
			delete fInfo;

			try {
				dabFile::Copy(dabToSysStr(sPath1.c_str()), dabToSysStr(sPath2.c_str()), true);
			}
			catch (...) {
				std::vector<std::string> params(1);
				params[0] = sPath1;
				throw err("!SFL2", params);
			}
		} else {
			/* Tworzenie folderu */
			made += iDIR_CREATING_TIME;

			std::string sPath = dest[i][dabKeyRealPath];
			if (sPath[sPath.size() - 1] != '\\') sPath.push_back('\\');
			sPath += dest[i][dabKeyName];
			sPath = str::fixDelims(sPath);

			try {
				if (!dabDir::Exists(dabToSysStr(sPath.c_str()))) {
					dabDir::CreateDirectory(dabToSysStr(sPath.c_str()));
				}
			}
			catch (...) {
				std::vector<std::string> params(1);
				params[0] = sPath;
				throw err("!SFL3", params);
			}
		}

		pProgress.show(made * 100 / len);
	}

	pProgress.show(100);
}

/* Usuwa obiekty z folderu */
void sfolder::del(std::vector< std::map< std::string, std::string > > path) {
	long len, made = 0;

	/* Szacowanie calkowitego czasu usuwania */
	pProgress.show(0);
	len = iDIR_CREATING_TIME * path.size();

	/* Usuwanie */
	for (unsigned int i = 0; i < path.size(); i++) {
		if (bCanceled) return;

		made += iDIR_CREATING_TIME;

		std::string sPath = sRealPath;
		if (sPath[sPath.size() - 1] != '\\') sPath.push_back('\\');
		sPath += path[i][dabKeyRelativePath];
		if (sPath[sPath.size() - 1] != '\\') sPath.push_back('\\');
		sPath += path[i][dabKeyName];
		sPath = str::fixDelims(sPath);

		try {
			if (path[i][dabKeyAtrDirectory] == dabFalse) {
				/* Usuwanie pliku */
				dabFile::Delete(dabToSysStr(sPath.c_str()));
			} else {
				/* Usuwanie folderu */
				if (dabDir::Exists(dabToSysStr(sPath.c_str()))) {
					dabDir::Delete(dabToSysStr(sPath.c_str()), true);
				}
			}

		}
		catch (...) {
			std::vector<std::string> params(1);
			params[0] = sPath;
			throw err("!SFL4", params);
		}

		pProgress.show(made * 100 / len);
	}

	pProgress.show(100);
}

/* Przenosi obiekty wewnatrz DFB */
void sfolder::modify(std::vector< std::map< std::string, std::string > > src, 
					 std::vector< std::map< std::string, std::string > > dest) {
}

/* Kopiuje obiekty wewnatrz DFB */
void sfolder::copyInside(std::vector< std::map< std::string, std::string > > src, 
						 std::vector< std::map< std::string, std::string > > dest) {
}

/* Zwraca zawartosc folderu z parametru */
std::vector< std::map< std::string, std::string > > sfolder::getContent(std::string path) {
	std::vector< std::map< std::string, std::string > > content(0);

	/* TODO: Pobieranie informacji o atrybutach plikow/folderow */
	
	/* Przygotowywanie listy plikow i folderow */
	System::String^ fullPath;
	cli::array<System::String^>^ subDirs;
	int filesStartAt;
	pProgress.show(0);
	try {
		fullPath = dabIoPath::Combine(dabToSysStr(sRealPath.c_str()), dabToSysStr(path.c_str())); 
		
		//Foldery
		subDirs = dabDir::GetDirectories(fullPath);
		//Pliki
		filesStartAt = subDirs->Length;
		cli::array<System::String^>^ subFiles = dabDir::GetFiles(fullPath);
		cli::array<System::String^>::Resize(subDirs, subDirs->Length + subFiles->Length);
		subFiles->CopyTo(subDirs, filesStartAt);
	}
	catch (...) {
		std::vector<std::string> params(1);
		params[0] = str::sysStrToCppStr(fullPath);
		throw err("!SFL0", params);
	}

	int i;
	try {
		/* Przygotowywanie informacji */
		for (i = 0; i < subDirs->Length; i++) {
			std::map< std::string, std::string > dirEntry;
			dabFileSysInfo^ fSysInfo;
			std::string sTemp;

			if (bCanceled) return std::vector< std::map< std::string, std::string > > (0);

			if (i < filesStartAt) {
				fSysInfo = gcnew dabDirInfo(subDirs[i]->ToString());
			} else {
				fSysInfo = gcnew dabFileInfo(subDirs[i]->ToString());
			}

			//Nazwa i sciezka rzeczywista
			sTemp = str::sysStrToCppStr(dabIoPath::GetDirectoryName(subDirs[i]->ToString()));
			std::string::size_type pos = sTemp.length() - 1;
			if (sTemp[pos] != '\\') sTemp.push_back('\\');
			sTemp = str::fixDelims(sTemp);
			dirEntry[dabKeyName] = str::sysStrToCppStr(dabIoPath::GetFileName(subDirs[i]->ToString()));
			dirEntry[dabKeyRealPath] = sTemp;

			//Data utworzenia
			sTemp = str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Day.ToString()) + ".";
			sTemp += str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Month.ToString()) + ".";
			sTemp += str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Year.ToString()) + " ";
			sTemp += str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Hour.ToString()) + ":";
			sTemp += str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Minute.ToString()) + ":";
			sTemp += str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Second.ToString());
			dirEntry[dabKeyCreationTimeUtc] = sTemp;

			//Data modyfikacji
			sTemp = str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Day.ToString()) + ".";
			sTemp += str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Month.ToString()) + ".";
			sTemp += str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Year.ToString()) + " ";
			sTemp += str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Hour.ToString()) + ":";
			sTemp += str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Minute.ToString()) + ":";
			sTemp += str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Second.ToString());
			dirEntry[dabKeyLastWriteTimeUtc] = sTemp;

			//Data ostatniego dostepu
			sTemp = str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Day.ToString()) + ".";
			sTemp += str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Month.ToString()) + ".";
			sTemp += str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Year.ToString()) + " ";
			sTemp += str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Hour.ToString()) + ":";
			sTemp += str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Minute.ToString()) + ":";
			sTemp += str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Second.ToString());
			dirEntry[dabKeyLastAccessTimeUtc] = sTemp;

			//Tymczasowy
			if (((fSysInfo->Attributes) & (dabFAtr::Temporary)) == dabFAtr::Temporary) {
				dirEntry[dabKeyAtrTemporary] = dabTrue;
			} else {
				dirEntry[dabKeyAtrTemporary] = dabFalse;
			}

			//Systemowy
			if (((fSysInfo->Attributes) & (dabFAtr::System)) == dabFAtr::System) {
				dirEntry[dabKeyAtrSystem] = dabTrue;
			} else {
				dirEntry[dabKeyAtrSystem] = dabFalse;
			}

			//Archiwum
			if (((fSysInfo->Attributes) & (dabFAtr::Archive)) == dabFAtr::Archive) {
				dirEntry[dabKeyAtrArchive] = dabTrue;
			} else {
				dirEntry[dabKeyAtrArchive] = dabFalse;
			}

			//Skompresowany
			if (((fSysInfo->Attributes) & (dabFAtr::Compressed)) == dabFAtr::Compressed) {
				dirEntry[dabKeyAtrCompressed] = dabTrue;
			} else {
				dirEntry[dabKeyAtrCompressed] = dabFalse;
			}

			//Zaszyfrowany
			if (((fSysInfo->Attributes) & (dabFAtr::Encrypted)) == dabFAtr::Encrypted) {
				dirEntry[dabKeyAtrEncrypted] = dabTrue;
			} else {
				dirEntry[dabKeyAtrEncrypted] = dabFalse;
			}

			//Ukryty
			if (((fSysInfo->Attributes) & (dabFAtr::Hidden)) == dabFAtr::Hidden) {
				dirEntry[dabKeyAtrHidden] = dabTrue;
			} else {
				dirEntry[dabKeyAtrHidden] = dabFalse;
			}

			//Nie indeksowany
			if (((fSysInfo->Attributes) & (dabFAtr::NotContentIndexed)) == dabFAtr::NotContentIndexed) {
				dirEntry[dabKeyAtrNotContentIndexed] = dabTrue;
			} else {
				dirEntry[dabKeyAtrNotContentIndexed] = dabFalse;
			}

			//Offline
			if (((fSysInfo->Attributes) & (dabFAtr::Offline)) == dabFAtr::Offline) {
				dirEntry[dabKeyAtrOffline] = dabTrue;
			} else {
				dirEntry[dabKeyAtrOffline] = dabFalse;
			}

			//ReadOnly
			if (((fSysInfo->Attributes) & (dabFAtr::ReadOnly)) == dabFAtr::ReadOnly) {
				dirEntry[dabKeyAtrReadOnly] = dabTrue;
			} else {
				dirEntry[dabKeyAtrReadOnly] = dabFalse;
			}

			//Folder
			if (((fSysInfo->Attributes) & (dabFAtr::Directory)) == dabFAtr::Directory) {
				dirEntry[dabKeyAtrDirectory] = dabTrue;
			} else {
				dirEntry[dabKeyAtrDirectory] = dabFalse;
			}

			delete fSysInfo;
			dabFileInfo^ fInfo = gcnew dabFileInfo(subDirs[i]->ToString());

			//Rozmiar
			if (i < filesStartAt) {
				dirEntry[dabKeyLength] = dabUnknown;	/* TODO: Zliczanie wielkosci folderow i plikow wewnatrz */
			} else {
				dirEntry[dabKeyLength] = str::sysStrToCppStr(fInfo->Length.ToString());
			}

			delete fInfo;

			content.push_back(dirEntry);

			pProgress.show(i * 100 / subDirs->Length);
		}
	}
	catch (...) {
		std::vector<std::string> params(1);
		params[0] = str::sysStrToCppStr(subDirs[i]->ToString());
		throw err("!SFL1", params);
	}

	pProgress.show(100);
	return content;
}

/********************************************************************/