/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	String Header
	str.h

*************************************************/

#ifndef _STR_H
#define _STR_H

#include <string>

class str {
public:
	struct path {
		std::string drive;
		std::string dir;
		std::string fileName;
		std::string extension;
	};

	static path splitPath(std::string p, const std::string delimiter = _delimiter);
	static std::string fixDelims(std::string, std::string = _delimiter);
	static void setDelimiter(const std::string delimiter);
	static void setAppPath(path appPath);
	static path getAppPath(void);
	static str::path fixAppPath(void);

	static std::string sysStrToCppStr(System::String^ sysStr);
	static char* strToChar(const std::string);
	static std::string charToStr(const char c);
	static long stringToLong(const std::string s);
//	static std::string longToString(const long);

private:
	static std::string _delimiter;
	static path _appPath;
};

#endif

/************************************************/