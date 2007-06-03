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

	Tab Panel Header
	tabPnl.h

*********************************************************************/

#ifndef _TAB_PNL_H
#define _TAB_PNL_H

#include "tab.h"
#include "stdMcrs.h"

namespace dabster {

public ref class tabPnl {
public:
	tabPnl(dabForm^ ownerForm);

	void addTab(void);
	void removeTab(int index);
	int size(void);
	tab^ getTab(int index);

	void Width(int w);
	void Height(int h);
	void Top(int t);
	void Left(int l);

	void resize();

protected:
	~tabPnl(void);

private:
	dabForm^ owner;
	dabComponentResourceManager^ resources;

	cli::array< tab^ >^ tabPages;
	dabComboBox^ cboAddress;
	dabPanel^ pnlBackground;
	dabTabControl^ tbcPanel;
	
	int width, height;
	int top, left;
};

}

#endif

/********************************************************************/