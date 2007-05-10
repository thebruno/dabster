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
#include "oStck.h"

item* oStck::get(int index) {
	return static_cast< item* >(0);
}

int oStck::type(int index) {
	return 0;
}

int oStck::parent(int index) {
	return 0;
}

std::string oStck::relativePath(int index) {
	return "";
}

item* oStck::left(void) {
	return static_cast< item* >(0);
}

item* oStck::right(void) {
	return static_cast< item* >(0);
}

int oStck::size(void) {
	return 0;
}

void oStck::push(item* newItem) {
}

item* oStck::pop(void) {
	return static_cast< item* >(0);
}

/********************************************************************/