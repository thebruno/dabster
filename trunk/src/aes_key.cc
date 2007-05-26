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
/// @file   aes_key.cc
/// @author Jan Swierzawski
///



#include <cstring>
#include <string>

#ifdef DEBUG
#include <iostream>
#endif

#include "aes_macros.H"
#include "aes_bytes.H"
#include "aes_polynomial.H"
#include "aes_sbox.H"
#include "aes_key.H"

#include "err.h"
#include "com.h"

using namespace aes;


polynomial
key::rcon (uint i)
{
     static byte rcon_[256] = {
	  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36,
	  0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97,
	  0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72,
	  0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66,
	  0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
	  0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d,
	  0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
	  0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61,
	  0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
	  0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
	  0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc,
	  0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5,
	  0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a,
	  0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d,
	  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c,
	  0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
	  0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4,
	  0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
	  0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08,
	  0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
	  0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d,
	  0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2,
	  0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74,
	  0xe8, 0xcb
     };
     //   return polynomial (rcon_[i], 0x0, 0x0, 0x0);
     return polynomial (0x0, 0x0, 0x0, rcon_[i]);
}


const  uint
key::current_round() const
{return curr_round_;}

void
key::current_round(uint i)
{ curr_round_ = i; }

key::key(size_t keylen) 
     : d_Nk (keylen / 32) , d_Nr (d_Nk + 6)
     , d_keysched (std::vector <polynomial> (d_Nk * (d_Nr + 1)))
{}

key::key (const char * key, size_t bytes)
     : d_Nk (strlen(key) / 32), d_Nr (d_Nk + 6)
{ set_key(key, bytes); }

key::key (const std::string key)
     : d_Nk (key.length() / 32), d_Nr (d_Nk + 6)
{ set_key(key); }

key::key (const std::vector <byte> key)
{set_key (key);}

void
key::set_key_length (size_t bits)
{
     if ( (192 != bits) && (128 != bits) && (256 != bits) )
	  throw err("!AES0");
     d_Nk = bits / 32;
     d_Nr = d_Nk + 6;
     if ( (d_keysched.size() != d_Nk)) {
	  std::vector <polynomial> temp (d_Nk * (d_Nr + 1));
	  d_keysched.clear();
	  d_keysched = temp;
     }
}

const size_t
key::Nk() const
{return d_Nk;}

const size_t
key::Nr() const
{return d_Nr;}


void
key::set_key (const std::string key)
{
     static subst_box sbox;
     uint length = key.length();
     set_key_length (8 * length);
     for (uint i = 0; i < d_Nk; i++) {
	  d_keysched[i] = polynomial (key[4*i + 3], key[4*i + 2],
				      key[4*i + 1], key[4*i]);
     }
     
     for (uint i = d_Nk; i < d_Nk * (d_Nr + 1); i++) {
	  polynomial temp (d_keysched[i-1]);
	  if (0 == (i % d_Nk)) {
	       temp <<= 1;
	       sbox.sub_word (temp);

	       temp += rcon (i/d_Nk);
	  } else if ( (d_Nk > 6 ) && (4 == (i % d_Nk))) {
	       sbox.sub_word (temp);
	  }
	  d_keysched[i] = d_keysched[i - d_Nk] + temp;
     }
}

/// tworzy klucz z tablicy charów o rozmiarze length, nawet je¶li
/// pomiêdzy znajd± siê jakie¶ '\0'.
///
/// @param key 
/// @param length 
///
inline void
key::set_key (char const * key, const size_t length)
{
     std::vector<byte> tmp (length);
     for (uint i = 0; i < length; i++) 
	  tmp[length] = key[length];
     set_key (tmp);
}


void
key::set_key (const std::vector <byte> key)
{
     static subst_box sbox;
     uint length = key.size();
     set_key_length (8 * length);
     for (uint i = 0; i < d_Nk; i++) {
// 	  d_keysched[i] = polynomial (key[4*i + 3], key[4*i + 2],
// 				      key[4*i + 1], key[4*i]);
	  for (uint j = 0; j < 4; j++)
	       d_keysched[i][j] = key[4*i + j];
     }
     
     for (uint i = d_Nk; i < d_Nk * (d_Nr + 1); i++) {
	  polynomial temp (d_keysched[i-1]);
	  if (0 == (i % d_Nk)) {
	       temp <<= 1;
	       sbox.sub_word (temp);

	       temp += rcon (i/d_Nk);
	  } else if ( (d_Nk > 6 ) && (4 == (i % d_Nk))) {
	       sbox.sub_word (temp);
	  }
	  d_keysched[i] = d_keysched[i - d_Nk] + temp;
     }
}
     

state
key::round_key()
{
     state result;
     static uint C = result.col_count();
     static uint R = result.row_count();
     if (0 != curr_round_) {
	  for (uint c = 0; c < C; c++) {
	       polynomial temp (d_keysched[curr_round_ * d_Nb + c]);
	       for (uint r = 0; r < R; r++)
		    result[r][c] = temp[r];
	  }
     } else {
	  for (int i=0; i < 4; i++)
	       result[i] = d_keysched[i];
     }
     return result;
}

state
key::round_key(uint round)
{
     state result;
     static uint C = result.col_count();
     static uint R = result.row_count();
     for (uint c = 0; c < C; c++) {
	  polynomial temp (d_keysched[round * d_Nb + c]);
	  for (uint r = 0; r < R; r++)
	       result[r][c] = temp[r];
     }
     return result;
}



#if ( (CURRENT == DKEY) && DEBUG )
#include <iostream>
using namespace std;
int
main(int argc, char *argv[])
{
//      char example [17] = {
// 	  0x2b, 0x7e, 0x15, 0x16,
// 	  0x28, 0xae, 0xd2, 0xa6,
// 	  0xab, 0xf7, 0x15, 0x88,
// 	  0x09, 0xcf, 0x4f, 0x3c,
// 	  0x00 };
//      char example[25] = {
// 	  0x8e, 0x73, 0xb0, 0xf7,
// 	  0xda, 0x0e, 0x64, 0x52,
// 	  0xc8, 0x10, 0xf3, 0x2b,
// 	  0x80, 0x90, 0x79, 0xe5,
// 	  0x62, 0xf8, 0xea, 0xd2,
// 	  0x52, 0x2c, 0x6b, 0x7b,
// 	  0x00
//      };

     char example[33] = {
	  0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
	  0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
	  0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
	  0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4, 0x00
     };
     
     //     std::string example (sample);
     //     std::cout << std::hex << std::showbase << example <<
     //     std::endl;

     std::cout << strlen (example) << std::endl;
     for (int i = 0; i < 32; i++) {
	  std::cout << std::hex << std::showbase
		    << (int) example[i] << " ";
	  if (0 == ( (i + 1) % 4)) std::cout << "\n";
     }
     std::cout << std::endl;

     // ROUND KEY TEST
     char test_round_key[17] = {
	  0x2b,  0x7e,  0x15,  0x16,
	  0x28,  0xae,  0xd2,  0xa6,
	  0xab,  0xf7,  0x15,  0x88,
	  0x09,  0xcf,  0x4f,  0x3c,
	  0x00
     };
     for (int i = 0; i < 32; i++) {
	  std::cout << std::hex << std::showbase
		    << (int) test_round_key[i] << " ";
	  if (0 == ( (i + 1) % 4)) std::cout << "\n";
     }
     std::cout << std::endl;

     key a(test_round_key);
     for (int i = 0; i < 11; i++) {
	  a.current_round (i);
	  state test_state = a.round_key();
	  for (uint r = 0; r < 4; r ++) {
	       std::cout << std::dec << "RKEY " << i << "\t";
	       for (uint c = 0; c < a.Nk(); c++)
		    std::cout << std::hex <<test_state (r, c).to_int() << " ";
	       std::cout << std::endl;
	  }
	  std::cout << std::endl;
     }
     
     for (int i = 0; i < 11; i++) {
	  std::cout << std::noshowbase << i << ":\t"
		    << a.round_key (i) << std::endl;
     }
     

     string t (example);
//     key tmp (t);
     
     cout << "ok!" << endl;

     key test_key;
     test_key.set_key (example);

     return 0;
}

#endif
