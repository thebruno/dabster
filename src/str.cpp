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

	String
	str.cpp

*********************************************************************/

#pragma warning (disable: 4996)

//#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include "stdMcrs.h"
#include "err.h"
#include "str.h"

std::string str::sDelimiter = "\\";
str::path str::sAppPath;

/* Dzieli sciezke na skladowe */
str::path str::splitPath(std::string p, const std::string delimiter) {
	path sPath;
	sPath.dir = sPath.drive = sPath.extension = sPath.fileName = "";
	if (p.length() == 0) { return sPath; }

	/* Ujednolicanie sciezki */
	std::string::size_type pos = 0;
	std::string delim;
	if (delimiter == "\\") { delim = "/"; } else { delim = "\\"; }
	while ((pos = p.find_first_of(delim, pos)) != std::string::npos) {
		p.replace(pos++, 1, delimiter);
	}
		
	/* Wykrywanie rozszerzenia */
	pos = p.find_last_of(".");
	if (pos != std::string::npos) {
		if (pos == 0) {
			/* Sciezka nie moze zaczynac sie od kropki */
			std::vector<std::string> params(1);
			params[0] = p;
			throw err("!STR0", params);
		}

		std::string::size_type dl = p.length() - pos;
		char *temp = new char[dl];
		p.copy(temp, dl - 1, pos + 1);
		temp[dl - 1] = 0;
		sPath.extension = temp;
	}

	/* Wykrywanie nazwy pliku */
	pos = p.find_last_of("\\");
	std::string::size_type dl;
	if (sPath.extension.length() != 0) {
		/* Jesli bylo rozszerzenie */
		if (pos == std::string::npos) { pos = 0; }
		dl = p.length() - pos - sPath.extension.length() - 1;
	} else {
		/* Jesli nie bylo rozszerzenia i jest nazwa pliku */
		if (pos == std::string::npos) { pos = 0; }
		dl = p.length() - pos + 1;
	}
	if (pos != p.length()) {
		char *temp = new char[dl];
		p.copy(temp, dl - 1, pos + 1);
		temp[dl - 1] = 0;
		sPath.fileName = temp;
	}

	/* Wykrywanie dysku */
	pos = p.find_first_of("\\");
	if (pos == std::string::npos) {
		if (sPath.fileName.length() == 0) {
			/* Sciezka musi zawierac nazwe pliku lub dysk */
			std::vector<std::string> params(1);
			params[0] = p;
			throw err("!STR1", params);
		}
	} else {
		if (pos == 0) {
			/* Sciezka nie moze zaczynac sie od (back)slasha */
			std::vector<std::string> params(1);
			params[0] = p;
			throw err("!STR2", params);
		}
		char *temp = new char[pos + 2];
		p.copy(temp, pos + 1, 0);
		temp[pos + 1] = 0;
		sPath.drive = temp;
	}

	/* Wykrywanie sciezki */
	std::string::size_type pos2 = p.find_last_of("\\");
	if (pos != pos2) {
		if (pos2 - pos == 1) {
			/* Sciezka nie moze zawierac podwojnego (back)slasha */
			std::vector<std::string> params(1);
			params[0] = p;
			throw err("!STR3", params);
		}
		char *temp = new char[pos2 - pos + 1];
		p.copy(temp, pos2 - pos, pos + 1);
		temp[pos2 - pos] = 0;
		sPath.dir = temp;
	}

	return sPath;
}

/* Naprawia symbole rozdzielajace w podanej sciezce */
std::string str::fixDelims(std::string path, std::string delimiter) {
	std::string::size_type pos = 0;
	std::string delim;
	if (delimiter == "\\") { delim = "/"; } else { delim = "\\"; }
	while ((pos = path.find_first_of(delim, pos)) != std::string::npos) {
		path.replace(pos++, 1, delimiter);
	}
	return path;
}

/* Ustawia nowy symbol rozdzielajacy */
void str::setDelimiter(const std::string delimiter) {
	sDelimiter = delimiter;
}

/* Zapisuje sciezke do programu */
void str::setAppPath(path appPath) {
	sAppPath = appPath;
}

/* Zwraca sciezke do programu */
str::path str::getAppPath(void) {
	return sAppPath;
}

/* Ustala sciezke do programu */
str::path str::fixAppPath(void) {
	str::path appPath;
	try {
		std::string path = sysStrToCppStr(dabCommandLine::get());

		// Usuwanie cudzyslowow i innych parametrow z linii rozkazow
		std::string::size_type pos;
		if ((pos = path.find_first_of("\"")) != std::string::npos) {
			path.erase(pos, 1);
		}
		if ((pos = path.find_first_of("\"")) != std::string::npos) {
			path.erase(pos);
		}

		str::setDelimiter(str::charToStr(path.at(path.find_first_of(":") + 1)));
		appPath = str::splitPath(path);
		str::setAppPath(appPath);
	}
	catch (...) { /* Niepoprawna sciezka */	throw 1; }
	return appPath;
}

/* Konwertuje systemowego stringa (System::String^) do standardowego (std::string) */
std::string str::sysStrToCppStr(System::String^ sysStr) {
	char *str = (char*)(void*)(dabMarshal::StringToHGlobalAnsi(sysStr));
	std::string ret(str);
	dabMarshal::FreeHGlobal((System::IntPtr)str);
	return ret; 
}

/* Konwertuje string na tablice char */
char* str::strToChar(const std::string s) {
	char* p = new char[s.length() + 1];
	if (!p) {
		/* Alokowanie pamieci nie powiodlo sie */
		std::vector<std::string> params(1);
		params[0] = s;
		throw err("!STR4", params);
	}
	s.copy(p, std::string::npos);
	p[s.length()] = 0;
	return p;
}

/* Konwertuje char na string */
std::string str::charToStr(const char c) {
	char *pc = new char[2];
	if (!pc) {
		/* Alokowanie pamieci nie powiodlo sie */
		std::vector<std::string> params(1);
		params[0] = c;
		throw err("!STR5", params);
	}
	pc[0] = c;
	pc[1] = 0;
	return pc;
}

/* Konwertuje int na string */
long str::stringToLong(const std::string s) {
	const char *cTemp = s.c_str();
	const std::string digits = "0123456789";	/* (48 + i) nie wchodzi w gre - jest nieprzenosne */
	long l = 0;
	int first;

	if ((cTemp[0] == '-') || (cTemp[0] == '+')) { first = 1; } else { first = 0; }
	for (int i = s.length() - 1; i >= first; --i) {
		std::string::size_type pos = 0;
		if ((pos = digits.find(cTemp[i], pos)) == std::string::npos) {
			/* Niepoprawny znak */
			std::vector<std::string> params(1);
			params[0] = cTemp[i];
			throw err("!STR6", params);
		}
		l += pos * static_cast<long>(std::pow(10.0, static_cast<int>(s.length() - i - 1)));
	}
	if (cTemp[0] == '-') { l *= -1; }

	return l;
}

/* Konwertuje int na string */
//std::string str::longToString(const long l) {
//	std::ostringstream s("");
//	s << l;
//	return s.str();
//}

/********************************************************************/