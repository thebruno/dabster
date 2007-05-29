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

	String Header
	str.h

*********************************************************************/

#ifndef _STR_H
#define _STR_H

#include <string>
#include <vector>

class str {
public:
	struct path {
		std::string drive;
		std::string dir;
		std::string fileName;
		std::string extension;
	};

	static path splitPath(std::string p, const std::string delimiter = sDelimiter);
	static std::vector< std::string > itemNames(std::string p);
	static std::string fixDelims(std::string path, std::string delimiter = sDelimiter);
	static void setDelimiter(const std::string delimiter);
	static void setAppPath(path appPath);
	static path getAppPath(void);
	static str::path fixAppPath(void);
#ifndef _POSIX_SOURCE
	static std::string sysStrToCppStr(System::String^ sysStr);
#endif
	static char* strToChar(const std::string s);
	static std::string charToStr(const char c);
	static long stringToLong(const std::string s);

private:
	static std::string sDelimiter;
	static path sAppPath;
};

#endif

/********************************************************************/
