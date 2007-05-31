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

	Opens Stack Header
	oStck.h

*********************************************************************/

#ifndef _OSTCK_H
#define _OSTCK_H

#include <string>
#include <vector>
#include "item.h"

class oStck {
public:
	~oStck(void);

	void push(item* newItem);
	item* pop(void);
	void clear(void);

	item* left(void);
	item* right(void);

	item* get(int index);
	void set(int index, item* newItem);

	int size(void);
	int parent(int index);

	int type(int index);
	std::string relativePath(int begin, int end);

	static const int iROOT_INDEX = -1;

private:
	std::vector< item* > vStack;
};

#endif

/********************************************************************/