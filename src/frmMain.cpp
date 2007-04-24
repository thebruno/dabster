/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Main Form
	frmMain.cpp

*************************************************/

#include "frmMain.h"
#include "stdMcrs.h"
#include "err.h"
#include "com.h"
#include "set.h"
#include "str.h"
#include "sfolder.h"

const int _MENU_ITEMS = 7;
const int _TOOLS_PANELS = 10;
const int _MAX_TOOLS_PER_TOOLBAR = 100;
const int _STAT_LABELS = 3;
const int _STAT_PRGRSS = 1;

/* Inicjalizacja frmMain */
void dabster::frmMain::initializeComponent(void) {
	resources = (gcnew System::ComponentModel::ComponentResourceManager(frmMain::typeid));
	this->SuspendLayout();

	//frmMain
	this->AutoScaleDimensions = _dSizeF(6, 13);
	this->AutoScaleMode = _autoScaleModeFont;
	this->BackColor = _white;
	this->ClientSize = _dSize(792, 566);
	this->Font = _sansSerif825;
	this->ForeColor = _black;
	this->Icon = (_toIcon(resources->GetObject(L"icon")));
	this->Name = L"frmMain";
	this->Text = L"Sigma Dabster 5";
	this->Load += gcnew System::EventHandler(this, &frmMain::frmMain_Load);
	this->Resize += gcnew System::EventHandler(this, &frmMain::frmMain_Resize);
	this->ResumeLayout(false);
	this->PerformLayout();
}

/* £adowanie frmMain */
System::Void dabster::frmMain::frmMain_Load(System::Object^ sender, System::EventArgs^ e) {
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

	sfolder p;
	p.setRealPath("D:\\");
	p.getContent("Metzger\\");
}

/* Zmiana rozmiaru frmMain */
System::Void dabster::frmMain::frmMain_Resize(System::Object^ sender, System::EventArgs^ e) {
	resizeMenu();
	resizeTools();
	resizeStat();
}

/* Buduje glowne menu */
void dabster::frmMain::loadMenu(void) {
	/* Budowanie elementow menu glownego */
	this->mniMain = gcnew cli::array<_toolStripMenuItem^>(_MENU_ITEMS);
	for (int i = 0; i < _MENU_ITEMS; ++i) {
		this->mniMain[i] = gcnew _toolStripMenuItem();
		this->mniMain[i]->BackColor = _lightBlue;
		this->mniMain[i]->Name = gcnew System::String(L"mniMain" + i);
	}

	/* Budowanie glownego menu */
	this->MainMenuStrip = this->mnuMain;
	this->mnuMain = gcnew _menuStrip;
	this->mnuMain->AutoSize = false;
	this->mnuMain->BackColor = _lightBlue;
	this->mnuMain->BackgroundImage = _toImg(this->resources->GetObject(L"gradient"));
	this->mnuMain->Dock = _dockStyleNone;
	this->mnuMain->Items->AddRange(mniMain);
	this->mnuMain->Location = _dPoint(0, 0);
	this->mnuMain->Name = L"mnuMain";
	this->Controls->Add(this->mnuMain);

	fillOutMenu();
	resizeMenu();
}

/* Dostosowuje rozmiary menu */
void dabster::frmMain::resizeMenu(void) {
	static const int _MENU_HEIGHT = 24;
	static const int _MENU_WIDTH = 565;

	this->mnuMain->Size = _dSize((this->ClientSize.Width < _MENU_WIDTH ? this->ClientSize.Width : _MENU_WIDTH), _MENU_HEIGHT);
}

/* Uaktualnia tresci menu */
void dabster::frmMain::fillOutMenu(void) {
	for (int i = 0; i < _MENU_ITEMS; ++i) {
		this->mniMain[i]->Text = com::get(L"@MNI" + i);
	}
}

/* Buduje pasek narzedzi */
void dabster::frmMain::loadTools(void) {
	static const int _TOOLS_LOGO_WIDTH = 169;
	static const int _TOOLS_LOGO_HEIGHT = 60;

	this->pnlMainTools = gcnew cli::array<_panel^>(_TOOLS_PANELS + 1);
	this->picMainTools = gcnew cli::array<cli::array<_pictureBox^>^>(_TOOLS_PANELS + 1);
	this->lblMainTools = gcnew cli::array<cli::array<_label^>^>(_TOOLS_PANELS + 1);

	this->pnlMainTools[_TOOLS_PANELS] = gcnew _panel();
	this->pnlMainTools[_TOOLS_PANELS]->BackColor = _lightBlue;
	this->pnlMainTools[_TOOLS_PANELS]->Location = _dPoint(0, 0);
	this->pnlMainTools[_TOOLS_PANELS]->Name = gcnew System::String(L"pnlMainTools" + _TOOLS_PANELS);
	this->Controls->Add(this->pnlMainTools[_TOOLS_PANELS]);

	this->picMainTools[_TOOLS_PANELS] = gcnew cli::array<_pictureBox^>(1);
	this->picMainTools[_TOOLS_PANELS][0] = gcnew _pictureBox();
	this->picMainTools[_TOOLS_PANELS][0]->BackColor = _transparent;
	this->picMainTools[_TOOLS_PANELS][0]->Image = _toImg(resources->GetObject(L"logo"));
	this->picMainTools[_TOOLS_PANELS][0]->Name = gcnew System::String(L"picMainTools" + _TOOLS_PANELS + "_0");
	this->picMainTools[_TOOLS_PANELS][0]->Size = _dSize(_TOOLS_LOGO_WIDTH, _TOOLS_LOGO_HEIGHT);

	fillOutTools();
	resizeTools();
}

/* Dostosowuje rozmiary paska narzedzi */
void dabster::frmMain::resizeTools(void) {
	static const int _TOOLS_HEIGHT = 30;
	static const int _TOOLS_TOP = 24;
	static const int _TOOLS_RIGHT = 9;
	static const int _TOOLS_SPACE_WIDTH = 18;
	static const int _TOOLS_INSIDE_TOP_BOTTOM = 3;
	static const int _TOOLS_INSIDE_LEFT = 4;
	static const int _TOOLS_INSIDE_RIGHT = 8;
	static const int _TOOLS_LOGO_RIGHT = 9;
	static const int _TOOLS_OPTIONS_LEFT = 4;
	static const int _TOOLS_OPTIONS_WIDTH = 7;

	int left, j;
	std::string toolOption;
	int labels, pictures;
	int lines = -1;
	int toolData[_TOOLS_PANELS][3];	// 0 - preferowana szerokosc, 1 - nr linii, 2 - pozycja w linii
	/* Zbieranie danych o paskach */
	for (int i = 0; i < _TOOLS_PANELS; ++i) {
		if (set::get(L"$bTOOLBAR" + i).b) {
			j = 0;
			labels = 0, pictures = 0;
			left = _TOOLS_INSIDE_LEFT;

			toolData[i][1] = set::get(L"$iTOOLBAR" + i + "_LINE").i;
			if (toolData[i][1] > lines) lines = toolData[i][1];
			toolData[i][2] = set::get(L"$iTOOLBAR" + i + "_POS").i;
			while (true) {
				toolOption = set::get(L"$sTOOLBAR" + i + "_" + j++).s;

				if ((toolOption == "end") || j > _MAX_TOOLS_PER_TOOLBAR) {
					toolData[i][0] = left + _TOOLS_OPTIONS_WIDTH + _TOOLS_OPTIONS_LEFT + _TOOLS_INSIDE_RIGHT;
					break;
				}
				if (toolOption == "space") {
					left += _TOOLS_SPACE_WIDTH;
				}
				if (toolOption.find(" pic") != std::string::npos) {
					this->picMainTools[i][pictures]->Location = _dPoint(left, _TOOLS_INSIDE_TOP_BOTTOM);
					this->pnlMainTools[i]->Controls->Add(this->picMainTools[i][pictures]);
					left += this->picMainTools[i][pictures]->Width;
					pictures++;
				}
				if (toolOption.find(" more") != std::string::npos) {
					this->picMainTools[i][pictures]->Location = _dPoint(left, _TOOLS_INSIDE_TOP_BOTTOM);
					this->pnlMainTools[i]->Controls->Add(this->picMainTools[i][pictures]);
					left += this->picMainTools[i][pictures]->Width;
					pictures++;
				}
				if (toolOption.find(" text") != std::string::npos) {
					this->lblMainTools[i][labels]->Location = _dPoint(left, _TOOLS_INSIDE_TOP_BOTTOM);
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
	if (this->pnlMainTools[_TOOLS_PANELS]->Controls->Contains(this->picMainTools[_TOOLS_PANELS][0])) {
		this->pnlMainTools[_TOOLS_PANELS]->Controls->Remove(this->picMainTools[_TOOLS_PANELS][0]);
	}
	int availableWidth = this->ClientSize.Width - this->picMainTools[_TOOLS_PANELS][0]->Width - _TOOLS_RIGHT - _TOOLS_LOGO_RIGHT;

	this->pnlMainTools[_TOOLS_PANELS]->Size = 
		_dSize(this->ClientSize.Width, _TOOLS_TOP + (lines + 1) * (_TOOLS_HEIGHT + 2 * _TOOLS_INSIDE_TOP_BOTTOM));

	if (availableWidth < this->mnuMain->Width - _TOOLS_RIGHT) {
		availableWidth += this->picMainTools[_TOOLS_PANELS][0]->Width + _TOOLS_LOGO_RIGHT + _TOOLS_RIGHT;
	} else if (lines >= 0) {
		/* dodawanie loga */
		this->picMainTools[_TOOLS_PANELS][0]->Location = 
			_dPoint(this->ClientSize.Width - this->picMainTools[_TOOLS_PANELS][0]->Width - _TOOLS_LOGO_RIGHT, (this->pnlMainTools[_TOOLS_PANELS]->Height - this->picMainTools[_TOOLS_PANELS][0]->Height) / 2);
		this->pnlMainTools[_TOOLS_PANELS]->Controls->Add(this->picMainTools[_TOOLS_PANELS][0]);
	}

	for (int i = 0; i < _TOOLS_PANELS; ++i) {
		if (toolData[i][1] != -1) {
			/* Obliczanie skali i wielkosci panelu */
			int prefferedWidth = 0;
			for (j = 0; j < _TOOLS_PANELS; ++j) {
				if (toolData[j][1] == toolData[i][1]) prefferedWidth += toolData[j][0];
			}
			double scale = 1.0;
			if (prefferedWidth > availableWidth) scale = static_cast<double>(availableWidth) / prefferedWidth;

			left = 0;
			for (j = 0; j < _TOOLS_PANELS; ++j) {
				if ((toolData[j][1] == toolData[i][1]) && (toolData[j][2] < toolData[i][2])) {
					left += toolData[j][0];
				}
			}
			this->pnlMainTools[i]->Top = _TOOLS_TOP + toolData[i][1] * (2 * _TOOLS_INSIDE_TOP_BOTTOM + _TOOLS_HEIGHT);
			this->pnlMainTools[i]->Left = static_cast<int>(left * scale);
			this->pnlMainTools[i]->Width = static_cast<int>((toolData[i][0] + _TOOLS_OPTIONS_WIDTH) * scale);
			this->picMainTools[i][this->picMainTools[i]->Length - 1]->Location = 
				_dPoint(this->pnlMainTools[i]->Width - _TOOLS_OPTIONS_WIDTH - _TOOLS_INSIDE_RIGHT, 0);
			this->pnlMainTools[i]->Controls->Add(this->picMainTools[i][this->picMainTools[i]->Length - 1]);

			j = 0;
			labels = 0;
			pictures = 0;
			left = _TOOLS_INSIDE_LEFT;
			int lastPictures = 0, lastLabels = 0;
			/* Usuwanie niemieszczacych sie elementow */
			while (true) {
				toolOption = set::get(L"$sTOOLBAR" + i + "_" + j++).s;

				if ((toolOption == "end") || j > _MAX_TOOLS_PER_TOOLBAR) { break; }
				if (toolOption == "space") {
					left += _TOOLS_SPACE_WIDTH;
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

				if (left + _TOOLS_INSIDE_RIGHT + _TOOLS_OPTIONS_WIDTH + _TOOLS_OPTIONS_LEFT > this->pnlMainTools[i]->Width) {
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
	const int _TOOLS_HEIGHT = 30;
	const int _TOOLS_ICON_WIDTH = 28;
	const int _TOOLS_MORE_WIDTH = 7;

	/* Inicjalizacja kolejnych paskow narzedzi */
	for (int i = 0; i < _TOOLS_PANELS; ++i) {
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
			if (this->pnlMainTools[_TOOLS_PANELS]->Controls->Contains(this->pnlMainTools[i])) {
				this->pnlMainTools[_TOOLS_PANELS]->Controls->Remove(this->pnlMainTools[i]);
			}
			delete this->pnlMainTools[i];
			this->pnlMainTools[i] = nullptr;
		}

		if (set::get(L"$bTOOLBAR" + i).b) {
			/* Pasek 'i' ma byc widoczny */
			if (this->pnlMainTools[i] == nullptr) {
				this->pnlMainTools[i] = gcnew _panel();
				this->pnlMainTools[i]->BackColor = _lightBlue;
				this->pnlMainTools[i]->Height = _TOOLS_HEIGHT;
				this->pnlMainTools[i]->Name = gcnew System::String(L"pnlMainTools" + i);
				this->pnlMainTools[_TOOLS_PANELS]->Controls->Add(this->pnlMainTools[i]);
			}

			int j = 0;
			std::string toolOption;
			int labels = 0, pictures = 0;
			/* Liczenie co bedzie potrzebne */
			while (true) {
				toolOption = set::get(L"$sTOOLBAR" + i + "_" + j++).s;

				if ((toolOption == "end") || j > _MAX_TOOLS_PER_TOOLBAR) { break; }
				if (toolOption.find(" pic") != std::string::npos) { pictures++; }
				if (toolOption.find(" more") != std::string::npos) { pictures++; }
				if (toolOption.find(" text") != std::string::npos) { labels++; }
			}

			this->picMainTools[i] = gcnew cli::array<_pictureBox^>(pictures + 1);	//Ostatni element na potrzeby przycisku 'opcje paska narzedzi'
			this->lblMainTools[i] = gcnew cli::array<_label^>(labels);

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
					this->picMainTools[i][pictures] = gcnew _pictureBox();
					this->picMainTools[i][pictures]->BackColor = _transparent;
					this->picMainTools[i][pictures]->Name = gcnew System::String(L"picMainTools" + i + "_" + pictures);
					this->picMainTools[i][pictures]->Size = _dSize(_TOOLS_ICON_WIDTH, _TOOLS_HEIGHT);
					this->picMainTools[i][pictures]->Image = _toImg(resources->GetObject(gcnew System::String(("tool_" + function + "_active").c_str())));	
					pictures++;
				}
				if (toolOption.find(" more") != std::string::npos) {
					/* Przycisk 'wiecej' */
					this->picMainTools[i][pictures] = gcnew _pictureBox();
					this->picMainTools[i][pictures]->BackColor = _transparent;
					this->picMainTools[i][pictures]->Name = gcnew System::String(L"picMainTools" + i + "_" + pictures);
					this->picMainTools[i][pictures]->Size = _dSize(_TOOLS_MORE_WIDTH, _TOOLS_HEIGHT);
					this->picMainTools[i][pictures]->Image = _toImg(resources->GetObject(gcnew System::String(L"tool_more_active")));
					pictures++;
				}
				if (toolOption.find(" text") != std::string::npos) {
					/* Tekst */
					this->lblMainTools[i][labels] = gcnew _label();
					this->lblMainTools[i][labels]->Name = gcnew System::String(L"lblMainTools" + i + "_" + labels);
					this->lblMainTools[i][labels]->TextAlign = _alignMiddleLeft;
					this->lblMainTools[i][labels]->BackColor = _lightBlue;
					this->lblMainTools[i][labels]->Font = _arial9;
					this->lblMainTools[i][labels]->Height = _TOOLS_HEIGHT;
					this->lblMainTools[i][labels]->Text = com::get(gcnew System::String(("@TOOL_" + function).c_str()));
					this->lblMainTools[i][labels]->Width = this->lblMainTools[i][labels]->PreferredWidth;
					labels++;
				}
			}

			/* Przycisk 'opcje paska narzedzi' */
			this->picMainTools[i][pictures] = gcnew _pictureBox();
			this->picMainTools[i][pictures]->BackColor = _transparent;
			this->picMainTools[i][pictures]->Name = gcnew System::String(L"picMainTools" + i + "_" + pictures);
			this->picMainTools[i][pictures]->Size = _dSize(_TOOLS_MORE_WIDTH, _TOOLS_HEIGHT);
			this->picMainTools[i][pictures]->Image = _toImg(resources->GetObject(gcnew System::String(L"tool_toolOptions_active")));
		} else if (this->pnlMainTools[i] != nullptr) {
			/* Pasek 'i' ma byc niewidoczny */
			if (this->pnlMainTools[_TOOLS_PANELS]->Controls->Contains(this->pnlMainTools[i])) {
				this->pnlMainTools[_TOOLS_PANELS]->Controls->Remove(this->pnlMainTools[i]);
			}
			delete this->pnlMainTools[i];
			this->pnlMainTools[i] = nullptr;
		}
	}
}

/* Buduje pasek stanu */
void dabster::frmMain::loadStat(void) {
	cli::array<_toolStripItem^>^ stiMain = gcnew cli::array<_toolStripItem^>(_STAT_LABELS + _STAT_PRGRSS);

	/* Budowanie elementow paska stanu: etykiety */
	this->slbMain = gcnew cli::array<_toolStripStatusLabel^>(_STAT_LABELS);
	for (int i = 0; i < _STAT_LABELS; ++i) {
		this->slbMain[i] = (gcnew _toolStripStatusLabel());
		this->slbMain[i]->BackColor = _transparent;
		this->slbMain[i]->Font = _tahoma825;
		this->slbMain[i]->Name = gcnew System::String(L"slbMain" + i);
		this->slbMain[i]->TextAlign = _alignMiddleRight;
		this->slbMain[i]->AutoSize = false;
		stiMain[i] = this->slbMain[i];
	}
	this->slbMain[0]->TextAlign = _alignMiddleLeft;
	this->slbMain[0]->AutoSize = true;
	this->slbMain[0]->Tag = L"0";
	this->slbMain[1]->Tag = L"1";
	this->slbMain[2]->Tag = L"";
	stiMain[_STAT_LABELS] = this->slbMain[2];

	/* Budowanie elementow paska stanu: pasek postepu */
	this->spbMain = gcnew cli::array<_toolStripProgressBar^>(_STAT_PRGRSS);
	this->spbMain[0] = gcnew _toolStripProgressBar();
	this->spbMain[0]->BackColor = _lightBrown;
	this->spbMain[0]->Name = L"spbMain0";
	this->spbMain[0]->Tag = L"";
	stiMain[_STAT_LABELS - 1] = this->spbMain[0];

	/* Budowanie paska stanu */
	this->stsMain = gcnew _statusStrip();
	this->stsMain->Items->AddRange(stiMain);
	this->stsMain->Name = L"stsMain";
	this->Controls->Add(this->stsMain);

	fillOutStat();
	resizeStat();
}

/* Dostosowuje rozmiary paska stanu */
void dabster::frmMain::resizeStat(void) {
	static const int _SLBMAIN0_WIDTH = 150;
	static const int _STAT_CORNER_WIDTH = 15;
	static const int _STAT_LABEL_HEIGHT = 17;
	static const int _STAT_PROGRESS_HEIGHT = 16;
	static const int _STAT_PROGRESS_WIDTH = 100;
	static const int _STAT_HEIGHT = 22;

	this->spbMain[0]->Size = _dSize(_STAT_PROGRESS_WIDTH, _STAT_PROGRESS_HEIGHT);

	this->slbMain[0]->Size = _dSize(_SLBMAIN0_WIDTH, _STAT_LABEL_HEIGHT);
	this->slbMain[1]->Size = _dSize(this->ClientSize.Width / 4, _STAT_LABEL_HEIGHT);
	int width = 0;
	width += this->slbMain[0]->Width;
	width += this->slbMain[1]->Width;
	if (this->spbMain[0]->Tag != L"") { width += this->spbMain[0]->Width; }
	this->slbMain[2]->Size = _dSize(this->stsMain->Width - width - _STAT_CORNER_WIDTH, _STAT_LABEL_HEIGHT);

	this->stsMain->Size = _dSize(this->ClientSize.Width, _STAT_HEIGHT);
	this->stsMain->Location = _dPoint(0, this->ClientSize.Height - _STAT_HEIGHT);
}

/* Uaktualnia tresci paska stanu */
void dabster::frmMain::fillOutStat(void) {
	if (this->spbMain[0]->Tag != L"") {
		this->spbMain[0]->Value = str::stringToLong(str::sysStrToCppStr(_toString(this->spbMain[0]->Tag)));
		this->spbMain[0]->Visible = true;
	} else {
		this->spbMain[0]->Visible = false;
	}

	for (int i = 0; i < _STAT_LABELS; ++i) {
		if (this->slbMain[i]->Tag != L"") {
			this->slbMain[i]->Text = com::get(L"@SLB" + this->slbMain[i]->Tag);
		} else {
			this->slbMain[i]->Text = L"";
		}
	}
}

/************************************************/