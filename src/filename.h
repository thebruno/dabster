#ifndef _H_DABSTER_FILENAME_
#define _H_DABSTER_FILENAME_

#include <string>
#include <cstdlib>

#include <iostream>		/* czy to jest potrzebne? */
#include <fstream>

#ifdef _POSIX_SOURCE
#define PATH_DELIMITER '/'
#elif W32  // jakie jest odpowiednie makro w Windowsie 
#define PATH_DELIMITER '\\'
#else
#error "nieobs³ugiwany system operacyjny"
#endif

class filename
{
     char * c_path;
     string stl_path;
     static char delimiter = PATH_DELIMITER;
public:
     char * getcpath (FILE * filep);
     char * getcpath (filebuf file);
     string getstlpath (FILE * filep);
     string getstlpath (filebuf file);
     
};

     

#endif	// _H_DABSTER_FILENAME_
