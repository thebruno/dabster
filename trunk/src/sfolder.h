/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Simple Folder Header
	sfolder.h

*************************************************/

#ifndef _SFOLDER_H
#define _SFOLDER_H

#include <string>
#include <vector>
#include <map>
#include "folder.h"

class sfolder : public folder {
public:
	~sfolder(void);

	void store(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest);
	void extract(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest);

	void del(std::vector< std::map< std::string, std::string > > path);
	void modify(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest);
	void copyInside(std::vector< std::map< std::string, std::string > > src, std::vector< std::map< std::string, std::string > > dest);

	std::vector< std::map< std::string, std::string > > getContent(std::string path);
};

#endif

/************************************************/