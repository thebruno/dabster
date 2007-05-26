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
/// @file   aes_bytes.H
/// @author Jan Swierzawski
/// @date   Fri May 11 01:04:54 2007
/// 
/// @brief  obs³uga bajtów na potrzeby szyfrowania AES
/// 
/// Implementacja sekcji 4.2 dokumentu FIPS-197
///

#ifndef _AES_BYTES_
#define _AES_BYTES_

#include <bitset>

namespace aes {
     class byte
     {
	  std::bitset<8> byte_;
	  static std::bitset<8> m;
     public:
	  byte () : byte_ () {};
	  byte (char c) : byte_ (c) {};
	  byte (std::bitset<8> b) : byte_ (b) {};

	  const std::bitset <8>  get_byte () const;
	  inline const bool get_bit (int i) const;

	  unsigned int to_int();
	  inline char to_char() { return (char) to_int(); };
	  byte operator * (const byte op);
	  void operator *= (const byte op);
	  byte operator - (const byte op);
	  byte operator + (const byte op);
	  byte operator + (const std::bitset <8> op);
	  void operator += (const byte op );
	  void operator += (const std::bitset <8> op);
	  bool operator [] (const short int pos);
	  bool operator == (const byte other);
	  bool operator != (const byte other);
	  void operator = (const byte & other);
	  void operator = (const char other);
     };
}

#endif	 // _AES_BYTES_

