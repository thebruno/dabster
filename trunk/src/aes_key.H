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
/// @file   aes_key.H
/// @author Jan Swierzawski
///

#ifndef AES_KEY
#define AES_KEY

#include <vector>
#include <string>

#include "aes_bytes.H"
#include "aes_polynomial.H"
#include "aes_sbox.H"
#include "aes_macros.H"

#include "err.h"
#include "com.h"

namespace aes {
     class key
     {
     private:
	  size_t d_Nk;		///< ilo¶æ 32b s³ów
	  size_t d_Nr;		///< ilo¶æ rund
	  static const size_t d_Nb = 4;
	  std::vector <polynomial> d_keysched;
	  uint curr_round_;
	  static subst_box sbox;
	  polynomial rcon (const uint i);
     public:
	  key (size_t key_s = AES_DEFAULT_KEYLEN);
	  key (char const * key, size_t bytes);
	  key (const std::string key);
	  key (const std::vector<byte> key);
	  
	  state round_key();
	  state round_key (uint round);
	  
	  const uint current_round() const;
	  void current_round (const uint i);

	  void set_key_length (size_t bits);
	  
	  void set_key (const std::vector <byte> key);
	  
	  void set_key (char const * key, const size_t length);
	  void set_key (const std::string key);

	  const uint Nk() const;
	  const uint Nr() const;
     };
}

#endif	 // AES_KEY
