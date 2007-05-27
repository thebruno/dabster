// -*- mode: C++ -*-
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

///
/// @file   aes.h 
/// @author Jan ¦wierzawski
/// @date   Thu Mar 15 20:24:06 2007
/// 
/// @brief  szyfrowanie danych przy u¿yciu algorytmu blokowego
/// Rijndael opisanego przez  National Institute of  Standards and
/// Technology. Standard jest dostêpny pod adresem
/// http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
///
#ifndef _AES_H
#define _AES_H

// nag³ówki od Piotrka:
#include "drive.h"
#include "file.h"
#include "folder.h"
#include "item.h"
#include "prgrss.h"

// moje nag³ówki
#include "aes_base.H"

#define DEFAULT_BLOCK_SIZE 192

//   Klasa dziedzicz±ca po klasach folder i file. Szyfruje dane przy
//   pomocy algorytmu blokowego Rijndael opisanego w publikacji
//   numer 197 amerykañskiego Narodowego Instytutu Standardów i
//   Technologii (NIST), znajduj±cej siê pod adresem
//   http://csrc.nist.gov/publications.
 
//   Umo¿liwia dodatkowo ustalenie i pobranie klucza (setKey,
//   getKey), oraz wielko¶ci szyfrowanego bloku (setBlockSize,
//   getBlockSize).
class aes
     : public folder,
       public file,
{
private:
     aes_base aes_;
public:
     aes();
     
     void setKey (std::string key);
     void setKey (char * key);
     
     std::string getKey ();
     char * getKey ();
};

#endif // _AES_H
