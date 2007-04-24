/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Main Form Header
	frmMain.h

*************************************************/

#ifndef _FRM_MAIN_H_
#define _FRM_MAIN_H_

#include "stdMcrs.h"

namespace dabster {

public ref class frmMain : public System::Windows::Forms::Form {
public:
	frmMain(void) {	initializeComponent(); }

protected:
	~frmMain() { if (components) delete components; }

private:
	/* Glowne okno */
	_componentResourceManager^ resources;
	_container^ components;
	void initializeComponent(void);
	System::Void frmMain_Load(System::Object^ sender, System::EventArgs^ e);
	System::Void frmMain_Resize(System::Object^ sender, System::EventArgs^ e);

	/* Menu */
	_menuStrip^ mnuMain;
	cli::array<_toolStripMenuItem^>^ mniMain;
	inline void loadMenu(void);
	void resizeMenu(void);
	void fillOutMenu(void);

	/* Narzedzia */
	cli::array<_panel^>^ pnlMainTools;
	cli::array<cli::array<_pictureBox^>^>^ picMainTools;
	cli::array<cli::array<_label^>^>^ lblMainTools;
	inline void loadTools(void);
	void resizeTools(void);
	void fillOutTools(void);

	/* Pasek stanu */
	_statusStrip^ stsMain;
	cli::array<_toolStripStatusLabel^>^ slbMain;
	cli::array<_toolStripProgressBar^>^ spbMain;
	inline void loadStat(void);
	void resizeStat(void);
	void fillOutStat(void);
};

}

#endif

/************************************************/