/*
  Sigma Dabster 5
  Copyright (C) 2007  Zespó³ Dabstera

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
*/


/// @file   encryption_template.H
/// @author Jan Swierzawski
/// @date   Sat May 26 22:19:12 2007
///
/// @brief  Ogólne szyfrowanie dla Dabstera
///  One Template to rule them all, One Template to find them,
///  One Template to bring them all and in the darkness bind them
///  In the Land of C++ where the Bugs lie.
/// 
///   Dok³adniejsze dane na temat tego szablonu:
///   http://prezesgsz.xorg.pl/~jansw/Dabster/szyfrowanie.pdf
///   http://prezesgsz.xorg.pl/~jansw/Dabster/szyfrowanie.odt

#ifndef _H_ENCRYPTION_TEMPLATE
#define _H_ENCRYPTION_TEMPLATE

#include "drive.h"
#include "file.h"
#include "folder.h"
#include "item.h"
#include "prgrss.h"

#include "file.h"
#include "folder.h"

namespace dabster
{

     template <class Algorithm, int blocksize,>
     class crypt
	  : public file
	  , public folder

     {
     private:
	  T engine_;
     public:
	  void setKey (const std::string key);
	  void setKey (const char * key);
	  void setKey (const std::vector <char> key);

	  const std::vector <char> getKey() const;
	  inline const int getBlockSize() const { return blocksize; }; /// <w
								       /// bitach
	  // ta metoda nie ma sensu, rozmiar bloku jest podany w
	  // argumentach wzorca:
#ifdef CRYPT_ENABLE_SET_BLOCK_SIZE
     /// ta metoda nie wykonuje ¿adnej czynno¶ci i jest tu tylko dla
     /// zgodno¶ci z reszt± projektu
	  void setBlockSize(const int bsize) {};
#endif

	  virtual void edit();

	  virtual void
	  store (std::vector <std::map <std::string, std::string> > src,
		std::vector <std::map <std::string, std::string> > dest);

	  virtual void
	  extract (std::vector <std::map <std::string, std::string> > src,
		  std::vector <std::map <std::string, std::string> > dest);

	  virtual void
	  del(std::vector <std::map <std::string, std::string> > path);

	  virtual void
	  modify (std::vector <std::map <std::string, std::string> > src,
		 std::vector <std::map <std::string, std::string> > dest);

	  virtual void
	  copyInside (std::vector <std::map <std::string, std::string> > src,
		     std::vector <std::map <std::string, std::string> > dest);

	  virtual std::vector <std::map <std::string, std::string> >
	  getContent (std::string path = "");
     };
}

#endif	// _H_ENCRYPTION_TEMPLATE
