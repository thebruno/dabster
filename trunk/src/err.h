/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Error Header
	err.h

*************************************************/

#ifndef _ERR_H
#define _ERR_H

#include <string>
#include <vector>
//#include "set.h"

class err {
public:
	err(void);
	err(const std::string number, const std::vector<std::string> params = std::vector<std::string>(0));

	//static void show(std::string = set::get("$sENV").s, std::string text = "Critical error!");

private:
	std::string _number;
	std::string _description;
	std::string _help;
};

#endif

/************************************************/