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

	Tab Header
	tab.h

*********************************************************************/

#ifndef _TAB_H
#define _TAB_H

#include <string>
#include <vector>
#include <map>

#include "stdMcrs.h"
#include "oStck.h"

namespace dabster {

public ref class tab {
public:
	tab(dabTabControl^ ownerTabControl);

	void resize(void);

	void open(std::string path);

	void store(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest);
	void extract(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest);

	void del(std::vector< std::map< std::string, std::string > > path);
	void modify(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest);
	void copyInside(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest);

	std::vector< std::map< std::string, std::string > > getContent(void);
	unsigned long long getCapacity(void);

protected:
	~tab(void);

private:
	dabTabControl^ owner;
	
	dabTabPage^ tpgTab;
	dabGlacialList^ gltList;
	dabPictureBox^ picExit;
	dabPictureBox^ picHeaderUnderline;

	oStck* opensStack;
};

}

#endif

/********************************************************************/