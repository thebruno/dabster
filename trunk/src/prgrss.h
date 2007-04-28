/*********************************************************************

	Sigma Dabster 5
	Copyright (C) The Dabster Team 2007.
	All rights reserved.

	http://www.dabster.prv.pl/
	http://code.google.com/p/dabster/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 dated June, 1991. 

	This program is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	General Public License for more details. 

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
	02110-1301 USA. 

	Progress Header
	prgrss.h

*********************************************************************/

#ifndef _PRGRSS_H
#define _PRGRSS_H

#include <string>

class prgrss {
public:
	prgrss(int progressPort = iNO_PROGRESS_PORT, int labelPort = iNO_LABEL_PORT);
	~prgrss(void);

	void setValue(int value);
	int getValue(void);

	void setText(std::string text);
	std::string getText(void);

	void setStart(int start);
	int getStart(void);

	void setStop(int stop);
	int getStop(void);

	void show();
	void show(int value);

	static const int iNO_PROGRESS_PORT  = -1;
	static const int iNO_LABEL_PORT  = -1;

private:
	int iProgressPort;
	int iLabelPort;

	int iValue;
	std::string sText;
	int iStart;
	int iStop;
};

#endif

/********************************************************************/