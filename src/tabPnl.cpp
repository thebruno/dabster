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

	Tab Panel
	tab.cpp

*********************************************************************/

#include "frmMain.h"
#include "stdMcrs.h"
#include "drvLst.h"
#include "str.h"
#include "tabPnl.h"

static const int iCOMBO_HORIZONTAL_MARGINS = 4;
static const int iCOMBO_TOP_MARGIN = 3;
static const int iCOMBO_HEIGHT = 21;
static const int iCOMBO_TAB_CONTROL_SPACE = 6;

namespace dabster { int active; }

dabster::tabPnl::tabPnl(dabForm^ ownerForm) {
	owner = ownerForm;
	resources = gcnew dabComponentResourceManager(frmMain::typeid);

	tbcPanel = gcnew dabTabControl();
	cboAddress = gcnew dabComboBox();
	tabPages = gcnew cli::array< tab^ >(0);

	// TabControl
	tbcPanel->Location = dabDPoint(0, 0);
	tbcPanel->Name = L"tabControl";
	++dabster::a;
	tbcPanel->Tag = dabster::active = dabster::a;
	tbcPanel->Size = dabDSize(350, 450);
	tbcPanel->SelectedIndexChanged += gcnew System::EventHandler(this, &tabPnl::tbcPanel_SelectedIndexChanged);
	tbcPanel->TextChanged += gcnew System::EventHandler(this, &tabPnl::tbcPanel_TextChanged);

	// Tlo pod ComboBoxami z adresem
	pnlBackground = gcnew dabPanel();
	pnlBackground->BackColor = dabLightBlue;
	pnlBackground->Name = "pnlBackground";

	// ComboBox
	cboAddress->FormattingEnabled = true;
	cboAddress->Location = System::Drawing::Point(4, 63);
	cboAddress->Name = L"comboBox";
	cboAddress->Size = System::Drawing::Size(350, 21);
	for (int i = 0; i < drvLst::size(); i++) {
		cboAddress->Items->Add(gcnew System::String((drvLst::get(i)->getName() + ":\\").c_str()));
	}

	ownerForm->Controls->Add(tbcPanel);
	ownerForm->Controls->Add(cboAddress);
	ownerForm->Controls->Add(pnlBackground);
}

dabster::tabPnl::~tabPnl(void) {
	for (int i = 0; i < tabPages->Length; i++) {
		delete tabPages[i];
	}
	delete [] tabPages;
	owner->Controls->Remove(cboAddress);
	owner->Controls->Remove(tbcPanel);
	owner->Controls->Remove(pnlBackground);
	delete cboAddress;
	delete tbcPanel;
	delete pnlBackground;
	delete resources;
}

/* Dodaje zakladke */
void dabster::tabPnl::addTab(void) {
	int last = tabPages->Length;
	cli::array< tab^ >::Resize(tabPages, last + 1);
	tabPages[last] = gcnew dabster::tab(tbcPanel);
}

/* Usuwa zakladke */
void dabster::tabPnl::removeTab(int index) {
	delete tabPages[index];
	for (int i = index; i < tabPages->Length - 1; i++) tabPages[i] = tabPages[i+1];
	cli::array< tab^ >::Resize(tabPages, tabPages->Length - 1);
}

/* Zwraca ilosc zakladek */
int dabster::tabPnl::size(void) {
	return tabPages->Length;
}

/* Zwraca zakladke o podanym numerze */
dabster::tab^ dabster::tabPnl::getTab(int index) {
	return tabPages[index];
}

/* Zwrtaca numer aktywnej zakladki */
int dabster::tabPnl::activeTab(void) {
	return tbcPanel->SelectedIndex;
}

/* Ustawia szerokosc */
void dabster::tabPnl::Width(int w) {
	width = w;
	resize();
}

/* Ustawia wysokosc */
void dabster::tabPnl::Height(int h) {
	height = h;
	resize();
}

/* Ustawia odleglosc od gory owner'a */
void dabster::tabPnl::Top(int t) {
	top = t;
	resize();
}

/* Ustawia odleglosc od lewego boku owner'a */
void dabster::tabPnl::Left(int l) {
	left = l;
	resize();
}

/* Dostosowuje rozmiary panelu */
void dabster::tabPnl::resize() {
	tbcPanel->Top = top + iCOMBO_TOP_MARGIN + iCOMBO_TAB_CONTROL_SPACE + iCOMBO_HEIGHT;
	tbcPanel->Left = left;
	tbcPanel->Width = width;
	tbcPanel->Height = height - iCOMBO_HEIGHT - iCOMBO_TAB_CONTROL_SPACE - iCOMBO_TOP_MARGIN;

	cboAddress->Left = left + iCOMBO_HORIZONTAL_MARGINS;
	cboAddress->Top = top + iCOMBO_TOP_MARGIN;
	cboAddress->Width = width - 2 * iCOMBO_HORIZONTAL_MARGINS;
	cboAddress->Height = iCOMBO_HEIGHT;

	pnlBackground->Left = left;
	pnlBackground->Width = width + 10;
	pnlBackground->Top = top;
	pnlBackground->Height = iCOMBO_HEIGHT + iCOMBO_TAB_CONTROL_SPACE;

	for (int i = 0; i < tabPages->Length; i++) tabPages[i]->resize();
}

/* Zmieniono adres */
System::Void dabster::tabPnl::tbcPanel_SelectedIndexChanged(System::Object^ sender, System::EventArgs^  e) {
	tbcPanel->Text = tbcPanel->TabPages[tbcPanel->SelectedIndex]->Text;
}

/* Otwarto inny folder */
System::Void dabster::tabPnl::tbcPanel_TextChanged(System::Object^ sender, System::EventArgs^  e) {
	cboAddress->Text = tbcPanel->Text;
}

/********************************************************************/