// Sigma Dabster 5
// Copyright (C) 2007  Zespół Dabstera

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

// #include "err.h"

// #include "aes.H"
#include "aes_state.H"
#include "aes_bytes.H"
#include "aes_polynomial.H"
#include "aes_sbox.H"
#include "aes_key.H"

#include <iostream>
#include <bitset>
#include <vector>


using namespace aes;

// stayczne dane:

polynomial
state::a()
{
     static polynomial a;
     a[0] = 0x02;
     a[1] = 0x01;
     a[2] = 0x01;
     a[3] = 0x03;

     return a;
}

polynomial
state::inv_a()
{
     static polynomial a;
     a[0] = 0xe;
     a[1] = 0x9;
     a[2] = 0xd;
     a[3] = 0xb;
     return a;
}

state::state (std::vector <byte> bytes)
{
     int max = bytes.size();
     int i = 0;
     for (int r =0; r < 4; r++) {
	  for (int c = 0; c < d_Nb; c++) {
	       d_state[r][c] = bytes[i++];
	       if (i == max) break;
	  }
	  if (i == max) break;
     }
}


/// W przekształceniu shift_rows(), bajty statnich trzech wierszy
/// stanu są cyklicznie przesuwane o różne przesunięcia. Pierwszy
/// wiersz (r = 0) nie jest przesuwany. Dla poszczególnych elementów
/// Stanu shift_rows postępuje następująco
///     s'[r][c] = s[r][(c + shift (r, Nb)) mod Nb]
///        dla 0 < r < 4, 0 ≤ c < Nb
/// to skutkuje przesuwaniem bajtów do "niższych" pozycji (małe
/// wartości C)  w wierszu, podczas gdy "najniższe" bajty jest są
/// przestawiane do "górnych" części rzędu (tj. wyższych wartości c).
///
///
void
state::shift_rows()
{
     // dla r = 0 shift() == 0;
     for (int r = 1; r < 4; r++)
	  d_state[r] <<= shift (r, d_Nb);
}

/// przekształcenie odwrotne do shift_rows ()
///
void
state::inv_shift_rows()
{
     // dla r = 0 shift() == 0.
     for (int r = 1; r < 4; r++)
	  d_state[r] >>= shift (r, d_Nb);
}


void
state::mix_(polynomial direction)
{
      for (register int i = 0; i < d_Nb; i++) {
	  std::vector <byte> tmp (4);
	  for (int k = 0; k < 4; k++)
	       tmp[k] = d_state[k][i];
	  polynomial column (tmp);
	  column *= direction;
	  for (int r = 0; r < 4; r++)
	       d_state[r][i] = column[r];
     }
}


/// Przekształcenie mix_columns() operuje kolumnami na Stanie, każdą z
/// kolumn traktując jak czteroelementowy wielomian w GF(2^8), jak
/// opisane w FIPS-197, sekcja 4.3. Kolumny są traktowane jako
/// wielomiany w GF (2^8) i  mnożone modulo x^4 + 1 z wielomianym
/// danym jako a(x) = {03}x^3 + {01}x^2 + {01}x + {02}.
///
///
void
state::mix_columns()
{ mix_ (a()); }

void
state::inv_mix_columns()
{ mix_ (inv_a()); }

byte &
state::operator () (unsigned int row, unsigned int col)
{ return  d_state[col][row]; }

polynomial &
state::operator[] (unsigned int row)
{return d_state[row];}

void
state::operator += (state other_state)
{
     for (int c = 0; c < d_Nb; c++)
	  for (int r = 0; r < 4; r++)
//	       d_state (r, c) += other_state (r, c);
	       d_state [r][c] += other_state[r][c];
}

// state &
// state::operator + (state round_key)
// {
//      state result;
//      for (int c = 0; c < d_Nb; c++)
// 	  for (int r = 0; r < 4; r++)
// 	       result (r, c) += round_key (r, c);
//      return result;
// }

void
state::operator = (std::vector <polynomial> &other)
{
	  d_state = other;
}




polynomial
state::column (unsigned int c)
{
     return polynomial (d_state[3][c],
			d_state[2][c],
			d_state[1][c],
			d_state[0][c]);
}

polynomial
state::row (unsigned int r)
{return d_state[r];}




void
state::add_round_key(state round_key)
//{d_state += round_key;}
{
     for (uint i = 0; i < d_state.size(); i++)
	  d_state[i] += round_key[i];
}


// void
// state::sub_bytes()
// {
//      for (int i = 0; i < d_Nb; i++)
// 	  sub_word (d_state[i]);
// }
const size_t
state::col_count() const
{return d_state.size();}

const size_t
state::row_count() const
{
     return d_Nb;
}

const size_t
state::size() const
{return d_state.size() * d_Nb;}



void
state::clear()
{d_state.clear();}

std::ostream &
operator<< (std::ostream & out, state st)
{
     static int i = st.row_count();
     static int j = st.col_count();
     for (int r = 0; r < i; r++)
	  for (int c = 0; c < j; c++)
//	       out << std::hex << st(r, c).to_int();
	       out << st (r, c).to_int();
     //     out << std::endl;
     return out;
}


#if (CURRENT == DSTATE && DEBUG)
int
main (int argc, byte ** argv)
{
     using namespace std;
     byte input[4][4]= {{byte (0xd4), byte (0xe0), byte (0xb8), byte (0x1e)},
		      {byte (0x27), byte (0xbf), byte (0xb4), byte (0x41)},
		      {byte (0x11), byte (0x98), byte (0x5d), byte (0x52)},
		      {byte (0xae), byte (0xf1), byte (0xe5), byte (0x30)}};
     vector <polynomial> testarr (4);
     state blast (std::vector <polynomial> (103));
     state test_state;
     for (int c = 0; c < 4; c++) { // kolumny
	  for (int r = 0; r < 4; r++) {
	       test_state (r, c) = input[r][c];
	  }
     }

     cout << "initial state:" << test_state << endl;
     test_state.shift_rows ();
     cout << "after shift_rows():" << test_state <<  endl;
     //      test_state.inv_shift_rows ();
     test_state.mix_columns();
     cout << "after mix_columns():" << test_state << endl;
//      test_state.inv_mix_columns();


     return 0;
}
#endif

