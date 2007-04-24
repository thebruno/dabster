/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Item
	item.cpp

*************************************************/

#include <string>
#include "item.h"

item::item(void) {
	_canceled = false;
	_name = "";
	_realPath = "";
}

item::~item(void) {
}

/* Ustawia nazwe */
void item::setName(std::string name) {
	_name = name;
}

/* Ustawia rzeczywista sciezke */
void item::setRealPath(std::string path) {
	_realPath = path;
}

/* Pobiera nazwe */
std::string item::getName(void) {
	return _name;
}

/* Pobiera rzeczywista sciezke */
std::string item::getRealPath(void) {
	return _realPath;
}

/************************************************/