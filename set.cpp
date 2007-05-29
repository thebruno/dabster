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

	Settings
	set.cpp

*********************************************************************/

//#pragma warning (disable: 4786)
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "str.h"
#include "err.h"
#include "set.h"

std::map<std::string, set::multidat> set::mSet;

/* Wczytuje ustawienia z pliku path */
void set::load(const std::string &path) {
	std::ifstream file(path.c_str(), std::ios_base::in);
	if (!file) {
		/* Plik nie istnieje */
		std::vector<std::string> params(1);
		params[0] = path;
		throw err("!SET0", params);
	}
	unsigned int i = 0;
	char temp[iMAX_LINE_LEN];
	while ((i++ < iMAX_LINES) && (file.getline(temp, iMAX_LINE_LEN))) {
		std::string line(temp);
		if ((line.length() >= 2) && (line[0] == '$')) {
			/* Ladowanie identyfikatora */
			std::istringstream l(line, std::ios_base::in);
			l.get(temp, iMAX_LINE_LEN, ' ');
			std::string number(temp);
			if ((number.length() < 3) || (!l) || (!(l.ignore()))) {
				/* Za krotki identyfikator */
				std::vector<std::string> params(2);
				params[0] = path;
				params[1] = line;
				throw err("!SET1", params);
			}
			if (!l) {
				/* Brak tekstu za identyfikatorem */
				std::vector<std::string> params(2);
				params[0] = path;
				params[1] = line;
				throw err("!SET2", params);
			}

			/* Ladowanie wartosci */
			l.get(temp, iMAX_LINE_LEN);
			std::string text(temp);
			if (text.length() < 1) {
				/* Tekst za identyfikatorem niepoprawny */
				std::vector<std::string> params(2);
				params[0] = path;
				params[1] = line;
				throw err("!SET3", params);
			}

			/* Zapisywanie */
			std::istringstream t(text, std::ios_base::in);
			switch (number[1]) {
			case 'l':
				t >> mSet[number].l;
				break;
			case 'i':
				t >> mSet[number].i;
				break;
			case 'd':
				t >> mSet[number].d;
				break;
			case 'b':
				if ((text == "true") || (text == "on") || (text == "yes")) {
					mSet[number].b = true;
				} else if ((text == "false") || (text == "off") || (text == "no")) {
					mSet[number].b = false;
				} else {
					/* Tekst za identyfikatorem niepoprawny */
					std::vector<std::string> params(2);
					params[0] = path;
					params[1] = line;
					throw err("!SET3", params);
				}
				break;
			case 's':
				try { mSet[number].s = str::strToChar(text); }
				catch (err) {
					/* Nie mozna przekonwertowac */
					std::vector<std::string> params(2);
					params[0] = path;
					params[1] = line;
					throw err("!SET4", params);
				}
			}
		}
	}

	/* Interpretacja wczytanych danych */
	if (std::string(set::get("$sTMP_DIR").s) == "default") {
		set::multidat tempPath;
		str::path appPath = str::getAppPath();
		tempPath.s = str::strToChar(appPath.drive + appPath.dir + "temp\\");
		set::change("$sTMP_DIR", tempPath);
	}
}

/* Zwraca wartosc ustawienia o numerze number */
set::multidat set::get(const std::string &number) {
	return mSet[number];
}

/* Zwraca wartosc ustawienia o numerze number */
set::multidat set::get(System::String^ number) {
	return mSet[str::sysStrToCppStr(number)];
}

/* Zmienia wartosc ustawienia o numerze number na value */
void set::change(const std::string &number, const set::multidat &value) {
	mSet[number] = value;
}

/********************************************************************/