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

	Item Header
	item.h

*********************************************************************/

#ifndef _ITEM_H
#define _ITEM_H

#include <string>
#include "prgrss.h"

class item {
public:
	item(void);
	item(prgrss progress);
	virtual ~item(void) = 0;

	void setName(std::string name);
	std::string getName(void);

	void setRealPath(std::string path);
	std::string getRealPath(void);

protected:
	std::string sName;
	std::string sRealPath;
	bool bCanceled;
	prgrss pProgress;
};

#endif

/********************************************************************/