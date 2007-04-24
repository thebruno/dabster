/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Error Header
	err.h

*************************************************/

#pragma warning (disable: 4018)

//#include <iostream>
#include "com.h"
//#include "set.h"
#include "err.h"

/* Domyslny konstruktor */
err::err(void) {
	_number = "CRITICAL ERROR";
	_description = "Unknown error occured";
	_help = "Please note all circumstances this error occured in and contact us via www.dabster.prv.pl. We will try to help you and fix this problem as soon as possible.";
}

/* Konstruktor ustawiajacy wartosc bledu */
err::err(const std::string number, const std::vector<std::string> params) {
	_number = number;
	_description = com::get(number + "d");
	_help = com::get(number + "h");

	int i;
	for (i=0; (i < params.size()) && (i < 10); i++) {
		std::string::size_type pos = 0;
		const std::string digits = "0123456789";	/* (48 + i) nie wchodzi w gre - jest nieprzenosne */
		while ((pos = _description.find(std::string("<param") + digits[i] + std::string(">"), pos)) \
			   != std::string::npos) {
			_description.replace(pos++, 8, params[i]);
		}
		pos = 0;
		while ((pos = _help.find(std::string("<param") + digits[i] + std::string(">"), pos)) \
			   != std::string::npos) {
			_help.replace(pos++, 8, params[i]);
		}
	}
}

/* Wyswietla blad */
//void error::show(std::string env, std::string text) {
//	if (env == "console") {
//		/* wyswietl blad w konsoli */
//		std::cout << "\n" << com::get("@ERR000") << _number << ": " << _description << "\n"
//			<< _help << "\n" <<"\7";
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

/************************************************/