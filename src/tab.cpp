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

#include "folder.h"
#include "item.h"

#include "sfile.h"
#include "sfolder.h"

#include "stdMcrs.h"
#include "frmMain.h"
#include "oStck.h"
#include "drvLst.h"
#include "str.h"
#include "set.h"
#include "com.h"
#include "err.h"
#include "tab.h"

static const int iICON_COLUMN = 0;
static const int iNAME_COLUMN = 1;
static const int iSIZE_COLUMN = 2;
static const int iTYPE_COLUMN = 3;
static const int iMODIFIED_COLUMN = 4;
static const int iHORIZONTAL_MARGIN = 8;
static const int iVERTICAL_MARGIN = 26;

dabster::tab::tab(dabTabControl^ ownerTabControl) {
	owner = ownerTabControl;
	tpgTab = gcnew dabTabPage();
	gltList = gcnew dabGlacialList();
	picExit = gcnew dabPictureBox();
	picHeaderUnderline = gcnew dabPictureBox();
	resources = gcnew dabComponentResourceManager(frmMain::typeid);

	opensStack = new oStck;

	tpgTab->BackColor = dabWhite;
	tpgTab->Location = dabDPoint(4, 22);
	tpgTab->Name = L"tabPage";
	tpgTab->Padding = System::Windows::Forms::Padding(3);
	tpgTab->Text = com::get("@TPG0");
	tpgTab->Width = owner->Width - iHORIZONTAL_MARGIN;
	tpgTab->Height = owner->Height - iVERTICAL_MARGIN;

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
	gltList->Columns->Add("", 20);
	gltList->Columns->Add(com::get("@GLT0"), 250);
	gltList->Columns->Add(com::get("@GLT1"), 60);
	gltList->Columns->Add(com::get("@GLT2"), 60);
	gltList->Columns->Add(com::get("@GLT3"), 110);
	gltList->DoubleClick += gcnew System::EventHandler(this, &tab::gltList_DoubleClick);
	gltList->Click += gcnew System::EventHandler(this, &tab::gltList_Click);
	tpgTab->Controls->Add(gltList);

	picHeaderUnderline = gcnew System::Windows::Forms::PictureBox();
	picHeaderUnderline->Location = System::Drawing::Point(0, 18);
	picHeaderUnderline->BackColor = System::Drawing::Color::DarkGray;
	gltList->Controls->Add(picHeaderUnderline);

	tpgTab->Controls->Add(picExit);
	owner->TabPages->Add(tpgTab);

	resize();
}

dabster::tab::~tab(void) {
	owner->TabPages->Remove(tpgTab);
	delete gltList;
	delete picExit;
	delete picHeaderUnderline;
	delete resources;
	delete tpgTab;

	if (opensStack) delete opensStack;
	if (content) delete content;
}

/* Dostosowuje rozmiary zakladki */
void dabster::tab::resize(void) {
	tpgTab->Width = owner->Width - iHORIZONTAL_MARGIN;
	tpgTab->Height = owner->Height - iVERTICAL_MARGIN;

	gltList->Width = tpgTab->Width;
	gltList->Height = tpgTab->Height;

	picHeaderUnderline->Size = System::Drawing::Size(tpgTab->Width, 2);

	for (int i = 1; i < gltList->Columns->Count; i++) {
		gltList->Columns[i]->Width = static_cast< long >(gltList->Width) \
			* set::get("$iCOLUMN" + i + "_WIDTH").i / 1000;
	}
}

/* Odswierza widok zakladki */
void dabster::tab::refresh(void) {
	int lFB = opensStack->parent(opensStack->size());	// ostatni FB
	std::string relativePath = "";
	if (lFB + 1 < opensStack->size()) {
		relativePath = opensStack->relativePath(lFB + 1, opensStack->size() - 1);
	}

	/* Konwersja ostatniego FB do rzeczywistego typu */
	int rType = item::realType(opensStack->get(lFB));
	if (opensStack->type(lFB) != rType) {
		opensStack->set(lFB, item::convert(opensStack->get(lFB), rType));
	}

	folder *lastFB = dynamic_cast< folder* >(opensStack->get(lFB));
	if (content) delete content;
	content = new std::vector< std::map< std::string, std::string > >;
	*content = lastFB->getContent(relativePath);

	/* Usuwanie starych elementow */
	gltList->Items->Clear();

	int up = 0;
	if (opensStack->size() > 1) {
		up = 1;
		gltList->Items->Add("");
		gltList->Items[0]->BackColor = dabWhite;
		gltList->Items[0]->ForeColor = dabBlack;
		gltList->Items[0]->RowBorderSize = 0;
		dabPictureBox^ picTemp = gcnew dabPictureBox();
		picTemp->Image = dabToImg(resources->GetObject(L"up"));
		picTemp->Size = dabDSize(20, 18);
		gltList->Items[0]->SubItems[iICON_COLUMN]->Control = picTemp;
		gltList->Items[0]->SubItems[iNAME_COLUMN]->Text = com::get("@GLT8");
	}

	/* Dodawanie nowych elementow */
	for (unsigned int i = 0; i < content->size(); i++) {
		// i = numer w content; i+up numer w Items
		gltList->Items->Add("");
		gltList->Items[i+up]->BackColor = dabWhite;
		gltList->Items[i+up]->ForeColor = dabBlack;
		gltList->Items[i+up]->RowBorderSize = 0;

		dabPictureBox^ picTemp = gcnew dabPictureBox();

		/* Ikona i typ */
		if ((*content)[i][dabKeyAtrDirectory] == dabTrue) {
			// folder
			picTemp->Image = dabToImg(resources->GetObject(L"folder"));
			gltList->Items[i+up]->SubItems[iTYPE_COLUMN]->Text = com::get("@GLT4");
		} else {
			str::path p = str::splitPath((*content)[i][dabKeyName]);

			if (p.extension == "bmp") {
				// bmp
				picTemp->Image = dabToImg(resources->GetObject(L"bmp"));
				gltList->Items[i+up]->SubItems[iTYPE_COLUMN]->Text = com::get("@GLT6");
			} else {
				// plik
				picTemp->Image = dabToImg(resources->GetObject(L"file"));
				gltList->Items[i+up]->SubItems[iTYPE_COLUMN]->Text = com::get("@GLT5");
			}
		}
		picTemp->Size = dabDSize(20, 18);
		gltList->Items[i+up]->SubItems[0]->Control = picTemp;

		// Nazwa
		gltList->Items[i+up]->SubItems[iNAME_COLUMN]->Text = 
			gcnew System::String((*content)[i][dabKeyName].c_str());

		// Rozmiar
		if ((*content)[i][dabKeyName] != "") {
			gltList->Items[i+up]->SubItems[iSIZE_COLUMN]->Text = 
				gcnew System::String((*content)[i][dabKeyLength].c_str());
		} else {
			gltList->Items[i+up]->SubItems[iSIZE_COLUMN]->Text = com::get("@GLT7");
		}

		// Data modyfikacji
		gltList->Items[i+up]->SubItems[iMODIFIED_COLUMN]->Text = 
			gcnew System::String((*content)[i][dabKeyLastAccessTimeUtc].c_str());
	}

	gltList->PerformLayout();
	gltList->SuspendLayout();
	gltList->ResumeLayout(false);
}

/* Otwiera nowy katalog */
void dabster::tab::open(std::string path) {
	std::vector< std::string > pth = str::itemNames(path);
	unsigned int level = 0;

	if (pth.size() == 0) {
		/* Sciezka niepoprawna */
		std::vector<std::string> params(1);
		params[0] = path;
		throw err("!TAB0", params);
	}

	/* Porownywanie ze stara sciezka */
	while ((pth.size() > level) && 
		   (static_cast<unsigned int>(opensStack->size()) > level) && 
		   (opensStack->get(level)->getName() == pth[level])) {
		++level;
	}

	/* Usuwanie rozniacej sie czesci stosu */
	for (int i = level; i < opensStack->size(); i++) {
		opensStack->pop();
	}

	/* Dodawanie dysku */
	if (level == 0) {
		++level;
		int index;
		if ((index = drvLst::find(pth[0])) == drvLst::iNOT_FOUND) {
			/* Nie znaleziono partycji */
			std::vector<std::string> params(1);
			params[0] = pth[0];
			throw err("!TAB1", params);
		}

		drive *dTemp = drvLst::get(index);
		dTemp->attach();
		opensStack->push(dTemp);
	}

	/* Dodawanie reszty sciezki */
	for (unsigned int i = level; i < pth.size(); i++) {
		int prnt = opensStack->parent(i);	// parent

		/* Zapewnianie rzeczywistej sciezki rodzica */
		if (opensStack->get(prnt)->getRealPath() == "") {
			int gp = opensStack->parent(prnt);	// grand parent
			folder *grandParent = dynamic_cast< folder* >(opensStack->get(gp));
			std::vector< std::map< std::string, std::string > > src(1), dest(1);

			/* Przygotowywanie informacji o zrodle */
			src[0][dabKeyRelativePath] = opensStack->relativePath(gp + 1, prnt);
			src[0][dabKeyName] = opensStack->get(prnt)->getName();
			src[0][dabKeyAtrDirectory] = dabTrue;

			/* Przygotowywanie informacji o celu */
			dest[0][dabKeyName] = str::sysStrToCppStr(dabIoPath::GetTempFileName());
			std::string tempPath = set::get(L"$sTMP_DIR").s + dest[0][dabKeyName];
			str::fixDelims(tempPath);
			dest[0][dabKeyRealPath] = tempPath;
			
			/* Wypakowywanie rodzica */
			grandParent->extract(src, dest);
			opensStack->get(prnt)->setRealPath(tempPath);
		}

		/* Konwersja rodzica z sfile do rzeczywistego typu */
		int realParentType;
		switch (realParentType = item::realType(opensStack->get(prnt))) {
		case dabSFolder: break;
		case dabSDrive: break;
		case dabFtp: break;
		case dabMlDrv: break;

		case dabSFile: {
			/* Wewnetrzny element sciezki nie moze byc plikiem prostym */
			std::vector<std::string> params(2);
			params[0] = path;
			params[1] = opensStack->get(prnt)->getName();
			throw err("!TAB3", params);
		}
			
		default:
			item* converted = item::convert(opensStack->get(prnt), realParentType);
			opensStack->set(prnt, converted);
		}

		/* Pobieranie zawartosci rodzica */
		folder *parent = dynamic_cast< folder* >(opensStack->get(prnt));
		std::vector< std::map< std::string, std::string > > parentContent;
		std::string relPath = "";
		if (prnt + 1 < opensStack->size()) {
			relPath = opensStack->relativePath(prnt + 1, opensStack->size());
		}
		parentContent = parent->getContent(relPath);

		/* Przegladanie zawartosci */
		unsigned int j = 0;
		while (j < parentContent.size()) {
			if (parentContent[j][dabKeyName] == pth[i]) break; else ++j;
		}

		if (j < parentContent.size()) {
			/* Tworzenie odpowiedniego obiektu */
			item *newPathItem;
			if (parentContent[j][dabKeyAtrDirectory] == dabTrue) {
				newPathItem = new sfolder;
			} else {
				newPathItem = new sfile;
			}
			newPathItem->setName(pth[i]);
			opensStack->push(newPathItem);
			if ((realParentType == dabSFolder) || (realParentType == dabSDrive)) {
				newPathItem->setRealPath(opensStack->relativePath(0, opensStack->size() - 1));
			}
		} else {
			/* Wewnetrzny element sciezki nie moze byc plikiem prostym */
			std::vector<std::string> params(2);
			params[0] = path;
			params[1] = pth[i];
			throw err("!TAB4", params);
		}
	}

	/* Zapewnianie rzeczywistej sciezki ostatniego FB */
	int prnt = opensStack->parent(opensStack->size());	// ostatni FB
	if (opensStack->get(prnt)->getRealPath() == "") {
		int gp = opensStack->parent(prnt);	// grand parent
		folder *grandParent = dynamic_cast< folder* >(opensStack->get(gp));
		std::vector< std::map< std::string, std::string > > src(1), dest(1);

		/* Przygotowywanie informacji o zrodle */
		src[0][dabKeyRelativePath] = opensStack->relativePath(gp + 1, prnt);
		src[0][dabKeyName] = opensStack->get(prnt)->getName();
		if ((opensStack->type(prnt) & (dabFolder)) == dabFolder) {
			src[0][dabKeyAtrDirectory] = dabTrue;
		} else {
			src[0][dabKeyAtrDirectory] = dabFalse;
		}

		/* Przygotowywanie informacji o celu */
		dest[0][dabKeyRealPath] = str::sysStrToCppStr(dabIoPath::GetTempFileName());
		str::fixDelims(dest[0][dabKeyRealPath]);
		str::path p = str::splitPath(dest[0][dabKeyRealPath]);
		dest[0][dabKeyName] = p.fileName;
		if (p.extension != "") dest[0][dabKeyName] += "." + p.extension;
		
		/* Wypakowywanie rodzica */
		grandParent->extract(src, dest);
		opensStack->get(prnt)->setRealPath(dest[0][dabKeyRealPath]);
	}

	tpgTab->Text = gcnew System::String(path.c_str());
	owner->Text = tpgTab->Text;
}

/* Zapisuje pliki i foldery w aktualnym katalogu */
void dabster::tab::store(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
	int lFB = opensStack->parent(opensStack->size());	// ostatni FB
	std::string relativePath = "";
	if (lFB + 1 < opensStack->size()) {
		relativePath = opensStack->relativePath(lFB + 1, opensStack->size() - 1);
	}

	folder *lastFB = dynamic_cast< folder* >(opensStack->get(lFB));

	for (unsigned int i = 0; i < dest.size(); i++) {
		dest[i][dabKeyRelativePath] = relativePath + dest[i][dabKeyRelativePath];
	}

	lastFB->store(src, dest);
}

/* Wypakowuje pliki lub foldery z aktualnego katalogu */
void dabster::tab::extract(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
	int lFB = opensStack->parent(opensStack->size());	// ostatni FB
	std::string relativePath = "";
	if (lFB + 1 < opensStack->size()) {
		relativePath = opensStack->relativePath(lFB + 1, opensStack->size() - 1);
	}

	folder *lastFB = dynamic_cast< folder* >(opensStack->get(lFB));

	for (unsigned int i = 0; i < src.size(); i++) {
		src[i][dabKeyRelativePath] = relativePath + src[i][dabKeyRelativePath];
	}

	lastFB->extract(src, dest);
}

void dabster::tab::del(std::vector< std::map< std::string, std::string > > path) {
}

void dabster::tab::modify(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
}

void dabster::tab::copyInside(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) {
}

unsigned long long dabster::tab::getCapacity(void) {
	return 0;
}

/* Obsluga zdarzenia DoubleClick */
System::Void dabster::tab::gltList_DoubleClick(System::Object^ sender, System::EventArgs^  e) {
	std::string path = opensStack->relativePath(0, opensStack->size() - 1);
	dabArrayList^ sel = gltList->SelectedIndicies;

	int up = 0;
	if (opensStack->size() > 1) up = 1;

	if (sel->Count == 1) {
		int i = static_cast< int >(sel[0]);

		if (up && !i) {
			/* Kliknieto w up */
			std::string::size_type pos;
			if (path[path.size() - 1] == '\\') path.erase(path.size() - 1);
			if ((pos = path.rfind("\\")) != std::string::npos) {
				path.erase(pos + 1);
			}
			str::fixDelims(path);
			open(path);
			refresh();
		} else if (gltList->Items[i]->SubItems[iTYPE_COLUMN]->Text != com::get("@GLT5")) {
			/* Kliknieto w element typu folder */
			if (path[path.size() - 1] != '\\') path.push_back('\\');
			path += str::sysStrToCppStr(gltList->Items[i]->SubItems[iNAME_COLUMN]->Text);	// name
			if (path[path.size() - 1] != '\\') path.push_back('\\');
			str::fixDelims(path);
			open(path);
			refresh();
		} else {
			/* Kliknieto w plik */
		}
	}

	dabster::active = static_cast< int >(owner->Tag);
}

/* Obsluga zdarzenia Clikc */
System::Void dabster::tab::gltList_Click(System::Object^ sender, System::EventArgs^  e) {
	dabster::active = static_cast< int >(owner->Tag);
}

/********************************************************************/