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

	Error Header
	err.h

*********************************************************************/

#pragma warning (disable: 4018)

//#include <iostream>
#include "com.h"
//#include "set.h"
#include "err.h"

/* Domyslny konstruktor */
err::err(void) {
	sNumber = "CRITICAL ERROR";
	sDescription = "Unknown error occured";
	sHelp = "Please note all circumstances this error occured in and contact us via www.dabster.prv.pl. We will try to help you and fix this problem as soon as possible.";
}

/* Konstruktor ustawiajacy wartosc bledu */
err::err(const std::string number, const std::vector<std::string> params) {
	sNumber = number;
	sDescription = com::get(number + "d");
	sHelp = com::get(number + "h");

	int i;
	for (i=0; (i < params.size()) && (i < 10); i++) {
		std::string::size_type pos = 0;
		const std::string digits = "0123456789";	/* (48 + i) nie wchodzi w gre - jest nieprzenosne */
		while ((pos = sDescription.find(std::string("<param") + digits[i] + std::string(">"), pos)) \
			   != std::string::npos) {
			sDescription.replace(pos++, 8, params[i]);
		}
		pos = 0;
		while ((pos = sHelp.find(std::string("<param") + digits[i] + std::string(">"), pos)) \
			   != std::string::npos) {
			sHelp.replace(pos++, 8, params[i]);
		}
	}
}

/* Wyswietla blad */
//void error::show(std::string env, std::string text) {
//	if (env == "console") {
//		/* wyswietl blad w konsoli */
//		std::cout << "\n" << com::get("@ERR000") << sNumber << ": " << sDescription << "\n"
//			<< sHelp << "\n" <<"\7";
//		char c;
//		std::cin.get(c);
//	} else if (env == "gui") {
//		/* TODO: wyswietl blad w gui */
//	} else {
//		/* wyswietl blad w konsoli, w najprostszej formie (basic) */
//		std::cout << text;
//		char c;
//		std::cin.get(c);
//	}
//}

/********************************************************************/