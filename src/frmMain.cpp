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

	Main Form
	frmMain.cpp

*********************************************************************/

#include "frmMain.h"
#include "stdMcrs.h"
#include "drvLst.h"
#include "err.h"
#include "com.h"
#include "set.h"
#include "str.h"

#include "tab.h"
#include "tabPnl.h"

//#define TESTING

#ifdef TESTING
#include "sfolder.h"
#include "oStck.h"
#endif

const int iMENU_ITEMS = 7;
const int iTOOLS_PANELS = 10;
const int iMAX_TOOLS_PER_TOOLBAR = 100;
const int iSTAT_LABELS = 3;
const int iSTAT_PRGRSS = 1;

dabster::frmMain::frmMain(void) {
	resources = gcnew dabComponentResourceManager(frmMain::typeid);
	this->SuspendLayout();

	/* Budowanie frmMain */
	this->AutoScaleDimensions = dabDSizeF(6, 13);
	this->AutoScaleMode = dabAutoScaleModeFont;
	this->BackColor = dabWhite;
	this->ClientSize = dabDSize(792, 566);
	this->Font = dabSansSerif825;
	this->ForeColor = dabBlack;
	this->Icon = (dabToIcon(resources->GetObject(L"icon")));
	this->Name = L"frmMain";
	this->Text = L"Sigma Dabster 5";
	this->Resize += gcnew System::EventHandler(this, &frmMain::frmMain_Resize);
	this->ResumeLayout(false);
	this->PerformLayout();

	/* Ustalanie appPath */
	str::path appPath;
	try { appPath = str::fixAppPath(); }
	catch (...) { /* Niepoprawna sciezka */	throw 1; }

	/* Wczytywanie ustawien */
	try { set::load(str::fixDelims(appPath.drive + appPath.dir + "data\\$set.dat")); }
	catch (err) { /* Nie mozna zaladowac ustawien */ throw 2; }

	/* Wczytywanie bazy komunikatow i bledow */
	try { com::load(str::fixDelims(appPath.drive + appPath.dir + "data\\$com" + set::get("$sLANG").s + ".dat")); }
	catch (err) { /* Nie mozna wczytac bazy */ throw 3;	}

	loadMenu();
	loadTools();
	loadStat();

	drvLst::refresh();

	tplViews = gcnew cli::array< tabPnl^ >(2);	// 0 - lewa, 1 - prawa
	tplViews[0] = gcnew tabPnl(this);
	tplViews[1] = gcnew tabPnl(this);

	frmMain_Resize(this, gcnew System::EventArgs());

	tplViews[0]->addTab();
	tplViews[0]->addTab();
	tplViews[1]->addTab();
	tplViews[1]->addTab();

	tplViews[0]->getTab(0)->open("C:\\");
	tplViews[0]->getTab(0)->refresh();
	tplViews[0]->getTab(1)->open("D:\\");
	tplViews[0]->getTab(1)->refresh();
	tplViews[1]->getTab(0)->open("E:\\");
	tplViews[1]->getTab(0)->refresh();
	tplViews[1]->getTab(1)->open("F:\\");
	tplViews[1]->getTab(1)->refresh();

	#ifdef TESTING
	/* Testy klasy sfolder */

	sfolder p;
	p.setRealPath("D:\\");
	std::vector< std::map< std::string, std::string > > y = p.getContent("Metzger\\");

	std::vector< std::map< std::string, std::string > > vmTempSrc(2);
	vmTempSrc[0][dabKeyRealPath] = "D:\\Y\\";
	vmTempSrc[0][dabKeyName] = "Y";
	vmTempSrc[0][dabKeyLength] = "0";
	vmTempSrc[0][dabKeyAtrDirectory] = dabTrue;

	vmTempSrc[1][dabKeyRealPath] = "D:\\Y\\pl.txt";
	vmTempSrc[1][dabKeyName] = "pl.txt";
	vmTempSrc[1][dabKeyLength] = "12";
	vmTempSrc[1][dabKeyAtrDirectory] = dabFalse;

	std::vector< std::map< std::string, std::string > > vmTempDst(2);
	vmTempDst[0][dabKeyRelativePath] = "YY\\";
	vmTempDst[0][dabKeyName] = "YY";
	vmTempDst[0][dabKeyAtrDirectory] = dabTrue;

	vmTempDst[1][dabKeyRelativePath] = "YY\\pl2.txt";
	vmTempDst[1][dabKeyName] = "pl2.txt";
	vmTempDst[1][dabKeyAtrDirectory] = dabFalse;

	p.store(vmTempSrc, vmTempDst);

	vmTempSrc[0][dabKeyRealPath] = "D:\\Metzger\\Y";
	vmTempSrc[0][dabKeyName] = "Y";

	vmTempSrc[1][dabKeyRealPath] = "D:\\Metzger\\Y\\pl.txt";
	vmTempSrc[1][dabKeyName] = "pl.txt";

	p.extract(vmTempDst, vmTempSrc);

	vmTempSrc[0][dabKeyRelativePath] = "Metzger\\Y\\pl.txt";
	vmTempSrc[0][dabKeyName] = "pl.txt";
	vmTempSrc[0][dabKeyAtrDirectory] = dabFalse;

	vmTempSrc[1][dabKeyRelativePath] = "Metzger\\Y\\";
	vmTempSrc[1][dabKeyName] = "Y";
	vmTempSrc[1][dabKeyAtrDirectory] = dabTrue;

	p.del(vmTempSrc);

	vmTempDst.resize(3);
	vmTempDst[2] = vmTempDst[0];
	vmTempDst[0] = vmTempDst[1];
	vmTempDst[1] = vmTempDst[2];
	vmTempDst.resize(2);

	p.del(vmTempDst);

	/* Testy oStck i drvLst */

	oStck s;
	drvLst dl;
	folder *f;
	int i;

	/* D:\ */
	dl.refresh();
	if ((i = dl.find("D")) == drvLst::iNOT_FOUND) return;
	s.push(dl.get(i));

	/* Metzger\ */
	if ((s.type(0) & dabFolder) == dabFolder) f = dynamic_cast< folder* >(s.get(0));
	std::vector< std::map< std::string, std::string > > cntnt = f->getContent();
	i = 0;
	while (i < static_cast< int >(cntnt.size())) {
		if (cntnt[i][dabKeyName] == "Metzger") break;
		i++;
	}
	if (i == cntnt.size()) return;
	f = new sfolder();
	f->setName("Metzger");
	f->setRealPath("D:\\Metzger\\");
	s.push(dynamic_cast< item* >(f));

	i = s.parent(1);
	#endif
}

dabster::frmMain::~frmMain() {
}

/* Zmiana rozmiaru frmMain */
System::Void dabster::frmMain::frmMain_Resize(System::Object^ sender, 
											  System::EventArgs^ e) {
	resizeMenu();
	resizeTools();
	resizeStat();

	tplViews[0]->Width((this->Width - 14) / 2);
	tplViews[0]->Height(this->Height - 116);
	tplViews[0]->Left(0);
	tplViews[0]->Top(60);

	tplViews[1]->Width((this->Width - 14) / 2);
	tplViews[1]->Height(this->Height - 116);
	tplViews[1]->Left((this->Width - 14) / 2 + 6);
	tplViews[1]->Top(60);
}

/* Buduje glowne menu */
void dabster::frmMain::loadMenu(void) {
	/* Budowanie elementow menu glownego */
	this->mniMain = gcnew cli::array<dabToolStripMenuItem^>(iMENU_ITEMS);
	for (int i = 0; i < iMENU_ITEMS; ++i) {
		this->mniMain[i] = gcnew dabToolStripMenuItem();
		this->mniMain[i]->BackColor = dabLightBlue;
		this->mniMain[i]->Name = gcnew System::String(L"mniMain" + i);
	}

	/* Budowanie glownego menu */
	this->MainMenuStrip = this->mnuMain;
	this->mnuMain = gcnew dabMenuStrip;
	this->mnuMain->AutoSize = false;
	this->mnuMain->BackColor = dabLightBlue;
	this->mnuMain->BackgroundImage = dabToImg(this->resources->GetObject(L"gradient"));
	this->mnuMain->Dock = dabDockStyleNone;
	this->mnuMain->Items->AddRange(mniMain);
	this->mnuMain->Location = dabDPoint(0, 0);
	this->mnuMain->Name = L"mnuMain";
	this->Controls->Add(this->mnuMain);

	fillOutMenu();
	resizeMenu();
}

/* Dostosowuje rozmiary menu */
void dabster::frmMain::resizeMenu(void) {
	static const int iMENU_HEIGHT = 24;
	static const int iMENU_WIDTH = 565;

	this->mnuMain->Size = dabDSize((this->ClientSize.Width < iMENU_WIDTH ? this->ClientSize.Width : iMENU_WIDTH), iMENU_HEIGHT);
}

/* Uaktualnia tresci menu */
void dabster::frmMain::fillOutMenu(void) {
	for (int i = 0; i < iMENU_ITEMS; ++i) {
		this->mniMain[i]->Text = com::get(L"@MNI" + i);
	}
}

/* Buduje pasek narzedzi */
void dabster::frmMain::loadTools(void) {
	static const int iTOOLS_LOGO_WIDTH = 169;
	static const int iTOOLS_LOGO_HEIGHT = 60;

	this->pnlMainTools = gcnew cli::array<dabPanel^>(iTOOLS_PANELS + 1);
	this->picMainTools = gcnew cli::array<cli::array<dabPictureBox^>^>(iTOOLS_PANELS + 1);
	this->lblMainTools = gcnew cli::array<cli::array<dabLabel^>^>(iTOOLS_PANELS + 1);

	// Panel glowny
	this->pnlMainTools[iTOOLS_PANELS] = gcnew dabPanel();
	this->pnlMainTools[iTOOLS_PANELS]->BackColor = dabLightBlue;
	this->pnlMainTools[iTOOLS_PANELS]->Location = dabDPoint(0, 0);
	this->pnlMainTools[iTOOLS_PANELS]->Name = gcnew System::String(L"pnlMainTools" + iTOOLS_PANELS);
	this->Controls->Add(this->pnlMainTools[iTOOLS_PANELS]);

	// Logo
	this->picMainTools[iTOOLS_PANELS] = gcnew cli::array<dabPictureBox^>(1);
	this->picMainTools[iTOOLS_PANELS][0] = gcnew dabPictureBox();
	this->picMainTools[iTOOLS_PANELS][0]->BackColor = dabTransparent;
	this->picMainTools[iTOOLS_PANELS][0]->Image = dabToImg(resources->GetObject(L"logo"));
	this->picMainTools[iTOOLS_PANELS][0]->Name = gcnew System::String(L"picMainTools" + iTOOLS_PANELS + "_0");
	this->picMainTools[iTOOLS_PANELS][0]->Size = dabDSize(iTOOLS_LOGO_WIDTH, iTOOLS_LOGO_HEIGHT);

	fillOutTools();
	resizeTools();
}

/* Dostosowuje rozmiary paska narzedzi */
void dabster::frmMain::resizeTools(void) {
	static const int iTOOLS_HEIGHT = 30;
	static const int iTOOLS_TOP = 24;
	static const int iTOOLS_RIGHT = 9;
	static const int iTOOLS_SPACE_WIDTH = 18;
	static const int iTOOLS_INSIDE_TOP_BOTTOM = 3;
	static const int iTOOLS_INSIDE_LEFT = 4;
	static const int iTOOLS_INSIDE_RIGHT = 8;
	static const int iTOOLS_LOGO_RIGHT = 9;
	static const int iTOOLS_OPTIONS_LEFT = 4;
	static const int iTOOLS_OPTIONS_WIDTH = 7;

	int left, j;
	std::string toolOption;
	int labels, pictures;
	int lines = -1;
	int toolData[iTOOLS_PANELS][3];	// 0 - preferowana szerokosc, 1 - nr linii, 2 - pozycja w linii

	/* Zbieranie danych o paskach */
	for (int i = 0; i < iTOOLS_PANELS; ++i) {
		if (set::get(L"$bTOOLBAR" + i).b) {
			j = 0;
			labels = 0, pictures = 0;
			left = iTOOLS_INSIDE_LEFT;

			toolData[i][1] = set::get(L"$iTOOLBAR" + i + "_LINE").i;
			if (toolData[i][1] > lines) lines = toolData[i][1];
			toolData[i][2] = set::get(L"$iTOOLBAR" + i + "_POS").i;
			while (true) {
				toolOption = set::get(L"$sTOOLBAR" + i + "_" + j++).s;

				if ((toolOption == "end") || j > iMAX_TOOLS_PER_TOOLBAR) {
					toolData[i][0] = left + iTOOLS_OPTIONS_WIDTH + iTOOLS_OPTIONS_LEFT + iTOOLS_INSIDE_RIGHT;
					break;
				}
				if (toolOption == "space") {
					left += iTOOLS_SPACE_WIDTH;
				}
				if (toolOption.find(" pic") != std::string::npos) {
					this->picMainTools[i][pictures]->Location = dabDPoint(left, iTOOLS_INSIDE_TOP_BOTTOM);
					this->pnlMainTools[i]->Controls->Add(this->picMainTools[i][pictures]);
					left += this->picMainTools[i][pictures]->Width;
					pictures++;
				}
				if (toolOption.find(" more") != std::string::npos) {
					this->picMainTools[i][pictures]->Location = dabDPoint(left, iTOOLS_INSIDE_TOP_BOTTOM);
					this->pnlMainTools[i]->Controls->Add(this->picMainTools[i][pictures]);
					left += this->picMainTools[i][pictures]->Width;
					pictures++;
				}
				if (toolOption.find(" text") != std::string::npos) {
					this->lblMainTools[i][labels]->Location = dabDPoint(left, iTOOLS_INSIDE_TOP_BOTTOM);
					this->pnlMainTools[i]->Controls->Add(this->lblMainTools[i][labels]);
					left += this->lblMainTools[i][labels]->Width;
					labels++;
				}
			}
		} else {
			toolData[i][1] = -1;	// brak paska
		}
	}

	/* Ustalanie wielkosci glownego panelu i tworzenie loga */
	if (this->pnlMainTools[iTOOLS_PANELS]->Controls->Contains(this->picMainTools[iTOOLS_PANELS][0])) {
		this->pnlMainTools[iTOOLS_PANELS]->Controls->Remove(this->picMainTools[iTOOLS_PANELS][0]);
	}
	int availableWidth = this->ClientSize.Width - this->picMainTools[iTOOLS_PANELS][0]->Width - iTOOLS_RIGHT - iTOOLS_LOGO_RIGHT;

	this->pnlMainTools[iTOOLS_PANELS]->Size = 
		dabDSize(this->ClientSize.Width, iTOOLS_TOP + (lines + 1) * (iTOOLS_HEIGHT + 2 * iTOOLS_INSIDE_TOP_BOTTOM));

	if (availableWidth < this->mnuMain->Width - iTOOLS_RIGHT) {
		availableWidth += this->picMainTools[iTOOLS_PANELS][0]->Width + iTOOLS_LOGO_RIGHT + iTOOLS_RIGHT;
	} else if (lines >= 0) {
		/* dodawanie loga */
		this->picMainTools[iTOOLS_PANELS][0]->Location = 
			dabDPoint(this->ClientSize.Width - this->picMainTools[iTOOLS_PANELS][0]->Width - iTOOLS_LOGO_RIGHT, (this->pnlMainTools[iTOOLS_PANELS]->Height - this->picMainTools[iTOOLS_PANELS][0]->Height) / 2);
		this->pnlMainTools[iTOOLS_PANELS]->Controls->Add(this->picMainTools[iTOOLS_PANELS][0]);
	}

	for (int i = 0; i < iTOOLS_PANELS; ++i) {
		if (toolData[i][1] != -1) {
			/* Obliczanie skali i wielkosci panelu */
			int prefferedWidth = 0;
			for (j = 0; j < iTOOLS_PANELS; ++j) {
				if (toolData[j][1] == toolData[i][1]) prefferedWidth += toolData[j][0];
			}
			double scale = 1.0;
			if (prefferedWidth > availableWidth) scale = static_cast<double>(availableWidth) / prefferedWidth;

			left = 0;
			for (j = 0; j < iTOOLS_PANELS; ++j) {
				if ((toolData[j][1] == toolData[i][1]) && (toolData[j][2] < toolData[i][2])) {
					left += toolData[j][0];
				}
			}
			this->pnlMainTools[i]->Top = iTOOLS_TOP + toolData[i][1] * (2 * iTOOLS_INSIDE_TOP_BOTTOM + iTOOLS_HEIGHT);
			this->pnlMainTools[i]->Left = static_cast<int>(left * scale);
			this->pnlMainTools[i]->Width = static_cast<int>((toolData[i][0] + iTOOLS_OPTIONS_WIDTH) * scale);
			this->picMainTools[i][this->picMainTools[i]->Length - 1]->Location = 
				dabDPoint(this->pnlMainTools[i]->Width - iTOOLS_OPTIONS_WIDTH - iTOOLS_INSIDE_RIGHT, 0);
			this->pnlMainTools[i]->Controls->Add(this->picMainTools[i][this->picMainTools[i]->Length - 1]);

			j = 0;
			labels = 0;
			pictures = 0;
			left = iTOOLS_INSIDE_LEFT;
			int lastPictures = 0, lastLabels = 0;
			/* Usuwanie niemieszczacych sie elementow */
			while (true) {
				toolOption = set::get(L"$sTOOLBAR" + i + "_" + j++).s;

				if ((toolOption == "end") || j > iMAX_TOOLS_PER_TOOLBAR) { break; }
				if (toolOption == "space") {
					left += iTOOLS_SPACE_WIDTH;
				}
				if (toolOption.find(" pic") != std::string::npos) {
					left += this->picMainTools[i][pictures]->Width;
					pictures++;
				}
				if (toolOption.find(" more") != std::string::npos) {
					left += this->picMainTools[i][pictures]->Width;
					pictures++;
				}
				if (toolOption.find(" text") != std::string::npos) {
					left += this->lblMainTools[i][labels]->Width;
					labels++;
				}

				if (left + iTOOLS_INSIDE_RIGHT + iTOOLS_OPTIONS_WIDTH + iTOOLS_OPTIONS_LEFT > this->pnlMainTools[i]->Width) {
					for (int k = lastPictures; k < this->picMainTools[i]->Length - 1; ++k) {
						if (this->pnlMainTools[i]->Controls->Contains(this->picMainTools[i][k])) {
							this->pnlMainTools[i]->Controls->Remove(this->picMainTools[i][k]);
						}
					}

					for (int k = lastLabels; k < this->lblMainTools[i]->Length; ++k) {
						if (this->pnlMainTools[i]->Controls->Contains(this->lblMainTools[i][k])) {
							this->pnlMainTools[i]->Controls->Remove(this->lblMainTools[i][k]);
						}
					}

					break;
				}

				lastPictures = pictures;
				lastLabels = labels;
			}
		}
	}
}

/* Uaktualnia tresci paska narzedzi */
void dabster::frmMain::fillOutTools(void) {
	const int iTOOLS_HEIGHT = 30;
	const int iTOOLS_ICON_WIDTH = 28;
	const int iTOOLS_MORE_WIDTH = 7;

	/* Inicjalizacja kolejnych paskow narzedzi */
	for (int i = 0; i < iTOOLS_PANELS; ++i) {
		/* Usuwanie starych elementow paska */
		if (this->picMainTools[i] != nullptr) {
			for (int k = 0; k < this->picMainTools[i]->Length; ++k) {
				if (this->picMainTools[i][k] != nullptr) {
					if (this->pnlMainTools[i]->Controls->Contains(this->picMainTools[i][k])) {
						this->pnlMainTools[i]->Controls->Remove(this->picMainTools[i][k]);
					}
					delete this->picMainTools[i][k];
				}
			}
			delete this->picMainTools[i];
			this->picMainTools[i] = nullptr;
		}
		if (this->lblMainTools[i] != nullptr) {
			for (int k = 0; k < this->lblMainTools[i]->Length; ++k) {
				if (this->lblMainTools[i][k] != nullptr) {
					if (this->pnlMainTools[i]->Controls->Contains(this->lblMainTools[i][k])) {
						this->pnlMainTools[i]->Controls->Remove(this->lblMainTools[i][k]);
					}
					delete this->lblMainTools[i][k];
				}
			}
			delete this->lblMainTools[i];
			this->lblMainTools[i] = nullptr;
		}
		if (this->pnlMainTools[i] != nullptr) {
			if (this->pnlMainTools[iTOOLS_PANELS]->Controls->Contains(this->pnlMainTools[i])) {
				this->pnlMainTools[iTOOLS_PANELS]->Controls->Remove(this->pnlMainTools[i]);
			}
			delete this->pnlMainTools[i];
			this->pnlMainTools[i] = nullptr;
		}

		if (set::get(L"$bTOOLBAR" + i).b) {
			/* Pasek 'i' ma byc widoczny */
			if (this->pnlMainTools[i] == nullptr) {
				this->pnlMainTools[i] = gcnew dabPanel();
				this->pnlMainTools[i]->BackColor = dabLightBlue;
				this->pnlMainTools[i]->Height = iTOOLS_HEIGHT;
				this->pnlMainTools[i]->Name = gcnew System::String(L"pnlMainTools" + i);
				this->pnlMainTools[iTOOLS_PANELS]->Controls->Add(this->pnlMainTools[i]);
			}

			int j = 0;
			std::string toolOption;
			int labels = 0, pictures = 0;
			/* Liczenie co bedzie potrzebne */
			while (true) {
				toolOption = set::get(L"$sTOOLBAR" + i + "_" + j++).s;

				if ((toolOption == "end") || j > iMAX_TOOLS_PER_TOOLBAR) { break; }
				if (toolOption.find(" pic") != std::string::npos) { pictures++; }
				if (toolOption.find(" more") != std::string::npos) { pictures++; }
				if (toolOption.find(" text") != std::string::npos) { labels++; }
			}

			this->picMainTools[i] = gcnew cli::array<dabPictureBox^>(pictures + 1);	//Ostatni element na potrzeby przycisku 'opcje paska narzedzi'
			this->lblMainTools[i] = gcnew cli::array<dabLabel^>(labels);

			j = 0;
			labels = 0;
			pictures = 0;
			std::string function;
			/* Ustawianie wlasciwosci narzedzi */
			while (true) {
				toolOption = set::get(L"$sTOOLBAR" + i + "_" + j++).s;

				if (toolOption == "end") { break; }
				if (toolOption != "space") { function = toolOption.substr(0, toolOption.find(" ")); }
				if (toolOption.find(" pic") != std::string::npos) {
					/* Ikona */
					this->picMainTools[i][pictures] = gcnew dabPictureBox();
					this->picMainTools[i][pictures]->BackColor = dabTransparent;
					this->picMainTools[i][pictures]->Name = gcnew System::String(L"picMainTools" + i + "_" + pictures);
					this->picMainTools[i][pictures]->Size = dabDSize(iTOOLS_ICON_WIDTH, iTOOLS_HEIGHT);
					this->picMainTools[i][pictures]->Image = dabToImg(resources->GetObject(gcnew System::String(("tool_" + function + "_active").c_str())));	
					pictures++;
				}
				if (toolOption.find(" more") != std::string::npos) {
					/* Przycisk 'wiecej' */
					this->picMainTools[i][pictures] = gcnew dabPictureBox();
					this->picMainTools[i][pictures]->BackColor = dabTransparent;
					this->picMainTools[i][pictures]->Name = gcnew System::String(L"picMainTools" + i + "_" + pictures);
					this->picMainTools[i][pictures]->Size = dabDSize(iTOOLS_MORE_WIDTH, iTOOLS_HEIGHT);
					this->picMainTools[i][pictures]->Image = dabToImg(resources->GetObject(gcnew System::String(L"tool_more_active")));
					pictures++;
				}
				if (toolOption.find(" text") != std::string::npos) {
					/* Tekst */
					this->lblMainTools[i][labels] = gcnew dabLabel();
					this->lblMainTools[i][labels]->Name = gcnew System::String(L"lblMainTools" + i + "_" + labels);
					this->lblMainTools[i][labels]->TextAlign = dabAlignMiddleLeft;
					this->lblMainTools[i][labels]->BackColor = dabLightBlue;
					this->lblMainTools[i][labels]->Font = dabArial9;
					this->lblMainTools[i][labels]->Height = iTOOLS_HEIGHT;
					this->lblMainTools[i][labels]->Text = com::get(gcnew System::String(("@TOOL_" + function).c_str()));
					this->lblMainTools[i][labels]->Width = this->lblMainTools[i][labels]->PreferredWidth;
					labels++;
				}
			}

			/* Przycisk 'opcje paska narzedzi' */
			this->picMainTools[i][pictures] = gcnew dabPictureBox();
			this->picMainTools[i][pictures]->BackColor = dabTransparent;
			this->picMainTools[i][pictures]->Name = gcnew System::String(L"picMainTools" + i + "_" + pictures);
			this->picMainTools[i][pictures]->Size = dabDSize(iTOOLS_MORE_WIDTH, iTOOLS_HEIGHT);
			this->picMainTools[i][pictures]->Image = dabToImg(resources->GetObject(gcnew System::String(L"tool_toolOptions_active")));
		} else if (this->pnlMainTools[i] != nullptr) {
			/* Pasek 'i' ma byc niewidoczny */
			if (this->pnlMainTools[iTOOLS_PANELS]->Controls->Contains(this->pnlMainTools[i])) {
				this->pnlMainTools[iTOOLS_PANELS]->Controls->Remove(this->pnlMainTools[i]);
			}
			delete this->pnlMainTools[i];
			this->pnlMainTools[i] = nullptr;
		}
	}
}

/* Buduje pasek stanu */
void dabster::frmMain::loadStat(void) {
	cli::array<dabToolStripItem^>^ stiMain = gcnew cli::array<dabToolStripItem^>(iSTAT_LABELS + iSTAT_PRGRSS);

	/* Budowanie elementow paska stanu: etykiety */
	this->slbMain = gcnew cli::array<dabToolStripStatusLabel^>(iSTAT_LABELS);
	for (int i = 0; i < iSTAT_LABELS; ++i) {
		this->slbMain[i] = (gcnew dabToolStripStatusLabel());
		this->slbMain[i]->BackColor = dabTransparent;
		this->slbMain[i]->Font = dabTahoma825;
		this->slbMain[i]->Name = gcnew System::String(L"slbMain" + i);
		this->slbMain[i]->TextAlign = dabAlignMiddleRight;
		this->slbMain[i]->AutoSize = false;
		stiMain[i] = this->slbMain[i];
	}
	this->slbMain[0]->TextAlign = dabAlignMiddleLeft;
	this->slbMain[0]->AutoSize = true;
	this->slbMain[0]->Tag = L"0";
	this->slbMain[1]->Tag = L"1";
	this->slbMain[2]->Tag = L"";
	stiMain[iSTAT_LABELS] = this->slbMain[2];

	/* Budowanie elementow paska stanu: pasek postepu */
	this->spbMain = gcnew cli::array<dabToolStripProgressBar^>(iSTAT_PRGRSS);
	this->spbMain[0] = gcnew dabToolStripProgressBar();
	this->spbMain[0]->BackColor = dabLightBrown;
	this->spbMain[0]->Name = L"spbMain0";
	this->spbMain[0]->Tag = L"";
	stiMain[iSTAT_LABELS - 1] = this->spbMain[0];

	/* Budowanie paska stanu */
	this->stsMain = gcnew dabStatusStrip();
	this->stsMain->Items->AddRange(stiMain);
	this->stsMain->Name = L"stsMain";
	this->Controls->Add(this->stsMain);

	fillOutStat();
	resizeStat();
}

/* Dostosowuje rozmiary paska stanu */
void dabster::frmMain::resizeStat(void) {
	static const int iSLBMAIN0_WIDTH = 150;
	static const int iSTAT_CORNER_WIDTH = 15;
	static const int iSTAT_LABEL_HEIGHT = 17;
	static const int iSTAT_PROGRESS_HEIGHT = 16;
	static const int iSTAT_PROGRESS_WIDTH = 100;
	static const int iSTAT_HEIGHT = 22;

	this->spbMain[0]->Size = dabDSize(iSTAT_PROGRESS_WIDTH, iSTAT_PROGRESS_HEIGHT);

	this->slbMain[0]->Size = dabDSize(iSLBMAIN0_WIDTH, iSTAT_LABEL_HEIGHT);
	this->slbMain[1]->Size = dabDSize(this->ClientSize.Width / 4, iSTAT_LABEL_HEIGHT);
	int width = 0;
	width += this->slbMain[0]->Width;
	width += this->slbMain[1]->Width;
	if (this->spbMain[0]->Tag != L"") { width += this->spbMain[0]->Width; }
	this->slbMain[2]->Size = dabDSize(this->stsMain->Width - width - iSTAT_CORNER_WIDTH, iSTAT_LABEL_HEIGHT);

	this->stsMain->Size = dabDSize(this->ClientSize.Width, iSTAT_HEIGHT);
	this->stsMain->Location = dabDPoint(0, this->ClientSize.Height - iSTAT_HEIGHT);
}

/* Uaktualnia tresci paska stanu */
void dabster::frmMain::fillOutStat(void) {
	if (this->spbMain[0]->Tag != L"") {
		this->spbMain[0]->Value = str::stringToLong(str::sysStrToCppStr(dabToString(this->spbMain[0]->Tag)));
		this->spbMain[0]->Visible = true;
	} else {
		this->spbMain[0]->Visible = false;
	}

	for (int i = 0; i < iSTAT_LABELS; ++i) {
		if (this->slbMain[i]->Tag != L"") {
			this->slbMain[i]->Text = com::get(L"@SLB" + this->slbMain[i]->Tag);
		} else {
			this->slbMain[i]->Text = L"";
		}
	}
}

/********************************************************************/