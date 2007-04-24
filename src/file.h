/*************************************************

	SIGMA Dabster 5
	(c) 2007 The Dabster Team

	File Header
	file.h

*************************************************/


#ifndef _FILE_H_
#define _FILE_H_

#include "item.h"

class file : public item {
public:
	file(void);
	virtual ~file(void) = 0;

	virtual void edit(void);
};

#endif

/************************************************/