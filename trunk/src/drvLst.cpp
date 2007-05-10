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

	Drive List
	drvLst.cpp

*********************************************************************/

#include <vector>
#include "drive.h"
#include "sdrive.h"
#include "ftp.h"
#include "mldrv.h"
#include "stdMcrs.h"
#include "err.h"
#include "drvLst.h"

/* Dodaje nowy dysk do listy */
void drvLst::add(drive* newDrive) {
}

/* Usuwa dysk o wybranym numerze */
void drvLst::remove(int index) {
}

/* Zwraca dysk o wybranym numerze */
drive* drvLst::get(int index) {
	return &sdrive();
}

/* Zwraca typ dysku o wybranym numerze */
int drvLst::type(int index) {
	if (dynamic_cast< sdrive* >(vDrives[index])) return dabSDrive;
	if (dynamic_cast< ftp* >(vDrives[index])) return dabFtp;
	if (dynamic_cast< sdrive* >(vDrives[index])) return dabMldrv;

	throw err("!DLT0");
}

/* Odœwie¿a informacje o dyskach twardych */
void drvLst::refresh(void) {
	for (unsigned int i = 0; i < vDrives.size(); i++) {
		//if (vDrives
	}
}

/********************************************************************/