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

	Drive List Header
	drvLst.h

*********************************************************************/

#include <string>

#include "item.h"
#include "drive.h"

#include "sfile.h"
#include "sfolder.h"
#include "sdrive.h"

#include "ftp.h"
#include "mldrv.h"
#include "bmp.h"
#include "aes.h"
#include "bwt.h"
#include "wav.h"
#include "twfsh.h"

#include "stdMcrs.h"
#include "err.h"
#include "oStck.h"

/* UWAGA! Stos "lezy" poziomo, tak ze przypomina sciezke.
Stad nie ma gory ani spodu, tylko lewy i prawy koniec.
Prawy koniec pelni role gory stosu. */

/* Wrzuca nowy element na prawy koniec stosu */
void oStck::push(item* newItem) {
	vStack.push_back(newItem);
}

/* Sciaga element z prawego konca stosu */
item* oStck::pop(void) {
	item *iTemp = vStack[vStack.size() - 1];
	vStack.pop_back();
	return iTemp;
}

/* Usuwa wszystkie elementy ze stosu */
void oStck::clear(void) {
	vStack.clear();
}

/* Zwraca element z lewego konca stosu */
item* oStck::left(void) {
	if (!vStack.size()) return 0;
	return vStack[0];
}

/* Zwraca element z prawego konca stosu */
item* oStck::right(void) {
	if (!vStack.size()) return 0;
	return vStack[vStack.size() - 1];
}

/* Zwraca element o wybranym numerze */
item* oStck::get(int index) {
	if (!(vStack.size() > static_cast<unsigned int>(index))) return 0;
	return vStack[index];
}

/* Zwraca wielkosc stosu */
int oStck::size(void) {
	return vStack.size();
}

int oStck::parent(int index) {
	return 0;
}

/* Zwraca typ obiektu o wybranym numerze */
int oStck::type(int index) {
	if (dynamic_cast< sdrive* >(vStack[index])) return dabSDrive;
	if (dynamic_cast< sfile* >(vStack[index])) return dabSFile;
	if (dynamic_cast< sfolder* >(vStack[index])) return dabSFolder;
	if (dynamic_cast< ftp* >(vStack[index])) return dabFtp;
	if (dynamic_cast< bmp::BMP* >(vStack[index])) return dabBmp;
	if (dynamic_cast< aes* >(vStack[index])) return dabAes;
	if (dynamic_cast< bwt* >(vStack[index])) return dabBwt;
	if (dynamic_cast< mldrv* >(vStack[index])) return dabMldrv;
	if (dynamic_cast< wav* >(vStack[index])) return dabWav;
	if (dynamic_cast< twfsh* >(vStack[index])) return dabTwfsh;

	throw err("!OSK0");
}

std::string oStck::relativePath(int index) {
	return "";
}

/********************************************************************/