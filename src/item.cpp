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
#include "item.h"

item::item(void) {
	bCanceled = false;
	sName = "";
	sRealPath = "";
}

item::~item(void) {
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

/********************************************************************/