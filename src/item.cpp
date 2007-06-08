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

	Item
	item.cpp

*********************************************************************/

#include <string>

#include "file.h"
#include "drive.h"

#include "sfile.h"
#include "sdrive.h"
#include "sfolder.h"

#include "bmp.h"
#include "aes.h"
#include "bwt.h"
#include "wav.h"
#include "twfsh.h"

#include "stdMcrs.h"
#include "str.h"
#include "err.h"
#include "item.h"

item::item(void) : bCanceled(false), sName(""), sRealPath(""), refCount(0) {
}

item::item(prgrss progress) : 
	pProgress(progress), bCanceled(false), 
	sName(""), sRealPath(""), refCount(0) {
}

item::~item(void) {
	/* TODO: Sprawdzanie czy plik jest tymczasowy i czy mozna go usunac
	         (najlepiej jakas flaga w item, wskazujaca czy plik zostal
			 stworzony przez wypakowanie) */
	if ((sRealPath.length() > 0) && (sRealPath[sRealPath.length() - 1] != '\\')) {
		str::path p = str::splitPath(sRealPath);
		if ((p.extension == "tmp") && (p.fileName.length() >= 3) 
			&& (p.fileName.substr(0, 3) == "tmp")) {
			try {
				dabFile::Delete(gcnew System::String(sRealPath.c_str()));
			}
			catch (...) { /* Jak sie nie udalo - trudno ;] */ }
		}
	}
}

/* Ustawia nazwe */
void item::setName(std::string name) {
	sName = name;
}

/* Pobiera nazwe */
std::string item::getName(void) {
	return sName;
}

/* Ustawia rzeczywista sciezke */
void item::setRealPath(std::string path) {
	sRealPath = path;
}

/* Pobiera rzeczywista sciezke */
std::string item::getRealPath(void) {
	return sRealPath;
}

/* Zwieksza licznik wskazan */
void item::attach(void) {
	++refCount;
}

/* Zmniejsza licznik wskazan i w razie potrzeby usuwa obiekt */
void item::detach(void) {
	if (--refCount <= 0) {
		delete this;
	}
}

/* Zwraca wartosc licznika wskazan */
int item::getRefCount(void) {
	return refCount;
}

/* Sprawdza rzeczywisty typ obiektu (nie uwzglednia dyskow!) */
int item::realType(item* itemToCheck) {
	if (!itemToCheck) /* Nie przekazano obiektu */ throw err("!TAB2");

	if (file *f = dynamic_cast< file* >(itemToCheck)) {
		if (bmp::isBmp(f)) return dabBmp;
		return dabSFile;
	} else if (dynamic_cast< sdrive* >(itemToCheck)) {
		return dabSDrive;
	//} else if (dynamic_cast< ftp* >(itemToCheck)) {
	//	return dabFtp;
	//} else if (dynamic_cast< mldrv* >(itemToCheck)) {
	//	return dabMlDrv;
	} else {
		return dabSFolder;
	}
}

/* Konwertuje obiekt do wskazanego typu szczegolowego (nie uwzglednia dyskow!) */
item* item::convert(item *objectToConvert, int type) {
	if (!objectToConvert) /* Nie przekazano obiektu */ throw err("!TAB2");
	
	item *rt;
	switch (type) {
		case dabBmp: rt = new bmp::BMP(); break;
		case dabSFolder: rt = new sfolder(); break;
		case dabSFile: rt = new sfile(); break;
	}

	rt->setName(objectToConvert->getName());
	rt->setRealPath(objectToConvert->getRealPath());

	return rt;
}

/********************************************************************/