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

	Communicates
	com.cpp

*********************************************************************/

//#pragma warning (disable: 4786)
#include <fstream>
#include <sstream>
#include "err.h"
#include "str.h"
#include "com.h"

std::map<std::string, std::string> com::mCom;
bool com::bLoaded;

/* Domyslny konstruktor */
com::com(void) {
	bLoaded = false;
}

/* Laduje komunikaty z pliku path */
void com::load(const std::string &path) {
	std::ifstream file(path.c_str(), std::ios_base::in);
	if (!file) { throw err(); }
	unsigned int i = 0;
	char temp[iMAX_LINE_LEN];
	while ((i++ < iMAX_LINES) && (file.getline(temp, iMAX_LINE_LEN))) {
		std::string line(temp);
		if ((line.length() > 1) && ((line[0] == '!') || (line[0] == '@'))) {
			std::istringstream l(line, std::ios_base::in);
			l.get(temp, iMAX_LINE_LEN, ' ');
			std::string number(temp);
			if ((number.length() < 2) || (!l) || (!(l.ignore()))) { throw err(); }
			l.getline(temp, iMAX_LINE_LEN);
			std::string text(temp);
			if (text.length() < 2) { throw err(); }
			mCom[number] = text;
		}
	}
	bLoaded = true;
}

/* Zwraca komunikat o podanym numerze */
std::string com::get(const std::string &number) {
	if (bLoaded) {
		return mCom[number];
	} else {
		throw err();
	}
}

/* Zwraca komunikat o podanym numerze */
System::String^ com::get(System::String^ number) {
	if (bLoaded) {
		return gcnew System::String((mCom[str::sysStrToCppStr(number)]).c_str());
	} else {
		throw err();
	}
}

/********************************************************************/