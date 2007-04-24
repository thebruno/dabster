/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Communicates
	com.cpp

*************************************************/

//#pragma warning (disable: 4786)
#include <fstream>
#include <sstream>
#include "err.h"
#include "str.h"
#include "com.h"

std::map<std::string, std::string> com::_com;
bool com::_loaded;

/* Domyslny konstruktor */
com::com(void) {
	_loaded = false;
}

/* Laduje komunikaty z pliku path */
void com::load(const std::string &path) {
	std::ifstream file(path.c_str(), std::ios_base::in);
	if (!file) { throw err(); }
	unsigned int i = 0;
	char temp[MAX_LINE_LEN];
	while ((i++ < MAX_LINES) && (file.getline(temp, MAX_LINE_LEN))) {
		std::string line(temp);
		if ((line.length() > 1) && ((line[0] == '!') || (line[0] == '@'))) {
			std::istringstream l(line, std::ios_base::in);
			l.get(temp, MAX_LINE_LEN, ' ');
			std::string number(temp);
			if ((number.length() < 2) || (!l) || (!(l.ignore()))) { throw err(); }
			l.getline(temp, MAX_LINE_LEN);
			std::string text(temp);
			if (text.length() < 2) { throw err(); }
			_com[number] = text;
		}
	}
	_loaded = true;
}

/* Zwraca komunikat o podanym numerze */
std::string com::get(const std::string &number) {
	if (_loaded) {
		return _com[number];
	} else {
		throw err();
	}
}

/* Zwraca komunikat o podanym numerze */
System::String^ com::get(System::String^ number) {
	if (_loaded) {
		return gcnew System::String((_com[str::sysStrToCppStr(number)]).c_str());
	} else {
		throw err();
	}
}

/************************************************/