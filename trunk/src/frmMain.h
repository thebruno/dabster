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

	Main Form Header
	frmMain.h

*********************************************************************/

#ifndef _FRM_MAIN_H_
#define _FRM_MAIN_H_

#include "stdMcrs.h"

namespace dabster {

public ref class frmMain : public System::Windows::Forms::Form {
public:
	frmMain(void);

protected:
	~frmMain();

private:
	/* Glowne okno */
	dabComponentResourceManager^ resources;
	System::Void frmMain_Resize(System::Object^ sender, System::EventArgs^ e);

	/* Menu */
	dabMenuStrip^ mnuMain;
	cli::array<dabToolStripMenuItem^>^ mniMain;
	inline void loadMenu(void);
	void resizeMenu(void);
	void fillOutMenu(void);

	/* Narzedzia */
	cli::array<dabPanel^>^ pnlMainTools;
	cli::array<cli::array<dabPictureBox^>^>^ picMainTools;
	cli::array<cli::array<dabLabel^>^>^ lblMainTools;
	inline void loadTools(void);
	void resizeTools(void);
	void fillOutTools(void);

	/* Pasek stanu */
	dabStatusStrip^ stsMain;
	cli::array<dabToolStripStatusLabel^>^ slbMain;
	cli::array<dabToolStripProgressBar^>^ spbMain;
	inline void loadStat(void);
	void resizeStat(void);
	void fillOutStat(void);
};

}

#endif

/********************************************************************/