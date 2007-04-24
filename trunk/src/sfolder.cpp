/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Simple Folder
	sfolder.cpp

*************************************************/

#include <string>
#include <vector>
#include <map>
#include "sfolder.h"
#include "str.h"
#include "stdMcrs.h"

sfolder::~sfolder(void) {
}

/* Kopiuje do folderu obiekty podane w parametrze */
void sfolder::store(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
}

/* Kopiuje obiekty podane w parametrze poza FB */
void sfolder::extract(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
}

/* Usuwa obiekty z folderu */
void sfolder::del(std::vector< std::map< std::string, std::string > > path) {
}

/* Przenosi obiekty wewnatrz DFB */
void sfolder::modify(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
}

/* Kopiuje obiekty wewnatrz DFB */
void sfolder::copyInside(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
}

/* Zwraca zawartosc folderu z parametru */
std::vector< std::map< std::string, std::string > > sfolder::getContent(std::string path) {
	std::vector< std::map< std::string, std::string > > content(0);

	System::String^ fullPath = _ioPath::Combine(_toSysStr(_realPath.c_str()), _toSysStr(path.c_str())); 
	
	//Foldery
	cli::array<System::String^>^ subDirs = _dir::GetDirectories(fullPath);
	//Pliki
	int filesStartAt = subDirs->Length;
	cli::array<System::String^>^ subFiles = _dir::GetFiles(fullPath);
	cli::array<System::String^>::Resize(subDirs, subDirs->Length + subFiles->Length);
	subFiles->CopyTo(subDirs, filesStartAt);

	/* Przygotowywanie informacji */
	for (int i=0; i < subDirs->Length; i++) {
		std::map< std::string, std::string > dirEntry;
		_fileSysInfo^ fSysInfo;
		std::string sTemp;

		if (i < filesStartAt) {
			fSysInfo = gcnew _dirInfo(subDirs[i]->ToString());
		} else {
			fSysInfo = gcnew _fileInfo(subDirs[i]->ToString());
		}

		//Nazwa i sciezka rzeczywista
		sTemp = str::sysStrToCppStr(_ioPath::GetDirectoryName(subDirs[i]->ToString()));
		std::string::size_type pos = sTemp.length() - 1;
		if (sTemp[pos] != '\\') sTemp.push_back('\\');
		dirEntry["name"] = str::sysStrToCppStr(_ioPath::GetFileName(subDirs[i]->ToString()));
		dirEntry["realPath"] = sTemp;

		//Data utworzenia
		sTemp = str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Day.ToString()) + ".";
		sTemp += str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Month.ToString()) + ".";
		sTemp += str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Year.ToString()) + " ";
		sTemp += str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Hour.ToString()) + ":";
		sTemp += str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Minute.ToString()) + ":";
		sTemp += str::sysStrToCppStr(fSysInfo->CreationTimeUtc.Second.ToString());
		dirEntry["creationTimeUtc"] = sTemp;

		//Data modyfikacji
		sTemp = str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Day.ToString()) + ".";
		sTemp += str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Month.ToString()) + ".";
		sTemp += str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Year.ToString()) + " ";
		sTemp += str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Hour.ToString()) + ":";
		sTemp += str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Minute.ToString()) + ":";
		sTemp += str::sysStrToCppStr(fSysInfo->LastWriteTimeUtc.Second.ToString());
		dirEntry["lastWriteTimeUtc"] = sTemp;

		//Data ostatniego dostepu
		sTemp = str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Day.ToString()) + ".";
		sTemp += str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Month.ToString()) + ".";
		sTemp += str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Year.ToString()) + " ";
		sTemp += str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Hour.ToString()) + ":";
		sTemp += str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Minute.ToString()) + ":";
		sTemp += str::sysStrToCppStr(fSysInfo->LastAccessTimeUtc.Second.ToString());
		dirEntry["lastAccessTimeUtc"] = sTemp;

		//Tymczasowy
		if (((fSysInfo->Attributes) & (_fAtr::Temporary)) == _fAtr::Temporary) {
			dirEntry["atrTemporary"] = "true";
		} else {
			dirEntry["atrTemporary"] = "false";
		}

		//Systemowy
		if (((fSysInfo->Attributes) & (_fAtr::System)) == _fAtr::System) {
			dirEntry["atrSystem"] = "true";
		} else {
			dirEntry["atrSystem"] = "false";
		}

		//Archiwum
		if (((fSysInfo->Attributes) & (_fAtr::Archive)) == _fAtr::Archive) {
			dirEntry["atrArchive"] = "true";
		} else {
			dirEntry["atrArchive"] = "false";
		}

		//Skompresowany
		if (((fSysInfo->Attributes) & (_fAtr::Compressed)) == _fAtr::Compressed) {
			dirEntry["atrCompressed"] = "true";
		} else {
			dirEntry["atrCompressed"] = "false";
		}

		//Zaszyfrowany
		if (((fSysInfo->Attributes) & (_fAtr::Encrypted)) == _fAtr::Encrypted) {
			dirEntry["atrEncrypted"] = "true";
		} else {
			dirEntry["atrEncrypted"] = "false";
		}

		//Ukryty
		if (((fSysInfo->Attributes) & (_fAtr::Hidden)) == _fAtr::Hidden) {
			dirEntry["atrHidden"] = "true";
		} else {
			dirEntry["atrHidden"] = "false";
		}

		//Nie indeksowany
		if (((fSysInfo->Attributes) & (_fAtr::NotContentIndexed)) == _fAtr::NotContentIndexed) {
			dirEntry["atrNotContentIndexed"] = "true";
		} else {
			dirEntry["atrNotContentIndexed"] = "false";
		}

		//Offline
		if (((fSysInfo->Attributes) & (_fAtr::Offline)) == _fAtr::Offline) {
			dirEntry["atrOffline"] = "true";
		} else {
			dirEntry["atrOffline"] = "false";
		}

		//ReadOnly
		if (((fSysInfo->Attributes) & (_fAtr::ReadOnly)) == _fAtr::ReadOnly) {
			dirEntry["atrReadOnly"] = "true";
		} else {
			dirEntry["atrReadOnly"] = "false";
		}

		//Folder
		if (((fSysInfo->Attributes) & (_fAtr::Directory)) == _fAtr::Directory) {
			dirEntry["atrDirectory"] = "true";
		} else {
			dirEntry["atrDirectory"] = "false";
		}

		delete fSysInfo;
		_fileInfo^ fInfo = gcnew _fileInfo(subDirs[i]->ToString());

		//Rozmiar
		if (i < filesStartAt) {
			dirEntry["length"] = "unknown";
		} else {
			dirEntry["length"] = str::sysStrToCppStr(fInfo->Length.ToString());
		}

		delete fInfo;

		content.push_back(dirEntry);
	}

	return content;
}

/************************************************/