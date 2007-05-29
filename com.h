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

	Communicates Header
	com.h

*********************************************************************/

#ifndef _COM_H
#define _COM_H

//#pragma warning (disable: 4786)
#include <string>
#include <map>

class com {
public:
	com(void);

	static void load(const std::string& path);
	static std::string get(const std::string& number);
#ifndef _POSIX_SOURCE
	static System::String^ get(System::String^ number);
#endif

private:
	static const unsigned int iMAX_LINES = 65535;
	static const unsigned int iMAX_LINE_LEN = 65535;

	static bool bLoaded;
	static std::map<std::string, std::string> mCom;
};

#endif

/********************************************************************/
