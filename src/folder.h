/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Folder Header
	folder.h

*************************************************/

#ifndef _FOLDER_H
#define _FOLDER_H

#include <string>
#include <vector>
#include <map>
#include "item.h"

class folder : public item {
public:
	virtual ~folder(void) = 0;

	virtual void store(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) = 0;
	virtual void extract(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) = 0;

	virtual void del(std::vector< std::map< std::string, std::string > > path) = 0;
	virtual void modify(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) = 0;
	virtual void copyInside(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest) = 0;

	virtual std::vector< std::map< std::string, std::string > > getContent(std::string path) = 0;
};

#endif

/************************************************/