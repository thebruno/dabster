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

	Glacial ListView
	Copyright Glacial Components Software 2004
	http://www.glacialcomponents.com/

	Tab
	tab.cpp

*********************************************************************/

#include <string>
#include <vector>
#include <map>

#include "stdMcrs.h"
#include "oStck.h"
#include "drvLst.h"
#include "str.h"
#include "com.h"
#include "err.h"
#include "tab.h"

dabster::tab::tab(dabTabControl^ ownerTabControl) {
	owner = ownerTabControl;
	tpgTab = gcnew dabTabPage();
	gltList = gcnew dabGlacialList();
	picExit = gcnew dabPictureBox();
	picHeaderUnderline = gcnew dabPictureBox();

	opensStack = new oStck;

	tpgTab->BackColor = dabWhite;
	tpgTab->Location = dabDPoint(4, 22);
	tpgTab->Name = L"tabPage";
	tpgTab->Padding = System::Windows::Forms::Padding(3);
	tpgTab->Width = owner->Width - 8;
	tpgTab->Height = owner->Height - 26;
	tpgTab->Text = com::get("@TPG0");

	gltList->AllowColumnResize = true;
	gltList->AllowMultiselect = true;
	gltList->AlternatingColors = false;
	gltList->BackColor = dabWhite;
	gltList->ControlStyle = dabStyleSuperFlat;
	gltList->FullRowSelect = true;
	gltList->GridLineStyle = dabGridNone;
	gltList->HeaderHeight = 20;
	gltList->HeaderVisible = true;
	gltList->HeaderWordWrap = false;
	gltList->HotColumnTracking = false;
	gltList->HotItemTracking = false;
	gltList->HoverEvents = false;
	gltList->ImageList = nullptr;
	gltList->ItemHeight = 18;
	gltList->ItemWordWrap = false;
	gltList->Location = dabDPoint(0, 0);
	gltList->Name = L"glacialList";
	gltList->Selectable = true;
	gltList->SelectedTextColor = dabBlack;
	gltList->SelectionColor = dabLightBlue;
	gltList->ShowBorder = false;
	gltList->ShowFocusRect = false;
	gltList->Width = tpgTab->Width;
	gltList->Height = tpgTab->Height;
	gltList->SortType = dabQuickSort;
	gltList->SuperFlatHeaderColor = dabWhite;
	gltList->Text = L"glacialList";

	owner->Controls->Add(tpgTab);
	tpgTab->Controls->Add(gltList);
	tpgTab->Controls->Add(picExit);
	tpgTab->Controls->Add(picHeaderUnderline);
}

dabster::tab::~tab(void) {
	if (opensStack) delete opensStack;
}

/* Dostosowuje rozmiary zakladki */
void dabster::tab::resize(void) {
}

/* Otwiera nowy katalog */
void dabster::tab::open(std::string path) {
	std::vector< std::string > p = str::itemNames(path);
	unsigned int level = 0;

	if (p.size() == 0) {
		/* Sciezka niepoprawna */
		std::vector<std::string> params(1);
		params[0] = path;
		throw err("!TAB0", params);
	}

	/* Porownywanie ze stara sciezka */
	while ((p.size() > level) && 
		   (static_cast<unsigned int>(opensStack->size()) > level) && 
		   (opensStack->get(level)->getName() == p[level])) {
		++level;
	}

	/* Usuwanie rozniacej sie czesci stosu */
	for (int i = level; i < opensStack->size(); i++) {
		opensStack->pop();
	}

	/* Dodawanie dysku */
	if (level == 0) {
		int index;
		if ((index = drvLst::find(p[0])) == drvLst::iNOT_FOUND) {
			/* Nie znaleziono partycji */
			std::vector<std::string> params(1);
			params[0] = p[0];
			throw err("!TAB1", params);
		}

		drive *dTemp = drvLst::get(index);
		dTemp->attach();
		opensStack->push(dTemp);
	}

	/* Dodawanie reszty sciezki */
}

/* Zapisuje pliki i foldery w aktualnym katalogu */
void dabster::tab::store(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
}

void dabster::tab::extract(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
}

void dabster::tab::del(std::vector< std::map< std::string, std::string > > path) {
}

void dabster::tab::modify(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
}

void dabster::tab::copyInside(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
}

std::vector< std::map< std::string, std::string > > dabster::tab::getContent(void) {
	return std::vector< std::map< std::string, std::string > >(0);
}

unsigned long long dabster::tab::getCapacity(void) {
	return 0;
}

/********************************************************************/