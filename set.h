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

	Settings Header
	set.h

*********************************************************************/

#ifndef _SET_H
#define _SET_H

//#pragma warning (disable: 4786)
#include <map>
#include <string>

class set {
public:	
	union multidat {
		long l;
		int i;
		double d;
		bool b;
		char *s;
	};

	static void load(const std::string &path);
	/* TODO static void save(const std::string); */

	static multidat get(const std::string &number);
	static multidat get(System::String^ number);
	static void change(const std::string &number, const multidat &value);

private:
	static const unsigned int iMAX_LINES = 65535;
	static const unsigned int iMAX_LINE_LEN = 65535;

	static std::map<std::string, multidat> mSet;
};

#endif

/********************************************************************/