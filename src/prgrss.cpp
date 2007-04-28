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

	Progress
	prgrss.cpp

*********************************************************************/

#include <string>
#include "prgrss.h"

prgrss::prgrss(int progressPort, int labelPort) {
	iProgressPort = progressPort;
	iLabelPort = labelPort;

	iValue = 0;
	sText = "";
	iStart = 0;
	iStop = 100;
}

prgrss::~prgrss(void) {
}

/* Ustawia aktualna wartosc paska postepu */
void prgrss::setValue(int value) {
	iValue = value;
}

/* Pobiera aktualna wartosc paska postepu */
int prgrss::getValue(void) {
	return iValue;
}

/* Ustawia tekst wyswietlany przy pasku postepu */
void prgrss::setText(std::string text) {
	sText = text;
}

/* Pobiera tekst wyswietlany przy pasku postepu */
std::string prgrss::getText(void) {
	return sText;
}

/* Ustawia aktualna wartosc poczatkowa */
void prgrss::setStart(int start) {
	iStart = start;
}

/* Pobiera aktualna wartosc poczatkowa */
int prgrss::getStart(void) {
	return iStart;
}

/* Ustawia aktualna wartosc koncowa */
void prgrss::setStop(int stop) {
	iStop = stop;
}

/* Pobiera aktualna wartosc koncowa */
int prgrss::getStop(void) {
	return iStop;
}

/* Uaktualnia widok paska */
void prgrss::show() {
	show(iValue);
}
/* Uaktualnia widok paska */
void prgrss::show(int value) {
	iValue = value;
	/* TODO: Odswierzanie */
}

/********************************************************************/