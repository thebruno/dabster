/*************************************************

	SIGMA Dabster 5
	(c) 2007 The Dabster Team

	File Header
	bmptypes.h

*************************************************/

#ifndef DTYPESHEADERIFLE
#define DTYPESHEADERIFLE

#include <exception>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

namespace bmptypes
{
	#define __BMP_DABSTER_PACKING 1
	typedef unsigned long long uint64;
	typedef signed long long int64;
	typedef unsigned long int uint32;

	typedef signed long int int32;
	typedef unsigned short int uint16;
	typedef unsigned char uint8;

	typedef char int8;

	typedef std::string string;
	typedef std::map<string, string> smap;
	typedef std::vector<smap> vmstring;
	//typedef std::basic_fstream<uint8> ufstream;
	typedef std::basic_fstream<char> fstream;

	class DAB_EXCEPTION: public std::exception {
	};
	// nie znalesiono pliku przy otwieraniu
	class NO_SUCH_FILE: public DAB_EXCEPTION {
	};
	// bledy podczas odczytow z buforow 
	class BMP_INTERNAL_ERROR: public DAB_EXCEPTION {
	};
	// zla nazwa - plik to folder, za maly plik, nie bmp
	class BMP_WRONG_FILE: public DAB_EXCEPTION {
	};
	// bitmapa nie zawierajaca zadnego naglowka dabstera
	class BMP_NO_DAB: public DAB_EXCEPTION {
	};
	class BMP_OPERATION_SUCC: public DAB_EXCEPTION {
	};
	class BMP_OPERATION_FAIL: public DAB_EXCEPTION {
	};
	class BMP_NOT_ENOUGH_SPACE: public DAB_EXCEPTION{
	};


/*
class BMP_EMPTY: public DAB_EXCEPTION {
};
class BMP_DAB_EMPTY: public DAB_EXCEPTION {
};*/
}

#endif
