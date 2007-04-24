/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Settings Header
	set.h

*************************************************/

#ifndef _SET_H
#define _SET_H

//#pragma warning (disable: 4786)
#include <map>
#include <string>

class set {
public:	
	union multidat {
		long l;
		int i;
		double d;
		bool b;
		char *s;
	};

	static void load(const std::string &path);
	/* TODO static void save(const std::string); */

	static multidat get(const std::string &number);
	static multidat get(System::String^ number);
	static void change(const std::string &number, const multidat &value);

private:
	static const unsigned int MAX_LINES = 65535;
	static const unsigned int MAX_LINE_LEN = 65535;

	static std::map<std::string, multidat> _set;
};

#endif

/************************************************/