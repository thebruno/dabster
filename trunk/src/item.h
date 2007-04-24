/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Item Header
	item.h

*************************************************/

#ifndef _ITEM_H
#define _ITEM_H

#include <string>

class item {
public:
	item(void);
	virtual ~item(void) = 0;

	void setName(std::string name);
	void setRealPath(std::string path);

	std::string getName(void);
	std::string getRealPath(void);

protected:
	std::string _name;
	std::string _realPath;
	bool _canceled;
};

#endif

/************************************************/