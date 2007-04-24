/*************************************************

	SIGMA Dabster 5
	(c) 2007 The Dabster Team

	Drive Header
	drive.h

*************************************************/


#ifndef _DRIVE_H_
#define _DRIVE_H_

#include "folder.h"

class drive : public folder {
public:
	drive(void);
	virtual ~drive(void) = 0;

	virtual long getCapacity(void) = 0;
};

#endif

/************************************************/