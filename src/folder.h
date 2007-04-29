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

	Folder Header
	folder.h

*********************************************************************/

#ifndef _FOLDER_H
#define _FOLDER_H

#include <string>
#include <vector>
#include <map>
#include "item.h"

class folder : public virtual item {
public:
	folder(void);
	virtual ~folder(void) = 0;

	virtual void store(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) = 0;
	virtual void extract(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) = 0;

	virtual void del(std::vector< std::map< std::string, std::string > > path) = 0;
	virtual void modify(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) = 0;
	virtual void copyInside(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) = 0;

	virtual std::vector< std::map< std::string, std::string > > getContent(std::string path) = 0;
};

#endif

/********************************************************************/