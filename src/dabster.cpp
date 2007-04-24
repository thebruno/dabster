/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Dabster
	dabster.cpp

*************************************************/

#include "frmMain.h"
#include "str.h"

using namespace dabster;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	try {
		Application::Run(gcnew frmMain());
	}
	catch (int e) { return e; }
	catch (...) { return -1; }

	return 0;
}

/************************************************/