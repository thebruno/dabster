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

#ifndef _DRV_LST_H
#define _DRV_LST_H

#include <vector>
#include <string>
#include "drive.h"

class drvLst {
public:
	~drvLst(void);

	void add(drive* newDrive);
	void remove(int index);
	void clear(void);

	drive* get(int index);

	int type(int index);
	void refresh(void);
	int find(std::string path);

	static const int iNOT_FOUND = -1;

private:
	std::vector< drive* > vDrives;
};

#endif

/********************************************************************/