/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Communicates Header
	com.h

*************************************************/

#ifndef _COM_H
#define _COM_H

//#pragma warning (disable: 4786)
#include <string>
#include <map>

class com {
public:
	com(void);

	static void load(const std::string& path);
	static std::string get(const std::string& number);
	static System::String^ get(System::String^ number);

private:
	static const unsigned int MAX_LINES = 65535;
	static const unsigned int MAX_LINE_LEN = 65535;

	static bool _loaded;
	static std::map<std::string, std::string> _com;
};

#endif

/************************************************/