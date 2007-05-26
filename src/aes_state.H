// Sigma Dabster 5
// Copyright (C) 2007  Zespó³ Dabstera

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 dated June, 1991.

// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301 USA.

///
/// @file   aes_state.H
/// @author Jan Swierzawski
/// @date   Sat May 19 05:08:55 2007
/// 


#ifndef AES_STATE_H
#define AES_STATE_H

#include <vector>
#include <bitset>
#ifdef DEBUG
#include <string>
#endif

#include "aes_bytes.H"
#include "aes_polynomial.H"

namespace aes {
     class state
     {
     private:
	  static const int d_Nb = 4;
	  polynomial a();
	  polynomial inv_a();
	  void mix_(polynomial direction);
	  
	  std::vector <polynomial> d_state;

	  // na wypadek zmian w przysz³ych wersjach standardu:
	  inline int shift (int r, int Nb) { return r; }
     public:
	  state ()
	       :  d_state (d_Nb)
	       {};
	  state (std::vector <polynomial> bytes)
	       :  d_state (bytes)
	       {};
	  state (std::vector <byte> bytes);
	  
	  byte & operator() (unsigned int row, unsigned int col);
	  polynomial & operator[] (unsigned int row);
	  state & operator + (state other_state);
	  void operator += (state other_state);
	  void operator = (std::vector <polynomial> & other);
	  void shift_rows ();
	  void mix_columns ();
	  void add_round_key (state keysched);
	  void inv_shift_rows ();
	  void inv_mix_columns ();
	  void clear();
	  const size_t size() const; ///< ilo¶æ bajtów
	  const size_t row_count() const;
	  const size_t col_count() const;
	  polynomial column (unsigned int col);
	  polynomial row (unsigned int row);
	  const int block_size() const { return d_Nb; };
     };
}
std::ostream & operator<< (std::ostream & out, aes::state st);

#endif	 // AES_STATE_H

