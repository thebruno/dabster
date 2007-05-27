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

#include <vector>
#include <string>
#include <cstring>

#include "encryption_template.H"

#include "drive.h"
#include "file.h"
#include "folder.h"
#include "item.h"
#include "prgrss.h"

using namespace dabster;

/// ustawia klucz szyfru. argument keyLength musi byæ podany ze
/// wzglêdu na to, ¿e klucz mo¿e zawieraæ znak 0x0. Bardzo podatne na
/// przepe³nienie bufora.
///
/// @param key 
/// @param strlen 
///
template <class Algorithm, int blocksize>
void
crypt <Algorithm, blocksize>::setKey (char * key,
				      int keyLength = strlen (key))
{
     std::vector <char> tmp
	  for (int i = 0; i < keyLength; i++) 
	       tmp[i] = key[i];
     engine_.set_key (tmp);
}


template <class Algorithm, int blocksize>
void
crypt <Algorithm, blocksize>::setKey (std::string key) 
{
     std::vector <char> tmp;
     int len = key.length();
     for (int i = 0; i < len; i++)
	  tmp[i] = key[i];
     engine_.set_key(tmp);
     
}

template <class Algorithm, int blocksize>
void
crypt <Algorithm, blocksize>::setKey (std::vector <char> key) 
{ engine_.set_key (key); }

template <class Algorithm, int blocksize>
const std::vector <char>
crypt <Algorithm, blocksize>::getKey() 
{
     /*


      */
}
